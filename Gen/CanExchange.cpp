#include "../Helicopter/Helicopter_controls.h"
#include "../Helicopter/Helicopter_BoardSystem.h"
void Helicopter_controls::CanExchange()
{
// class.property unsupported type // commentarry
// 1 ���. unsupported  // 
	Helicopter->rp_azs.t_ta_start = GetGPIOINBitValue( 0 );	//������������ ������
	Helicopter->rp_azs.t_ta_ign = GetGPIOINBitValue( 1 );	//������������ ���������
	Helicopter->rp_azs.t_eng_start = GetGPIOINBitValue( 2 );	//��������� ������
	Helicopter->rp_azs.t_eng_ign = GetGPIOINBitValue( 3 );	//��������� ���������
	Helicopter->rp_azs.t_pump_consum = GetGPIOINBitValue( 4 );	//��������� ������� - ������ ������� ����� - ������
	Helicopter->rp_azs.t_pump_left = GetGPIOINBitValue( 5 );	//��������� ������� - ������ ������� ����� - ������
	Helicopter->rp_azs.t_pump_right = GetGPIOINBitValue( 6 );	//��������� ������� - ������ ������� ����� - �������
	Helicopter->rp_azs.t_fuel_gauge = GetGPIOINBitValue( 7 );	//��������� ������� - ����������
	Helicopter->rp_azs.t_spuu_52 = GetGPIOINBitValue( 8 );	//���� - 52
	Helicopter->rp_azs.t_auto_main = GetGPIOINBitValue( 9 );	//��������� - ������
	Helicopter->rp_azs.t_auto_fric = GetGPIOINBitValue( 10 );	//��������� - �����
	Helicopter->rp_azs.t_auto_emufta = GetGPIOINBitValue( 11 );	//��������� - ������������
	Helicopter->rp_azs.t_hidro_main = GetGPIOINBitValue( 12 );	//��������� - ������
	Helicopter->rp_azs.t_hidro_sec = GetGPIOINBitValue( 13 );	//��������� - ������
	Helicopter->rp_azs.t_sas = GetGPIOINBitValue( 14 );	//���
	Helicopter->rp_azs.t_nav1 = GetGPIOINBitValue( 15 );	//���1
	Helicopter->rp_azs.t_nav2 = GetGPIOINBitValue( 16 );	//���2
	Helicopter->rp_azs.t_distmeter = GetGPIOINBitValue( 17 );	//���������
	Helicopter->rp_azs.t_n_ctrl = GetGPIOINBitValue( 18 );	//���������� ���������
	Helicopter->rp_azs.t_close_left = GetGPIOINBitValue( 19 );	//������������� ����� - �����
	Helicopter->rp_azs.t_close_rght = GetGPIOINBitValue( 20 );	//������������� ����� - ������
	Helicopter->rp_azs.t_close_bypass = GetGPIOINBitValue( 21 );	//���� �������
	Helicopter->rp_azs.t_ppo_signal = GetGPIOINBitValue( 22 );	//������� ����������
	Helicopter->rp_azs.t_ppo_1_auto = GetGPIOINBitValue( 23 );	//������� I ������� �����
	Helicopter->rp_azs.t_ppo_1_hand = GetGPIOINBitValue( 24 );	//������� I ������� ����
	Helicopter->rp_azs.t_ppo_2_auto = GetGPIOINBitValue( 25 );	//������� 2 ������� �����(������-������ �����������������)
	Helicopter->rp_azs.t_ppo_2_hand = GetGPIOINBitValue( 26 );	//������� 2 ������� ������
	Helicopter->rp_azs.t_r_spu = GetGPIOINBitValue( 27 );	//����� - ���
	Helicopter->rp_azs.t_r_altimetr = GetGPIOINBitValue( 28 );	//����� - ���������
	Helicopter->rp_azs.t_r_comand_osnv = GetGPIOINBitValue( 29 );	//����� - ������ �����
	Helicopter->rp_azs.t_pust_01 = GetGPIOINBitValue( 30 );	//������� - ������
	Helicopter->rp_azs.t_r_compas_sv = GetGPIOINBitValue( 31 );	//����� - ������ ��
	Helicopter->rp_azs.t_r_compas_ukv = GetGPIOINBitValue( 32 );	//����� - ������ ���
	Helicopter->rp_azs.t_resp_uvd1 = GetGPIOINBitValue( 33 );	//����� - �������� ��� �1
	Helicopter->rp_azs.t_resp_uvd2 = GetGPIOINBitValue( 34 );	//����� - �������� ��� �2
	Helicopter->rp_azs.t_r_rls = GetGPIOINBitValue( 35 );	//����� - ���
	Helicopter->rp_azs.t_far_left_ctrl = GetGPIOINBitValue( 36 );	//���� - ����� - ������
	Helicopter->rp_azs.t_far_left_flash = GetGPIOINBitValue( 37 );	//���� - ����� - ����
	Helicopter->rp_azs.t_far_right_ctrl = GetGPIOINBitValue( 38 );	//���� - ������ - ������
	Helicopter->rp_azs.t_far_right_flash = GetGPIOINBitValue( 39 );	//���� - ������ - ����
	Helicopter->rp_azs.t_siren_missle = GetGPIOINBitValue( 40 );	//������ ������
	Helicopter->rp_azs.t_strov_light = GetGPIOINBitValue( 41 );	//������ ����
	Helicopter->rp_azs.t_ano = GetGPIOINBitValue( 42 );	//���
	Helicopter->rp_azs.t_tact_sbros = GetGPIOINBitValue( 43 );	//���� ����� �����
	Helicopter->rp_azs.t_avar_sbros = GetGPIOINBitValue( 44 );	//���� ����� �����
	Helicopter->rp_azs.t_ais_ctrl = GetGPIOINBitValue( 45 );	//���������������������� ������� - ����������
	Helicopter->rp_azs.t_ais_pzu_left = GetGPIOINBitValue( 46 );	//���������������������� ������� - ��� ������ ����
	Helicopter->rp_azs.t_ais_pzu_rght = GetGPIOINBitValue( 47 );	//���������������������� ������� - ��� ������� ����
	Helicopter->rp_azs.t_ais_co_121 = GetGPIOINBitValue( 48 );	//���������������������� ������� - �� - 121
	Helicopter->rp_azs.t_ais_glass = GetGPIOINBitValue( 49 );	//���������������������� ������� - ������
	Helicopter->rp_azs.t_ais_wipe_left = GetGPIOINBitValue( 50 );	//���������������� - �����
	Helicopter->rp_azs.t_ais_wipe_rght = GetGPIOINBitValue( 51 );	//���������������� - ������
	Helicopter->rp_azs.t_p_nv = GetGPIOINBitValue( 52 );	//� ��
	Helicopter->rp_azs.t_ko_50 = GetGPIOINBitValue( 53 );	//�� - 50
// 2 ����� ������� ������ �������������. unsupported  // 
// 2.1 ��������. unsupported  // 
	Helicopter->lsp_elp.t_IV_500 = GetGPIOINBitValue( 54 );	//������ ��-500�
	Helicopter->lsp_elp.t_ground = GetGPIOINBitValue( 55 );	//������ �����
	Helicopter->lsp_elp.t_air = GetGPIOINBitValue( 56 );	//������ ������
// Helicopter->lsp_elp.t_rt_left unsupported  // �� ��������� ������(��� �� ������ � ���������)
// Helicopter->lsp_elp.t_rt_rght unsupported  // �� ��������� �������(��� �� ������ � ���������)
	Helicopter->lsp_elp.t_bur = GetGPIOINBitValue( 57 );	//���� ���(invers t_tbn)
	Helicopter->lsp_elp.t_tbn = GetGPIOINBitValue( 58 );	//���
// 2.2 ����� ���������� �������� ��-662. unsupported  // 
	Helicopter->lsp_elp.t_tumblr_1 = GetGPIOINBitValue( 59 );	//������� 1
	Helicopter->lsp_elp.t_button_1_1 = GetGPIOINBitValue( 60 );	//������ 1
	Helicopter->lsp_elp.t_button_1_2 = GetGPIOINBitValue( 61 );	//������ 2(�� ������� �����)
	Helicopter->lsp_elp.t_button_1_3 = GetGPIOINBitValue( 62 );	//������ 3
	Helicopter->lsp_elp.t_button_1_4 = GetGPIOINBitValue( 63 );	//������ 4
	Helicopter->lsp_elp.t_tumblr_2 = GetGPIOINBitValue( 64 );	//������� 2
	Helicopter->lsp_elp.t_button_2_1 = GetGPIOINBitValue( 65 );	//������ 1
	Helicopter->lsp_elp.t_button_2_2 = GetGPIOINBitValue( 66 );	//������ 2
	Helicopter->lsp_elp.t_button_2_3 = GetGPIOINBitValue( 67 );	//������ 3
	Helicopter->lsp_elp.t_button_2_4 = GetGPIOINBitValue( 68 );	//������ 4
// 2.3 ����� �������������� ���������� ����������� ��-65�. unsupported  // 
	Helicopter->lsp_elp.t_ri_off = GetGPIOINBitValue( 69 );	//������ ����
	Helicopter->lsp_elp.t_ri_check = GetGPIOINBitValue( 70 );	//������ ��������
	Helicopter->lsp_elp.t_ri_rep = GetGPIOINBitValue( 71 );	//������ ����
// Helicopter->lsp_elp.t_ri_gain unsupported  // ������� ����(����������� � ���������)
	//SetGPIOOUTBitValue( Helicopter->lsp_elp.i_hv_tral || Helicopter->rsp_ep.t_check_lamp, 0 );	//����� �� ���� ������
	SetGPIOOUTBitValue(Helicopter->lsp_elp.i_ogr_nTr_Left || Helicopter->rsp_ep.t_check_lamp, 0 );// ����� ���nTr LEFT
	SetGPIOOUTBitValue(Helicopter->lsp_elp.i_ogr_nTr_Right || Helicopter->rsp_ep.t_check_lamp, 194);// ����� ���nTr RIGHT
	SetVIDSteps( 0, Helicopter->lsp_elp.f_p_air );	//�������� ���-10�(0-255,100-344)�� ����� ����� ������, ����� 1004(3)
	SetVIDSteps( 1, Helicopter->lsp_elp.f_p_brake );	//�������� ��-60�(0-355,60-3521)�� ����� ����� �����, ����� 1004(2)
	Helicopter->lsp_elp.t_ano = GetGPIOINBitValue( 72 );	//������ ���-���
	Helicopter->lsp_elp.t_siren = GetGPIOINBitValue( 73 );	//������ ������
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_siren_on || Helicopter->rsp_ep.t_check_lamp, 1 );	//��������� ������ ��������
	Helicopter->lsp_elp.t_ext_lock = GetGPIOINBitValue( 74 );	//������� ������� ��������
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_cargo_dump || Helicopter->rsp_ep.t_check_lamp, 2 );	//��������� ���� �������
	Helicopter->lsp_elp.t_sublight_1 = GetGPIOINBitValue( 75 );	//������� ������� ������ 1
	Helicopter->lsp_elp.t_sublight_2 = GetGPIOINBitValue( 76 );	//������� ������� ������ 2
// Helicopter->lsp_elp.t_fan unsupported  // ����������(������� �� ������)
	Helicopter->lsp_elp.t_cargo_dump = GetGPIOINBitValue( 77 );	//������� ��������� ����� ����� ������� / ������� ��������
// 2.4 ����� ���������� ������������ �503�. unsupported  // 
	Helicopter->lsp_elp.t_recorder = GetGPIOINBitValue( 78 );	//������� ��� / ����
	Helicopter->lsp_elp.t_autostart = GetGPIOINBitValue( 79 );	//������� ��������� / ����������� ������
	Helicopter->lsp_elp.t_spu_lar = GetGPIOINBitValue( 80 );	//������� ��� / ���
// Helicopter->lsp_elp.f_sublight unsupported  // �������
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_record || Helicopter->rsp_ep.t_check_lamp || Helicopter->lsp_elp.t_recorder, 3 );	//��������� ������ up
// 2.5 ��������� ��������. unsupported  // 
	Helicopter->lsp_elp.t_disasters = GetGPIOINBitValue( 81 );	//������� ������
	Helicopter->lsp_elp.t_stiran = GetGPIOINBitValue( 82 );	//������� ������
	Helicopter->lsp_elp.t_smoke = GetGPIOINBitValue( 83 );	//������� �� ������ 1-�����
	Helicopter->lsp_elp.t_belts = GetGPIOINBitValue( 84 );	//������� �� ������ ����������� ����� 8-���
	Helicopter->lsp_elp.t_exit_tab = GetGPIOINBitValue( 85 );	//������� ����� �����
	Helicopter->lsp_elp.t_ri = GetGPIOINBitValue( 86 );	//������� ������� �����������
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_falut_rr || Helicopter->rsp_ep.t_check_lamp, 4 );	//����� ����� ��
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_vkl_zapas || Helicopter->rsp_ep.t_check_lamp, 5 );	//����� ������ ��������
// 5 ����� ������ ���. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_weapon_chain || Helicopter->rsp_ep.t_check_lamp, 6 );	//��������������� ����� ���� ������ ����
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_ab || Helicopter->rsp_ep.t_check_lamp, 7 );	//��������������� ����� ��(�������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_explosion || Helicopter->rsp_ep.t_check_lamp, 8 );	//��������������� ����� �����(�������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_pus_bd1 || Helicopter->rsp_ep.t_check_lamp, 9 );	//��������������� ����� ��� ������ ��1 ��6(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_nps || Helicopter->rsp_ep.t_check_lamp, 10 );	//��������������� ����� ���(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_pus_bd2 || Helicopter->rsp_ep.t_check_lamp, 11 );	//��������������� ����� ��� ������ ��2 ��5(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk || Helicopter->rsp_ep.t_check_lamp, 12 );	//��������������� ����� ���(�������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd1_load || Helicopter->rsp_ep.t_check_lamp, 13 );	//��������������� ����� �� 1 ������(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd2_load || Helicopter->rsp_ep.t_check_lamp, 14 );	//��������������� ����� �� 2 ������(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd3_load || Helicopter->rsp_ep.t_check_lamp, 15 );	//��������������� ����� �� 3 ������(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd4_load || Helicopter->rsp_ep.t_check_lamp, 16 );	//��������������� ����� �� 4 ������(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd5_load || Helicopter->rsp_ep.t_check_lamp, 17 );	//��������������� ����� �� 5 ������(������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd6_load || Helicopter->rsp_ep.t_check_lamp, 18 );	//��������������� ����� �� 6 ������(������)
	Helicopter->lp_azs.t_pus_cocking = GetGPIOINBitValue( 87 );	//������ ��������� ���
	Helicopter->lp_azs.t_main_switch_on = GetGPIOINBitValue( 88 );	//������� ������� ����������� ���
	Helicopter->lp_azs.t_main_switch_off = GetGPIOINBitValue( 89 );	//������� ������� ����������� ����
	Helicopter->lp_azs.t_explosion = GetGPIOINBitValue( 90 );	//������� ����� - �������
	Helicopter->lp_azs.t_emerg_dump = GetGPIOINBitValue( 91 );	//������� ��������� ����� ���� �8 ���
	Helicopter->lp_azs.t_sns_nav = GetGPIOINBitValue( 92 );	//������� ��� ���
	Helicopter->lp_azs.t_uvd_count = GetGPIOINBitValue( 93 );	//������� ������� ���
	Helicopter->lp_azs.t_distmeter = GetGPIOINBitValue( 94 );	//������� ���������
	Helicopter->lp_azs.t_sublight_pkv = GetGPIOINBitValue( 95 );	//������� ������� ���
	Helicopter->lp_azs.t_power_onv_left = GetGPIOINBitValue( 96 );	//������� ������� ��� �����
	Helicopter->lp_azs.t_power_onv_board = GetGPIOINBitValue( 97 );	//������� ������� ��� ���� - ������
	Helicopter->lp_azs.t_power_onv_right = GetGPIOINBitValue( 98 );	//������� ������� ��� ������
// Helicopter->lp_azs.f_sublight_pkv unsupported  // �������� ������� ���
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_right || Helicopter->rsp_ep.t_check_lamp, 19 );	//����� ��� ������ ���
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_right, 20 );	//������� �������� ��� ������� ��� �1 (����������)
// Helicopter->lp_azs.f_ammo_count_upk_right_2 unsupported  // ������� �������� ��� ������� ��� �2 (����������)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_left || Helicopter->rsp_ep.t_check_lamp, 21 );	//����� ��� ����� ���
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_left, 22 );	//������� �������� ��� ������ ��� �1 (����������)
// Helicopter->lp_azs.f_ammo_count_upk_left_2 unsupported  // ������� �������� ��� ������ ��� �2 (����������)
// 6 ����� ����� �������������. unsupported  // 
// Helicopter->lsh_elp.sw_glass_wiper unsupported  // ������������� ����������������(unit-1: 40-2����,38-����,36-�����,35-1����)
	Helicopter->lsh_elp.t_rv = GetGPIOINBitValue( 99 );	//������� ����������� ���������������
	Helicopter->lsh_elp.t_spuu_52 = GetGPIOINBitValue( 100 );	//������� ����������� ����-52
// Helicopter->lsh_elp.t_plafon unsupported  // ������� ������(��������� �� ������)
	Helicopter->lsh_elp.t_ag_left = GetGPIOINBitValue( 101 );	//������� �� ���
	Helicopter->lsh_elp.t_ag_res = GetGPIOINBitValue( 102 );	//������� �� ������
	Helicopter->lsh_elp.t_bkk_18 = GetGPIOINBitValue( 103 );	//������� ���-18
	Helicopter->lsh_elp.t_ri_65 = GetGPIOINBitValue( 104 );	//������� ��-65
	Helicopter->lsh_elp.t_bkk_contr_up = GetGPIOINBitValue( 105 );	//������� I ����� ���
	Helicopter->lsh_elp.t_bkk_contr_down = GetGPIOINBitValue( 106 );	//������� II ����� ���
	Helicopter->lsh_elp.t_pvd_heat = GetGPIOINBitValue( 107 );	//������ �������� �������� ���
	SetGPIOOUTBitValue( Helicopter->lsh_elp.i_bkk_ok || Helicopter->rsp_ep.t_check_lamp, 23 );	//��������� �������. ���
	SetGPIOOUTBitValue( Helicopter->lsh_elp.i_ri_65 || Helicopter->rsp_ep.t_check_lamp, 24 );	//��������� ������ ��-65
	SetGPIOOUTBitValue( Helicopter->lsh_elp.i_pvd_heat_ok || Helicopter->rsp_ep.t_check_lamp, 25 );	//��������� ������� ��������
// 7 ����� ������ �������������. unsupported  // 
	Helicopter->lp_elp.t_deicing_auto = GetGPIOINBitValue( 108 );	//�������(���������������������� �������) �������
	Helicopter->lp_elp.t_deicing_off = GetGPIOINBitValue( 109 );	//������(���������������������� �������) ����
	Helicopter->lp_elp.t_deicing_left_pzu = GetGPIOINBitValue( 110 );	//�������(���������������������� �������) ���� ��� ���
	Helicopter->lp_elp.t_deicing_right_pzu = GetGPIOINBitValue( 111 );	//�������(���������������������� �������) ���� ��� ����
	Helicopter->lp_elp.t_deicing_glass = GetGPIOINBitValue( 112 );	//�������(���������������������� �������) ������� 
// Helicopter->lp_elp.sw_consumers unsupported  // �������� ������������� ��� ������������
	Helicopter->lp_elp.t_A_offret = GetGPIOINBitValue( 113 );	//���
	Helicopter->lp_elp.t_A_blade_1 = GetGPIOINBitValue( 114 );	//1
	Helicopter->lp_elp.t_A_blade_2 = GetGPIOINBitValue( 115 );	//2
	Helicopter->lp_elp.t_A_blade_3 = GetGPIOINBitValue( 116 );	//3
	Helicopter->lp_elp.t_A_blade_4 = GetGPIOINBitValue( 117 );	//4
	Helicopter->lp_elp.t_A_blade_5 = GetGPIOINBitValue( 118 );	//5
	Helicopter->lp_elp.t_A_blade_tail = GetGPIOINBitValue( 119 );	//����� ����
	Helicopter->lp_elp.t_A_glass = GetGPIOINBitValue( 120 );	//������
	Helicopter->lp_elp.t_A_PZU_l = GetGPIOINBitValue( 121 );	//��� ���
	Helicopter->lp_elp.t_A_PZU_r = GetGPIOINBitValue( 122 );	//��� ����
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_icing || Helicopter->rsp_ep.t_check_lamp, 26 );	//��������� �������.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_pos_on || Helicopter->rsp_ep.t_check_lamp, 27 );	//��������������� ����� ��� ��������
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_fault_pos || Helicopter->rsp_ep.t_check_lamp, 28 );	//��������������� ����� ����� ��� ��
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_heat_left || Helicopter->rsp_ep.t_check_lamp, 29 );	//��������������� ����� ������� ��.���
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_heat_right || Helicopter->rsp_ep.t_check_lamp, 30 );	//��������������� ����� ������� ��.����.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_left_pzu_front || Helicopter->rsp_ep.t_check_lamp, 31 );	//��������������� ����� ���.���.������.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_right_pzu_front || Helicopter->rsp_ep.t_check_lamp, 32 );	//��������������� ����� ����.���.������
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_left_pzu_back || Helicopter->rsp_ep.t_check_lamp, 33 );	//��������������� ����� ���.���.����.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_right_pzu_back || Helicopter->rsp_ep.t_check_lamp, 34 );	//��������������� ����� ����.���.����
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_so_121_prop || Helicopter->rsp_ep.t_check_lamp, 35 );	//��������������� ����� �� - 121 ��������
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_flight || Helicopter->rsp_ep.t_check_lamp, 36 );	//��������������� ����� �����
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_parking || Helicopter->rsp_ep.t_check_lamp, 37 );	//��������������� ����� �������
	SetVIDSteps( 2, Helicopter->lp_elp.f_ampermetr );	//��������� - �������(3-345, 15-3040), ����� 1044(3)
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_1 || Helicopter->rsp_ep.t_check_lamp, 38 );	//��������������� ����� 1 ������
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_2 || Helicopter->rsp_ep.t_check_lamp, 39 );	//��������������� ����� 2 ������
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_3 || Helicopter->rsp_ep.t_check_lamp, 40 );	//��������������� ����� 3 ������
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_4 || Helicopter->rsp_ep.t_check_lamp, 41 );	//��������������� ����� 4 ������
	SetLEDINDICATORValue( Helicopter->lp_elp.freq, 0 );	//����� ������
	Helicopter->lp_elp.f_radio_baklan20_1 = POTNormalValue(1);  // �������� ������������ ������ 20 �1
	Helicopter->lp_elp.f_radio_baklan20_2 = POTNormalValue(2);  // �������� ������������ ������ 20 �2
	Helicopter->lp_elp.e_radio_baklan20_1 = GetENCValue(6);
	Helicopter->lp_elp.e_radio_baklan20_2 = GetENCValue(7);
	Helicopter->lp_elp.t_radio_baklan20_1 = GetGPIOINBitValue( 123 );	//������� ������������ ������ 20 �1 ��
	Helicopter->lp_elp.t_radio_baklan20_2 = GetGPIOINBitValue( 124 );	//������� ������������ ������ 20 �2 ��
	Helicopter->lp_elp.t_bark_eng_left_on = GetGPIOINBitValue( 125 );	//������� ���� ���� ����� �������
	Helicopter->lp_elp.t_bark_eng_left_wep = GetGPIOINBitValue( 126 );	//������� ���� ���� ����� ��
	Helicopter->lp_elp.t_bark_eng_left_ct1 = GetGPIOINBitValue( 127 );	//������� ���� ���� ����� ��1(����?)
	Helicopter->lp_elp.t_bark_eng_left_ct2 = GetGPIOINBitValue( 128 );	//������� ���� ���� ����� ��2
	Helicopter->lp_elp.t_bark_eng_tg_left = GetGPIOINBitValue( 129 );	//������� ���� ���� �� �����
	Helicopter->lp_elp.t_bark_eng_tg_right = GetGPIOINBitValue( 130 );	//������� ���� ���� �� ������
	Helicopter->lp_elp.t_bark_eng_tk_left = GetGPIOINBitValue( 131 );	//������� ���� ���� �� �����
	Helicopter->lp_elp.t_bark_eng_tk_right = GetGPIOINBitValue( 132 );	//������� ���� ���� �� �����
	Helicopter->lp_elp.t_bark_eng_right_ct1 = GetGPIOINBitValue( 133 );	//������� ���� ���� ������ ��1(����?)
	Helicopter->lp_elp.t_bark_eng_right_ct2 = GetGPIOINBitValue( 134 );	//������� ���� ���� ������ ��2
	Helicopter->lp_elp.t_bark_eng_right_wep = GetGPIOINBitValue( 135 );	//������� ���� ���� ������ ��
	Helicopter->lp_elp.t_bark_eng_right_on = GetGPIOINBitValue( 136 );	//������� ���� ���� ������ �������
// 8 ������� ������ �������������. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_left_fire || Helicopter->rsp_ep.t_check_lamp, 42 );	//��������� ����� ��� ��
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_left_1_queue || Helicopter->rsp_ep.t_check_lamp, 43 );	//��������� ��� �� I �������
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_left_2_queue || Helicopter->rsp_ep.t_check_lamp, 44 );	//��������� ��� �� II �������
	Helicopter->cep.t_eng_left_1_queue = GetGPIOINBitValue( 137 );	//������ I ������� ��� ��
	Helicopter->cep.t_eng_left_2_queue = GetGPIOINBitValue( 138 );	//������ II ������� ��� ��
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_rght_fire || Helicopter->rsp_ep.t_check_lamp, 45 );	//��������� ����� ���� ��
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_rght_1_queue || Helicopter->rsp_ep.t_check_lamp, 46 );	//��������� ���� �� I �������
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_rght_2_queue || Helicopter->rsp_ep.t_check_lamp, 47 );	//��������� ���� �� II �������
	Helicopter->cep.t_eng_rght_1_queue = GetGPIOINBitValue( 139 );	//������ I ������� ���� ��
	Helicopter->cep.t_eng_rght_2_queue = GetGPIOINBitValue( 140 );	//������ II ������� ���� ��
	SetGPIOOUTBitValue( Helicopter->cep.i_ko_50_fire || Helicopter->rsp_ep.t_check_lamp, 48 );	//��������� ����� ��-50
	SetGPIOOUTBitValue( Helicopter->cep.i_ko_50_1_queue || Helicopter->rsp_ep.t_check_lamp, 49 );	//��������� ��-50 I �������
	SetGPIOOUTBitValue( Helicopter->cep.i_ko_50_2_queue || Helicopter->rsp_ep.t_check_lamp, 50 );	//��������� ��-50 II �������
	Helicopter->cep.t_ko_50_1_queue = GetGPIOINBitValue( 141 );	//������ I ������� ��-50
	Helicopter->cep.t_ko_50_2_queue = GetGPIOINBitValue( 142 );	//������ II ������� ��-50
	SetGPIOOUTBitValue( Helicopter->cep.i_red_vsu_fire || Helicopter->rsp_ep.t_check_lamp, 51 );	//��������� ����� ���. ���
	SetGPIOOUTBitValue( Helicopter->cep.i_red_vsu_1_queue || Helicopter->rsp_ep.t_check_lamp, 52 );	//��������� ��� ��� I �������
	SetGPIOOUTBitValue( Helicopter->cep.i_red_vsu_2_queue || Helicopter->rsp_ep.t_check_lamp, 53 );	//��������� ��� ��� II �������
	Helicopter->cep.t_red_vsu_1_queue = GetGPIOINBitValue( 143 );	//������ I ������� ��� ���
	Helicopter->cep.t_red_vsu_2_queue = GetGPIOINBitValue( 144 );	//������ II ������� ��� ���
	Helicopter->cep.t_contr_sensors = GetGPIOINBitValue( 145 );	//������� �������� �������� true
	Helicopter->cep.t_firefighting = GetGPIOINBitValue( 146 );	//������� �����������(inverse t_contr_sensors)
	Helicopter->cep.t_1 = GetGPIOINBitValue( 147 );	//������� �������� ������������ true
	Helicopter->cep.t_2 = GetGPIOINBitValue( 148 );	//������� II(inverse t_1)
// Helicopter->cep.sw_contr_sensors unsupported  // ������������� �������� �������� 
	Helicopter->cep.t_contr_sensors_0 = GetGPIOINBitValue( 149 );	//���
	Helicopter->cep.t_contr_sensors_1 = GetGPIOINBitValue( 150 );	//��������
	Helicopter->cep.t_contr_sensors_2 = GetGPIOINBitValue( 151 );	//1-�
	Helicopter->cep.t_contr_sensors_3 = GetGPIOINBitValue( 152 );	//2-�
	Helicopter->cep.t_contr_sensors_4 = GetGPIOINBitValue( 153 );	//3-�
	Helicopter->cep.t_contr_sensors_5 = GetGPIOINBitValue( 154 );	//4-�
	Helicopter->cep.t_contr_sensors_6 = GetGPIOINBitValue( 155 );	//5-�
	Helicopter->cep.t_contr_sensors_7 = GetGPIOINBitValue( 156 );	//6-�
	SetGPIOOUTBitValue( Helicopter->cep.i_contr_sensors || Helicopter->rsp_ep.t_check_lamp, 54 );	//��������� �������� ��������
	SetVIDSteps( 3, Helicopter->cep.f_p_hidro_main );	//����� �����-������� (0-267,10-1242), ����� 1034(4)
	SetVIDSteps( 4, Helicopter->cep.f_p_hidro_second );	//������ �����-������� (0-323,100-1140), ����� 1034(1)
	Helicopter->cep.t_hidro_main = GetGPIOINBitValue( 157 );	//������� ������������ ������ true=���
	SetGPIOOUTBitValue( Helicopter->cep.i_hidro_main || Helicopter->rsp_ep.t_check_lamp, 55 );	//��������� ������ ��������
	Helicopter->cep.t_hidro_second = GetGPIOINBitValue( 158 );	//������� ������
	SetGPIOOUTBitValue( Helicopter->cep.i_hidro_second || Helicopter->rsp_ep.t_check_lamp, 56 );	//��������� ������ ��������
	Helicopter->cep.t_off_second_hidro = GetGPIOINBitValue( 159 );	//������ ���� ������
	Helicopter->cep.t_ta_sel_false_start = GetGPIOINBitValue( 160 );	//������� ������� bool
	Helicopter->cep.t_ta_sel_start = GetGPIOINBitValue( 161 );	//������� ������
	Helicopter->cep.t_ta_start = GetGPIOINBitValue( 162 );	//������ ������ �������������
	Helicopter->cep.t_ta_stop = GetGPIOINBitValue( 163 );	//������ ���� ��� ��-9�
	SetGPIOOUTBitValue( Helicopter->cep.i_ta_automat_on || Helicopter->rsp_ep.t_check_lamp, 57 );	//��������� ������� �������
	SetGPIOOUTBitValue( Helicopter->cep.i_p_oil_norm || Helicopter->rsp_ep.t_check_lamp, 58 );	//��������� ������� ��� ����� ����(12 �� 11 ����� ���������� �������)
	SetGPIOOUTBitValue( Helicopter->cep.i_rpm_norm || Helicopter->rsp_ep.t_check_lamp, 59 );	//��������� ������� ������� �����(13 �� 10 ����� ���������� �������)
	SetGPIOOUTBitValue( Helicopter->cep.i_rpm_max || Helicopter->rsp_ep.t_check_lamp, 60 );	//��������� ������� ������
	SetGPIOOUTBitValue( Helicopter->cep.i_p_oil_warn || Helicopter->rsp_ep.t_check_lamp, 61 );	//��������� ������ ��� ����� (12 �� 11 ����� ���������� �������)
	SetGPIOOUTBitValue( Helicopter->cep.i_TF || Helicopter->rsp_ep.t_check_lamp, 62 );	//��������� ������ ��������� ������(13 �� 10 ����� ���������� �������)
	SetGPIOOUTBitValue( Helicopter->cep.i_ta_brok || Helicopter->rsp_ep.t_check_lamp, 63 );	//��������� ��� �����
	Helicopter->cep.t_eng_start = GetGPIOINBitValue( 164 );	//������ ������ ����������
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_automat_on || Helicopter->rsp_ep.t_check_lamp, 64 );	//��������� ������� �������
	Helicopter->cep.t_eng_stop = GetGPIOINBitValue( 165 );	//������ ������� ������� ����������
	Helicopter->cep.t_eng_left = GetGPIOINBitValue( 166 );	//������� ������ ���
	Helicopter->cep.t_eng_rght = GetGPIOINBitValue( 167 );	//������� ������ ����
	Helicopter->cep.t_eng_sel_start = GetGPIOINBitValue( 168 );	//������� ������
	Helicopter->cep.t_eng_sel_scroll = GetGPIOINBitValue( 169 );	//������� �������
	SetGPIOOUTBitValue( Helicopter->cep.i_ta_work || Helicopter->rsp_ep.t_check_lamp, 65 );	//��������� ������� ��������
	Helicopter->cep.t_eng_left_ign_check = GetGPIOINBitValue( 170 );	//������� �������� ����� ���
	Helicopter->cep.t_eng_right_ign_check = GetGPIOINBitValue( 171 );	//������� �������� ����� ����
	SetVIDSteps( 5, Helicopter->cep.f_t_ta );	//����� ����������� (0-300,9-2636), ����� 1034(2)
	SetVIDSteps( 6, Helicopter->cep.f_p_ta );	//����� �������� (0-84, 3-1032), ����� 1034(3)
	Helicopter->cep.t_fuel_left_close = GetGPIOINBitValue( 172 );	//������� ��-9� ������ ��� ���� true=����
	Helicopter->cep.t_fuel_right_close = GetGPIOINBitValue( 173 );	//������� ��-9� ������ ���� ���� true=����
	Helicopter->cep.t_fuel_bypass_close = GetGPIOINBitValue( 174 );	//������� ��-9� ������ ������� ����� true=����
	Helicopter->cep.t_fuel_perepusk = GetGPIOINBitValue( 175 );	//������� ��������
	SetGPIOOUTBitValue( Helicopter->cep.i_fuel_left_close || Helicopter->rsp_ep.t_check_lamp, 66 );	//��������� ����� ������
	SetGPIOOUTBitValue( Helicopter->cep.i_fuel_right_close || Helicopter->rsp_ep.t_check_lamp, 67 );	//��������� ������ ������
	SetGPIOOUTBitValue( Helicopter->cep.i_fuel_bypass_close || Helicopter->rsp_ep.t_check_lamp, 68 );	//��������� �������. ����
	Helicopter->cep.t_fpump_refil = GetGPIOINBitValue( 176 );	//������� ��-9�  ����� true=���
	Helicopter->cep.t_refil = GetGPIOINBitValue( 177 );	//������� ��-9� ������  true=�����(����� ����������)
	Helicopter->cep.t_fpump_consum = GetGPIOINBitValue( 178 );	//������� ��-9�  ������ true=�����
	Helicopter->cep.t_fpump_left = GetGPIOINBitValue( 179 );	//������� ��-9� ����� true=�����
	Helicopter->cep.t_fpump_right = GetGPIOINBitValue( 180 );	//������� ��-9� ������ true=�����
	SetGPIOOUTBitValue( Helicopter->cep.i_fpump_consum_off || Helicopter->rsp_ep.t_check_lamp, 69 );	//��������� ������ �� �����
	SetGPIOOUTBitValue( Helicopter->cep.i_fpump_left_off || Helicopter->rsp_ep.t_check_lamp, 70 );	//��������� ����� �� �����
	SetGPIOOUTBitValue( Helicopter->cep.i_fpump_right_off || Helicopter->rsp_ep.t_check_lamp, 71 );	//��������� ������ �� �����
// 9 ������ ������ �������������. unsupported  // 
// Helicopter->rsp_elp.ark_9.f_rk_vol unsupported  // �������� ���������
	Helicopter->rsp_elp.ark_9.t_rk_tlf = GetGPIOINBitValue( 181 );	//������� ���  false   
	Helicopter->rsp_elp.ark_9.t_rk_tlg = GetGPIOINBitValue( 182 );	//������� ���(inverse t_rk_vol)
	Helicopter->rsp_elp.ark_9.t_rk_ramka = GetGPIOINBitValue( 183 );	//������ �����
	Helicopter->rsp_elp.ark_9.t_rk_ctrl = GetGPIOINBitValue( 184 );	//������ ���
	Helicopter->rsp_elp.ark_9.t_rk_chanel = GetGPIOINBitValue( 185 );	//������� �����
// Helicopter->rsp_elp.ark_9.sw_rk_mode unsupported  // �������� �������������
	Helicopter->rsp_elp.ark_9.t_rk_off = GetGPIOINBitValue( 186 );	//���
	Helicopter->rsp_elp.ark_9.t_rk_com = GetGPIOINBitValue( 187 );	//���
	Helicopter->rsp_elp.ark_9.t_rk_ant = GetGPIOINBitValue( 188 );	//���
	Helicopter->rsp_elp.ark_9.t_rk_ram = GetGPIOINBitValue( 189 );	//���
// Helicopter->rsp_elp.f_t_ko50 unsupported  // �������� �������� � ��-50
	Helicopter->rsp_elp.ark_ud.t_rkud_sens = GetGPIOINBitValue( 190 );	//������� ������
	Helicopter->rsp_elp.ark_ud.t_rkud_ukv = GetGPIOINBitValue( 191 );	//������� ���-��� true-���
// Helicopter->rsp_elp.ark_ud.f_rkud_vol unsupported  // �������� ��������� 
	SetGPIOOUTBitValue( Helicopter->rsp_elp.ark_ud.i_rkud_1 || Helicopter->rsp_ep.t_check_lamp, 72 );	//����� �� 
	SetGPIOOUTBitValue( Helicopter->rsp_elp.ark_ud.i_rkud_2 || Helicopter->rsp_ep.t_check_lamp, 73 );	//����� ��
	SetGPIOOUTBitValue( Helicopter->rsp_elp.ark_ud.i_rkud_3 || Helicopter->rsp_ep.t_check_lamp, 74 );	//����� �
// Helicopter->rsp_elp.ark_ud.sw_rkud_mode unsupported  // �������� ������������� ���-�� uint8 
	Helicopter->rsp_elp.ark_ud.t_rkud_off = GetGPIOINBitValue( 192 );	//0-���
	Helicopter->rsp_elp.ark_ud.t_rkud_up = GetGPIOINBitValue( 193 );	//1-��
	Helicopter->rsp_elp.ark_ud.t_rkud_shp = GetGPIOINBitValue( 194 );	//2-��
	Helicopter->rsp_elp.ark_ud.t_rkud_i = GetGPIOINBitValue( 195 );	//3-�
	Helicopter->rsp_elp.ark_ud.t_rkud_rpk = GetGPIOINBitValue( 196 );	//4-���
// Helicopter->rsp_elp.ark_ud.sw_rkud_chanel unsupported  // �������� ������������� ������  uint8 
	Helicopter->rsp_elp.ark_ud.t_rkud_1 = GetGPIOINBitValue( 197 );	//1-�����
	Helicopter->rsp_elp.ark_ud.t_rkud_2 = GetGPIOINBitValue( 198 );	//2-�����
	Helicopter->rsp_elp.ark_ud.t_rkud_3 = GetGPIOINBitValue( 199 );	//3-�����
	Helicopter->rsp_elp.ark_ud.t_rkud_4 = GetGPIOINBitValue( 200 );	//4-�����
	Helicopter->rsp_elp.ark_ud.t_rkud_5 = GetGPIOINBitValue( 201 );	//5-�����
	Helicopter->rsp_elp.ark_ud.t_rkud_6 = GetGPIOINBitValue( 202 );	//6-�����
	Helicopter->rsp_elp.ark_ud.t_contr = GetGPIOINBitValue( 203 );	//������ �����
	Helicopter->rsp_elp.ark_ud.t_ant_l = GetGPIOINBitValue( 204 );	//������ ��� �
	Helicopter->rsp_elp.ark_ud.t_ant_r = GetGPIOINBitValue( 205 );	//������ ��� �
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_ep.t_check_lamp, 75 );	//��������� �������������
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_ko50_ignition || Helicopter->rsp_ep.t_check_lamp, 76 );	//��������� ���������
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_ko50_work || Helicopter->rsp_ep.t_check_lamp, 77 );	//��������� ��-50 ��������
	Helicopter->rsp_elp.t_ko50_ta_start = GetGPIOINBitValue( 206 );	//������ ������ �������������
	Helicopter->rsp_elp.t_ko50_auto = GetGPIOINBitValue( 207 );	//������� ��50-��� 
	Helicopter->rsp_elp.t_ko50_man = GetGPIOINBitValue( 208 );	//������� ��50-����
	Helicopter->rsp_elp.t_ko50_fill = GetGPIOINBitValue( 209 );	//������� ��50-�������
	Helicopter->rsp_elp.t_ko50_midl = GetGPIOINBitValue( 210 );	//������� ��50-����� �����
	Helicopter->rsp_elp.t_ko50_vent = GetGPIOINBitValue( 211 );	//������� ������
	Helicopter->rsp_elp.t_N = GetGPIOINBitValue( 212 );	//������� ���
	Helicopter->rsp_elp.t_S = GetGPIOINBitValue( 213 );	//������� ��
	Helicopter->rsp_elp.gmk.t_contr300 = GetGPIOINBitValue( 214 );	//������� ����� 300
	Helicopter->rsp_elp.gmk.t_contr0 = GetGPIOINBitValue( 215 );	//������� ����� 0
// Helicopter->rsp_elp.f_latitude unsupported  // �������� ������ 0:01:00
	Helicopter->rsp_elp.t_mk = GetGPIOINBitValue( 216 );	//������� ��
	//Helicopter->rsp_elp.t_mk = GetGPIOINBitValue( 217 );	//������� ��
	Helicopter->rsp_elp.t_zk_l = GetGPIOINBitValue( 218 );	//������� �� ���
	Helicopter->rsp_elp.t_zk_r = GetGPIOINBitValue( 219 );	//������� �� ����
	Helicopter->rsp_elp.t_dubl_osnv = GetGPIOINBitValue( 220 );	//������� ������ 
	Helicopter->rsp_elp.t_osnv = GetGPIOINBitValue( 221 );	//������� �����(����� ����������)
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_dubl || Helicopter->rsp_ep.t_check_lamp, 78 );	//��������� ������(����� ����������)
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_osnv || Helicopter->rsp_ep.t_check_lamp, 79 );	//��������� �����(����� ����������)
	SetGPIOOUTBitValue( Helicopter->rsp_elp.akb_hight_temp_1 || Helicopter->rsp_ep.t_check_lamp, 195 ); //��������� ������� ����������� ��� 1
	SetGPIOOUTBitValue( Helicopter->rsp_elp.akb_hight_temp_2 || Helicopter->rsp_ep.t_check_lamp, 196 );	//��������� ������� ����������� ��� 2
// 10 ������ ����� �������������. unsupported  // 
	Helicopter->rsh_elp.t_pvd_heat_contr = GetGPIOINBitValue( 222 );	//�������� �������� ���
	SetGPIOOUTBitValue( Helicopter->rsh_elp.i_pvd_heat_ok || Helicopter->rsp_ep.t_check_lamp, 80 );	//��������� ������� ��������
	Helicopter->rsh_elp.t_diss = GetGPIOINBitValue( 223 );	//������� ����
	Helicopter->rsh_elp.t_rs = GetGPIOINBitValue( 224 );	//������� ������� ��
	Helicopter->rsh_elp.t_gmk = GetGPIOINBitValue( 225 );	//������� ���-1
	Helicopter->rsh_elp.t_sgu35 = GetGPIOINBitValue( 226 );	//������� ��-53 (���35)
	Helicopter->rsh_elp.t_ag_right = GetGPIOINBitValue( 227 );	//������� ������ ��
// Helicopter->rsh_elp.t_plafon unsupported  // ������� ������(�� ������ ���������)
// Helicopter->rsh_elp.sw_wiper_mode unsupported  // ������������� ����������������(���� 3: 22-����,23-�����,19-2����,27-1����)
	Helicopter->rsh_elp.t_rkud = GetGPIOINBitValue( 228 );	//������� ���-��
// 11 ������ ������� ������ �������������. unsupported  // 
	Helicopter->rsp_ep.t_check_lamp = GetGPIOINBitValue( 229 );	//������� �������� ����
	Helicopter->rsp_ep.t_main_second = GetGPIOINBitValue( 230 );	//������� �����
	Helicopter->rsp_ep.t_main_reserv = GetGPIOINBitValue( 459 );	//������� ������
// Helicopter->rsp_ep.f_board unsupported  // �������� �����
	Helicopter->rsp_ep.t_day = GetGPIOINBitValue( 231 );	//������� ����-true, ����-false
	Helicopter->rsp_ep.t_night = GetGPIOINBitValue( 232 );	//������� ����-false
// Helicopter->rsp_ep.f_gain unsupported  // �������� ��������� ����(��������)
// Helicopter->rsp_ep.sw_scan unsupported  // ������������� ����� ����(��������)
// Helicopter->rsp_ep.t_disable unsupported  // ���� 
// Helicopter->rsp_ep.t_PrDU unsupported  // ����
// Helicopter->rsp_ep.t_ZU unsupported  // ��
// Helicopter->rsp_ep.t_MSH unsupported  // ��
// Helicopter->rsp_ep.t_NU unsupported  // ��
// Helicopter->rsp_ep.t_instal unsupported  // �����
// Helicopter->rsp_ep.t_nabor unsupported  // �����
// Helicopter->rsp_ep.t_enter unsupported  // ����
	Helicopter->rsp_ep.t_light_emeg = GetGPIOINBitValue( 233 );	//������� �������� ���������
	Helicopter->rsp_ep.t_light_generl = GetGPIOINBitValue( 234 );	//������� ����� ���������
	Helicopter->rsp_ep.t_ano = GetGPIOINBitValue( 235 );	//������� ���
	Helicopter->rsp_ep.t_light_drill = GetGPIOINBitValue( 236 );	//������� ���� ������
	Helicopter->rsp_ep.t_light_contur = GetGPIOINBitValue( 237 );	//������� ���� ������
	Helicopter->rsp_ep.t_flashlight = GetGPIOINBitValue( 238 );	//������� �������
	Helicopter->rsp_ep.t_beacon_down = GetGPIOINBitValue( 239 );	//������� ���� ���
	Helicopter->rsp_ep.t_pzu_left = GetGPIOINBitValue( 240 );	//������� ��� ������ ���
	Helicopter->rsp_ep.t_pzu_right = GetGPIOINBitValue( 241 );	//������� ��� ������ ����
	Helicopter->rsp_ep.t_pvd_heat_left = GetGPIOINBitValue( 242 );	//������� ������� ��� ���
	Helicopter->rsp_ep.t_pvd_heat_right = GetGPIOINBitValue( 243 );	//������� ������� ��� ����
	Helicopter->rsp_ep.t_clock_heat = GetGPIOINBitValue( 244 );	//������� ������� �����
// Helicopter->rsp_ep.t_heat_accum unsupported  // ������� ������� �����
	Helicopter->rsp_ep.t_sublight_1 = GetGPIOINBitValue( 245 );	//������ 1
	Helicopter->rsp_ep.t_sublight_2 = GetGPIOINBitValue( 246 );	//������ 2
// Helicopter->rsp_ep.f_day_night unsupported  // ���� - ����
	SetGPIOOUTBitValue( Helicopter->rsp_ep.i_pzu_left_on || Helicopter->rsp_ep.t_check_lamp, 81 );	//��������� ���.��� �������
	SetGPIOOUTBitValue( Helicopter->rsp_ep.i_pzu_right_on || Helicopter->rsp_ep.t_check_lamp, 82 );	//��������� ����.��� �������
// 12 ������������ �������������. unsupported  // 
	SetVIDSteps( 7, Helicopter->ep_elp.f_amper_1 );	//����� ���������� ��� ���, ����� 1064(2)
	SetVIDSteps( 8, Helicopter->ep_elp.f_amper_2 );	//����� ���������� ��� �����, ����� 1064(1)
	SetVIDSteps( 9, Helicopter->ep_elp.f_amper_3 );	//����� ���������� ��� ����, ����� 1064(4)
	Helicopter->ep_elp.t_akkum_1 = GetGPIOINBitValue( 247 );	//������� ������������ 1
	Helicopter->ep_elp.t_akkum_2 = GetGPIOINBitValue( 248 );	//������� ������������ 2
// Helicopter->ep_elp.get_ctrl() unsupported  // ������������� �������� ����
	Helicopter->ep_elp.t_ctrl_vu1 = GetGPIOINBitValue( 249 );	//�� 1 
	Helicopter->ep_elp.t_ctrl_vu2 = GetGPIOINBitValue( 250 );	//�� 2
// Helicopter->ep_elp.get_dc_volt_mode() unsupported  // ������������� �����
	Helicopter->ep_elp.t_dc_volt_mode_0 = GetGPIOINBitValue( 251 );	//���� 1
	Helicopter->ep_elp.t_dc_volt_mode_1 = GetGPIOINBitValue( 252 );	//��������� 1
	Helicopter->ep_elp.t_dc_volt_mode_2 = GetGPIOINBitValue( 253 );	//��������� 2
	Helicopter->ep_elp.t_dc_volt_mode_3 = GetGPIOINBitValue( 254 );	//�� 1
	Helicopter->ep_elp.t_dc_volt_mode_4 = GetGPIOINBitValue( 255 );	//�� 2
	Helicopter->ep_elp.t_dc_volt_mode_5 = GetGPIOINBitValue( 256 );	//���� �� ���1
	Helicopter->ep_elp.t_dc_volt_mode_6 = GetGPIOINBitValue( 257 );	//���� �� ���2
	Helicopter->ep_elp.t_dc_volt_mode_7 = GetGPIOINBitValue( 258 );	//���� ��-���
	Helicopter->ep_elp.t_dc_volt_mode_8 = GetGPIOINBitValue( 259 );	//������ �����
	Helicopter->ep_elp.t_dc_volt_mode_9 = GetGPIOINBitValue( 260 );	//���� 2
// Helicopter->ep_elp.f_volt_reg unsupported  // �������� ����������� ����������
	Helicopter->ep_elp.t_ACDC_1 = GetGPIOINBitValue( 261 );	//������� ����������� 1
	Helicopter->ep_elp.t_ACDC_2 = GetGPIOINBitValue( 262 );	//������� ����������� 2
	Helicopter->ep_elp.t_aero_pit_dc = GetGPIOINBitValue( 263 );	//������� ��� ����� (������ �����)
	Helicopter->ep_elp.t_test_sys = GetGPIOINBitValue( 264 );	//������� �������� ������
	SetVIDSteps( 10, Helicopter->ep_elp.f_volt );	//���������, ����� 1064(3)
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_vu1_brok || Helicopter->rsp_ep.t_check_lamp, 83 );	//��������� ��1 �� �����
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_vu2_brok || Helicopter->rsp_ep.t_check_lamp, 84 );	//��������� ��2 �� �����
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_aero_pit_dc || Helicopter->rsp_ep.t_check_lamp, 85 );	//��������� ��� ��� 
	SetVIDSteps( 11, Helicopter->ep_elp.f_ac_amper_1 );	//���������� ���-��������� ���, ����� 1074(1)
	SetVIDSteps( 14, Helicopter->ep_elp.f_ac_amper_2 );	//���������� ���-��������� �����, ����� 1074(3)
	SetVIDSteps( 13, Helicopter->ep_elp.f_ac_volt );	//���������� ���-���������, ����� 1074(4)
	SetVIDSteps( 12, Helicopter->ep_elp.f_ac_amper_3 );	//���������� ���-��������� ����, ����� 1074(2)
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_gen_1_off || Helicopter->rsp_ep.t_check_lamp, 86 );	//��������� ���. 1 ����
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_gen_2_off || Helicopter->rsp_ep.t_check_lamp, 87 );	//��������� ���. 2 ����
	SetGPIOOUTBitValue( Helicopter->ep_elp.t_res_gen || Helicopter->rsp_ep.t_check_lamp, 88 );	//��������� ��� ��� �����
	Helicopter->ep_elp.t_gen_1 = GetGPIOINBitValue( 265 );	//������� ���������� 1
	Helicopter->ep_elp.t_gen_2 = GetGPIOINBitValue( 266 );	//������� ���������� 2
	Helicopter->ep_elp.t_gen_1_contr = GetGPIOINBitValue( 461 );	//������� ���������� 1 �������� ����������
	Helicopter->ep_elp.t_gen_2_contr = GetGPIOINBitValue( 462 );	//������� ���������� 2 �������� ����������
	Helicopter->ep_elp.t_res_gen = GetGPIOINBitValue( 267 );	//������� ���?
	Helicopter->ep_elp.t_r36_auto = GetGPIOINBitValue( 268 );	//������� ��-�36� �������
	Helicopter->ep_elp.t_r36_man = GetGPIOINBitValue( 269 );	//������� ��-�36� ������
	Helicopter->ep_elp.t_pts_auto = GetGPIOINBitValue( 270 );	//������� ��� �������
	Helicopter->ep_elp.t_pts_man = GetGPIOINBitValue( 271 );	//������� ��� ������
	Helicopter->ep_elp.t_aero_pit_ac = GetGPIOINBitValue( 272 );	//������� ����� �����
	Helicopter->ep_elp.t_gen = GetGPIOINBitValue( 273 );	//������� ��� ��� �����(10-���)
	Helicopter->ep_elp.t_gen_pts_contr = GetGPIOINBitValue(505); //������� ��� ��� �����(3-�����)
// Helicopter->ep_elp.get_ac_volt_mode() unsupported  // ������������� �������� ����������
	Helicopter->ep_elp.t_ac_volt_mode_pts_gen1a = GetGPIOINBitValue( 274 );	//��� ��������� �
	Helicopter->ep_elp.t_ac_volt_mode_pts_gen1b = GetGPIOINBitValue( 275 );	//��� ��������� �
	Helicopter->ep_elp.t_ac_volt_mode_pts_gen1c = GetGPIOINBitValue( 276 );	//��� ��������� �
	Helicopter->ep_elp.t_ac_volt_mode_aer_gen1a = GetGPIOINBitValue( 277 );	//��� ����� ��������� 2 �
	Helicopter->ep_elp.t_ac_volt_mode_aer_gen1b = GetGPIOINBitValue( 278 );	//��� ����� ��������� 2 �
	Helicopter->ep_elp.t_ac_volt_mode_aer_gen1c = GetGPIOINBitValue( 279 );	//��� ����� ��������� 2 �
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_reser36 || Helicopter->rsp_ep.t_check_lamp, 89 );	//��������� -38� ������
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_ptc_on || Helicopter->rsp_ep.t_check_lamp, 90 );	//��������� �� �������
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_resev_on || Helicopter->rsp_ep.t_check_lamp, 91 );	//��������� ������ ����� ���
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_rap_on || Helicopter->rsp_ep.t_check_lamp, 92 );	//��������� ��� ��� �����
// 13 ����� ��������� ������. unsupported  // 
// Helicopter->lpb.t_rulejka unsupported  // ������� �����(��� � ���������)
	Helicopter->lpb.t_svet_ubran = GetGPIOINBitValue( 280 );	//������� ���� ������
	Helicopter->lpb.t_svet = GetGPIOINBitValue( 281 );	//������� ���� ����
	Helicopter->lpb.t_fpp = GetGPIOINBitValue( 282 );	//������� ������ ���
	Helicopter->lpb.t_project = GetGPIOINBitValue( 283 );	//������� ������ �������
	Helicopter->lpb.t_dnmr = GetGPIOINBitValue(503);        // ������� ���/����
// Helicopter->lpb.sw_PVD unsupported  // ������������� ����������� ������� ��� 0-�����, 1-������������, 2-������
	Helicopter->lpb.t_pvd_left = GetGPIOINBitValue( 284 );	//��� �����
	Helicopter->lpb.t_pvd_unitd = GetGPIOINBitValue( 285 );	//��� ������������
	Helicopter->lpb.t_pvd_right = GetGPIOINBitValue( 286 );	//��� ������
	Helicopter->lpb.t_roll_33 = GetGPIOINBitValue( 287 );	//
	SetVIDSteps( 15, Helicopter->lpb.rv_5.alt( ) );	//���������-������� 1134(2) 0=40, 100=1910, 700=3670
	Helicopter->lpb.rv_5.t_test = GetGPIOINBitValue( 288 );	//���������-������ ����
	Helicopter->lpb.rv_5.encoder = GetENCValue(0);	//���������-�������� ������� (600,)
	SetGPIOOUTBitValue(Helicopter->lpb.rv_5.t_triangle || Helicopter->rsp_ep.t_check_lamp, 184 );// ����� � �������� �������� (����� ����������)
// Helicopter->lpb.rv_5.down unsupported  // 
	SetVIDSteps( 16, Helicopter->lpb.rv_5.index );	//���������-������ 1134(1)
	SetVIDSteps( 17, Helicopter->lpb.rv_5.blank );	//���������-������� 1134(3)
	Helicopter->lpb.t_apk_sv = GetGPIOINBitValue( 289 );	//������� ��� ��� ��
	Helicopter->lpb.t_apk_ukv = GetGPIOINBitValue( 290 );	//������� ��� ��� ���
	SetVIDSteps( 18, Helicopter->lpb.ut_6k.get_temp_1()/*Helicopter->lpb.ut_6k.temp1 */);	//���������-�������-��� ����  244(2)
	SetVIDSteps( 19, Helicopter->lpb.ut_6k.get_temp_2()/*Helicopter->lpb.ut_6k.temp2*/ );	//���������-�������-���� ���� 244(3)
	SetCLOCKSteps( 0, Helicopter->lpb.ut_6k.get_t100_1()/* Helicopter->lpb.ut_6k.get_temp1_fine() */ );	//���������-�������-��� ��� 254(3)  (0=160)
	SetCLOCKSteps( 1, Helicopter->lpb.ut_6k.get_t100_2()/* Helicopter->lpb.ut_6k.get_temp2_fine()  */);	//���������-�������-���� ��� 254(4) (0=123)
	SetVIDSteps( 20, Helicopter->lpb.uc_450k.get_speed() );	//���������-������� 1144(2) 40=0 150=935
	SetVIDSteps( 21, Helicopter->lpb.vd_10vk.get_km() );	//���������� (��)-������� 1144(1) 0km=0 7km=3078
	SetCLOCKSteps( 2, Helicopter->lpb.vd_10vk.get_m()  );	//���������� (�)-������� 1154(4) 0=2035
// Helicopter->lpb.vd_10vk.p unsupported  // ���������� (�)-�������� 0:10:00
	Helicopter->lpb.vd_10vk.f_barometr = 671.f + MABNormalValue(14);  // ���������� (�)-�������� 0:10:00
	SetVIDSteps( 22, Helicopter->lpb.up_21_15.fosh );	//��� ����� ������� 1:15:00  234(1) (1-296,15-2633)
	SetVIDSteps( 23, Helicopter->lpb.ir_117.index );	//���������� ������� ����� 234(2)
	SetVIDSteps( 24, Helicopter->lpb.ir_117.eng1 );	//���������� �������-����� ��������� 234(4)
	SetVIDSteps( 25, Helicopter->lpb.ir_117.eng2 );	//���������� �������-������ ��������� 234(3)
	SetVIDSteps( 26, Helicopter->lpb.avg_left.roll );	//������������ ���-���� 0  ����-roll 1214(2)
	SetVIDSteps( 27, Helicopter->lpb.avg_left.getPitch() );	//������������ ���-������ 0 ������ 1204(4)
	SetVIDSteps( 28, Helicopter->lpb.avg_left.yawl );	//������������ ���-�������� 964(4) 0  ����-roll 
// Helicopter->lpb.avg_left.index unsupported  // ������������ ���-������ 0,125 ������� �� �����
// Helicopter->lpb.avg_left.shift_pitch unsupported  // ������������ ���-��������� ����� ������� �������� �� ���������
	SetVIDSteps( 29, Helicopter->lpb.avg_left.blank );	//������������ ���:������ ���. ������� 1214(1)
// Helicopter->lpb.avg_left.alignment unsupported  // ������������ �������� ���������
	Helicopter->lpb.avg_left.encoder = GetENCValue(1);	//
// Helicopter->lpb.ugr_4uk.dir() unsupported  // ��� - ������� ����
	SetCLOCKStepsUGR( 3, Helicopter->lpb.ugr_4uk.dir()  );	//���-������� ����  284(3)  0=4224
// Helicopter->lpb.ugr_4uk.d1 unsupported  // ��� - �������� �������
	SetCLOCKSteps( 4, Helicopter->lpb.ugr_4uk.ark()  );	//��� - ������ �������  284(4)  0=1604
// Helicopter->lpb.ugr_4uk.corrector unsupported  // ��� - �������� ���������
	SetGPIOOUTBitValue( Helicopter->lpb.ss.i_ss || Helicopter->rsp_ep.t_check_lamp, 93 );	//��������� �� - ����� ���
	SetVIDSteps( 30, Helicopter->lpb.ss.speed_x );	//��������� ��-����� 934(3)
	SetVIDSteps( 31, Helicopter->lpb.ss.speed_x_down );	//��������� ��-����� 934(4)
	SetVIDSteps( 32, Helicopter->lpb.ss.speed_z_left );	//��������� ��-����� 944(2)
	SetVIDSteps( 33, Helicopter->lpb.ss.speed_z );	//��������� ��-������ 944(3)
	SetVIDSteps( 34, Helicopter->lpb.ss.speed_y );	//��������� ��-�����-���� 934(1)
	SetVIDSteps( 35, Helicopter->lpb.vr_30mk.var );	//������/����� ������� 1144(3) +25=90  0=2015 -25=3925
	SetVIDSteps( 36, Helicopter->lpb.ite_1t.nv );	//������� ���� ������� - �������  934(2)
	SetVIDSteps( 37, Helicopter->lpb.avg_res.roll );	//������������ ���� - ROLL  1224(1)
	SetVIDSteps( 38, Helicopter->lpb.avg_res.getPitch() );	//������������ ���� - PITCH  1214(3)
	SetVIDSteps( 39, Helicopter->lpb.avg_res.yawl );	//������������ ���� - YAWL 964(2)
// Helicopter->lpb.avg_res.index unsupported  // ������������ ���� - ������ 0,125 ������� �� �����
// Helicopter->lpb.avg_res.shift_pitch unsupported  // ������������ ����-��������� ����� ������� �������� �� ���������
	SetVIDSteps( 40, Helicopter->lpb.avg_res.blank );	//������������ ���� : ������ ���. �������  1214(4)
// Helicopter->lpb.avg_res.alignment unsupported  // ������������ ���� : �������� ���������
	Helicopter->lpb.avg_res.encoder = GetENCValue(2);	//
	SetVIDSteps( 41, Helicopter->lpb.ite_2t.eng1 );	//��������� 1 ������� - ������� (��� ��������)  964(3) 
	SetVIDSteps( 42, Helicopter->lpb.ite_2t.eng2 );	//��������� 2 ������� - ������� (��� ��������) 964(1)
	SetGPIOOUTBitValue( Helicopter->lpb.i_warning_earth || Helicopter->rsp_ep.t_check_lamp, 94 );	//��������� �������� �����
	SetGPIOOUTBitValue( Helicopter->lpb.i_danger_earth || Helicopter->rsp_ep.t_check_lamp, 95 );	//��������� ������ �����
	SetGPIOOUTBitValue( Helicopter->lpb.i_roll_max || Helicopter->rsp_ep.t_check_lamp, 96 );	//��������� ���� �����
	Helicopter->lpb.t_cso1_yellow = GetGPIOINBitValue( 291 );	//������ ���1 ������
	Helicopter->lpb.t_cso1_red = GetGPIOINBitValue( 292 );	//������ ���1 �������   
	SetGPIOOUTBitValue( Helicopter->lpb.i_cso1_yellow || Helicopter->rsp_ep.t_check_lamp, 97 );	//��������� ���1 ������
	SetGPIOOUTBitValue( Helicopter->lpb.i_cso1_red || Helicopter->rsp_ep.t_check_lamp, 98 );	//��������� ���1 �������   
	SetGPIOOUTBitValue( Helicopter->lpb.i_ag_net_kontr || Helicopter->rsp_ep.t_check_lamp, 99 );	//��������� ��� ����� ��
	SetGPIOOUTBitValue( Helicopter->lpb.i_ag_left_brok || Helicopter->rsp_ep.t_check_lamp, 100 );	//��������� ����� �� ���
	SetGPIOOUTBitValue( Helicopter->lpb.i_ag_res_brok || Helicopter->rsp_ep.t_check_lamp, 101 );	//��������� ����� �� ������
	SetGPIOOUTBitValue( Helicopter->lpb.i_main_gear_swarf || Helicopter->rsp_ep.t_check_lamp, 102 );	//��������� ������� ��. �����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_inter_gear_swarf || Helicopter->rsp_ep.t_check_lamp, 103 );	//��������� ������� ����. ���.
	SetGPIOOUTBitValue( Helicopter->lpb.i_tail_gear_swarf || Helicopter->rsp_ep.t_check_lamp, 104 );	//��������� ������� ��. �����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_vibr_dang || Helicopter->rsp_ep.t_check_lamp, 105 );	//��������� ����. ����. ���. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_swarf || Helicopter->rsp_ep.t_check_lamp, 106 );	//��������� ������� ���.����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_p_oil || Helicopter->rsp_ep.t_check_lamp, 107 );	//��������� ����. ��. ���. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_emrg || Helicopter->rsp_ep.t_check_lamp, 108 );	//��������� �.�.���.����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_nv_max || Helicopter->rsp_ep.t_check_lamp, 109 );	//��������� �.�.� �������
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_tf || Helicopter->rsp_ep.t_check_lamp, 110 );	//��������� �����. ��. ���. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_vibr_max || Helicopter->rsp_ep.t_check_lamp, 111 );	//��������� ����. ���. ���. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_max_rpm || Helicopter->rsp_ep.t_check_lamp, 112 );	//��������� ���������� �������� ��������� ������� ������ ���������.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_erd_off || Helicopter->rsp_ep.t_check_lamp, 113 );	//��������� ����. ���. ���. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_nv_min || Helicopter->rsp_ep.t_check_lamp, 114 );	//��������� �.�.� ������
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_vibr_dang || Helicopter->rsp_ep.t_check_lamp, 115 );	//��������� ���� ���� ���� ����
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_swarf || Helicopter->rsp_ep.t_check_lamp, 116 );	//��������� ������� ����.����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_p_oil || Helicopter->rsp_ep.t_check_lamp, 117 );	//��������� ���� ��. ����. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_emrg || Helicopter->rsp_ep.t_check_lamp, 118 );	//��������� �.�.����.����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_main_gear_p_oil || Helicopter->rsp_ep.t_check_lamp, 119 );	//��������� ���� �� �� �����
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_tf || Helicopter->rsp_ep.t_check_lamp, 120 );	//��������� �����. ��. ����. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_vibr_max || Helicopter->rsp_ep.t_check_lamp, 121 );	//��������� ����. ���. ����. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_max_rpm || Helicopter->rsp_ep.t_check_lamp, 122 );	//��������� ���������� �������� ��������� ������� ������� ���������
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_erd_off || Helicopter->rsp_ep.t_check_lamp, 123 );	//��������� ����. ���. ����. ����.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_limit || Helicopter->rsp_ep.t_check_lamp, 124 );	//��������� ����������� ������ ������ ���������
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_limit || Helicopter->rsp_ep.t_check_lamp, 125 );	//��������� ����������� ������ ������� ���������
// Helicopter->lpb.ki_206.course() unsupported  // KI206 ��������� �����(�������) �������� ���������� ������� ��������������
	Helicopter->lpb.ki_206.set_radial( MABNormalValue( 0 ) );	//����� ��������� 313(4) N=1371
	Helicopter->rpb.ki_206.set_radial(MABNormalValue(0));	//����� ��������� 313(4) N=1371
	SetVIDSteps( 43, Helicopter->lpb.ki_206.i_from_to );	//��������� ��������� FROM - TO  1234(3) TO=3492 FROM=118
	SetVIDSteps( 44, Helicopter->lpb.ki_206.i_gs );	//��������� ������ GS  1244(1) 1=1450 0=3107
	SetVIDSteps( 45, Helicopter->lpb.ki_206.i_nav );	//��������� ������ NAV  1244(2) 1=1438 0=2767
	SetVIDSteps( 46, Helicopter->lpb.ki_206.dir );	//��������� - ����� ��������� 1234(1) 0=2164 1373-2936
	SetVIDSteps( 47, Helicopter->lpb.ki_206.glis );	//��������� - ����� �����������  1234(2) 0=2159  1275-3164
// Helicopter->lpb.ki_206.roll unsupported  // ����
// 14 ����������� �����. unsupported  // 
	SetVIDSteps( 48, Helicopter->ccp.f_t_oil_inter_gb );	//����� T ����� ����. ���  914(1) (-50-207,150-2130)
	SetVIDSteps( 49, Helicopter->ccp.f_t_oil_tail_gb );	//����� T ����� �����. ���  904(2) -50=165 150=1738
	SetVIDSteps( 50, Helicopter->ccp.f_p_oil_main_gb );	//����� P ����� ��. �����. 914(4) (0-202,8-1798)
	SetVIDSteps( 51, Helicopter->ccp.f_t_oil_main_gb );	//����� T ����� ��. �����. 904(1) -50=243 150=1571
	SetVIDSteps( 52, Helicopter->ccp.f_p_oil_left_eng );	//����� ����� ��������� ��������  914(2) (0=226,8=1883)
	SetVIDSteps( 53, Helicopter->ccp.f_t_oil_left_eng );	//����� ����� ��������� T �����  914(3)(-50-157,150-1740)
	SetVIDSteps( 54, Helicopter->ccp.f_p_oil_rght_eng );	//����� ������ ��������� ��������  904(3) (0-289,8-1911)
	SetVIDSteps( 55, Helicopter->ccp.f_t_oil_rght_eng );	//����� ������ ��������� T �����  904(4)  (-50=195, 150=1786)
	Helicopter->ccp.t_ap_speed = GetGPIOINBitValue( 293 );	//������� ���� V
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_speed || Helicopter->rsp_ep.t_check_lamp, 126 );	//��������� ���� V
//  unsupported  // �������� ����� ���������� ����-2�
// Helicopter->ccp.t_zbn_on unsupported  // ������� ��� ���
	Helicopter->ccp.t_bur_on = GetGPIOINBitValue( 294 );	//������� ��� ���
	Helicopter->ccp.t_kontr_lamp = GetGPIOINBitValue( 295 );	//������ ����� ����(����� ���)
	SetGPIOOUTBitValue( Helicopter->ccp.i_otkaz_bur || Helicopter->ccp.t_kontr_lamp || Helicopter->rsp_ep.t_check_lamp, 127 );	//��������� ����� ���-1
	SetGPIOOUTBitValue( Helicopter->ccp.i_otkaz_en || Helicopter->ccp.t_kontr_lamp || Helicopter->rsp_ep.t_check_lamp, 128 );	//��������� ����� ��
// Helicopter->ccp.t_day_night unsupported  // ������� ����-����
// Helicopter->ccp.t_kontr_vvoda unsupported  // ������ ����� �����
	Helicopter->ccp.t_vvod = GetGPIOINBitValue( 296 );	//������ ����
// Helicopter->ccp.t_sdvig unsupported  // ������ �����
	Helicopter->ccp.t_cbros = GetGPIOINBitValue( 297 );	//������ �����
	Helicopter->ccp.t_ust = GetGPIOINBitValue( 298 );	//������ ���
	Helicopter->ccp.t_od = GetGPIOINBitValue( 299 );	//������ ��
	SetLEDINDICATORValue(Helicopter->ccp.led_bur, 3);	//��� ���
// Helicopter->ccp.f_1 unsupported  // ��������
// Helicopter->ccp.f_2 unsupported  // ��������
// Helicopter->ccp.f_3 unsupported  // ��������
	SetVIDSteps( 56, Helicopter->ccp.f_get_curs );	//������ ����������� 204(1) 10-410-830
	SetVIDSteps( 57, Helicopter->ccp.f_get_roll );	//������ ����  214(3) 380-890-1385 45
	SetVIDSteps( 58, Helicopter->ccp.f_get_pitch );	//������ ������  204(4)  30-390-710
	SetVIDSteps( 59, Helicopter->ccp.f_get_alt );	//������ ������ 204(2) 70-400-730
	SetVIDSteps( 60, Helicopter->ccp.f_spu52_ind );	//������ ���52 204(3) 110-320-710
	Helicopter->ccp.t_spu52_off = GetGPIOINBitValue( 300 );	//������ �������� ����(out 5-17)
	SetGPIOOUTBitValue( Helicopter->ccp.i_spu52_off || Helicopter->ccp.t_spu52_off || Helicopter->rsp_ep.t_check_lamp, 129 );	//��������� �������� ����(out 5-17)
	Helicopter->ccp.f_spu52 = POTNormalValue( 0 );	//�������� �������� 0:01:00
	Helicopter->ccp.t_spu52_p = GetGPIOINBitValue( 301 );	//������� �������� P
	Helicopter->ccp.t_spu52_t = GetGPIOINBitValue( 302 );	//������� �������� t
	Helicopter->ccp.t_sublight_ki_13 = GetGPIOINBitValue( 303 );	//������� ������� ��-13
	Helicopter->ccp.t_sublight_azs = GetGPIOINBitValue( 304 );	//������� ������� ������ ���
//  unsupported  // ������� ��
//  unsupported  // ������� ���������
	Helicopter->ccp.f_set_curs = MABNormalValue( 1 ) /*- 5.*/;	//��������� - �������� �����������  323(3)
	Helicopter->ccp.f_set_roll = MABNormalValue( 2 ) /*- 5.*/;	//��������� - �������� ����  323(2)
	Helicopter->ccp.f_set_pitch = MABNormalValue( 3 )/* - 5.*/;	//��������� - �������� ������ 323(1)
	Helicopter->ccp.t_set_curs = GetGPIOINBitValue( 305 );	//��������� - ������ ��  �����������  323(3)
	Helicopter->ccp.t_set_roll = GetGPIOINBitValue( 306 );	//��������� - ������ �� ����  323(2)
	Helicopter->ccp.t_set_pitch = GetGPIOINBitValue( 307 );	//��������� - ������ ������ 323(1)
	Helicopter->ccp.t_control = GetGPIOINBitValue( 308 );	//�������� � ������ ��� �������
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_dir_off || Helicopter->ccp.t_ap_dir_off || Helicopter->rsp_ep.t_check_lamp, 130 );	//��������� - �������� ����������� ����
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_dir_on || Helicopter->rsp_ep.t_check_lamp, 131 );	//��������� - �������� ����������� ���
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_on || Helicopter->rsp_ep.t_check_lamp, 132 );	//��������� - �������� ������� ���
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_alt_on || Helicopter->rsp_ep.t_check_lamp, 133 );	//�������� ������ ���
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_alt_off || Helicopter->ccp.t_ap_alt_off || Helicopter->rsp_ep.t_check_lamp, 134 );	//�������� ������ ����
	Helicopter->ccp.t_ap_dir_off = GetGPIOINBitValue( 309 );	//��������� - ������ ����������� ����
	Helicopter->ccp.t_ap_dir_on = GetGPIOINBitValue( 310 );	//��������� - ������ ����������� ���
	Helicopter->ccp.t_ap_on = GetGPIOINBitValue( 311 );	//��������� - ������ ������� ���
	Helicopter->ccp.t_ap_alt_on = GetGPIOINBitValue( 312 );	//��������� - ������ ������ ���
	Helicopter->ccp.t_ap_alt_off = GetGPIOINBitValue( 313 );	//��������� - ������ ������ ����
	Helicopter->ccp.t_main_off_ab = GetGPIOINBitValue( 314 );	//������� ������ ���� ��
	Helicopter->ccp.t_emerg_drop = GetGPIOINBitValue( 315 );	//������� �������� �����
	Helicopter->ccp.t_explosion = GetGPIOINBitValue( 316 );	//������� �����
	Helicopter->ccp.t_po1 = GetGPIOINBitValue( 317 );	//������� ��1
	Helicopter->ccp.t_po2 = GetGPIOINBitValue( 318 );	//������� ��2
	SetGPIOOUTBitValue( Helicopter->ccp.i_ab || Helicopter->rsp_ep.t_check_lamp, 135 );	//����� ��
	SetGPIOOUTBitValue( Helicopter->ccp.i_explosion || Helicopter->rsp_ep.t_check_lamp, 136 );	//����� �����
	Helicopter->ccp.t_cargo_cabin = GetGPIOINBitValue( 319 );	//������� ���� ������
	Helicopter->ccp.t_crew = GetGPIOINBitValue( 320 );	//������� ������
	Helicopter->ccp.t_day = GetGPIOINBitValue( 321 );	//������� ����
	Helicopter->ccp.t_night = GetGPIOINBitValue( 322 );	//������� ����
	SetGPIOOUTBitValue( Helicopter->ccp.i_power || Helicopter->rsp_ep.t_check_lamp, 137 );	//����� �������
	SetGPIOOUTBitValue( Helicopter->ccp.i_brace || Helicopter->rsp_ep.t_check_lamp, 138 );	//����� ��������
	SetGPIOOUTBitValue( Helicopter->ccp.i_removed || Helicopter->rsp_ep.t_check_lamp, 139 );	//����� ������
	SetGPIOOUTBitValue( Helicopter->ccp.i_work || Helicopter->rsp_ep.t_check_lamp, 140 );	//����� ������
	SetGPIOOUTBitValue( Helicopter->ccp.i_dropped || Helicopter->rsp_ep.t_check_lamp, 141 );	//����� ��������
	SetGPIOOUTBitValue( Helicopter->ccp.i_EL || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 142 );	//EL
	SetGPIOOUTBitValue( Helicopter->ccp.i_NO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 143 );	//NO
	SetGPIOOUTBitValue( Helicopter->ccp.i_VT || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 144 );	//VT
	SetGPIOOUTBitValue( Helicopter->ccp.i_PO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 145 );	//PO
	SetGPIOOUTBitValue( Helicopter->ccp.i_TZ || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 146 );	//TZ
	SetGPIOOUTBitValue( Helicopter->ccp.i_TO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 147 );	//TO
	SetGPIOOUTBitValue( Helicopter->ccp.i_SG || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 148 );	//SG
	SetGPIOOUTBitValue( Helicopter->ccp.i_OV || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 149 );	//OV
	SetGPIOOUTBitValue( Helicopter->ccp.i_ZV || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 150 );	//ZV
	Helicopter->ccp.t_ap_alt_up = GetGPIOINBitValue( 323 );	//��������� - ������ �������� ������ �����
	Helicopter->ccp.t_ap_alt_down = GetGPIOINBitValue( 324 );	//��������� - ������ �������� ������ ����
	//KT76
	SetLEDINDICATORValue(Helicopter->ccp.KT76_LCD, 6);
	Helicopter->ccp.KT76_t_IDT = GetGPIOINBitValue( 441 );
	Helicopter->ccp.KT76_t_0 = GetGPIOINBitValue( 442 );
	Helicopter->ccp.KT76_t_1 = GetGPIOINBitValue( 443 );
	Helicopter->ccp.KT76_t_2 = GetGPIOINBitValue( 444 );
	Helicopter->ccp.KT76_t_3 = GetGPIOINBitValue( 445 );
	Helicopter->ccp.KT76_t_4 = GetGPIOINBitValue( 446 );
	Helicopter->ccp.KT76_t_5 = GetGPIOINBitValue( 447 );
	Helicopter->ccp.KT76_t_6 = GetGPIOINBitValue( 448 );
	Helicopter->ccp.KT76_t_7 = GetGPIOINBitValue( 449 );
	Helicopter->ccp.KT76_t_CLR = GetGPIOINBitValue( 450 );
	Helicopter->ccp.KT76_t_VFR = GetGPIOINBitValue( 451 );
	Helicopter->ccp.KT76_gal_OFF = GetGPIOINBitValue( 452 );
	Helicopter->ccp.KT76_gal_SBY = GetGPIOINBitValue( 453 );
	Helicopter->ccp.KT76_gal_TST = GetGPIOINBitValue( 454 );
	Helicopter->ccp.KT76_gal_ON  = GetGPIOINBitValue( 455 );
	Helicopter->ccp.KT76_gal_ALT = GetGPIOINBitValue( 456 );
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_FL, 179);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_ALT, 180);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_ON, 181);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_R , 182);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_SBY, 183);
	//KN53
	SetLEDINDICATORValue(Helicopter->ccp.KN53_LCD_16, 7);
	SetLEDINDICATORValue(Helicopter->ccp.KN53_LCD, 8);
	Helicopter->ccp.KN53_str = GetGPIOINBitValue(457);
	Helicopter->ccp.KN53_on = GetGPIOINBitValue(458);
	Helicopter->ccp.KN53_freqM = GetENCValue(4);  // ������� ������������ ��� ������
	Helicopter->ccp.KN53_freqK = GetENCValue(5);  // ������� ������������ ��� ������
	Helicopter->ccp.KN53_pozuv = GetGPIOINBitValue(469); // ������������� �������� �� ��53
// 15 ������ ��������� ������. unsupported  // 
	Helicopter->rpb.t_dnmr = GetGPIOINBitValue(504);        // ������� ���/����
	Helicopter->rpb.t_fara_close = GetGPIOINBitValue( 325 );	//������� ���� ������
	Helicopter->rpb.t_fara_on = GetGPIOINBitValue( 326 );	//������� ���� ����
// Helicopter->rpb.t_projector unsupported  // ������� ������ ����
	SetVIDSteps( 61, Helicopter->rpb.f_fuel );	//����������  0 964(4)
	SetGPIOOUTBitValue( Helicopter->rpb.i_fuel_270 || Helicopter->rsp_ep.t_check_lamp, 151 );	//��������� �������� 270 �
	SetGPIOOUTBitValue( Helicopter->rpb.i_dptb_left || Helicopter->rsp_ep.t_check_lamp, 152 );	//��������� ���� ���� ���
	SetGPIOOUTBitValue( Helicopter->rpb.i_dptb_right || Helicopter->rsp_ep.t_check_lamp, 153 );	//��������� ���� ���� ����
// Helicopter->rpb.get_sw_fuel() unsupported  // ������������� �����������(���� ������?)
	Helicopter->rpb.t_fuel_0 = GetGPIOINBitValue( 327 );	//0 - ����
	Helicopter->rpb.t_fuel_1 = GetGPIOINBitValue( 328 );	//1 - ����� - ��������� ����� �������
	Helicopter->rpb.t_fuel_2 = GetGPIOINBitValue( 329 );	//2 - �� - ����� ���������
	Helicopter->rpb.t_fuel_3 = GetGPIOINBitValue( 330 );	//3 - ��� - ������ ���������
	Helicopter->rpb.t_fuel_4 = GetGPIOINBitValue( 331 );	//4 - ����. - ���������
	Helicopter->rpb.t_fuel_5 = GetGPIOINBitValue( 332 );	//5 - � - ������ ��������������
// Helicopter->rpb.get_dop_fuel() unsupported  // ������������� ��� �����
	Helicopter->rpb.t_fuel_low_left = GetGPIOINBitValue( 333 );	//����� ���
	Helicopter->rpb.t_fuel_low_right = GetGPIOINBitValue( 334 );	//����� ����
	Helicopter->rpb.t_fuel_hi_left = GetGPIOINBitValue( 335 );	//���� ���
	Helicopter->rpb.t_fuel_hi_right = GetGPIOINBitValue( 336 );	//���� ����
// Helicopter->rpb.kdi_572.sw_switch unsupported  // ������������� KDI-572 -OFF(����� ���������� �����������)
	Helicopter->rpb.kdi_572.t_N1 = GetGPIOINBitValue( 337 );	//N1
	Helicopter->rpb.kdi_572.t_HLD = GetGPIOINBitValue( 338 );	//HLD
	Helicopter->rpb.kdi_572.t_N2 = GetGPIOINBitValue( 339 );	//N2
	SetGPIOOUTBitValue( Helicopter->rpb.kdi_572.i_N1 || Helicopter->rsp_ep.t_check_lamp, 154 );	//��������� N1
	SetGPIOOUTBitValue( Helicopter->rpb.kdi_572.i_H || Helicopter->rsp_ep.t_check_lamp, 155 );	//��������� H
	SetGPIOOUTBitValue( Helicopter->rpb.kdi_572.i_N2 || Helicopter->rsp_ep.t_check_lamp, 156 );	//��������� N2
// Helicopter->rpb.kdi_572.i_HM unsupported  // ��������� HM
// Helicopter->rpb.kdi_572.i_RNV unsupported  // ��������� RNV
// Helicopter->rpb.kdi_572.i_KT unsupported  // ��������� KT
// Helicopter->rpb.kdi_572.i_MIN unsupported  // ��������� MIN
	SetLEDINDICATORValue( Helicopter->rpb.kdi_572.led, 1 );	//��������������� ���������
// Helicopter->rpb.kdi_572.f_1 unsupported  // ��������� KDI-572 1-��������
// Helicopter->rpb.kdi_572.f_2 unsupported  // ��������� KDI-572 2-���������
// Helicopter->rpb.kdi_572.f_3 unsupported  // ��������� KDI-572 3-�����
	Helicopter->rpb.t_alarm = GetGPIOINBitValue( 340 );	//������� ������������
	SetVIDSteps( 62, Helicopter->rpb.uc_450k.get_speed() );	//���������-������� p1 914(2) 0=104
	SetVIDSteps( 63, Helicopter->rpb.vd_10vk.get_km() );	//���������� (��)-������� 984(3) 0=2053
	SetCLOCKSteps( 5, Helicopter->rpb.vd_10vk.get_m()  );	//���������� (�)-�������  984(4) 0=2424
// Helicopter->rpb.vd_10vk.p unsupported  // ���������� �������� 670:790
	Helicopter->rpb.vd_10vk.f_barometr = 670.f + MABNormalValue(15);  // ���������� (�)-�������� 0:10:00
// Helicopter->rpb.vd_10vk.corrector unsupported  // 
	SetVIDSteps( 64, Helicopter->rpb.avg.roll );	//������������ ����-���� 0p 1204(3)  0=2001
	SetVIDSteps( 65, Helicopter->rpb.avg.getPitch() );	//������������ ����-������ 0p 1204(1)
	SetVIDSteps( 66, Helicopter->rpb.avg.yawl );	//������������ ����-�������� 1p 904(1) 0=399
// Helicopter->rpb.avg.index unsupported  // ������������ ����-������ :180
// Helicopter->rpb.avg.shift_pitch unsupported  // ������������ ����-��������� ����� �������-180:180
	SetVIDSteps( 67, Helicopter->rpb.avg.blank );	//������������ ����:������ ���. ������� 0p 1204(2)
// Helicopter->rpb.avg.alignment unsupported  // ������������ ���� �������� ���������
	Helicopter->rpb.avg.encoder = GetENCValue(3);	//600 7-8 
// Helicopter->rpb.avg.up unsupported  // 
// Helicopter->rpb.avg.down unsupported  // 
// Helicopter->rpb.ugr_4uk.dir() unsupported  // ��� - ������� ����
	SetCLOCKStepsUGR( 6, Helicopter->rpb.ugr_4uk.dir()  );	//���-������� ����  954(3)  0=
// Helicopter->rpb.ugr_4uk.d1 unsupported  // ��� - ������� �������
	SetCLOCKSteps( 7, Helicopter->rpb.ugr_4uk.ark()  );	//��� - ������ �������  p1 284(3)  0=
// Helicopter->rpb.ugr_4uk.f_set_course unsupported  // ��� - ���������
// Helicopter->rpb.ugr_4uk.corrector unsupported  // 
	SetVIDSteps( 68, Helicopter->rpb.snos.angle );	//���� ����� 964(1) (0 �������� � 2144)  -40=3462,40875
// Helicopter->rpb.snos.speed unsupported  // DISS ��������� � ��/���
	SetGPIOOUTBitValue( Helicopter->rpb.snos.i_angle || Helicopter->rsp_ep.t_check_lamp, 157 );	//���� ����� 
	Helicopter->rpb.snos.t_pk = GetGPIOINBitValue( 341 );	//���� ����� - ������������� �-� true-� 
	Helicopter->rpb.snos.t_cm = GetGPIOINBitValue( 342 );	//���� ����� - ������������� �-� true = � 
	SetVIDSteps( 69, Helicopter->rpb.vr_30mk.var );	//������/����� �������  974(2) 0=104
	SetVIDSteps( 70, Helicopter->rpb.ite_1t.nv );	//������� ���� ������� - ������� 904(2) (0=0, 105=4320)
	SetVIDSteps( 71, Helicopter->rpb.ite_2t.eng1 );	//��������� 1 ������� - ������� 904(4) 0=745
	SetVIDSteps( 72, Helicopter->rpb.ite_2t.eng2 );	//��������� 2 ������� - ������� 914(3) 0=4120
	Helicopter->rpb.diss_15.t_rght = GetGPIOINBitValue( 343 );	//������ ���
	Helicopter->rpb.diss_15.t_left = GetGPIOINBitValue( 344 );	//������ ��
// Helicopter->rpb.diss_15.sw_side unsupported  // ������� ��������� �����-������
// Helicopter->rpb.diss_15.get_side() unsupported  // ������� ���������-��������
	Helicopter->rpb.diss_15.t_v = GetGPIOINBitValue( 345 );	//������ �
	Helicopter->rpb.diss_15.t_n = GetGPIOINBitValue( 346 );	//������ �
// Helicopter->rpb.diss_15.get_way() unsupported  // ����-��������   TODO : ��������� ���� !!!
// Helicopter->rpb.diss_15.sw_way unsupported  // ���� ������-�����
	Helicopter->rpb.diss_15.t_plus = GetGPIOINBitValue( 347 );	//������ +
	Helicopter->rpb.diss_15.t_minus = GetGPIOINBitValue( 348 );	//������ -
// Helicopter->rpb.diss_15.f_angle unsupported  // ���� �����-�������� TODO ������� ��� �� ��������� ����
	Helicopter->rpb.diss_15.t_on = GetGPIOINBitValue( 349 );	//������ ���
	Helicopter->rpb.diss_15.t_off = GetGPIOINBitValue( 350 );	//������ ����
// Helicopter->rpb.diss_15.i_on unsupported  // ���� - ����� ��������
// !Helicopter->rpb.diss_15.i_on unsupported  // ���� - ����� ���� (�������� �� ���)
	SetGPIOOUTBitValue( Helicopter->rpb.i_diss_brok || Helicopter->rsp_ep.t_check_lamp, 158 );	//��������� ���� �����
	SetGPIOOUTBitValue( Helicopter->rpb.i_ag_brok || Helicopter->rsp_ep.t_check_lamp, 159 );	//��������� ����� �� ����
	SetGPIOOUTBitValue( Helicopter->rpb.i_warning_earth || Helicopter->rsp_ep.t_check_lamp, 160 );	//��������� �������� �����
	SetGPIOOUTBitValue( Helicopter->rpb.i_danger_earth || Helicopter->rsp_ep.t_check_lamp, 161 );	//��������� ������ �����
	SetGPIOOUTBitValue( Helicopter->rpb.i_pitch_max || Helicopter->rsp_ep.t_check_lamp, 162 );	//��������� ������ �����
	SetGPIOOUTBitValue( Helicopter->rpb.i_roll_max || Helicopter->rsp_ep.t_check_lamp, 163 );	//��������� ���� ����� bool IP->Par022
// Helicopter->rpb.ki_206.course() unsupported  // KI206 - ������� ����
	SetVIDSteps( 73, Helicopter->rpb.ki_206.i_from_to );	//KI206 ��������� FROM TO 1 1264(3) fr=1082, to=3224
	SetVIDSteps( 74, Helicopter->rpb.ki_206.i_gs );	//KI206 ������ GS
	SetVIDSteps( 75, Helicopter->rpb.ki_206.i_nav );	//KI206 ������ NAV
	SetVIDSteps( 76, Helicopter->rpb.ki_206.dir );	//KI206 - ����� ��������� 1 1264(1) 1373-2936
	SetVIDSteps( 77, Helicopter->rpb.ki_206.glis );	//KI206 - ����� ����������� 1 1264(2) 1275-3164
	SetVIDSteps( 78, Helicopter->rpb.f_temp );	//����������� � ������������ ������ 904(3) -60=207,602338
// 16 ����������� �����(��������� �����). unsupported  // 
// Helicopter->cpc.f_kt74_left unsupported  // �������� ��-74 �����
// Helicopter->cpc.t_msg unsupported  // ������ MSG
// Helicopter->cpc.t_obs unsupported  // ������ OBS
// Helicopter->cpc.t_alt unsupported  // ������ ALT
// Helicopter->cpc.t_nrst unsupported  // ������ NPRST
// Helicopter->cpc.t_d unsupported  // ������ D
// Helicopter->cpc.t_clr unsupported  // ������ CLR
// Helicopter->cpc.t_ent unsupported  // ������ ENT
// Helicopter->cpc.f_kt74_right unsupported  // �������� ��-74 ������
// Helicopter->cpc.i_kn62_use unsupported  // ��������� KN-62 USE
// Helicopter->cpc.i_kn62_stby unsupported  // ��������� KN-62 STBY
// Helicopter->cpc.t_kt74_ident unsupported  // KT-74 ������ IDENT
// Helicopter->cpc.t_kt74_vfr unsupported  // KT-74 ������ VFR
// Helicopter->cpc.t_kt74_0 unsupported  // KT-74 ������ 0
// Helicopter->cpc.t_kt74_1 unsupported  // KT-74 ������ 1
// Helicopter->cpc.t_kt74_2 unsupported  // KT-74 ������ 2
// Helicopter->cpc.t_kt74_3 unsupported  // KT-74 ������ 3
// Helicopter->cpc.t_kt74_4 unsupported  // KT-74 ������ 4
// Helicopter->cpc.t_kt74_5 unsupported  // KT-74 ������ 5
// Helicopter->cpc.t_kt74_6 unsupported  // KT-74 ������ 6
// Helicopter->cpc.t_kt74_7 unsupported  // KT-74 ������ 7
// Helicopter->cpc.t_kt74_ent unsupported  // KT-74 ������ ENT
// Helicopter->cpc.t_kt74_func unsupported  // KT-74 ������ FUNC
// Helicopter->cpc.f_kt74 unsupported  // ��-74 ��������
// Helicopter->cpc.sw_kn62a unsupported  // KN-62A ������� �������������
// Helicopter->cpc.t_kn62a_off unsupported  // KN-62A �����������
// Helicopter->cpc.f_kn62 unsupported  // KN-62A ��������
// 17 ������ � ������. unsupported  // 
	Helicopter->PC.f_roll = MABNormalValue( 4 );	//������� �����-������ 353(1) 1569-645  ������ 691-1491
	Helicopter->PC.f_pitch = MABNormalValue( 5 );	//������� ������-����� 353(3) 2803-3432  ������ 2596-3438
// Helicopter->PC.t_right_spu1 unsupported  // ��� ������� - ������ ��� �����(��������)
// Helicopter->PC.t_left_trim unsupported  // ��� ������� - ������ ���������������� ������(��������)
	Helicopter->PC.t_left_fire = GetGPIOINBitValue( 351 );	//��� ������� - ����� ������� ������
	Helicopter->PC.t_left_ap_off = GetGPIOINBitValue( 352 );	//��� ������� - ������ ���������� ����������
// Helicopter->PC.t_right_spu2 unsupported  // ���� ������� -������ ��� ����� 0-����, 1-������, 2- ���������(��������)
// Helicopter->PC.t_rght_centr unsupported  // ���� ������� - ������ ���������������� ������(��������)
	Helicopter->PC.t_right_fire = GetGPIOINBitValue( 353 );	//���� �������-����� ������� ������
	Helicopter->PC.t_right_ap_off = GetGPIOINBitValue( 354 );	//���� ������� - ������ ���������� ����������
// Helicopter->PC.t_fric unsupported  // ������� ��������� 0:1,0-�� ����, 1-�� ����
	Helicopter->PC.t_fpp_l_up = GetGPIOINBitValue( 355 );	//������ ���������� ����� �����
	Helicopter->PC.t_fpp_l_down = GetGPIOINBitValue( 356 );	//������ ���������� ����� ����
	Helicopter->PC.t_fpp_l_left = GetGPIOINBitValue( 357 );	//������ ���������� ����� �����
	Helicopter->PC.t_fpp_l_right = GetGPIOINBitValue( 358 );	//������ ���������� ����� ������
	Helicopter->PC.t_avar_drop = GetGPIOINBitValue( 359 );	//������ ���������� ������ �����
	Helicopter->PC.t_tact_drop = GetGPIOINBitValue( 360 );	//������ ������������ ������ �����
	Helicopter->PC.t_fr_l_fosh = GetGPIOINBitValue( 437 );  // ������ ���������
	Helicopter->PC.t_rpm_up = GetGPIOINBitValue( 361 );	//�������
	Helicopter->PC.t_rpm_down = GetGPIOINBitValue( 362 );	//�������
	Helicopter->PC.t_rpm_right_up = GetGPIOINBitValue( 363 );	//�������
	Helicopter->PC.t_rpm_right_down = GetGPIOINBitValue( 364 );	//�������
	Helicopter->PC.t_fpp_r_up = GetGPIOINBitValue( 365 );	//������ ���������� ����� �����
	Helicopter->PC.t_fpp_r_down = GetGPIOINBitValue( 366 );	//������ ���������� ����� ���� 
	Helicopter->PC.t_fpp_r_left = GetGPIOINBitValue( 367 );	//������ ���������� ����� �����
	Helicopter->PC.t_fpp_r_right = GetGPIOINBitValue( 368 );	//������ ���������� ����� ������
	Helicopter->PC.t_tact_drop = GetGPIOINBitValue( 369 );	//������ ������������ ������ �����
// Helicopter->PC.t_fr_l_fosh unsupported  // ������ ��������� bool ��� ���� �� ����
	Helicopter->PC.f_eng1 = MABNormalValue( 6 );	//��� �����  343(3) 1180-230 1697-225
	Helicopter->PC.f_eng2 = MABNormalValue( 7 );	//��� ������ 343(4) 2627-962  2058-1233
	Helicopter->PC.f_Fosh = MABNormalValue( 8 );	//�������� ���-��� 343(1) 1698-962
	Helicopter->PC.f_Ecor = MABNormalValue( 9 );	//����� ��������� 353(1) �=3450 �=3784
	Helicopter->PC.f_yawl = MABNormalValue( 10 );	//������ ��� ����� 343(2)  23-1053
	Helicopter->PC.f_break = MABNormalValue( 11 );	//������ ����� 323(4)  1496-2424
	Helicopter->PC.t_NV_brake_on = GetGPIOINBitValue( 370 );	//����� ������� �����
	Helicopter->PC.t_NV_brake_off = GetGPIOINBitValue( 371 );	//����� ������� �����
// Helicopter->PC.f_nv_brake unsupported  // ����� ������� ����� ������
	Helicopter->PC.t_eng_rght_brake = GetGPIOINBitValue( 372 );	//��������� ��������� ����
	Helicopter->PC.t_eng_left_brake = GetGPIOINBitValue( 373 );	//��������� ��������� ���
	Helicopter->PC.t_safety_lock = GetGPIOINBitValue( 374 );	//����������������� ����������� ����������
	SetGPIOOUTBitValue( Helicopter->PC.i_weapon_warning || Helicopter->rsp_ep.t_check_lamp, 164 );	//����� ��������� ���� ���������� ��� �����
	Helicopter->PC.t_bomb_drop_left = GetGPIOINBitValue( 375 );	//������ ������ ����
	Helicopter->PC.t_bomb_drop_right = GetGPIOINBitValue( 376 );	//������ ������ ����
	SetCLOCKSteps( 8, Helicopter->PC.f_compass  );	//��������� ������
	SetGPIOOUTBitValue( Helicopter->PC.reset, 165 );	//����� �������
	SetGPIOOUTBitValue( Helicopter->PC.vibro, 166 );	//��������� ����������� 0-1-0
	Helicopter->PC.t_ap_dir_sopr = GetGPIOINBitValue( 377 ) || GetGPIOINBitValue(438) || GetGPIOINBitValue(439) || GetGPIOINBitValue(440);	//���������� ���������� �� �������
	SetVIDSteps( 79, Helicopter->PC.f_vibro );	//������� �������
	Helicopter->PC.pkt_fire = GetGPIOINBitValue( 378 );	//�������� �� ��������
	Helicopter->PC.pkt_load = GetGPIOINBitValue( 379 );	//�����
	Helicopter->PC.pkt_x = MABNormalValue( 12 );	//������� �������� �� �����������
	Helicopter->PC.pkt_y = MABNormalValue( 13 );	//������� �������� �� ���������
// 18 ����� ���������� ���������. unsupported  // 
// Helicopter->_BoardSystem->get_sw_weapon_type() unsupported  // �������� �������������
// Helicopter->WeaponPult.t_MV unsupported  // MV
	Helicopter->WeaponPult.t_PKT = GetGPIOINBitValue( 380 );	//PKT
	Helicopter->WeaponPult.t_FAB = GetGPIOINBitValue( 381 );	//FAB
	Helicopter->WeaponPult.t_NAR = GetGPIOINBitValue( 382 );	//NAR
	Helicopter->WeaponPult.t_UPK23 = GetGPIOINBitValue( 383 );	//UPK23
// Helicopter->WeaponPult.t_AGS17 unsupported  // AGS17
	Helicopter->WeaponPult.t_fire_short = GetGPIOINBitValue( 384 );	//������� ������� ��� ��������
// Helicopter->WeaponPult.t_fire_long unsupported  // ������� ������� ��� �������(������� ��������� ��������)
	Helicopter->WeaponPult.t_fire_midle = GetGPIOINBitValue( 385 );	//������� ������� ��� �������
	Helicopter->WeaponPult.t_pod_ext = GetGPIOINBitValue( 386 );	//������� �������� �����
// Helicopter->WeaponPult.t_pod_all unsupported  // ������� �������� ���(������� ��������� ��������)
	Helicopter->WeaponPult.t_pod_inter = GetGPIOINBitValue( 387 );	//������� �������� 
// 19 ����� �������� ����. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_m || Helicopter->rsp_ep.t_check_lamp, 167 );	//��������� �
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_contr || Helicopter->rsp_ep.t_check_lamp, 168 );	//��������� �����
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_work || Helicopter->rsp_ep.t_check_lamp, 169 );	//��������� ������
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_b || Helicopter->rsp_ep.t_check_lamp, 170 );	//��������� �
// Helicopter->pc_diss.sw_mode unsupported  // �������� �������������
	Helicopter->pc_diss.t_p1 = GetGPIOINBitValue( 388 );	//
	Helicopter->pc_diss.t_p2 = GetGPIOINBitValue( 389 );	//
	Helicopter->pc_diss.t_p3 = GetGPIOINBitValue( 390 );	//
	Helicopter->pc_diss.t_p4 = GetGPIOINBitValue( 391 );	//
	Helicopter->pc_diss.t_p5 = GetGPIOINBitValue( 392 );	//
	/*********����-32**********/
	Helicopter->pc_diss.t_contr = GetGPIOINBitValue( 463 );
	Helicopter->pc_diss.t_poisk = GetGPIOINBitValue( 464 );
	Helicopter->pc_diss.t_up_left = GetGPIOINBitValue(465);
	Helicopter->pc_diss.t_back_right = GetGPIOINBitValue(466);
	Helicopter->pc_diss.t_spedd127 = GetGPIOINBitValue( 467 );
	Helicopter->pc_diss.t_speed258 = GetGPIOINBitValue( 468 );
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_vss || Helicopter->rsp_ep.t_check_lamp, 188);
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_vch || Helicopter->rsp_ep.t_check_lamp, 189 );
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_bvk || Helicopter->rsp_ep.t_check_lamp, 190 );
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_flight || Helicopter->rsp_ep.t_check_lamp, 191);
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_ispr || Helicopter->rsp_ep.t_check_lamp, 192);
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_search || Helicopter->rsp_ep.t_check_lamp, 193);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_contr_flag || Helicopter->rsp_ep.t_check_lamp, 197);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_poisk_flag || Helicopter->rsp_ep.t_check_lamp, 198);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_up_left ||  Helicopter->rsp_ep.t_check_lamp, 199);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_back_right || Helicopter->rsp_ep.t_check_lamp, 200);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_spedd127 || Helicopter->rsp_ep.t_check_lamp, 201);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_speed258 || Helicopter->rsp_ep.t_check_lamp, 202);
// 20 ����� ��������������������. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_1_load || Helicopter->rsp_ep.t_check_lamp, 171 );	//��������� ��1 ������
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_2_load || Helicopter->rsp_ep.t_check_lamp, 172 );	//��������� ��2 ������
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_3_load || Helicopter->rsp_ep.t_check_lamp, 173 );	//��������� ��3 ������
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_4_load || Helicopter->rsp_ep.t_check_lamp, 174 );	//��������� ��4 ������
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_vzriv || Helicopter->rsp_ep.t_check_lamp, 175 );	//��������� ���� �����
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_weapon_line || Helicopter->rsp_ep.t_check_lamp, 176 );	//��������� ���� �� �����
	Helicopter->WeaponPult.t_pus_load_PO1 = GetGPIOINBitValue( 393 );	//������� ��1
	Helicopter->WeaponPult.t_pus_load_PO2 = GetGPIOINBitValue( 394 );	//������� ��2(����� ����������)
	Helicopter->WeaponPult.t_main_switch = GetGPIOINBitValue( 395 );	//������� ������� ������
	Helicopter->WeaponPult.t_vzriv = GetGPIOINBitValue( 396 );	//������� �����
	Helicopter->WeaponPult.t_sbros = GetGPIOINBitValue( 397 );	//������� ����� ����
// � ��� �������� �������. unsupported  // 
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[0] = GetGPIOINBitValue( 398 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[1] = GetGPIOINBitValue( 399 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[2] = GetGPIOINBitValue( 400 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[3] = GetGPIOINBitValue( 401 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[4] = GetGPIOINBitValue( 402 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[0] = GetGPIOINBitValue( 403 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[1] = GetGPIOINBitValue( 404 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[2] = GetGPIOINBitValue( 405 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[3] = GetGPIOINBitValue( 406 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[4] = GetGPIOINBitValue( 407 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[0] = GetGPIOINBitValue( 408 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[1] = GetGPIOINBitValue( 409 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[2] = GetGPIOINBitValue( 410 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[3] = GetGPIOINBitValue( 411 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[4] = GetGPIOINBitValue( 412 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[0] = GetGPIOINBitValue( 413 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[1] = GetGPIOINBitValue( 414 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[2] = GetGPIOINBitValue( 415 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[3] = GetGPIOINBitValue( 416 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[4] = GetGPIOINBitValue( 417 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[0] = GetGPIOINBitValue( 418 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[1] = GetGPIOINBitValue( 419 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[2] = GetGPIOINBitValue( 420 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[3] = GetGPIOINBitValue( 421 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[4] = GetGPIOINBitValue( 422 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[0] = GetGPIOINBitValue(423);	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[1] = GetGPIOINBitValue( 424 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[2] = GetGPIOINBitValue( 425 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[3] = GetGPIOINBitValue( 426 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[4] = GetGPIOINBitValue( 427 );	//
// ����� ���������� ���������. unsupported  // 
// Helicopter->UV26.tablo unsupported  // 
	SetGPIOOUTBitValue( Helicopter->UV26.i_left || Helicopter->rsp_ep.t_check_lamp, 177 );	//
	SetGPIOOUTBitValue( Helicopter->UV26.i_right || Helicopter->rsp_ep.t_check_lamp, 178 );	//
	Helicopter->UV26.t_left = GetGPIOINBitValue( 428 );	//
	Helicopter->UV26.t_right = GetGPIOINBitValue( 429 );	//
	Helicopter->UV26.t_prog = GetGPIOINBitValue( 430 );	//
	Helicopter->UV26.t_seria = GetGPIOINBitValue( 431 );	//
	Helicopter->UV26.t_zalp = GetGPIOINBitValue( 432 );	//
	Helicopter->UV26.t_stop = GetGPIOINBitValue( 433 ) || GetGPIOINBitValue( 507 );	//
	Helicopter->UV26.t_interval = GetGPIOINBitValue( 434 );	//
	Helicopter->UV26.t_reset = GetGPIOINBitValue( 435 );	//
	Helicopter->UV26.t_pusk = GetGPIOINBitValue( 436 ) || GetGPIOINBitValue( 506 );	//
	SetLEDINDICATORValue(Helicopter->_BoardSystem->tablo, 2 );	//������� ����� ��� DISS � ��26
// ����� �������� ����������� 
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_imitation_hot, 185);
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_imitation_cold, 186);
	Helicopter->lp_elp.t_imitation = GetGPIOINBitValue(460);
// ���-7 ������ ������
	Helicopter->lpb.spu.t_ukr = GetGPIOINBitValue( 470 );         // �������� ���
	Helicopter->lpb.spu.t_sr = GetGPIOINBitValue( 471 );          // �������� ��
	Helicopter->lpb.spu.t_kr = GetGPIOINBitValue( 472 );          // �������� ��
	Helicopter->lpb.spu.t_dr = GetGPIOINBitValue( 473 );          // �������� ��
	Helicopter->lpb.spu.t_rk1 = GetGPIOINBitValue( 474 );         // �������� ��1
	Helicopter->lpb.spu.t_rk2 = GetGPIOINBitValue( 475 );         // �������� ��2
	Helicopter->lpb.spu.t_set_1 = GetGPIOINBitValue( 478 );      // ������� ���� 1
	Helicopter->lpb.spu.t_set_2 = GetGPIOINBitValue( 479 );      // ������� ���� 2
	Helicopter->lpb.spu.k_cv = GetGPIOINBitValue( 480 );          // ������ ��
	Helicopter->lpb.spu.t_spu = GetGPIOINBitValue( 476 );         // ������� ���
	Helicopter->lpb.spu.t_radio = GetGPIOINBitValue( 477 );       // ������� �����
// ���-7 ������� ������
	Helicopter->rpb.spu.t_ukr = GetGPIOINBitValue(481);         // �������� ���
	Helicopter->rpb.spu.t_sr = GetGPIOINBitValue(482);          // �������� ��
	Helicopter->rpb.spu.t_kr = GetGPIOINBitValue(483);          // �������� ��
	Helicopter->rpb.spu.t_dr = GetGPIOINBitValue(484);          // �������� ��
	Helicopter->rpb.spu.t_rk1 = GetGPIOINBitValue(485);         // �������� ��1
	Helicopter->rpb.spu.t_rk2 = GetGPIOINBitValue(486);         // �������� ��2
	Helicopter->rpb.spu.t_set_1 = GetGPIOINBitValue(489);      // ������� ���� 1
	Helicopter->rpb.spu.t_set_2 = GetGPIOINBitValue(490);      // ������� ���� 2
	Helicopter->rpb.spu.k_cv = GetGPIOINBitValue(491);          // ������ ��
	Helicopter->rpb.spu.t_spu = GetGPIOINBitValue(487);         // ������� ���
	Helicopter->rpb.spu.t_radio = GetGPIOINBitValue(488);       // ������� �����
// ���-7 ���� �������
	Helicopter->ep_elp.spu.t_ukr = GetGPIOINBitValue(492);        // �������� ���
	Helicopter->ep_elp.spu.t_sr = GetGPIOINBitValue(493);         // �������� ��
	Helicopter->ep_elp.spu.t_kr = GetGPIOINBitValue(494);         // �������� ��
	Helicopter->ep_elp.spu.t_dr = GetGPIOINBitValue(495);         // �������� ��
	Helicopter->ep_elp.spu.t_rk1 = GetGPIOINBitValue(496);        // �������� ��1
	Helicopter->ep_elp.spu.t_rk2 = GetGPIOINBitValue(497);        // �������� ��2
	Helicopter->ep_elp.spu.t_set_1 = GetGPIOINBitValue(500);     // ������� ���� 1
	Helicopter->ep_elp.spu.t_set_2 = GetGPIOINBitValue(501);     // ������� ���� 2
	Helicopter->ep_elp.spu.k_cv = GetGPIOINBitValue(502);         // ������ ��
	Helicopter->ep_elp.spu.t_spu = GetGPIOINBitValue(498);        // ������� ���
	Helicopter->ep_elp.spu.t_radio = GetGPIOINBitValue(499);      // ������� �����
}
