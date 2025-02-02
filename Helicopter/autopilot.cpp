#include "autopilot.h"

constexpr float Ra = 180.f / 3.14f;
constexpr float LIM20 = 255.f / 10.f;  // лимит управления от автопилота 20% от полного хода

Autopilot::Autopilot(QObject *parent): QObject(parent)
{
    // чтение коэффициентов из конфига
    ini = new QSettings("ap_config.ini", QSettings::IniFormat);
    ini->beginGroup("AUTOPILOT");
    {
        roll_params.k = ini->value("k_roll",2.f).toFloat();
        roll_params.kw = ini->value("kw_roll",0.f).toFloat();
        roll_params.k_pilot = ini->value("k_pilot_roll",0.5f).toFloat();

        pitch_params.k = ini->value("k_pitch",2.f).toFloat();
        pitch_params.kw = ini->value("kw_pitch",0.f).toFloat();
        pitch_params.k_pilot = ini->value("k_pilot_pitch",0.5f).toFloat();

        yaw_params.k = ini->value("k_yaw",2.f).toFloat();
        yaw_params.kw = ini->value("kw_yaw",0.f).toFloat();
        yaw_params.k_pilot = ini->value("k_pilot_yaw",.5f).toFloat();

        k_speed = ini->value("k_speed",1.f).toFloat();
        height_params.k = ini->value("k_height",1.f).toFloat();

        dParamAP.flagOldAP = ini->value("flagOldAP", false).toBool();
        dParamAP.d_roll_AP = ini->value("d_roll_AP", 1.f).toFloat();
        dParamAP.d_pitch_AP = ini->value("d_pitch_AP", 1.f).toFloat();
        dParamAP.d_yaw_AP = ini->value("d_yaw_AP", 1.f).toFloat();
        dParamAP.range_roll_AP = ini->value("range_roll_AP", 10.f).toFloat();
        dParamAP.range_pitch_AP = ini->value("range_pitch_AP", 10.f).toFloat();
        dParamAP.range_yaw_AP = ini->value("range_yaw_AP", 10.f).toFloat();
        dParamAP.range_height_AP = ini->value("range_height_AP", 10.f).toFloat();
        dParamAP.k_newAP = ini->value("k_newAP", 1.f).toFloat();
    }

    ini->endGroup();
    delete ini;

    turnOff_roll_pitch();
    turnOff_speed();
    turnOff_yaw();

    roll_params.set = 0;
    pitch_params.set = 0;
    yaw_params.set = 0;
    set_speed = 0;
    qDebug( ) << "AP Roll:" << roll_params.k << roll_params.kw << roll_params.k_pilot;
    qDebug( ) << "AP Pitch:" << pitch_params.k << pitch_params.kw << pitch_params.k_pilot;
    qDebug( ) << "k_speed" << k_speed;
    qDebug( ) << "height_params.k" << height_params.k;
}

Autopilot::~Autopilot()
{

}

float Autopilot::saturation(const float ap_input, const float p_input, const float limit)
{
    return std::min(std::max(ap_input, p_input - limit), p_input + limit);
}

float Autopilot::getPitchCoords( )      const 
{ 
    return delta_pitch < -127.f ? 0 : delta_pitch + 127.f; 
}

float Autopilot::getRollCoords( )       const 
{ 
    return delta_roll < -127.f ? 0 : delta_roll + 127.f; 
}

float Autopilot::getYawCoords( )        const 
{ 
    return delta_yaw < -127.f ? 0 : delta_yaw + 127.f; 
}

float Autopilot::getHeightCoords( )     const 
{ 
    return height_params.d_pilot; 
}

// КРЕН //
/*  input
    jx - сигнал с РУ
    roll - текущий крен
    d_roll - изменение крена
    w_gamma - текущая угловая скорость по крену
    roll_set - заданный крен (с рукояти центровки)
*/
void Autopilot::setRollParams(const float jx, const float roll, const float w_gamma, const float roll_set)
{
    roll_params.d_pilot = jx - 127.f; // центрируем сигнал с РУ (0-255)
    roll_params.angle = roll;
    roll_params.w = w_gamma;
    roll_params.set = roll_set;
}

void Autopilot::calc_roll()
{
    float d_roll = roll_params.k_pilot * roll_filter.computeNextOutput(roll_params.d_pilot) + (roll_params.set - roll_params.angle * Ra);
    delta_roll_calc = roll_params.k * d_roll + roll_params.kw * roll_params.w;
    delta_roll = saturation(delta_roll_calc, roll_params.d_pilot, LIM20);
    diff_roll = (roll_params.d_pilot - delta_roll) / LIM20;
}

// ТАНГАЖ //
/*  input
    jy - сигнал с РУ
    pitch - текущий тангаж
    d_pitch- изменение тангажа
    w_theta - текущая угловая скорость по тангажу
    pitch_set - заданный тангаж (с рукояти центровки)
    speed - текущая приборная скорость
*/
void Autopilot::setPitchParams(const float jy, const float pitch, const float w_theta, const float pitch_set)
{
    pitch_params.d_pilot = jy - 127.f;  // положение ручки с 0 в центре для расчета
    pitch_params.angle = pitch;
    pitch_params.w = w_theta;
    pitch_params.set = pitch_set;
}

void Autopilot::calc_pitch()
{
    float d_pitch = pitch_params.k_pilot * pitch_params.d_pilot + (pitch_params.set - pitch_params.angle * Ra);
    delta_pitch_calc = pitch_params.k * d_pitch + pitch_params.kw * pitch_filter.computeNextOutput(pitch_params.w);
    
    if (!ap_speed) { set_speed= speed; }
    else { delta_pitch_calc += k_speed * (set_speed - speed); }

    delta_pitch = saturation(delta_pitch_calc, pitch_params.d_pilot, LIM20);
    diff_pitch = (pitch_params.d_pilot - delta_pitch) / LIM20;
}

// СКОРОСТЬ //
/*  input
    speed - скорость
    set - ручка корректировки
*/
void Autopilot::setSpeedParams(const float speed)
{
    this->speed = speed;
}

// НАПРАВЛЕНИЕ //
/* input
   dx - сигнал с педалей
   yaw - текущее направление
   d_yaw изменение угла
   w_psi - текущая углавая скорость по направлению
   yaw_set - заданное направление
   pd - датчики на педалях
*/
void Autopilot::setYawParams(const float px, const float yaw, const float w_psi, const float yaw_set, const bool pd)
{
    pedal = pd;
    if(!ap_yaw || pd) // режим согласования курса
    { 
        yaw_params.set_angle = 360 - yaw * Ra;   // в момент включения автопилота перестает меняться (задает направление на включение)
    }
    yaw_params.d_pilot = px - 127.f;

    yaw_params.angle = 360 - yaw * Ra;
    yaw_params.w = w_psi;
    yaw_params.set = yaw_set;
}

void Autopilot::calc_yaw()
{
    if (pedal)
    {
        delta_yaw = yaw_params.d_pilot;
    }
    else
    {
        yaw_params.set_angle = (yaw_params.set_angle > 180 && yaw_params.angle < 180) ? 360 - yaw_params.set_angle : yaw_params.set_angle;
        yaw_params.angle = (yaw_params.set_angle < 180 && yaw_params.angle > 180) ? 360 - yaw_params.angle : yaw_params.angle;
        
        float d_yaw = yaw_params.set + yaw_params.k * (yaw_params.set_angle - yaw_params.angle);
        delta_yaw = d_yaw + yaw_params.kw * yaw_params.w;
    }

    delta_yaw = yaw_filter.computeNextOutput(delta_yaw);
    delta_yaw = saturation(delta_yaw, yaw_params.d_pilot, LIM20);
    diff_yaw = (yaw_params.d_pilot - delta_yaw) / LIM20;
}

// ВЫСОТА //
void Autopilot::setHeightParams(const float fosh, const float height)
{
    if (!ap_height) // режим согласования высоты
    {
        height_params.d_pilot = fosh;   // в момент включения автопилота перестает меняться (задает направление на включение)
        height_params.set_height = height;
    }
    height_params.height = height;
}

void Autopilot::calc_height()
{
    delta_height = height_params.k * (height_params.set_height - height_params.height);
    height_params.d_pilot += delta_height;
}

// основная функция расчета по таймеру
void Autopilot::calculate_delta()
{
    // проверка состояния каналов и запуск расчетов
    if (ap_roll_pitch)
    {
        calc_roll();
        calc_pitch();
    }
    else
    {
        delta_roll = roll_params.d_pilot;
        delta_pitch = pitch_params.d_pilot;
    }
    if (ap_yaw)
    {
        calc_yaw();
    }
    else
    {
        delta_yaw = yaw_params.d_pilot;
    }
    if (ap_height)
    {
        calc_height();
    }
}

float Autopilot::getRollDifference() const
{
    return -diff_roll;
}

float Autopilot::getPitchDifference() const
{
    return diff_pitch;
}

float Autopilot::getYawDifference() const
{
    return diff_yaw;
}

float Autopilot::getHeightDifference() const
{
    return diff_height;
}

void Autopilot::turnOn_roll_pitch()
{ 
    ap_roll_pitch = true; 
}

void Autopilot::turnOff_roll_pitch()
{
    ap_roll_pitch = false; 
}

void Autopilot::turnOn_yaw()
{
    ap_yaw = true;
}
void Autopilot::turnOff_yaw()
{ 
    ap_yaw = false;
}

void Autopilot::turnOn_height()
{
    ap_height = true;
}

void Autopilot::turnOff_height()
{
    ap_height = false;
}

void Autopilot::turnOn_speed()
{
    ap_speed = true;
}

void Autopilot::turnOff_speed()
{
    ap_speed = false;
}

QDebug operator<<(QDebug out, const Autopilot& ap) 
{
    out.nospace() << fixed << qSetRealNumberPrecision(1) << "{"
        << "\ta:" << ap.roll_params.angle * Ra
        << "\tw:" << ap.roll_params.w
        << "\tp:" << ap.roll_params.d_pilot
        << "\tdr:" << ap.delta_roll
        << "\tdr:" << ap.delta_roll_calc
        << "\tap:" << ap.getRollCoords( )
        << "\tdf:" << ap.diff_roll
        << "}, {"
        << "\ta:" << ap.pitch_params.angle * Ra
        << "\tw:" << ap.pitch_params.w
        << "\tp:" << ap.pitch_params.d_pilot
        << "\tdp:" << ap.delta_pitch
        << "\tdp:" << ap.delta_pitch_calc
        << "\tar:" << ap.getPitchCoords( )
        << "\tdf:" << ap.diff_pitch
        //<< "}, {"
        //<< "\t" << ap.yaw_params.angle
        //<< "\t" << ap.yaw_params.set_angle
        //<< "\t" << ap.yaw_params.w
        //<< "\t" << ap.yaw_params.d_pilot
        //<< "\t" << ap.delta_yaw
        //<< "\t" << ap.diff_yaw
        //<< "}, {"
        //<< "\t" << ap.height_params.height
        //<< "\t" << ap.height_params.set_height
        //<< "\t" << ap.delta_height
        << " } ";
    return out;
}
