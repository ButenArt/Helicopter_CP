#include "Helicopter_joystick.h"
#include <QDebug>
#include "Common/Printer.h"

QDebug operator<<(QDebug out, const JoystickInput& ji) {
    out.nospace() << fixed << qSetRealNumberPrecision(2) << "{ "
        << ji.X << ", "          // продольный канал
        << ji.Y << ", "          // поперечный канал
        // ШАГ-ГАЗ
        << ji.CorLEng << ", "    // коррекция левого двигателя
        << ji.CorREng << ", "    // коррекция правого двигателя
        << ji.Fosh << ", "       // общий шаг 
        << ji.Ecor << ", "       // коррекция оборотов двигателя
        // педали
        << ji.Xp << ", "         // положение педалей 
        << ji.Break << ", "      // тормоз на педалях
        //кнопки
        << ji.Gashetka << ", "    // гашетка 
        << ji.Fire << ", "     // fire
        << ji.Engine1 << ", "
        << ji.Engine2
        << "}";   // запуск двигателей TODO: пока все двигатели запускаются одновременно логика запуска не реализована
    return out;
}

Helicopter_Joystick::Helicopter_Joystick( QMainWindow* mainwindow, QSharedPointer<TLogging> log, QObject* parent ) : QObject( parent )
{
    _log = log;

    g_controller = new LCI::ControllerInput(reinterpret_cast<const HWND>(mainwindow->winId()));
    _JoystickActive = false;

    joystickTimer = new QTimer(this);
    connect(joystickTimer,SIGNAL(timeout()),this, SLOT(slotJoystick()));
    joystickTimer->start(1000); // опрос раз в секунду до запуска симуляции   
}

Helicopter_Joystick::~Helicopter_Joystick()
{
    qDebug() << __FUNCTION__;
    delete joystickTimer;
    delete g_controller;
}

void Helicopter_Joystick::Init()
{
    _JoystickActive = false;

}

void Helicopter_Joystick::Start()
{
    joystickTimer->setInterval(10);
}

void Helicopter_Joystick::Stop()
{
    joystickTimer->setInterval(1000);
}

void inline cycle_time() {
    static std::chrono::steady_clock::time_point prev_time;

    const auto start_time = std::chrono::steady_clock::now();
    const auto cycle = start_time - prev_time;
    prev_time = start_time;
    int64_t cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(cycle).count();
    qDebug() << "cycle" << "slotJoystick()" << cycle_time << "ms";
}

// TODO: функция ывзывается все время по таймеру, даже когда ФМ не запущен
// при не запущеной ФМ, вызывается раз в секунду, для контроля джойстиков
void Helicopter_Joystick::slotJoystick()
{
//    cycle_time();

//    _JoystickActive = false;
    g_controller->Update();
    static bool Trim;
    for (INT ii = 0; ii < LCI::LG_MAX_CONTROLLERS; ii++)
    {
        if (g_controller->IsConnected(ii, LCI::LG_MODEL_G940_JOYSTICK))
        {
            _JoystickActive = true;

            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput(ii);
            //static float cor_x = 153.f;
            //static float cor_y = 153.f;
            joystickInput.X = ((double)state_->lX / 256.f) + 128.f;              // X
            joystickInput.Y = ((float)state_->lY / 256.f) + 128.f;              // Y
            joystickInput.CorLEng = ((float)state_->lRz / 256.f) + 128.f;
            joystickInput.CorREng = ((float)state_->lRx / 256.f) + 128.f;
            joystickInput.Gashetka  = static_cast<bool>(state_->rgbButtons[0]);   // курок
            joystickInput.Fire   = static_cast<bool>(state_->rgbButtons[1]);   // fire
            joystickInput.APOff = static_cast<bool>(state_->rgbButtons[4]);    //S3
            joystickInput.Trim = static_cast<bool>(state_->rgbButtons[5]);    //S4
            joystickInput.Save = static_cast<bool>(state_->rgbButtons[6]);    //S5
            if (joystickInput.Trim && !Trim) { // 0->1
                cor_x = 127.f - joystickInput.X;
                cor_y = 127.f - joystickInput.Y;
            }
            joystickInput.Y += 10;  // поправка на наклон уставшего моего джойстика, который вперед наклонил
            joystickInput.Y = qBound( 0.f, joystickInput.Y, 255.f );
            //joystickInput.X -= cor_x;              // X  убираю коррекцию треммирования
            //joystickInput.Y -= cor_y;              // Y
        }
        if (g_controller->IsConnected(ii, LCI::LG_MODEL_G940_THROTTLE))
        {
            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput(ii);

            joystickInput.Fosh = (float)(state_->lX / 256.f) + 128.f;              // Шаг
            joystickInput.Ecor = (float)(state_->lZ / 256.f) + 128.f;              // Коррекция
            joystickInput.Engine1 = static_cast<bool>(state_->rgbButtons[7]); // P4 запуск двигателей
            joystickInput.Engine2 = static_cast<bool>(state_->rgbButtons[11]); // P8 запуск двигателей
            joystickInput.ADirOn   = static_cast<bool>(state_->rgbButtons[4]); // P1 автопилот по курсу
            joystickInput.APitchOn = static_cast<bool>(state_->rgbButtons[5]); // P2 автопилот по крену и тангажу
            joystickInput.AAltOn   = static_cast<bool>(state_->rgbButtons[6]); // P3 автопилот по высоте
            joystickInput.ADirOff = static_cast<bool>(state_->rgbButtons[8]); // P5 автопилот по курсу
            joystickInput.AAltOff = static_cast<bool>(state_->rgbButtons[10]); // P7 автопилот по высоте
        }
        if (g_controller->IsConnected(ii, LCI::LG_MODEL_G940_PEDALS))
        {
            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput(ii);

            joystickInput.Xp = ((float)state_->lRz / 256.f) + 128.f;              // руль направления
            joystickInput.Break = ((float)state_->lX / 256.f) + 128.f;            // тормоза
            if (joystickInput.Trim && !Trim) {
                cor_z = 127.f - joystickInput.Xp;
            }
            joystickInput.Xp -= cor_z;              // Z
        }
        if (g_controller->IsConnected(ii, LCI::LG_MODEL_X52_JOYSTICK))
        {
            _JoystickActive = true;

            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput(ii);
            joystickInput.X = ((float)state_->lX / 256.f) + 128.f;              // X
            joystickInput.Y = ((float)state_->lY / 256.f) + 128.f;              // Y
            joystickInput.CorLEng = ((float)state_->lRz / 256.f) + 128.f;
            joystickInput.CorREng = ((float)state_->lRx / 256.f) + 128.f;
            joystickInput.Gashetka = static_cast<bool>(state_->rgbButtons[0]);   // курок
            joystickInput.Fire = static_cast<bool>(state_->rgbButtons[1]);   // fire
            joystickInput.APOff = static_cast<bool>(state_->rgbButtons[4]);    //S3
            joystickInput.Trim = static_cast<bool>(state_->rgbButtons[5]);    //S3
            if (joystickInput.Trim && !Trim) { // 0->1
                cor_x = 127.f - joystickInput.X;
                cor_y = 127.f - joystickInput.Y;
            }
            joystickInput.X -= cor_x;              // X
            joystickInput.Y -= cor_y;              // Y
            joystickInput.Fosh = (float)(state_->lX / 256.f) + 128.f;              // Шаг
            joystickInput.Ecor = (float)(state_->lZ / 256.f) + 128.f;              // Коррекция
            joystickInput.Engine1 = static_cast<bool>(state_->rgbButtons[7]); // P4 запуск двигателей
            joystickInput.Engine2 = static_cast<bool>(state_->rgbButtons[11]); // P4 запуск двигателей
            joystickInput.ADirOn = static_cast<bool>(state_->rgbButtons[4]); // P1 автопилот по курсу
            joystickInput.APitchOn = static_cast<bool>(state_->rgbButtons[5]); // P2 автопилот по крену и тангажу
            joystickInput.AAltOn = static_cast<bool>(state_->rgbButtons[6]); // P3 автопилот по высоте
            joystickInput.ADirOff = static_cast<bool>(state_->rgbButtons[8]); // P5 автопилот по курсу
            joystickInput.AAltOff = static_cast<bool>(state_->rgbButtons[10]); // P7 автопилот по высоте
        }
        if (g_controller->IsConnected(ii, LCI::LG_MODEL_X52_PEDALS))
        {
            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput(ii);

            joystickInput.Xp = (float)(state_->lRz / 256.f) + 128.f;              // руль направления
            joystickInput.Break = (float)(state_->lX / 256.f) + 128.f;            // тормоза
            if (joystickInput.Trim && !Trim) {
                cor_z = 127.f - joystickInput.Xp;
            }
            joystickInput.Xp -= cor_z;              // Z
        }
        if ( g_controller->IsConnected( ii, LCI::LG_MODEL_F310_GAMEPAD ) )
        {
            _JoystickActive = true;
            // TODO это для ручного управления
            cor_x = 127.f;
            cor_y = 127.f;

            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput( ii );

            if ( g_controller->ButtonIsPressed( ii, 9 ) )  // кнопка правый стик
            {
                // если кнопка нажата, то проводим триммирование. Последнее положение джойстика запоминается
                // пока кнопка не отпущена, изменения положения джойстика не учитываются для его возвращения в нулевое положение
                // после отпускания кнопки, начинаем считывать новые положение с учетом коррекци запомненой в момент нажания
                if ( g_controller->ButtonTriggered( ii, 9 ) )  // из 0 в 1
                {
                    unblock_x = false;
                    unblock_y = false;
                    cor_x = ( (float)state_->lX / 256.f ) + cor_x;              // на сколько отличается положенеи от центра
                    cor_y = ( float( state_->lY ) / 256.f ) + cor_y;            // на сколько отличается положенеи от центра
                    PRINT_CHANGED_VAR( cor_x );
                    PRINT_CHANGED_VAR( cor_y );
                }
                joystickInput.X = cor_x;
                joystickInput.Y = cor_y;
            }
            else
            {
                // рабочий режим джойстика
                // пока стик не вернут в нейтраль просле триммирования, он не работает возвращая точку триммирования
                if ( qAbs(state_->lX) < 130 ) unblock_x = true;
                if ( qAbs(state_->lY) < 130 ) unblock_y = true;
                
                joystickInput.X = unblock_x ? ( float( state_->lX ) / 256.f ) + cor_x : cor_x;              // X левый стик
                joystickInput.Y = unblock_y ? ( float( state_->lY ) / 256.f ) + cor_y : cor_y;              // Y левый стик
            }

            joystickInput.Engine1 = g_controller->ButtonIsPressed( ii, 4 );
            joystickInput.Engine2 = g_controller->ButtonIsPressed( ii, 4 );
            joystickInput.CorLEng  = 127.f;
            joystickInput.CorREng  = 127.f;
            joystickInput.Gashetka = false;   // курок
            joystickInput.Fire  = g_controller->ButtonIsPressed( ii, 5 );    // fire R1

            auto AP = g_controller->ButtonTriggered( ii, 8 );   // кнопка левый стик
            static auto APON = false;
            if ( AP )
            { 
                joystickInput.APitchOn = APON;
                joystickInput.APOff = !APON;
                qDebug( ) << "AP_ON  =" << APON;
                APON = !APON;
            }
            auto rx = float( state_->lRx ) / 256.f;
            auto ry = float( state_->lRy ) / 256.f + 127.f;

            static float yawl = 127.f;
            yawl += g_controller->ButtonIsPressed( ii, 1 ) ? 1 : 0;
            yawl += g_controller->ButtonIsPressed( ii, 2 ) ? -1 : 0;
            yawl = qBound( 0.f, yawl, 255.f );
            joystickInput.Xp = yawl + rx;              // Педали

            static float fosh;
            fosh += g_controller->ButtonIsPressed( ii, 3 ) ? 2 : 0;
            fosh += g_controller->ButtonIsPressed( ii, 0 ) ? -2 : 0;
            fosh = qBound( 0.f, fosh, 255.f );
            joystickInput.Fosh = fosh + ry;              // Шаг

            joystickInput.Ecor = 0.f;              // Коррекция

            joystickInput.Break = fabs( ( state_->lZ ) / 127.f );

            //PRINT_CHANGED_VAR( state_->lY );
        }
        if ( g_controller->IsConnected( ii, LCI::LG_MODEL_T1600M_JOYSTICK ) )
        {
            _JoystickActive = true;

            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput( ii );
            //static float cor_x = 153.f;
            //static float cor_y = 153.f;
            if ( g_controller->ButtonIsPressed( ii, 1 ) )
            {
                if ( g_controller->ButtonTriggered( ii, 1 ) )
                {
                    unblock_x = false;
                    unblock_y = false;
                    cor_x = ( (float)state_->lX / 255.f ) + cor_x;              // на сколько отличается положенеи от центра
                    cor_y = ( float( state_->lY ) / 255.f ) + cor_y;            // на сколько отличается положенеи от центра
                    PRINT_CHANGED_VAR( cor_x );
                    PRINT_CHANGED_VAR( cor_y );
                }
                joystickInput.X = cor_x;
                joystickInput.Y = cor_y;
            }
            else
            {
                // рабочий режим джойстика
                // пока стик не вернут в нейтраль просле триммирования, он не работает возвращая точку триммирования
                if ( qAbs( state_->lX ) < 130 ) unblock_x = true;
                if ( qAbs( state_->lY ) < 130 ) unblock_y = true;

                joystickInput.X = unblock_x ? ( float( state_->lX ) / 255.f ) + cor_x : cor_x;              // X левый стик
                joystickInput.Y = unblock_y ? ( float( state_->lY ) / 255.f ) + cor_y : cor_y;              // Y левый стик
            }

            joystickInput.CorLEng = 127.f;  // нет осей для раздельного управления двигателями
            joystickInput.CorREng = 127.f;
            joystickInput.Break = ( (float)state_->rglSlider[0] / 255.f ) + 127.f; // тормоза

            //joystickInput.Gashetka = static_cast<bool>( state_->rgbButtons[0] );   // курок
            joystickInput.Fire = g_controller->ButtonIsPressed( ii, 0 );      // B1 fire
            joystickInput.APOff = g_controller->ButtonIsPressed( ii, 2 );     // Б3
            joystickInput.Trim = g_controller->ButtonIsPressed( ii, 1 );       // Б2
            //joystickInput.Save = static_cast<bool>( state_->rgbButtons[6] );   // S5
            joystickInput.Engine1 = g_controller->ButtonIsPressed( ii, 12 );  // B13 запуск двигателей
            joystickInput.Engine2 = g_controller->ButtonIsPressed( ii, 13 );  // B14 запуск двигателей
            joystickInput.ADirOn = g_controller->ButtonIsPressed( ii, 4 );    // P5 автопилот по курсу
            joystickInput.APitchOn = g_controller->ButtonIsPressed( ii, 5 );  // P6 автопилот по крену и тангажу
            joystickInput.AAltOn = g_controller->ButtonIsPressed( ii, 6 );    // P7 автопилот по высоте
            joystickInput.ADirOff = g_controller->ButtonIsPressed( ii, 9 );   // P10 автопилот по курсу
            joystickInput.AAltOff = g_controller->ButtonIsPressed( ii, 7 );   // P8 автопилот по высоте
        }
        if ( g_controller->IsConnected( ii, LCI::LG_MODEL_T1600M_THROTTLE ) )
        {
            // Get positional info
            DIJOYSTATE2* state_ = g_controller->GetStateDInput( ii );

            joystickInput.Fosh = (float)( state_->lZ / 255.f ) + 127.f;              // Шаг
            joystickInput.Ecor = (float)( state_->rglSlider[0] / 255.f ) + 127.f;              // Коррекция
            joystickInput.Xp =   (float)( state_->rglSlider[1] / 255.f ) + 127.f;              // руль направления
            //qDebug() << state_->rglSlider[1];
        }
    }
    Trim = joystickInput.Trim;
//    qDebug() << joystickInput;
}

