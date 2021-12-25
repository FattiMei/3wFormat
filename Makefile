CC = gcc
CFLAGS = `pkg-config --cflags nettle`
LIBS = `pkg-config --libs nettle`


SRC = src/aes.c src/memory_view.c src/crc32.c src/XYZ3W.c src/to_gcode.c


build: $(SRC)
	$(CC) -o to_gcode.out $(SRC) $(CFLAGS) $(LIBS) -Wall


check:
	$(CC) test/unity.c test/test_memory_view.c src/memory_view.c -o test_memory_view.out
	./test_memory_view.out


.PHONY: clean


clean:
	rm -f *.o *.out
