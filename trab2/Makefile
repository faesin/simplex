CC=g++
CFLAGS= -c -Wall -std=c++11
LFLAGS=
EXEC=main
SRCS=main.cpp
OBJ=$(SRCS:.cpp=.o)

.PHONY: clean

all: $(SRCS) $(EXEC)

$(EXEC): $(OBJ)
		$(CC) $(OBJ) -o $@ $(LFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	-rm *.o
	-rm $(EXEC)

