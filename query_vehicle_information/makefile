CC=gcc
MKDIR=mkdir -p
LIBP=../lib
OBJ=./obj
BIN=../bin
CFLAGS=

all: directories query_vehicle_information

debug: CFLAGS += -g
debug: all

query_vehicle_information: query_vehicle_information.o library.o can_write.o can_read.o
	$(CC) $(CFLAGS) -o $@ $(addprefix $(OBJ)/, $^)
	mv query_vehicle_information $(BIN)

library.o: $(LIBP)/library.c $(LIBP)/library.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

can_write.o: $(LIBP)/can_write.c $(LIBP)/can_write.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

can_read.o: $(LIBP)/can_read.c $(LIBP)/can_read.h
	$(CC) $(CFLAGS)  -o $(OBJ)/$@ -c $<

query_vehicle_information.o: query_vehicle_information.c
	$(CC) $(CFLAGS) -o $(OBJ)/$@ -c $<

.PHONY: directories clean

directories: 
	${MKDIR} $(OBJ) 

clean:
	rm -rf $(OBJ)/*.o
	rm -rf $(BIN)/query_vehicle_information
