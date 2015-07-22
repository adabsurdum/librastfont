
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

