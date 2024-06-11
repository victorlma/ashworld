CC=gcc
DFLAGS=-ggdb --std=c99 --pedantic -Wall
RFLAGS=-O2
LIBS=`pkg-config --libs ncurses`
BUILDIR=build
SRCDIR=src
BINPATH=$(BUILDIR)/ashworld
HEADERS=$(wildcard $(SRCDIR)/*.h)
SRCS=$(wildcard $(SRCDIR)/*.c)


.PHONY: clean debug 

$(BINPATH)-debug: $(SRCDIR)/main.c $(HEADERS) $(SRCS)
	mkdir -p build
	$(CC) $(DFLAGS) -o $(BINPATH)-debug $< $(LIBS)

debug: $(BINPATH)-debug
	@./$(BINPATH)-debug

clean:
	rm -rf build
