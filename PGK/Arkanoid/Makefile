CC = g++
GLFLAGS = -lGLEW -lGL -lglfw
CFLAGS = -Wall -Werror -Wextra -pedantic

#default: zad1.out

all: main.o extraFunctions.o intersection.o
	$(CC) extraFunctions.o intersection.o main.o -o main.out -lGLEW -lGL -lglfw -L/Desktop/lib common/shader.cpp common/controls.cpp


intersection.o: intersection.cpp intersection.hpp
	$(CC) $(CFLAGS) intersection.cpp -c -o intersection.o

extrafunctions.o: extraFunctions.cpp extraFunctions.hpp
	$(CC) $(CFLAGS) extraFunctions.cpp -c -o extraFunctions.o

main.o: main.cpp extraFunctions.cpp extraFunctions.hpp intersection.cpp intersection.hpp
	$(CC) $(GLFLAGS) main.cpp -c -o main.o -I/Desktop/include/ -I.

%.out: %.o
	g++ $<  -o  $@   -lGLEW -lGL -lglfw   -L/Desktop/lib common/shader.cpp common/controls.cpp

clean:
	rm *.o *.out *~
