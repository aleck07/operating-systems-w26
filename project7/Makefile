CC=gcc
CCOPTS=-Wall -Wextra -Werror
LIBS=-lpthread

.PHONY: all clean

all: grayscaler

grayscaler: grayscaler.c ppm.c ppm.h
	$(CC) $(CCOPTS) -o grayscaler grayscaler.c ppm.c $(LIBS)

clean:
	rm -f grayscaler
    
