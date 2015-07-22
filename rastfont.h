
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

