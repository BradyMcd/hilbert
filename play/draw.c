
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <cairo/cairo.h>

#include <hilbert.h>

int main( int argc, char* argv[] ){

  uint8_t recursions;
  uint32_t max_side;
  uint32_t img_size;

  if( argc != 3 ){

    printf( "This demo requires two numerical arguments\n" );
    exit(1);
  }

  recursions = atoi( argv[1] );

  if( recursions < 0 || recursions >= 32 ){
    printf("Make sure your first argument is a number above 0 and below 32\n");
    exit(1);
  }

  max_side = 1<<recursions;
  img_size = atoi( argv[2] );

  if( img_size < max_side*3 ){

    printf( "Try to keep your second argument greater than than %d, "
            "otherwise you won't have enough pixels\n", max_side * 3 + 2 );
  }

  struct hil_Point* draw_order = malloc( max_side*max_side *
                                         sizeof( struct hil_Point ) );
  if ( draw_order == NULL ){ printf("Couldn't allocate enough memory\n"); exit(1); }

  struct hil_Point curr;
  uint64_t index;

  printf( "Working...\n" );
  for( curr.y = 0; curr.y < max_side; ++curr.y ){
    for( curr.x = 0; curr.x < max_side; ++curr.x ){

      index = hil_iterative_c2d( recursions, curr );
      draw_order[index].x = curr.x;
      draw_order[index].y = curr.y;
    }
  }

  cairo_surface_t* surface =
    cairo_image_surface_create( CAIRO_FORMAT_ARGB32, img_size, img_size );
  cairo_t* cr = cairo_create( surface );
  cairo_move_to( cr, 1, 1 );
  cairo_set_source_rgba( cr, 0.0, 0.6, 1.0, 1.0 );
  cairo_set_line_width( cr, 2 );

  double x, y;

  for( index = 0; index < 1<<( recursions * 2 ); ++index ){

    curr = draw_order[index];
    x = ( (double )curr.x / max_side * ( img_size - 1 ) ) + 1;
    y = ( (double )curr.y / max_side * ( img_size - 1 ) ) + 1;

    cairo_line_to( cr, x, y );

  }

  free( draw_order );
  cairo_stroke( cr );


  printf( "Done, saving image to hilbert.png\n" );

  cairo_surface_write_to_png( surface, "hilbert.png" );
  cairo_destroy( cr );
  cairo_surface_destroy( surface );

  return 0;
}
