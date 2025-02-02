#include "Helicopter_PanelRadio.h"
#include "ui_Helicopter_PanelRadio.h"
#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Helicopter_controls.h"
#include <QDebug>


Helicopter_PanelRadio::Helicopter_PanelRadio(Helicopter_core* Helicopter, QWidget* parent)
	: QWidget(parent)
	, Helicopter(Helicopter)
	, ui(new Ui::Helicopter_PanelRadio)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle("RadioNavigation");

	UpdateTimer = new QTimer(this);
	UpdateTimer->setInterval(50);
	connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(on_ParamsUpdate()));

	//ui->FPar->display(Helicopter->Model_->GetFPar()->operator[](ui->sbFPar->value() - 1));
	//ui->lbFParamNotes->setText(FParam_notes[ui->sbFPar->value() - 1]);
	//ui->lbFParamNotes->adjustSize();
}

Helicopter_PanelRadio::~Helicopter_PanelRadio()
{
	delete ui;
}
void Helicopter_PanelRadio::closeEvent(QCloseEvent* event)
{
	event->ignore();
	UpdateTimer->stop();
	this->hide();
}

void Helicopter_PanelRadio::showEvent(QShowEvent* /*event*/)
{
	qDebug() << __FUNCTION__;
	//QWidget::showEvent(event);
	SetNavWindow();
}

void Helicopter_PanelRadio::on_t_contrButton_pressed()
{
	if (Helicopter->controls_->ManualControl()) {
		Helicopter->rsp_elp.ark_ud.t_contr = !Helicopter->rsp_elp.ark_ud.t_contr;
		ui->t_contrButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_contr);
	}
}

void Helicopter_PanelRadio::SetNavWindow()
{
	//APK-15
	if (Helicopter->rsp_elp.ark_9.t_rk_chanel)
		ui->chanel12Slider->setValue(1);
	else
		ui->chanel12Slider->setValue(2);
	ui->freqAktobeCB->setChecked(Helicopter->rsp_elp.ark_9.freqAkt_left || Helicopter->rsp_elp.ark_9.freqAkt_right);
	ui->freqHlebCB->setChecked(Helicopter->rsp_elp.ark_9.freqHleb_left || Helicopter->rsp_elp.ark_9.freqHleb_right);
	if(Helicopter->rsp_elp.ark_9.t_rk_tlf)
		ui->tlg_tlfSlider->setValue(1);
	else if(Helicopter->rsp_elp.ark_9.t_rk_tlg)
		ui->tlg_tlfSlider->setValue(-1);
	else
		ui->tlg_tlfSlider->setValue(0);
	if(Helicopter->rsp_elp.ark_9.t_rk_off)
		ui->kom_ant_ramSlider->setValue(0);
	else if(Helicopter->rsp_elp.ark_9.t_rk_com)
		ui->kom_ant_ramSlider->setValue(1);
	else if(Helicopter->rsp_elp.ark_9.t_rk_ant)
		ui->kom_ant_ramSlider->setValue(2);
	else if(Helicopter->rsp_elp.ark_9.t_rk_ram)
		ui->kom_ant_ramSlider->setValue(3);
	//ui->t_rk_ramButton->setCheckable(Helicopter->rsp_elp.ark_9.t_rk_ramka);
	//ui->t_rk_ctrlButton->setCheckable(Helicopter->rsp_elp.ark_9.t_rk_ctrl);
	//АРК-УД
	SetState(Helicopter->rsp_elp.ark_ud.i_rkud_3, ui->I_lamp, "background:red", "background:white");
	SetState(Helicopter->rsp_elp.ark_ud.i_rkud_2, ui->SHP_lamp, "background:red", "background:white");
	SetState(Helicopter->rsp_elp.ark_ud.i_rkud_1, ui->UP_lamp, "background:red", "background:white");
	//ui->t_lButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_ant_l);
	//ui->t_rButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_ant_r);
	//ui->t_contrButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_contr);
	if (Helicopter->rsp_elp.ark_ud.t_rkud_off)
		ui->t_rkudSlider->setValue(0);
	else if (Helicopter->rsp_elp.ark_ud.t_rkud_up)
		ui->t_rkudSlider->setValue(1);
	else if (Helicopter->rsp_elp.ark_ud.t_rkud_shp)
		ui->t_rkudSlider->setValue(2);
	else if (Helicopter->rsp_elp.ark_ud.t_rkud_i)
		ui->t_rkudSlider->setValue(3);
	else if (Helicopter->rsp_elp.ark_ud.t_rkud_rpk)
		ui->t_rkudSlider->setValue(4);
	
	if (Helicopter->rsp_elp.ark_ud.t_rkud_sens)
		ui->t_rkud_sensSlider->setValue(1);
	else
		ui->t_rkud_sensSlider->setValue(0);
	if (Helicopter->rsp_elp.ark_ud.t_rkud_ukv)
		ui->t_rkud_ukvSlider->setValue(1);
	else 
		ui->t_rkud_ukvSlider->setValue(0);
	if(Helicopter->rsp_elp.ark_ud.t_rkud_1)
		ui->t_chanelSlider->setValue(1);
	else if(Helicopter->rsp_elp.ark_ud.t_rkud_2)
		ui->t_chanelSlider->setValue(2);
	else if(Helicopter->rsp_elp.ark_ud.t_rkud_3)
		ui->t_chanelSlider->setValue(3);
	else if(Helicopter->rsp_elp.ark_ud.t_rkud_4)
		ui->t_chanelSlider->setValue(4);
	else if(Helicopter->rsp_elp.ark_ud.t_rkud_5)
		ui->t_chanelSlider->setValue(5);
	else if(Helicopter->rsp_elp.ark_ud.t_rkud_6)
		ui->t_chanelSlider->setValue(6);
	//ГМК-1ГЭ
	SetState(Helicopter->rsp_elp.i_dubl, ui->eservLamp, "background:red", "background:white");
	SetState(Helicopter->rsp_elp.i_osnv, ui->mainLamp, "background:red", "background:white");
	if (Helicopter->rsp_elp.gmk.t_contr300)
		ui->contr0_300Slider->setValue(1);
	else if (Helicopter->rsp_elp.gmk.t_contr0)
		ui->contr0_300Slider->setValue(-1);
	else
		ui->contr0_300Slider->setValue(0);
	//ui->longitude->setValue(50);
	ui->longLCD->display(ui->longitude->value());
	if(!Helicopter->rsp_elp.t_mk)
		ui->mk_gpkSlider->setValue(-1);
	else
		ui->mk_gpkSlider->setValue(1);
	if (Helicopter->rsp_elp.t_dubl_osnv)
		ui->reserv_mainSlider->setValue(1);
	else
		ui->reserv_mainSlider->setValue(-1);
	if (Helicopter->rsp_elp.t_N)
		ui->sev_ugSlider->setValue(-1);
	else if (Helicopter->rsp_elp.t_S)
		ui->sev_ugSlider->setValue(1);
	else 
		ui->sev_ugSlider->setValue(0);
	ui->t_gmkSlider->setValue(Helicopter->rsh_elp.t_gmk);
	if(Helicopter->rsp_elp.t_zk_l)
		ui->zkSlider->setValue(-1);
	else if(Helicopter->rsp_elp.t_zk_r)
		ui->zkSlider->setValue(1);
	else ui->zkSlider->setValue(0);

	//Пульт управления Радио
	ui->galSPU->setValue(6);
	ui->SPU_RADIO->setValue(1);

	//Приборная панель пилота
	ui->kurBigLCD->display(Helicopter->lpb.ugr_4uk.dir());
	ui->kurSmalLCD->display(Helicopter->lpb.ugr_4uk.ark());

	//UV-26
	if (Helicopter->UV26.t_left)
		ui->uv26Bort->setValue(0);
	if (Helicopter->UV26.t_right)
		ui->uv26Bort->setValue(1);
	//ui->uv26L45->setCheckable(Helicopter->UV26.t_seria);
	//ui->uv26L68->setCheckable(Helicopter->UV26.t_interval);
	//ui->uv26Prog->setValue(Helicopter->UV26.t_prog);
	//ui->uv26L8->setCheckable(Helicopter->UV26.t_zalp);
	//ui->uv26Pusk->setCheckable(Helicopter->UV26.t_pusk);
	//ui->uv26SBROS->setCheckable(Helicopter->UV26.t_reset);
	//ui->uv26Stop->setCheckable(Helicopter->UV26.t_stop);
	ui->uv26LCD->display(static_cast<int>(Helicopter->UV26.tablo));

}

void Helicopter_PanelRadio::SetState(bool flag, QWidget* widget, const QString style_true, const QString style_false)
{
	flag ? widget->setStyleSheet(style_true) : widget->setStyleSheet(style_false);
}

void Helicopter_PanelRadio::on_ParamsUpdate()
{
	// передаем в модель все кнопки из окна если ручной режим еили нет Мохи
	if (Helicopter->controls_->ManualControl() || (Helicopter->can->BoardCount() == 0))
	{
		//APK-15
		if (ui->chanel12Slider->value() == 1)
			Helicopter->rsp_elp.ark_9.t_rk_chanel = false;
		else if (ui->chanel12Slider->value() == 2)
			Helicopter->rsp_elp.ark_9.t_rk_chanel = true;
		Helicopter->rsp_elp.ark_9.freqAkt_left = ui->freqAktobeCB->isChecked();
		Helicopter->rsp_elp.ark_9.freqAkt_right = ui->freqAktobeCB->isChecked();
		Helicopter->rsp_elp.ark_9.freqHleb_left = ui->freqHlebCB->isChecked();
		Helicopter->rsp_elp.ark_9.freqHleb_right = ui->freqHlebCB->isChecked();
		if (ui->tlg_tlfSlider->value() == 1) {
			Helicopter->rsp_elp.ark_9.t_rk_tlf = true;
			Helicopter->rsp_elp.ark_9.t_rk_tlg = false;
		}
		else if (ui->tlg_tlfSlider->value() == -1) {
			Helicopter->rsp_elp.ark_9.t_rk_tlf = false;
			Helicopter->rsp_elp.ark_9.t_rk_tlg = true;
		}
		else {
			Helicopter->rsp_elp.ark_9.t_rk_tlf = false;
			Helicopter->rsp_elp.ark_9.t_rk_tlg = false;
		}
		if (ui->kom_ant_ramSlider->value() == 0) {
			Helicopter->rsp_elp.ark_9.t_rk_off = true;
			Helicopter->rsp_elp.ark_9.t_rk_com = false;
			Helicopter->rsp_elp.ark_9.t_rk_ant = false;
			Helicopter->rsp_elp.ark_9.t_rk_ram = false;
		}
		else if (ui->kom_ant_ramSlider->value() == 1) {
			Helicopter->rsp_elp.ark_9.t_rk_off = false;
			Helicopter->rsp_elp.ark_9.t_rk_com = true;
			Helicopter->rsp_elp.ark_9.t_rk_ant = false;
			Helicopter->rsp_elp.ark_9.t_rk_ram = false;
		}
		else if (ui->kom_ant_ramSlider->value() == 2) {
			Helicopter->rsp_elp.ark_9.t_rk_off = false;
			Helicopter->rsp_elp.ark_9.t_rk_com = false;
			Helicopter->rsp_elp.ark_9.t_rk_ant = true;
			Helicopter->rsp_elp.ark_9.t_rk_ram = false;
		}
		else if (ui->kom_ant_ramSlider->value() == 3) {
			Helicopter->rsp_elp.ark_9.t_rk_off = false;
			Helicopter->rsp_elp.ark_9.t_rk_com = false;
			Helicopter->rsp_elp.ark_9.t_rk_ant = false;
			Helicopter->rsp_elp.ark_9.t_rk_ram = true;
		}
		Helicopter->rsp_elp.ark_9.t_rk_ramka = ui->t_rk_ramButton->isChecked();
		Helicopter->rsp_elp.ark_9.t_rk_ctrl = ui->t_rk_ctrlButton->isChecked();
		//АРК-УД
		SetState(Helicopter->rsp_elp.ark_ud.i_rkud_3, ui->I_lamp, "background:red", "background:white");
		SetState(Helicopter->rsp_elp.ark_ud.i_rkud_2, ui->SHP_lamp, "background:red", "background:white");
		SetState(Helicopter->rsp_elp.ark_ud.i_rkud_1, ui->UP_lamp, "background:red", "background:white");
		Helicopter->rsp_elp.ark_ud.t_ant_l = ui->t_lButton->isChecked();
		Helicopter->rsp_elp.ark_ud.t_ant_r = ui->t_rButton->isChecked();
		Helicopter->rsp_elp.ark_ud.t_contr = ui->t_contrButton->isChecked();
		if (ui->t_rkudSlider->value() == 0) {
			Helicopter->rsp_elp.ark_ud.t_rkud_off = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_up = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_shp = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_i = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_rpk = false;
		}
		else if (ui->t_rkudSlider->value() == 1) {
			Helicopter->rsp_elp.ark_ud.t_rkud_off = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_up = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_shp = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_i = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_rpk = false;
		}
		else if (ui->t_rkudSlider->value() == 2) {
			Helicopter->rsp_elp.ark_ud.t_rkud_off = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_up = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_shp = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_i = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_rpk = false;
		}
		else if (ui->t_rkudSlider->value() == 3) {
			Helicopter->rsp_elp.ark_ud.t_rkud_off = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_up = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_shp = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_i = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_rpk = false;
		}
		else if (ui->t_rkudSlider->value() == 4) {
			Helicopter->rsp_elp.ark_ud.t_rkud_off = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_up = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_shp = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_i = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_rpk = true;
		}
		if (ui->t_rkud_sensSlider->value() == 1)
			Helicopter->rsp_elp.ark_ud.t_rkud_sens = true;
		else if (ui->t_rkud_sensSlider->value() == 0)
			Helicopter->rsp_elp.ark_ud.t_rkud_sens = false;
		if (ui->t_rkud_ukvSlider->value() == 1)
			Helicopter->rsp_elp.ark_ud.t_rkud_ukv = true;
		else if (ui->t_rkud_ukvSlider->value() == 0)
			Helicopter->rsp_elp.ark_ud.t_rkud_ukv = false;

		if (ui->t_chanelSlider->value() == 1) {
			Helicopter->rsp_elp.ark_ud.t_rkud_1 = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_2 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_3 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_4 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_5 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_6 = false;
		}
		else if (ui->t_chanelSlider->value() == 2) {
			Helicopter->rsp_elp.ark_ud.t_rkud_1 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_2 = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_3 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_4 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_5 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_6 = false;
		}
		else if (ui->t_chanelSlider->value() == 3) {
			Helicopter->rsp_elp.ark_ud.t_rkud_1 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_2 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_3 = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_4 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_5 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_6 = false;
		}
		else if (ui->t_chanelSlider->value() == 4) {
			Helicopter->rsp_elp.ark_ud.t_rkud_1 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_2 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_3 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_4 = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_5 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_6 = false;
		}
		else if (ui->t_chanelSlider->value() == 5) {
			Helicopter->rsp_elp.ark_ud.t_rkud_1 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_2 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_3 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_4 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_5 = true;
			Helicopter->rsp_elp.ark_ud.t_rkud_6 = false;
		}
		else if (ui->t_chanelSlider->value() == 6) {
			Helicopter->rsp_elp.ark_ud.t_rkud_1 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_2 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_3 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_4 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_5 = false;
			Helicopter->rsp_elp.ark_ud.t_rkud_6 = true;
		}
		//ГМК-1ГЭ
		SetState(Helicopter->rsp_elp.i_dubl, ui->eservLamp, "background:red", "background:white");
		SetState(Helicopter->rsp_elp.i_osnv, ui->mainLamp, "background:red", "background:white");
		if (ui->contr0_300Slider->value() == 1) {
			Helicopter->rsp_elp.gmk.t_contr300 = true;
			Helicopter->rsp_elp.gmk.t_contr0 = false;
		}
		else if (ui->contr0_300Slider->value() == -1) {
			Helicopter->rsp_elp.gmk.t_contr300 = false;
			Helicopter->rsp_elp.gmk.t_contr0 = true;
		}
		else if (ui->contr0_300Slider->value() == 0) {
			Helicopter->rsp_elp.gmk.t_contr300 = false;
			Helicopter->rsp_elp.gmk.t_contr0 = false;
		}
		//ui->longitude->setValue(50);
		ui->longLCD->display(ui->longitude->value());
		if (ui->mk_gpkSlider->value() == 1) {
			Helicopter->rsp_elp.t_mk = true;
		}
		else if (ui->mk_gpkSlider->value() == -1) {
			Helicopter->rsp_elp.t_mk = false;
		}
		else if (ui->mk_gpkSlider->value() == 0) {
			Helicopter->rsp_elp.t_mk = false;
		}
		if (ui->reserv_mainSlider->value() == 1) {
			Helicopter->rsp_elp.t_dubl_osnv = true;
		}
		else if (ui->reserv_mainSlider->value() == -1) {
			Helicopter->rsp_elp.t_dubl_osnv = false;
		}
		else if (ui->reserv_mainSlider->value() == 0) {
			Helicopter->rsp_elp.t_dubl_osnv = false;
		}

		if (ui->sev_ugSlider->value() == 1) {
			Helicopter->rsp_elp.t_S = true;
			Helicopter->rsp_elp.t_N = false;
		}
		else if (ui->sev_ugSlider->value() == -1) {
			Helicopter->rsp_elp.t_S = false;
			Helicopter->rsp_elp.t_N = true;
		}
		else if (ui->sev_ugSlider->value() == 0) {
			Helicopter->rsp_elp.t_S = true;
			Helicopter->rsp_elp.t_N = false;
		}
		if (ui->t_gmkSlider->value() == 1)
			Helicopter->rsh_elp.t_gmk = true;
		else if (ui->t_gmkSlider->value() == 0)
			Helicopter->rsh_elp.t_gmk = false;
		if (ui->zkSlider->value() == 1) {
			Helicopter->rsp_elp.t_zk_r = true;
			Helicopter->rsp_elp.t_zk_l = false;
		}
		else if (ui->zkSlider->value() == -1) {
			Helicopter->rsp_elp.t_zk_r = false;
			Helicopter->rsp_elp.t_zk_l = true;
		}
		else if (ui->zkSlider->value() == 0) {
			Helicopter->rsp_elp.t_zk_r = false;
			Helicopter->rsp_elp.t_zk_l = false;
		}

		//Пульт управления Радио
		ui->galSPU->setValue(6);
		ui->SPU_RADIO->setValue(1);

		//Приборная панель пилота
		ui->kurBigLCD->display(Helicopter->lpb.ugr_4uk.dir());
		ui->kurSmalLCD->display(Helicopter->lpb.ugr_4uk.ark());

		//UV-26
		if (ui->uv26Bort->value() == 0) {
			Helicopter->UV26.t_left = true;
			Helicopter->UV26.t_right = false;
		}
		else if (ui->uv26Bort->value() == 1) {
			Helicopter->UV26.t_left = false;
			Helicopter->UV26.t_right = true;
		}
		Helicopter->UV26.t_seria = ui->uv26L45->isChecked();
		Helicopter->UV26.t_interval = ui->uv26L68->isChecked();
		if (ui->uv26Prog->value() == 0)
			Helicopter->UV26.t_prog = false;
		if (ui->uv26Prog->value() == 1)
			Helicopter->UV26.t_prog = true;
		Helicopter->UV26.t_zalp = ui->uv26L8->isChecked();
		Helicopter->UV26.t_pusk = ui->uv26Pusk->isChecked();
		Helicopter->UV26.t_reset = ui->uv26SBROS->isChecked();
		Helicopter->UV26.t_stop = ui->uv26Stop->isChecked();
		ui->uv26LCD->display(static_cast<int>(Helicopter->UV26.tablo));
	}
	// режим работы с тренажером, индикация положения тумблеров из кабины
	if (!Helicopter->controls_->ManualControl())
	{
		//APK-15
		if (Helicopter->rsp_elp.ark_9.t_rk_chanel)
			ui->chanel12Slider->setValue(1);
		else
			ui->chanel12Slider->setValue(2);
		ui->freqAktobeCB->setChecked(Helicopter->rsp_elp.ark_9.freqAkt_left || Helicopter->rsp_elp.ark_9.freqAkt_right);
		ui->freqHlebCB->setChecked(Helicopter->rsp_elp.ark_9.freqHleb_left || Helicopter->rsp_elp.ark_9.freqHleb_right);
		if (Helicopter->rsp_elp.ark_9.t_rk_tlf)
			ui->tlg_tlfSlider->setValue(1);
		else if (Helicopter->rsp_elp.ark_9.t_rk_tlg)
			ui->tlg_tlfSlider->setValue(-1);
		else
			ui->tlg_tlfSlider->setValue(0);
		if (Helicopter->rsp_elp.ark_9.t_rk_off)
			ui->kom_ant_ramSlider->setValue(0);
		else if (Helicopter->rsp_elp.ark_9.t_rk_com)
			ui->kom_ant_ramSlider->setValue(1);
		else if (Helicopter->rsp_elp.ark_9.t_rk_ant)
			ui->kom_ant_ramSlider->setValue(2);
		else if (Helicopter->rsp_elp.ark_9.t_rk_ram)
			ui->kom_ant_ramSlider->setValue(3);
		ui->t_rk_ramButton->setCheckable(Helicopter->rsp_elp.ark_9.t_rk_ramka);
		ui->t_rk_ctrlButton->setCheckable(Helicopter->rsp_elp.ark_9.t_rk_ctrl);
		//АРК-УД
		SetState(Helicopter->rsp_elp.ark_ud.i_rkud_3, ui->I_lamp, "background:red", "background:white");
		SetState(Helicopter->rsp_elp.ark_ud.i_rkud_2, ui->SHP_lamp, "background:red", "background:white");
		SetState(Helicopter->rsp_elp.ark_ud.i_rkud_1, ui->UP_lamp, "background:red", "background:white");
		ui->t_lButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_ant_l);
		ui->t_rButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_ant_r);
		ui->t_contrButton->setCheckable(Helicopter->rsp_elp.ark_ud.t_contr);
		if (Helicopter->rsp_elp.ark_ud.t_rkud_off)
			ui->t_rkudSlider->setValue(0);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_up)
			ui->t_rkudSlider->setValue(1);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_shp)
			ui->t_rkudSlider->setValue(2);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_i)
			ui->t_rkudSlider->setValue(3);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_rpk)
			ui->t_rkudSlider->setValue(4);

		if (Helicopter->rsp_elp.ark_ud.t_rkud_sens)
			ui->t_rkud_sensSlider->setValue(1);
		else
			ui->t_rkud_sensSlider->setValue(0);
		if (Helicopter->rsp_elp.ark_ud.t_rkud_ukv)
			ui->t_rkud_ukvSlider->setValue(1);
		else
			ui->t_rkud_ukvSlider->setValue(0);
		if (Helicopter->rsp_elp.ark_ud.t_rkud_1)
			ui->t_chanelSlider->setValue(1);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_2)
			ui->t_chanelSlider->setValue(2);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_3)
			ui->t_chanelSlider->setValue(3);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_4)
			ui->t_chanelSlider->setValue(4);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_5)
			ui->t_chanelSlider->setValue(5);
		else if (Helicopter->rsp_elp.ark_ud.t_rkud_6)
			ui->t_chanelSlider->setValue(6);
		//ГМК-1ГЭ
		SetState(Helicopter->rsp_elp.i_dubl, ui->eservLamp, "background:red", "background:white");
		SetState(Helicopter->rsp_elp.i_osnv, ui->mainLamp, "background:red", "background:white");
		if (Helicopter->rsp_elp.gmk.t_contr300)
			ui->contr0_300Slider->setValue(1);
		else if (Helicopter->rsp_elp.gmk.t_contr0)
			ui->contr0_300Slider->setValue(-1);
		else
			ui->contr0_300Slider->setValue(0);
		ui->longitude->setValue(50);
		ui->longLCD->display(ui->longitude->value());
		if (!Helicopter->rsp_elp.t_mk)
			ui->mk_gpkSlider->setValue(-1);
		else
			ui->mk_gpkSlider->setValue(1);
		if (Helicopter->rsp_elp.t_dubl_osnv)
			ui->reserv_mainSlider->setValue(1);
		else
			ui->reserv_mainSlider->setValue(-1);
		if (Helicopter->rsp_elp.t_N)
			ui->sev_ugSlider->setValue(-1);
		else if (Helicopter->rsp_elp.t_S)
			ui->sev_ugSlider->setValue(1);
		else
			ui->sev_ugSlider->setValue(0);
		ui->t_gmkSlider->setValue(Helicopter->rsh_elp.t_gmk);
		if (Helicopter->rsp_elp.t_zk_l)
			ui->zkSlider->setValue(-1);
		else if (Helicopter->rsp_elp.t_zk_r)
			ui->zkSlider->setValue(1);
		else ui->zkSlider->setValue(0);

		//Пульт управления Радио
		ui->galSPU->setValue(6);
		ui->SPU_RADIO->setValue(1);

		//Приборная панель пилота
		ui->kurBigLCD->display(Helicopter->lpb.ugr_4uk.dir());
		ui->kurSmalLCD->display(Helicopter->lpb.ugr_4uk.ark());

		//UV-26
		if (Helicopter->UV26.t_left)
			ui->uv26Bort->setValue(0);
		if (Helicopter->UV26.t_right)
			ui->uv26Bort->setValue(1);
		ui->uv26L45->setCheckable(Helicopter->UV26.t_seria);
		ui->uv26L68->setCheckable(Helicopter->UV26.t_interval);
		ui->uv26Prog->setValue(Helicopter->UV26.t_prog);
		ui->uv26L8->setCheckable(Helicopter->UV26.t_zalp);
		ui->uv26Pusk->setCheckable(Helicopter->UV26.t_pusk);
		ui->uv26SBROS->setCheckable(Helicopter->UV26.t_reset);
		ui->uv26Stop->setCheckable(Helicopter->UV26.t_stop);
		ui->uv26LCD->display(static_cast<int>(Helicopter->UV26.tablo));
	}
}
template<typename... Args>
void Helicopter_PanelRadio::SetSlider(QSlider* slider, Args... args)
{
	bool params[] = { args... };    auto index = 0;
	for (int i = 0; i < sizeof...(args); i++)
	{
		if (params[i])
		{
			slider->setValue(i);
			break;
		}
	}
}

template<typename... Args>
void Helicopter_PanelRadio::CheckSlider(QSlider* slider, Args... args)
{
	bool params[] = { args... };
	auto index = slider->value();
	if (index < sizeof...(args))
	{
		params[index] = true;
	}
}