
#ifndef _HILBERT_H
#define _HILBERT_H

#include <stdint.h>
#include <stdbool.h>

struct hil_Point{
  uint32_t x;
  uint32_t y;
};

uint64_t hil_recursive_c2d( uint8_t, struct hil_Point );
uint64_t hil_iterative_c2d( uint8_t, struct hil_Point );

uint8_t hil_ideal( struct hil_Point, bool );
struct hil_Point hil_clobber( uint8_t, struct hil_Point, struct hil_Point );
uint64_t hil_rotation( uint8_t, uint64_t );

#endif//_HILBERT_H
