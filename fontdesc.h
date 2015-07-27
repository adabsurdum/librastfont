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
#ifndef _font_h_
#define _font_h_

typedef struct _font_image {
	int rows, cols;
	int w_px, h_px;
} font_image_t;

/**
  * Load a font description file of the form.
  * <image name line>
  * <char width> <char height> (of image)
  * Remaining lines contain characters in the positions at
  * which they appear in the image.
  */
extern int font_load_ascii_font( 
		const char *font, char *image, int maxlen, int *nr, int *nc );
extern int font_image_position( int k, int *r, int *c );

#endif

