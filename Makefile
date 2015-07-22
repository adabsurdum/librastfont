
GRAPHICS_FILE_FORMAT_DIR=../libgraphicsff
GRAPHICS_FILE_FORMAT_LIB=graphicsff

EXECUTABLES=mkfont
GENERATED_C=monaco13.c

CPPFLAGS+=-D_GNU_SOURCE -I$(GRAPHICS_FILE_FORMAT_DIR)
CPPFLAGS+=-DHAVE_EMIT_C

ifdef DEBUG
CFLAGS+=-g
CPPFLAGS+=-D_DEBUG
else
CFLAGS+=-O3
CPPFLAGS+=-DNDEBUG
endif

CFLAGS+=-Wall -std=c99 -Wno-unused-variable

LDFLAGS=-L$(GRAPHICS_FILE_FORMAT_DIR)
LDLIBS=-l$(GRAPHICS_FILE_FORMAT_LIB)

ifdef DEBUG
endif

all : monaco13.o

monaco13.o :

$(GENERATED_C) : mkfont fonts/monaco13-noantialias-208x119.font
	./mkfont fonts/monaco13-noantialias-208x119.font fonts > $@

mkfont : main.o fontdesc.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS)

clean :
	rm -f $(EXECUTABLES) *.o ut-* $(GENERATED_C)

.PHONY : clean

