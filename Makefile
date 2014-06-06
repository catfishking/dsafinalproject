OBJ1 = main
OBJ2 = command
PROB = $(OBJ1) $(OBJ2) link
TARGET = $(PROB) 
LINK = main.o command.o 
.PHONY: all 

all: $(TARGET)

$(OBJ1):
	g++ main.cpp -c -o main.o

$(OBJ2):
	g++ command.cpp -c -o command.o

link: $(LINK)
	g++ $(LINK) -o main

clean :
	rm -f main.o command.o main
