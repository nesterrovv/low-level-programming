set path=%path:git\bin=;%
set path=%path:mingw64\bin=;%
set path=%path:usr\bin=;%

IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR =out
LDIR =lib
PDIR = production

LIBS=-lm

_DEPS = hashset.h structures.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hashset.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv main.exe production/

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(PDIR)/*.exe
