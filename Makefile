binaries=scratch
all: 
	cc scratch.c -o scratch  -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libpng16 -lcairo -lm
.PHONY: clean
clean:
	rm -f $(binaries)
