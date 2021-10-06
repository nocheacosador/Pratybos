CCFLAGS = -std=gnu11
CC = gcc $(CCFLAGS)

all: build

run: build
	./yoda-transform

debug: CCFLAGS += -DDEBUG
debug: build

build:
	$(CC) -o yoda-transform main.c

clean:
	rm yoda-transform