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

#ifndef _rastfont_h_
#define _rastfont_h_

struct coord2d {
	uint8_t x, y;
};

struct raster_char {
	uint32_t count;
	const struct coord2d *foreground;
};

extern const int charcell_width;
extern const int charcell_height;

extern const struct raster_char *charmap[];

#endif

