ODIR=obj
LDIR=lib
CC=gcc-9
CFLAGS=-Iinclude -I/usr/local/include -D_THREAD_SAFE -L$(LDIR) -L/usr/local/lib -lSDL2

_OBJ=emulator.o cpu.o display.o input.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: clean main

main: $(ODIR)/main.o $(OBJ)
	$(CC) -o main $^ $(CFLAGS)

$(ODIR)/main.o: main.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ): $(ODIR)/%.o: src/%.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(ODIR)/*.o
