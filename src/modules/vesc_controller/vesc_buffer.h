#ifndef VESC_BUFFER_H
#define VESC_BUFFER_H

#include <stdint.h>

class Vesc_buffer
{
public:
  static void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index);
  static void buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index);
  static void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);
  static void buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index);
  static void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index);
  static void buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t *index);
  static int16_t buffer_get_int16(const uint8_t *buffer, int32_t *index);
  static uint16_t buffer_get_uint16(const uint8_t *buffer, int32_t *index);
  static int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index);
  static uint32_t buffer_get_uint32(const uint8_t *buffer, int32_t *index);
  static float buffer_get_float16(const uint8_t *buffer, float scale, int32_t *index);
  static float buffer_get_float32(const uint8_t *buffer, float scale, int32_t *index);
};

#endif
