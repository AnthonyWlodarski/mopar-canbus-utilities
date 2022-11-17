CC=gcc
MKDIR=mkdir -p
LIBP=../lib
OBJ=./obj
BIN=../bin
CFLAGS=

all: directories clear_trouble_codes

debug: CFLAGS += -g
debug: all

clear_trouble_codes: clear_trouble_codes.o library.o can_write.o can_read.o
	$(CC) $(CFLAGS) -o $@ $(addprefix $(OBJ)/, $^)
	mv clear_trouble_codes $(BIN)

library.o: $(LIBP)/library.c $(LIBP)/library.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

can_write.o: $(LIBP)/can_write.c $(LIBP)/can_write.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

can_read.o: $(LIBP)/can_read.c $(LIBP)/can_read.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

clear_trouble_codes.o: clear_trouble_codes.c
	$(CC) $(CFLAGS) -o $(OBJ)/$@ -c $<

.PHONY: directories clean

directories: 
	${MKDIR} $(OBJ)
	${MKDIR} $(BIN)

clean:
	rm -rf $(OBJ)/*.o
	rm -rf $(BIN)/clear_trouble_codes
