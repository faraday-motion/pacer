#include "./vesc_buffer.h"
#include "./vesc_interface.h"
#include <cstring>
#include <functional>
#include "../../logs/logger.h"

/**
 * Enable or disable can forwarding to other VESCs.
 *
 * @param vesc_id
 * The VESC ID to forward to. Setting this to -1 disables this feature.
 */
void Vesc_interface::set_forward_can(int32_t vesc_id) {
	can_fwd_vesc = vesc_id;
}

/**
 * Send a packet using the set send function.
 *
 * @param data
 * The packet data.
 *
 * @param len
 * The data length.
 */
void Vesc_interface::send(unsigned char *data, unsigned int len) {
	//Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_interface::send: ******************"));
	if (pSerial != nullptr)
		pSerial -> write(data, len);
}

void Vesc_interface::loop()
{
	while (pSerial -> available() > 0)
	{
		//Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_interface::pSerial -> available() "));
		pPacket -> addRx(pSerial -> read());
	}
}

/**
 * Process a received buffer with commands and data.
 *
 * @param data
 * The buffer to process.
 *
 * @param len
 * The length of the buffer.
 */
void Vesc_interface::recieve(unsigned char *data, unsigned int len) {
	//Logger::Instance().write(LogLevel::DEBUG, FPSTR("Vesc_interface::recieve ****************** "));
	if (!len) {
		return;
	}

	int32_t ind = 0;
	int i = 0;
	unsigned char id = data[0];
	data++;
	len--;

	switch (id) {
	case COMM_FW_VERSION:
		if (len == 2) {
			ind = 0;
			fw_major = data[ind++];
			fw_minor = data[ind++];
		} else {
			fw_major = -1;
			fw_minor = -1;
		}
		break;

	case COMM_ERASE_NEW_APP:
	case COMM_WRITE_NEW_APP_DATA:
		// TODO
		break;

	case COMM_GET_VALUES:
		ind = 0;
		values.temp_mos1 = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.temp_mos2 = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.temp_mos3 = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.temp_mos4 = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.temp_mos5 = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.temp_mos6 = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.temp_pcb = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.current_motor = Vesc_buffer::buffer_get_float32(data, 100.0, &ind);
		values.current_in = Vesc_buffer::buffer_get_float32(data, 100.0, &ind);
		values.duty_now = Vesc_buffer::buffer_get_float16(data, 1000.0, &ind);
		values.rpm = Vesc_buffer::buffer_get_float32(data, 1.0, &ind);
		values.v_in = Vesc_buffer::buffer_get_float16(data, 10.0, &ind);
		values.amp_hours = Vesc_buffer::buffer_get_float32(data, 10000.0, &ind);
		values.amp_hours_charged = Vesc_buffer::buffer_get_float32(data, 10000.0, &ind);
		values.watt_hours = Vesc_buffer::buffer_get_float32(data, 10000.0, &ind);
		values.watt_hours_charged = Vesc_buffer::buffer_get_float32(data, 10000.0, &ind);
		values.tachometer = Vesc_buffer::buffer_get_int32(data, &ind);
		values.tachometer_abs = Vesc_buffer::buffer_get_int32(data, &ind);
		values.fault_code = (mc_fault_code)data[ind++];

		if (pInterface != nullptr) {
			pInterface -> setValues(&values);
		}
		break;

	case COMM_PRINT:
		if (pInterface != nullptr) {
			data[len] = '\0';
			pInterface -> print((char*)data);
		}
		break;

	case COMM_SAMPLE_PRINT:
		// TODO
		break;

	case COMM_ROTOR_POSITION:
		ind = 0;
		rotor_pos = Vesc_buffer::buffer_get_float32(data, 100000.0, &ind);

		if (pInterface != nullptr) {
			pInterface -> setRotorPos(rotor_pos);
		}
		break;

	case COMM_EXPERIMENT_SAMPLE:
		// TODO
		break;

	case COMM_GET_MCCONF:
	case COMM_GET_MCCONF_DEFAULT:
		ind = 0;
		mcconf.pwm_mode = (mc_pwm_mode)data[ind++];
		mcconf.comm_mode = (mc_comm_mode)data[ind++];
		mcconf.motor_type = (mc_motor_type)data[ind++];
		mcconf.sensor_mode = (mc_sensor_mode)data[ind++];

		mcconf.l_current_max = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_current_min = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_in_current_max = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_in_current_min = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_abs_current_max = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_min_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_max_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_max_erpm_fbrake = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_max_erpm_fbrake_cc = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_min_vin = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_max_vin = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_battery_cut_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_battery_cut_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_slow_abs_current = data[ind++];
		mcconf.l_rpm_lim_neg_torque = data[ind++];
		mcconf.l_temp_fet_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_temp_fet_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_temp_motor_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_temp_motor_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.l_min_duty = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.l_max_duty = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);

		mcconf.sl_min_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.sl_min_erpm_cycle_int_limit = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.sl_max_fullbreak_current_dir_change = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.sl_cycle_int_limit = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.sl_phase_advance_at_br = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.sl_cycle_int_rpm_br = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.sl_bemf_coupling_k = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);

		memcpy(mcconf.hall_table, data + ind, 8);
		ind += 8;
		mcconf.hall_sl_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);

		mcconf.foc_current_kp = Vesc_buffer::buffer_get_float32(data, 1e5, &ind);
		mcconf.foc_current_ki = Vesc_buffer::buffer_get_float32(data, 1e5, &ind);
		mcconf.foc_f_sw = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_dt_us = Vesc_buffer::buffer_get_float32(data, 1e6, &ind);
		mcconf.foc_encoder_inverted = data[ind++];
		mcconf.foc_encoder_offset = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_encoder_ratio = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_sensor_mode = (mc_foc_sensor_mode)data[ind++];
		mcconf.foc_pll_kp = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_pll_ki = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_motor_l = Vesc_buffer::buffer_get_float32(data, 1e8, &ind);
		mcconf.foc_motor_r = Vesc_buffer::buffer_get_float32(data, 1e5, &ind);
		mcconf.foc_motor_flux_linkage = Vesc_buffer::buffer_get_float32(data, 1e5, &ind);
		mcconf.foc_observer_gain = Vesc_buffer::buffer_get_float32(data, 1e0, &ind);
		mcconf.foc_duty_dowmramp_kp = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_duty_dowmramp_ki = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_openloop_rpm = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_sl_openloop_hyst = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_sl_openloop_time = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_sl_d_current_duty = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		mcconf.foc_sl_d_current_factor = Vesc_buffer::buffer_get_float32(data, 1e3, &ind);
		memcpy(mcconf.foc_hall_table, data + ind, 8);
		ind += 8;
		mcconf.foc_hall_sl_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);

		mcconf.s_pid_kp = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.s_pid_ki = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.s_pid_kd = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.s_pid_min_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);

		mcconf.p_pid_kp = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.p_pid_ki = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.p_pid_kd = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.p_pid_ang_div = Vesc_buffer::buffer_get_float32(data, 1e5, &ind);

		mcconf.cc_startup_boost_duty = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.cc_min_current = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		mcconf.cc_gain = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.cc_ramp_step_max = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);

		mcconf.m_fault_stop_time_ms = Vesc_buffer::buffer_get_int32(data, &ind);
		mcconf.m_duty_ramp_step = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.m_duty_ramp_step_rpm_lim = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.m_current_backoff_gain = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		mcconf.m_encoder_counts = Vesc_buffer::buffer_get_uint32(data, &ind);

		if (pInterface != nullptr) {
			pInterface -> setMotorConfiguration(&mcconf);
		}
		break;

	case COMM_GET_APPCONF:
	case COMM_GET_APPCONF_DEFAULT:
		ind = 0;
		appconf.controller_id = data[ind++];
		appconf.timeout_msec = Vesc_buffer::buffer_get_uint32(data, &ind);
		appconf.timeout_brake_current = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.send_can_status = data[ind++];
		appconf.send_can_status_rate_hz = Vesc_buffer::buffer_get_uint16(data, &ind);

		appconf.app_to_use = (app_use)data[ind++];

		appconf.app_ppm_conf.ctrl_type = (ppm_control_type)data[ind++];
		appconf.app_ppm_conf.pid_max_erpm = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_ppm_conf.hyst = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_ppm_conf.pulse_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_ppm_conf.pulse_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_ppm_conf.median_filter = data[ind++];
		appconf.app_ppm_conf.safe_start = data[ind++];
		appconf.app_ppm_conf.rpm_lim_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_ppm_conf.rpm_lim_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_ppm_conf.multi_esc = data[ind++];
		appconf.app_ppm_conf.tc = data[ind++];
		appconf.app_ppm_conf.tc_max_diff = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);

		appconf.app_adc_conf.ctrl_type = (adc_control_type)data[ind++];
		appconf.app_adc_conf.hyst = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_adc_conf.voltage_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_adc_conf.voltage_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_adc_conf.use_filter = data[ind++];
		appconf.app_adc_conf.safe_start = data[ind++];
		appconf.app_adc_conf.cc_button_inverted = data[ind++];
		appconf.app_adc_conf.rev_button_inverted = data[ind++];
		appconf.app_adc_conf.voltage_inverted = data[ind++];
		appconf.app_adc_conf.rpm_lim_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_adc_conf.rpm_lim_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_adc_conf.multi_esc = data[ind++];
		appconf.app_adc_conf.tc = data[ind++];
		appconf.app_adc_conf.tc_max_diff = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_adc_conf.update_rate_hz = Vesc_buffer::buffer_get_uint16(data, &ind);

		appconf.app_uart_baudrate = Vesc_buffer::buffer_get_uint32(data, &ind);

		appconf.app_chuk_conf.ctrl_type = (chuk_control_type)data[ind++];
		appconf.app_chuk_conf.hyst = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_chuk_conf.rpm_lim_start = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_chuk_conf.rpm_lim_end = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_chuk_conf.ramp_time_pos = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_chuk_conf.ramp_time_neg = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_chuk_conf.stick_erpm_per_s_in_cc = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		appconf.app_chuk_conf.multi_esc = data[ind++];
		appconf.app_chuk_conf.tc = data[ind++];
		appconf.app_chuk_conf.tc_max_diff = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);

		appconf.app_nrf_conf.speed = (NRF_SPEED)data[ind++];
		appconf.app_nrf_conf.power = (NRF_POWER)data[ind++];
		appconf.app_nrf_conf.crc_type = (NRF_CRC)data[ind++];
		appconf.app_nrf_conf.retry_delay = (NRF_RETR_DELAY)data[ind++];
		appconf.app_nrf_conf.retries = data[ind++];
		appconf.app_nrf_conf.channel = data[ind++];
		memcpy(appconf.app_nrf_conf.address, data + ind, 3);
		ind += 3;
		appconf.app_nrf_conf.send_crc_ack = data[ind++];

		if (pInterface != nullptr) {
			pInterface -> setAppConfiguration(&appconf);
		}
		break;

	case COMM_DETECT_MOTOR_PARAM:
		ind = 0;
		detect_cycle_int_limit = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		detect_coupling_k = Vesc_buffer::buffer_get_float32(data, 1000.0, &ind);
		for (i = 0;i < 8;i++) {
			detect_hall_table[i] = (const signed char)(data[ind++]);
		}
		detect_hall_res = (const signed char)(data[ind++]);

		if (pInterface != nullptr) {
			pInterface -> setDetect(detect_cycle_int_limit, detect_coupling_k, detect_hall_table, detect_hall_res);
		}
		break;

	case COMM_DETECT_MOTOR_R_L: {
		// TODO!
	} break;

	case COMM_DETECT_MOTOR_FLUX_LINKAGE: {
		// TODO!
	} break;

	case COMM_DETECT_ENCODER: {
		// TODO!
	} break;

	case COMM_DETECT_HALL_FOC: {
		// TODO!
	} break;

	case COMM_GET_DECODED_PPM:
		ind = 0;
		dec_ppm = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		dec_ppm_len = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);

		if (pInterface != nullptr) {
				pInterface -> setPPM(dec_ppm, dec_ppm_len);
		}
		break;

	case COMM_GET_DECODED_ADC:
		ind = 0;
		dec_adc = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		dec_adc_voltage = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);
		// TODO for adc2
		if (pInterface != nullptr) {
				pInterface -> setADC(dec_adc, dec_adc_voltage);
		}
		break;

	case COMM_GET_DECODED_CHUK:
		ind = 0;
		dec_chuk = Vesc_buffer::buffer_get_float32(data, 1000000.0, &ind);

		if (pInterface != nullptr) {
			pInterface -> setChuk(dec_chuk);
		}
		break;

	case COMM_SET_MCCONF:
		// This is a confirmation that the new mcconf is received.
		if (pInterface != nullptr) {
			pInterface -> motorConfigurationRecieved();
		}
		break;

	case COMM_SET_APPCONF:
		// This is a confirmation that the new appconf is received.
		if (pInterface != nullptr) {
			pInterface -> appConfigurationRecieved();
		}
		break;

	default:
		break;
	}
}

void Vesc_interface::terminal_cmd(char* cmd) {
	int32_t send_index = 0;
	int len = strlen(cmd);
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_TERMINAL_CMD;
	memcpy(send_buffer + send_index, cmd, len);
	send_index += len;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_duty_cycle(float dutyCycle) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_DUTY;
	Vesc_buffer::buffer_append_float32(send_buffer, dutyCycle, 100000.0, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_current(float current) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_CURRENT;
	Vesc_buffer::buffer_append_float32(send_buffer, current, 1000.0, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_current_brake(float current) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_CURRENT_BRAKE;
	Vesc_buffer::buffer_append_float32(send_buffer, current, 1000.0, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_rpm(int rpm) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_RPM;
	Vesc_buffer::buffer_append_int32(send_buffer, rpm, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_pos(float pos) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_POS;
	Vesc_buffer::buffer_append_float32(send_buffer, pos, 1000000.0, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_servo_pos(float pos) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_SERVO_POS;
	Vesc_buffer::buffer_append_float16(send_buffer, pos, 1000.0, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_mcconf(const mc_configuration *mcconf) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_MCCONF;

	send_buffer[send_index++] = mcconf->pwm_mode;
	send_buffer[send_index++] = mcconf->comm_mode;
	send_buffer[send_index++] = mcconf->motor_type;
	send_buffer[send_index++] = mcconf->sensor_mode;

	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->l_current_max, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_current_min, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_in_current_max, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_in_current_min, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_abs_current_max, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_min_erpm, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_max_erpm, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_max_erpm_fbrake, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_max_erpm_fbrake_cc, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_min_vin, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_max_vin, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_battery_cut_start, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_battery_cut_end, 1000, &send_index);
	send_buffer[send_index++] = mcconf->l_slow_abs_current;
	send_buffer[send_index++] = mcconf->l_rpm_lim_neg_torque;
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_temp_fet_start, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_temp_fet_end, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_temp_motor_start, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_temp_motor_end, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_min_duty, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->l_max_duty, 1000000, &send_index);

	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_min_erpm, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_min_erpm_cycle_int_limit, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_max_fullbreak_current_dir_change, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_cycle_int_limit, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_phase_advance_at_br, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_cycle_int_rpm_br, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->sl_bemf_coupling_k, 1000, &send_index);

	memcpy(send_buffer + send_index, mcconf->hall_table, 8);
	send_index += 8;
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->hall_sl_erpm, 1000, &send_index);

	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_current_kp, 1e5, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_current_ki, 1e5, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_f_sw, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_dt_us, 1e6, &send_index);
	send_buffer[send_index++] = mcconf->foc_encoder_inverted;
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_encoder_offset, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_encoder_ratio, 1e3, &send_index);
	send_buffer[send_index++] = mcconf->foc_sensor_mode;
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_pll_kp, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_pll_ki, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_motor_l, 1e8, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_motor_r, 1e5, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_motor_flux_linkage, 1e5, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_observer_gain, 1e0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_duty_dowmramp_kp, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_duty_dowmramp_ki, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_openloop_rpm, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_sl_openloop_hyst, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_sl_openloop_time, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_sl_d_current_duty, 1e3, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, mcconf->foc_sl_d_current_factor, 1e3, &send_index);
	memcpy(send_buffer + send_index, mcconf->foc_hall_table, 8);
	send_index += 8;
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->foc_hall_sl_erpm, 1000, &send_index);

	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->s_pid_kp, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->s_pid_ki, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->s_pid_kd, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->s_pid_min_erpm, 1000, &send_index);

	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->p_pid_kp, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->p_pid_ki, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->p_pid_kd, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->p_pid_ang_div, 1e5, &send_index);

	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->cc_startup_boost_duty, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->cc_min_current, 1000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->cc_gain, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->cc_ramp_step_max, 1000000, &send_index);

	Vesc_buffer::buffer_append_int32(send_buffer, mcconf->m_fault_stop_time_ms, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->m_duty_ramp_step, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->m_duty_ramp_step_rpm_lim, 1000000, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer,mcconf->m_current_backoff_gain, 1000000, &send_index);
	Vesc_buffer::buffer_append_uint32(send_buffer, mcconf->m_encoder_counts, &send_index);

	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::set_appconf(const app_configuration *appconf) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_SET_APPCONF;
	send_buffer[send_index++] = appconf->controller_id;
	Vesc_buffer::buffer_append_uint32(send_buffer, appconf->timeout_msec, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->timeout_brake_current, 1000.0, &send_index);
	send_buffer[send_index++] = appconf->send_can_status;
	Vesc_buffer::buffer_append_uint16(send_buffer, appconf->send_can_status_rate_hz, &send_index);

	send_buffer[send_index++] = appconf->app_to_use;

	send_buffer[send_index++] = appconf->app_ppm_conf.ctrl_type;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.pid_max_erpm, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.hyst, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.pulse_start, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.pulse_end, 1000.0, &send_index);
	send_buffer[send_index++] = appconf->app_ppm_conf.median_filter;
	send_buffer[send_index++] = appconf->app_ppm_conf.safe_start;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.rpm_lim_start, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.rpm_lim_end, 1000.0, &send_index);
	send_buffer[send_index++] = appconf->app_ppm_conf.multi_esc;
	send_buffer[send_index++] = appconf->app_ppm_conf.tc;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_ppm_conf.tc_max_diff, 1000.0, &send_index);

	send_buffer[send_index++] = appconf->app_adc_conf.ctrl_type;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_adc_conf.hyst, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_adc_conf.voltage_start, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_adc_conf.voltage_end, 1000.0, &send_index);
	send_buffer[send_index++] = appconf->app_adc_conf.use_filter;
	send_buffer[send_index++] = appconf->app_adc_conf.safe_start;
	send_buffer[send_index++] = appconf->app_adc_conf.cc_button_inverted;
	send_buffer[send_index++] = appconf->app_adc_conf.rev_button_inverted;
	send_buffer[send_index++] = appconf->app_adc_conf.voltage_inverted;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_adc_conf.rpm_lim_start, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_adc_conf.rpm_lim_end, 1000.0, &send_index);
	send_buffer[send_index++] = appconf->app_adc_conf.multi_esc;
	send_buffer[send_index++] = appconf->app_adc_conf.tc;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_adc_conf.tc_max_diff, 1000.0, &send_index);
	Vesc_buffer::buffer_append_uint16(send_buffer, appconf->app_adc_conf.update_rate_hz, &send_index);

	Vesc_buffer::buffer_append_uint32(send_buffer, appconf->app_uart_baudrate, &send_index);

	send_buffer[send_index++] = appconf->app_chuk_conf.ctrl_type;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.hyst, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.rpm_lim_start, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.rpm_lim_end, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.ramp_time_pos, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.ramp_time_neg, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.stick_erpm_per_s_in_cc, 1000.0, &send_index);
	send_buffer[send_index++] = appconf->app_chuk_conf.multi_esc;
	send_buffer[send_index++] = appconf->app_chuk_conf.tc;
	Vesc_buffer::buffer_append_float32(send_buffer, appconf->app_chuk_conf.tc_max_diff, 1000.0, &send_index);

	send_buffer[send_index++] = appconf->app_nrf_conf.speed;
	send_buffer[send_index++] = appconf->app_nrf_conf.power;
	send_buffer[send_index++] = appconf->app_nrf_conf.crc_type;
	send_buffer[send_index++] = appconf->app_nrf_conf.retry_delay;
	send_buffer[send_index++] = appconf->app_nrf_conf.retries;
	send_buffer[send_index++] = appconf->app_nrf_conf.channel;
	memcpy(send_buffer + send_index, appconf->app_nrf_conf.address, 3);
	send_index += 3;
	send_buffer[send_index++] = appconf->app_nrf_conf.send_crc_ack;

	pPacket -> addTx(send_buffer, send_index);
}

// Getters
void Vesc_interface::get_fw_version(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_FW_VERSION;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::get_values(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_GET_VALUES;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::get_mcconf(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_GET_MCCONF;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::get_appconf(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_GET_APPCONF;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::get_decoded_ppm(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_GET_DECODED_PPM;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::get_decoded_adc(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_GET_DECODED_ADC;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::get_decoded_chuk(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_GET_DECODED_CHUK;
	pPacket -> addTx(send_buffer, send_index);
}

// Other functions
void Vesc_interface::detect_motor_param(float current, float min_rpm, float low_duty) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_DETECT_MOTOR_PARAM;
	Vesc_buffer::buffer_append_float32(send_buffer, current, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, min_rpm, 1000.0, &send_index);
	Vesc_buffer::buffer_append_float32(send_buffer, low_duty, 1000.0, &send_index);
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::reboot(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_REBOOT;
	pPacket -> addTx(send_buffer, send_index);
}

void Vesc_interface::send_alive(void) {
	int32_t send_index = 0;
	fwd_can_append(send_buffer, &send_index);
	send_buffer[send_index++] = COMM_ALIVE;
	pPacket -> addTx(send_buffer, send_index);
}

// Helpers
const char* Vesc_interface::fault_to_string(mc_fault_code fault) {
	switch (fault) {
	case FAULT_CODE_NONE: return "FAULT_CODE_NONE";
	case FAULT_CODE_OVER_VOLTAGE: return "FAULT_CODE_OVER_VOLTAGE";
	case FAULT_CODE_UNDER_VOLTAGE: return "FAULT_CODE_UNDER_VOLTAGE";
	case FAULT_CODE_DRV8302: return "FAULT_CODE_DRV8302";
	case FAULT_CODE_ABS_OVER_CURRENT: return "FAULT_CODE_ABS_OVER_CURRENT";
	case FAULT_CODE_OVER_TEMP_FET: return "FAULT_CODE_OVER_TEMP_FET";
	case FAULT_CODE_OVER_TEMP_MOTOR: return "FAULT_CODE_OVER_TEMP_MOTOR";
	default: return "Unknown fault";
	}
}

void Vesc_interface::fwd_can_append(uint8_t *data, int32_t *ind) {
	if (can_fwd_vesc >= 0) {
		data[*ind++] = COMM_FORWARD_CAN;
		data[*ind++] = can_fwd_vesc;
	}
}
