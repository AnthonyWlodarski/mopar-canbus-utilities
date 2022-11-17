CC=gcc
MKDIR=mkdir -p
LIBP=../lib
OBJ=./obj
BIN=../bin
CFLAGS=

all: directories query_pids

debug: CFLAGS += -g
debug: all

query_pids: query_pids.o library.o can_write.o can_read.o
	$(CC) $(CFLAGS) -o $@ $(addprefix $(OBJ)/, $^)
	mv query_pids $(BIN)

library.o: $(LIBP)/library.c $(LIBP)/library.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

can_write.o: $(LIBP)/can_write.c $(LIBP)/can_write.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

can_read.o: $(LIBP)/can_read.c $(LIBP)/can_read.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

query_pids.o: query_pids.c
	$(CC) $(CFLAGS) -o $(OBJ)/$@ -c $<

.PHONY: directories clean

directories: 
	${MKDIR} $(OBJ) 

clean:
	rm -rf $(OBJ)/*.o
	rm -rf $(BIN)/query_pids
