#ifndef _BITHACKS_H
#define _BITHACKS_H

#include <stdbool.h>
#include <stdint.h>

#define BITHACK_PATTERN "%c%c%c%c%c%c%c%c"
#define BITHACK_BINARY_OF(byte) \
  ( ( byte & 0x80 ) ? '1' : '0'),    \
  ( ( byte & 0x40 ) ? '1' : '0'),    \
  ( ( byte & 0x20 ) ? '1' : '0'),    \
  ( ( byte & 0x10 ) ? '1' : '0'),    \
  ( ( byte & 0x08 ) ? '1' : '0'),    \
  ( ( byte & 0x04 ) ? '1' : '0'),    \
  ( ( byte & 0x02 ) ? '1' : '0'),    \
    ( ( byte & 0x01 ) ? '1' : '0')


uint8_t bithack_highest_bit( uint32_t );
bool bithack_is_2s_power( uint32_t );
uint8_t bithack_bitcount( uint32_t );



#endif//_BITHACKS_H
