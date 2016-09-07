
#include <stdbool.h> //bools
#include <stdint.h>  //explicit bit length integer types
#include <endian.h>  //platform endianness

#include "bithacks.h"

/*bithack_highest_bit
 * returns the index of the highest set bit
 */
uint8_t bithack_highest_bit( uint32_t target ){

  union{ uint32_t u[2]; double d; } t; //temp

  t.u[__FLOAT_WORD_ORDER == __LITTLE_ENDIAN] = 0x43300000;
  t.u[__FLOAT_WORD_ORDER != __LITTLE_ENDIAN] = target;

  t.d -= 4503599627370496.0;

  return (uint8_t)((t.u[__FLOAT_WORD_ORDER==LITTLE_ENDIAN] >> 20) - 0x3FF);
}

/*bithack_is_2s_power
 * returns true if the target is some power of 2
 */
bool bithack_is_2s_power( uint32_t target ){

  return target && !(target & (target-1));
}

/*bithack_bitcount
 * returns the number of bits set in a 32bit word
 * Larger bitness will want a different implementation specialized to 64bit arch
 */
uint8_t bithack_bitcount( uint32_t target ){
  uint8_t count;
  for( count = 0; target; ++count ){
    target &= target - 1;
  }
  return count;
}

