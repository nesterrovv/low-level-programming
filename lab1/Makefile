set path=%path:git\bin=;%
set path=%path:mingw64\bin=;%
set path=%path:usr\bin=;%

IDIR = include
CC = gcc
CFLAGS = -I$(IDIR)

ODIR = out
LDIR = lib
PDIR = production

LIBS = -lm

_DEPS = arraylist.h hashset.h hashset_itr.h node.h structures.h string_methods.h hashmap_base.h hashmap.h file_manager.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = arraylist.o hashset.o hashset_itr.o node.o string_methods.o hashmap.o file_manager.o main.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv main.exe $(PDIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(PDIR)/*.exe
