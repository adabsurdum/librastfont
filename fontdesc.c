/**
  * Generate data describing pixels for a raster font from a PGM image
  * of characters.
  * Copyright (C) 2015  Roger Kramer
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strchr
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fontdesc.h"

typedef struct _charaddr {
	int x, y;
} charaddr_t;

/**
 * Careful with fscanf since it aggressively consumes whitespace which we
 * might want preserved.
 */
static int _load( const char *font, char *image, int maxlen, int *nr, int *nc, charaddr_t *charmap ) {

	FILE *fp = fopen( font, "r" );
	if( fp ) {
		char buf[32];
		int w, h, i;
		// Read the image filename, and NUL terminate it.
		if( image != fgets( image, maxlen, fp ) ) {
			return -__LINE__;
		}
		if( NULL == strchr( image, '\n' ) ) {
			return -__LINE__;
		}
		*strchr( image, '\n' ) = '\0'; // Replace NL with NULL terminator.
		// Read the character dimensions of the image.
		fgets( buf, 31, fp );
		if( 2 != sscanf( buf, "%d %d\n", &w, &h ) ) {
			return -__LINE__;
		}
		if( w < 1 || h < 1 ) {
			return -__LINE__;
		}
		*nr = h; *nc = w;
		// Remainder of file contains characters in the positions at which
		// they appear in the image file.
		i = 0;
		while( i < h ) {
			int n = 0;
			while(1) {
				const int c = fgetc(fp);
				if( c == '\n' ) break;
				charmap[ c ].y = i;
				charmap[ c ].x = n;
				n++;
			}
			i++;
		}
		fclose( fp );
	}
	return 0;
}


/**
  * This is really only for ASCII characters.
  */
#define CHARMAP_LEN (127)
static charaddr_t _charmap[CHARMAP_LEN];

/**
 * This thunk just simplifies conversion of this file to use
 * non-static data in the future...
 */
int font_load_ascii_font( const char *font, char *image, int maxlen, int *nr, int *nc ) {
	memset( _charmap, -1, sizeof(_charmap) );
	return _load( font, image, maxlen, nr, nc, _charmap );
}


int font_image_position( int k, int *r, int *c ) {
	if( 0x20 <= k && k < CHARMAP_LEN ) {
		const int R = _charmap[k].y;
		const int C = _charmap[k].x;
		if( 0 <= R && 0 <= C ) {
			*r = R;
			*c = C;
			return 1;
		}
	}
	return 0;
}

#ifdef UNIT_TEST_FONT
int main( int argc, char *argv[] ) {
	char fname[FILENAME_MAX+1];
	int c;
	if( argc > 1 ) {
		int nr, nc;
		font_load_ascii_font( argv[1], fname, FILENAME_MAX, &nr, &nc );
		printf( "image file = %s\n", fname );
		do {
			int row, col;
			c = fgetc( stdin );
			if( font_image_position(c, &row, &col ) )
				printf( "%c => %d, %d\n", c, row, col );
		} while( EOF != c );
	}
}
#endif

