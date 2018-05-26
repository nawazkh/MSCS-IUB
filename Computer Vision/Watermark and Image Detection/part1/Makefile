all: watermark

watermark: watermark.cpp
	g++ watermark.cpp -o watermark -O3 -lpng -I.

clean:
	rm watermark
	# -g - turn on debugging (so GDB gives more friendly output)
	# -Wall - turns on most warnings
	# -O or -O2 - turn on optimizations
	# -o <name> - name of the output file
	# -c - output an object file (.o)
	# -I<include path> - specify an include directory
	# -L<library path> - specify a lib directory
	# -l<library> - link with library lib<library>.a
