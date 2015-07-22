
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include "pgm.h"
#include "fontdesc.h"
#include "rastfont.h"

/**
  * Read the pixels in one cell of a composite image.
  * For each non-zero pixel emit a pair of coordinates and count
  * the number thereof.
  * static const struct coord2d _65_pts[] = {};
  * const struct raster_char _65 = {
  *		.count = 43,
  *		.foreground = {
  *			{1,2},{3,4},{5,6}
  * 	};
  * };
  * const struct raster_char *charmap[] = {
  *		&_31,
  *		&_32,
  * };
  */
static void _emit_charfg(
		int chr,
		const uint8_t *topleft, // ...pixel of cc in composite image
		int cc_w_px,
		int cc_h_px,
		size_t co_stride,
		FILE *fp ) {

	const uint8_t *px;
	uint32_t n = 0;

#ifdef HAVE_EMIT_C
#else
	const long START = ftell( fp );
	// Insure the struct starts at an address aligned to sizeof(uint32_t)
	while( ftell(fp) % 4 ) fputc( 0, fp );
#endif
	// First pass just counts points.
	px = topleft;
	for(int y = 0; y < cc_h_px; y++ ) {
		for(int x = 0; x < cc_w_px; x++ ) {
			if( px[x] != 0 ) n += 1;
		}
		px += co_stride;
	}
	// Second pass emits points.
	fprintf( fp, "static const struct coord2d _%02X_pts[%d] = {\n", chr, n );
	px = topleft;
	n = 0;
	for(int y = 0; y < cc_h_px; y++ ) {
		for(int x = 0; x < cc_w_px; x++ ) {
			if( px[x] != 0 ) {
#ifdef HAVE_EMIT_C
				fprintf( fp, "%s{%d,%d}",n>0?",":"",x,y );
#else
				uint8_t xy[2];
				xy[0] = (uint8_t)x;
				xy[1] = (uint8_t)y;
				fwrite( xy, sizeof(uint8_t), 2, fp );
#endif
				n += 1;
			}
		}
		px += co_stride;
	}
	fprintf( fp, "\n};\n" );
	// ...second emits the struct.
#ifdef HAVE_EMIT_C
	fprintf( fp,
			"static const struct raster_char _%02X = {\n"
			"\t.count = %d,\n"
			"\t.foreground = _%02X_pts\n"
			"};\n",
			chr,
			n,
			chr);
#endif

#ifdef HAVE_EMIT_C
#else
	// Back up and write the count...
	fseek( fp, START, SEEK_SET );
	fwrite( &n, sizeof(uint32_t), 1, fp );
	// ...then return to EOF.
	fseek( fp, 0,     SEEK_END );
#endif
}


/**
  * Write a sequence of <struct raster_char> which constitute a ragged-array
  * of pixel positions.
  */
void _emit( const uint8_t *image,
		int cc_w_px,
		int cc_h_px,
		int co_w_px,
		FILE *fp ) {

	//FILE *fp = tmpfile();
	for(int chr = 0x21; chr < 0x7F /* DEL */; chr++ ) {
		int r, c;
		if( font_image_position( chr, &r, &c ) ) {
			// Calculate the pixel position in the composite image of the
			// top-left character cell pixel.
			const int X = c*cc_w_px;
			const int Y = r*cc_h_px;
			const uint8_t *topleft
				= image + Y*co_w_px + X;
			_emit_charfg(
					chr,
					topleft,
					cc_w_px,
					cc_h_px,
					co_w_px,
					fp );
		}
	}
}


int main( int argc, char *argv[] ) {

	static char image_filename[FILENAME_MAX+1];
	static char image_filepath[FILENAME_MAX+1];
	if( argc > 1 ) {
		const char *fontfname = argv[1];
		const char *PATH
			= argc > 2
			? argv[2]
			: ".";
		int cc_w_px;
		int cc_h_px;
		if( access( fontfname, R_OK ) == 0 ) {
			int nr, nc, maxval;
			int w_px, h_px;
			uint8_t *img = NULL;
			font_load_ascii_font( fontfname, image_filename, FILENAME_MAX, &nr, &nc );
			sprintf( image_filepath, "%s/%s", PATH, image_filename );
			maxval = pgm_load( image_filepath, &w_px, &h_px, &img );
			if( maxval < 0 )
				abort();
			assert( w_px%nc == 0 );
			assert( h_px%nr == 0 );
			printf(
				"#include <stdint.h>\n"
				"#include \"rastfont.h\"\n"
				"// %dx%d\n",
				w_px, h_px );
			cc_w_px = w_px / nc;
			cc_h_px = h_px / nr;
			_emit( img, cc_w_px, cc_h_px, w_px, stdout );
			printf( "// %dx%d\n", cc_w_px, cc_h_px );
			free( img );
			////////////////////////////////////////////////////////////////
			printf(
					"const int charcell_width = %d;\n"
					"const int charcell_height = %d;\n"
					"const struct raster_char *charmap[] = {\n", cc_w_px, cc_h_px );
			for(int i = 0; i < 127; i++ ) {
				if( i < 0x21 )
					printf( "0,\n" );
				else
					printf( "&_%02X,\n", i );
			}
			printf( "};\n" );
			return EXIT_SUCCESS;
		}
	}
	return EXIT_FAILURE;
}

