
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <bithacks.h>
#include <hilbert.h>

int main( int argc, char* argv[] ){

uint8_t recursions;

if( argc != 2 ){

printf( "This demo requires one numerical argument\n" );
exit(1);
}else{

recursions = atoi( argv[1] );

if( recursions < 0 || recursions >= 32 ){
printf("Make sure your argument is a number above 0 and below 32\n");
exit(1);
}
}

  struct hil_Point curr;
  char input[8];
  int64_t buff = -1;
  uint64_t d;

  do{

    do{

      printf( "Enter an x coordinate (or 'q'): " );
      fgets( input, 9, stdin );

      if( strchr( input, 'q' ) > 0 ){ exit(0); }

      buff = atoi( input );
    }while( buff < 0 );

    curr.x = buff;

    do{

      printf( "Enter a y coordinate (or 'q'): " );
      fgets( input, 9, stdin );

      if( strchr( input, 'q' ) > 0 ){ exit(0); }

      buff = atoi( input );
    }while( buff < 0 );

    curr.y = buff;

    d = hil_iterative_c2d( recursions, curr );

    printf( "Distance: %llu\n", d );

    printf( BITHACK_PATTERN" ",
            BITHACK_BINARY_OF(d>>16) );
    printf( BITHACK_PATTERN" ",
            BITHACK_BINARY_OF(d>>8) );
    printf( BITHACK_PATTERN "\n",
            BITHACK_BINARY_OF(d) );

  }while( true );

}
