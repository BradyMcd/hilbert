
#include <stdio.h>

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "bithacks.h"

#include "hilbert.h"

void implementation_precond( uint8_t recursions ){
  assert( 0 < recursions && recursions < 32 );
}

void conversion_precond( uint8_t recursions, struct hil_Point loc ){
  implementation_precond( recursions );
  assert( loc.x <= ( ( 1 << recursions ) - 1 ) );
  assert( loc.y <= ( ( 1 << recursions ) - 1 ) );
}

void hil_normalize_step( uint32_t side_len, bool quad_x, bool quad_y,
                         struct hil_Point* loc ){

  loc->x = loc->x % side_len;
  loc->y = loc->y % side_len;

  if( !quad_y ){
    if( quad_x ){
      loc->x = ( side_len - 1 ) - loc->x;
      loc->y = ( side_len - 1 ) - loc->y;
    }

    uint32_t temp = loc->x;
    loc->x = loc->y;
    loc->y = temp;
  }
}

uint64_t hil_recursive_c2d( uint8_t recursions, struct hil_Point loc ){

  //this has an edge case at the "bottom"
  //conversion_precond( recursions, loc );

  uint32_t side_len= 1 << ( recursions - 1 );

  bool quad_x = loc.x >= side_len;
  bool quad_y = loc.y >= side_len;

  uint8_t coefficient = ( quad_x * 0b11 ) ^ quad_y;

  hil_normalize_step( side_len, quad_x, quad_y, &loc );

  if( recursions == 1 ){

    return coefficient;
  }else{

    return coefficient*side_len*side_len +
      hil_recursive_c2d( recursions - 1, loc );
  }
}

uint64_t hil_iterative_c2d( uint8_t recursions, struct hil_Point loc ){

  conversion_precond( recursions, loc );

  uint32_t side_len;
  uint64_t distance = 0;
  bool quad_x, quad_y;
  uint8_t coefficient;

  for( side_len = 1 << ( recursions - 1 ); side_len > 1; side_len /= 2 ){
    quad_x = loc.x >= side_len;
    quad_y = loc.y >= side_len;

    coefficient = ( quad_x * 3 ) ^ quad_y;

    hil_normalize_step( side_len, quad_x, quad_y, &loc );
    distance += coefficient * side_len * side_len;
  }

  quad_x = loc.x >= side_len;
  quad_y = loc.y >= side_len;

  distance += ( quad_x * 3 ) ^ quad_y;

  return distance;
}

/* hil_ideal
 *  Takes as arguments the size of a plane, and a boolean returns the closest
 *  level of recursion which will cover a geometry with the given plane size.
 *  Take the return of this function as a suggestion, not necessarily the best
 *  possible recursion level, test if performance is a major concern.
 *
 *  Arguments:
 *  - size: the size of some 2 dimensional plane
 *  - allow_fuzzy_matches: true if we don't mind multiple (x,y) pairs matching
 *    the same d values, false if all (x,y) pairs must have unique d values.
 *    Note that if false differences between neighbor d values will be inflated
 *
 *  Returns:
 *  - a level of recursion which will match an axis of the geometry given
 */

uint8_t hil_ideal( struct hil_Point size, bool allow_fuzzy_matches ){

  uint32_t determinant;
  uint8_t ret;

  if( allow_fuzzy_matches ){

    determinant = (size.x > size.y)? size.y : size.x; //select the minimum axis
    ret = bithack_highest_bit( determinant );
    ret += !bithack_is_2s_power( determinant );
  }else{

    determinant = (size.x > size.y)? size.x : size.y; //select the maximum axis
    ret = bithack_highest_bit( determinant );
    ret += !bithack_is_2s_power( determinant );
  }
  return ret;
}

/* hil_clobber
 *  Takes as arguments the size of a rectangular plane and a point on that plane
 *  returns the best fitting point in the integers along a square of size 2^n
 *
 * Arguments:
 * - nBits: the power of 2 used in the hilbert curve we are clobbering to fit
 * - max: the size of the rectangle we are clobbering coordinates from
 * - loc: the point we want to fit onto a square plane
 *
 * Returns:
 * - an equivelent point to loc along a square plane whose size is 2^n
 */
struct hil_Point hil_clobber( uint8_t recursions, struct hil_Point max,
                              struct hil_Point loc ){

  implementation_precond( recursions  );
  uint32_t side_len = 1 << recursions;
  struct hil_Point ret;

  ret.x = loc.x / max.x * side_len;
  ret.y = loc.y / max.y * side_len;

  return ret;
}

#define MAX(a,b)                                \
  ({ __typeof__ (a) _a = (a);                   \
     __typeof__ (b) _b = (b);                   \
     _a > _b ? _a : _b;                         \
  })
#define MIN(a,b)                                \
  ({__typeof__ (a) _a = (a);                    \
    __typeof__ (b) _b = (b);                    \
    _a < _b ? _a : _b;                          \
  })

bool hil_is_edgecase( uint8_t recursions, uint64_t d, uint64_t d_ ){

  uint64_t mask = 3 << ( ( recursions - 1 ) * 2 );

  if( MAX(d, d_) & mask == mask && MIN(d, d_) & mask == 0 ){
    return true;
  }
  return false;
}
