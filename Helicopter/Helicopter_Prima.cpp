#include "Helicopter_Prima.h"

Helicopter_Prima::Helicopter_Prima(Helicopter_core *Helicopter_)
{
    Helicopter = Helicopter_;
	PrimaTimer = new QTimer(this);
	connect(PrimaTimer, &QTimer::timeout, this, &Helicopter_Prima::PrimaSlot);

	for (int i = 1; i <= 40; ++i) {
		chanels[i] = {233647, 3, 100};			// Создаем вектор частот
		chanels_reserv[i] = { 233647, 3, 100 }; // Создаем вектор резервных частот
	}
}

Helicopter_Prima::~Helicopter_Prima()
{
}

void Helicopter_Prima::PrimaSlot() {
	if (chanel_ZU)
		chanel_ZU += 1;
}

void Helicopter_Prima::Algorithm()
{
	if (Helicopter->rsp_ep.prima.str_smen_freq && Helicopter->rsp_ep.prima.NU) {
		if (Helicopter->rsp_ep.prima.str_smen_freq != str_smen_freq_flag) {
			str_freq += 1;
			if (str_freq > 2)
				str_freq = 1;
			str_smen_freq_flag = true;
		}
	}
	if (!Helicopter->rsp_ep.prima.str_smen_freq)
		str_smen_freq_flag = false;
	if (Helicopter->rsp_ep.prima.ZU) {
		chanel_ZU += Helicopter->rsp_ep.prima.freq_nabor;
		if (chanel_ZU < 0 && !PrimaTimer->isActive())
			chanel_ZU = 40;
		if (chanel_ZU > 40 && !PrimaTimer->isActive())
			chanel_ZU = 0;
		if (Helicopter->rsp_ep.prima.str_smen_freq)
			PrimaTimer->start(1000);
		if (Helicopter->rsp_ep.prima.vvod) {
			PrimaTimer->stop();
			//chanel_freq.push_back({ 0xf, static_cast<BYTE>(chanel_ZU / 10),static_cast<BYTE>(chanel_ZU % 10),0x03,0x09,0x05,0x03,0x05,0x05,0x05,0x05,0x05 });
		}
	}
	if (Helicopter->rsp_ep.prima.NU) {
		if (Helicopter->rsp_ep.prima.vvod) {
			if (Helicopter->rsp_ep.prima.vvod != Helicopter->rsp_ep.prima.vvod_pred) {
				str_freq += 1;
				if (str_freq > 2)
					str_freq = 1;
				Helicopter->rsp_ep.prima.vvod_pred = true;
			}
		}
		if (!Helicopter->rsp_ep.prima.vvod)
			Helicopter->rsp_ep.prima.vvod_pred = false;
	}
	if (Helicopter->rsp_ep.prima.Msh) {

	}
	if (Helicopter->rsp_ep.prima.on_psh) {

	}
	if (Helicopter->rsp_ep.prima.Ust) {

	}
	/*
	bool on_psh = false;

		Helicopter->rsp_ep.prima.led_up_left = { 0x12,0x12,0x12,0x12,0x12 }; //экран с первой строкой 12 - ти разрядных
		Helicopter->rsp_ep.prima.led_down_left = { 0x12,0x12,0x12,0x12,0x12 }; //экран со второй строкой 12-ти разрядных
		Helicopter->rsp_ep.prima.led_up_right = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12 }; //экран с первой строкой 12 - ти разрядных
		Helicopter->rsp_ep.prima.led_down_right = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12 }; //экран со второй строкой 12-ти разрядных


	bool on_prima = true;
	bool PrdU = false;
	bool ZU = false;
	bool Msh = false;
	bool NU = false;
	bool Ust = false;
	bool Nabor = false;
	bool vvod = false;
	bool vvod_pred = false;
	*/
}