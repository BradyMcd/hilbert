
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <hilbert.h>

int main( int argc, char* argv[] ){

  uint8_t recursions;

  if( argc != 2){

    printf( "This demo requires one numerical argument\n" );
    exit( 0 );
  }else{

    recursions = atio(argv[1]);

    if( recursions < 0 || recursions >= 32 ){
      printf("Make sure your argument is a number above 0 and below 32\n");
      exit(1);
    }
  }

  struct hil_Point curr;
  char input[8];
  int8_t nth = 0; //selects bits
  uint64_t d;

  do{
    printf( "Select bit (%d recursions, -1 for exit): ", recursions );
    fgets( input, 4, stdin );
    nth = atoi(input);

    for( curr.y = 0; curr.y < 1<<recursions && nth >= 0; ++curr.y ){
      for( curr.x = 0; curr.x < 1<<recursions; ++curr.x ){

        d = hil_iterative_c2d( recursions, curr );
        printf( "%llu ", ( d >> nth ) & 1 );
      }
      printf("\n");
    }
  }while( nth >= 0 );
}
