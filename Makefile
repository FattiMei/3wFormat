CC = gcc
CFLAGS = `pkg-config --cflags nettle zlib`
LIBS = `pkg-config --libs nettle zlib`

SRC = src/aes.c src/memory_view.c src/XYZ3W.c src/to_gcode.c

build: $(SRC)
	$(CC) -o to_gcode.out $(SRC) $(CFLAGS) $(LIBS)
