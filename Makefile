CC = gcc
FLAGS = -Wall
OFLAGS = $(FLAGS) -c
OBJECTS = bithacks.o hilbert.o
DEPS =
SOURCES = $(OBJECTS:.o=.c)
LINKER = $(OBJECTS)
PWD = $(shell pwd)

%.o: %.c
	$(CC) $(OFLAGS) $< -o $@

all: hilbert.o

#The following are simple test programs
bin/draw_quads: $(OBJECTS) $(DEPS) play/draw_quads.c
	$(CC) $(FLAGS) -o bin/draw_quads play/draw_quads.c $(LINKER) -lcairo -I $(PWD)

bin/draw: $(OBJECTS) $(DEPS) play/draw.c
	$(CC) $(FLAGS) -o bin/draw play/draw.c $(LINKER) -lcairo -I $(PWD)

bin/point: $(OBJECTS) $(DEPS) play/point.c
	$(CC) $(FLAGS) -o bin/point play/point.c $(LINKER) -I $(PWD)

bin/bittable: $(OBJECTS) $(DEPS) play/bittable.c
	$(CC) $(FLAGS) -o bin/bittable play/point.c $(LINKER) -I $(PWD)

play: bin/point bin/bittable bin/draw bin/draw_quads

#Proof of function has not yet been met
#test: test.c $(OBJECTS) $(DEPS)
#	$(CC) $(FLAGS) -o test test.c $(LINKER)

.PHONY: clean done

clean:
	rm -f $(OBJECTS) bin/*

done: clean
	rm -rf *~
