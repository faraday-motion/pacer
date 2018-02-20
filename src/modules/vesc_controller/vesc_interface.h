#ifndef VESC_INTERFACE_H
#define VESC_INTERFACE_H

#include <HardwareSerial.h>
#include "./vesc_datatypes.h"
#include "./vesc_packet.h"
#include "./ivesc_interface.h"

class Vesc_interface : public IVesc_send_recieve
{
  private:
    Vesc_packet * pPacket = nullptr;
    IVesc_interface * pInterface = nullptr;
    unsigned char send_buffer[PACKET_MAX_PL_LEN];
    int32_t can_fwd_vesc = -1;
    mc_values values;
    int fw_major;
    int fw_minor;
    float rotor_pos;
    mc_configuration mcconf;
    app_configuration appconf;
    float detect_cycle_int_limit;
    float detect_coupling_k;
    signed char detect_hall_table[8];
    signed char detect_hall_res;
    float dec_ppm;
    float dec_ppm_len;
    float dec_adc;
    float dec_adc_voltage;
    float dec_chuk;

    // Private functions
    void fwd_can_append(uint8_t *data, int32_t *ind);
    void send(unsigned char *data, unsigned int len);
    void recieve(unsigned char *data, unsigned int len);
    HardwareSerial * pSerial = nullptr;
  public:
    /**
     * Initialize bldc_interface.
     *
     * @param func
     * A function to be used when sending packets. Null (0) means that no packets will be sent.
     */
    Vesc_interface(HardwareSerial * serial, IVesc_interface * interface = nullptr) : IVesc_send_recieve() {
      pInterface = interface;
      pSerial = serial;
    	pPacket = new Vesc_packet(this);
    	can_fwd_vesc = -1;
    }

    void loop();

    static void timerTimeouts()
    {
      Vesc_packet::timerTimeouts();
    }

    void timerTimeout()
    {
      pPacket -> timerTimeout();
    }

    void set_forward_can(int32_t vesc_id);

    // Setters
    void terminal_cmd(char* cmd);
    void set_duty_cycle(float dutyCycle);
    void set_current(float current);
    void set_current_brake(float current);
    void set_rpm(int rpm);
    void set_pos(float pos);
    void set_servo_pos(float pos);
    void set_mcconf(const mc_configuration *mcconf);
    void set_appconf(const app_configuration *appconf);

    // Getters
    void get_fw_version(void);
    void get_values(void);
    void get_mcconf(void);
    void get_appconf(void);
    void get_decoded_ppm(void);
    void get_decoded_adc(void);
    void get_decoded_chuk(void);

    // Other functions
    void detect_motor_param(float current, float min_rpm, float low_duty);
    void reboot(void);
    void send_alive(void);
    // Helpers
    const char* fault_to_string(mc_fault_code fault);
};

#endif
