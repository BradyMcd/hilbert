
//TODO: Add more colours, maybe by way of a precomputed colour table of bright, contrasting colours

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <cairo/cairo.h>

#include <hilbert.h>


struct _colour_t{
  double R;
  double G;
  double B;
};

const struct _colour_t colour[16] = {
  { .R = 1.0, .G = 0.0, .B = 0.0 },
  { .R = 0.0, .G = 1.0, .B = 0.0 },
  { .R = 0.0, .G = 0.0, .B = 1.0 },
  { .R = 1.0, .G = 0.0, .B = 1.0 },
  { .R = 0.0, .G = 1.0, .B = 1.0 },
  { .R = 1.0, .G = 1.0, .B = 0.0 },
  { .R = 1.0, .G = 0.0, .B = 1.0 },
  { .R = 1.0, .G = 0.5, .B = 0.5 },
  { .R = 0.5, .G = 1.0, .B = 0.5 },
  { .R = 0.5, .G = 0.5, .B = 1.0 },
  { .R = 1.0, .G = 0.75, .B = 0.0 },
  { .R = 1.0, .G = 0.0, .B = 0.75 },
  { .R = 0.75, .G = 1.0, .B = 0.0 },
  { .R = 0.0, .G = 1.0, .B = 0.75 },
  { .R = 0.75, .G = 0.0, .B = 1.0 },
  { .R = 0.0, .G = 0.75, .B = 1.0 }

};

#define CURRY( N ) colour[N].R, colour[N].G, colour[N].B

struct loc_colour_selector{

  struct hil_Point pt;
  uint64_t colour;
};

int main( int argc, char* argv[] ){

  uint8_t recursions;
  uint64_t divisions;
  uint32_t img_size;

  uint64_t offset;

  uint32_t max_side;

  if( argc < 4 || argc > 5 ){

    printf( "This demo requires three or four numerical arguments\n" );
    exit( 1 );
  }

  recursions = atoll( argv[1] );

  if( recursions < 0 || recursions >= 32 ){
    printf("Make sure your first argument is a number above 0 and below 32\n");
    exit( 1 );
  }

  divisions = atoll( argv[2] );

  if( divisions < 1|| divisions >= 1<<( recursions * 2 ) ){
    printf( "Make sure your second argument is a number above 1 and below %ull",
            1<<( recursions * 2 ) );
    exit( 1 );
  }

  max_side = 1<<recursions;
  img_size = atoi( argv[3] );


  if( img_size < max_side*3 + 2 ){

    printf( "Try to keep your second argument greater than than %d, "
            "otherwise you won't have enough pixels\n", max_side * 3 + 2 );
  }

  offset = ( argc == 5 ) ? atoll( argv[4] ) : 0;
  if( offset > max_side*max_side/divisions ){
    printf( "All meaningful offsets can be defined in the interval [0, %lld) ",
            max_side * max_side / divisions);
  }

  struct loc_colour_selector* draw_instructions= malloc( max_side*max_side *
                                        sizeof( struct loc_colour_selector ) );
  if( draw_instructions == NULL ){
    printf( "Couldn't allocate enough memory" );
    exit( 1 );
  }

  struct hil_Point curr;
  uint64_t index;
  uint64_t division_period = max_side * max_side / divisions;

  printf( "Working...\n" );

  for( curr.y = 0; curr.y < max_side; ++curr.y ){
    for( curr.x = 0; curr.x < max_side; ++curr.x ){

      index = hil_iterative_c2d( recursions, curr );
      draw_instructions[index].colour = ( ( index + offset ) % ( max_side * max_side ) )
        / division_period;
      draw_instructions[index].pt.x = curr.x;
      draw_instructions[index].pt.y = curr.y;
    }
  }

  cairo_surface_t* surface =
    cairo_image_surface_create( CAIRO_FORMAT_ARGB32, img_size, img_size );
  cairo_t* cr = cairo_create( surface );
  cairo_move_to( cr, 1, 1 );
  uint64_t last_colour = draw_instructions[0].colour;
  cairo_set_source_rgba( cr, CURRY( last_colour ), 1.0 );
  cairo_set_line_width( cr, 2 );

  double x, y;

  for( index = 0; index < 1<<( recursions * 2 ); ++index ){

    curr = draw_instructions[index].pt;
    x = ( (double )curr.x / max_side * ( img_size - 1 ) ) + 1;
    y = ( (double )curr.y / max_side * ( img_size - 1 ) ) + 1;

    if( draw_instructions[index].colour != last_colour ){

      cairo_stroke( cr );
      last_colour = draw_instructions[index].colour;
      cairo_set_source_rgba( cr, CURRY( last_colour ), 1.0 );
    }

    cairo_line_to( cr, x, y );
  }

  free( draw_instructions );
  cairo_stroke( cr );

  printf( "Done, saving image to hil_quads.png\n" );

  cairo_surface_write_to_png( surface, "hil_quads.png" );
  cairo_destroy( cr );
  cairo_surface_destroy( surface );

  return 0;
}
