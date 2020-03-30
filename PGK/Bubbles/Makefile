CC = g++
GLFLAGS = -lGLEW -lGL -lglfw
CFLAGS = -Wall -pedantic

#default: zad1.out

all: main.o Global.o Controls.o Mesh.o Scene.o Actor.o Ball.o Component.o MeshComponent.o Object.o
	$(CC) Global.o Controls.o Mesh.o Scene.o Actor.o Ball.o Component.o MeshComponent.o Object.o main.o -o main.out -lGLEW -lGL -lglfw -L/Desktop/lib common/shader.cpp common/texture.cpp common/objloader.cpp common/vboindexer.cpp

Object.o: Object.cpp Object.hpp
	$(CC) $(CFLAGS) Object.cpp -c -o Object.o

MeshComponent.o: MeshComponent.cpp MeshComponent.hpp
	$(CC) $(CFLAGS) MeshComponent.cpp -c -o MeshComponent.o

Component.o: Component.cpp Component.hpp
	$(CC) $(CFLAGS) Component.cpp -c -o Component.o

Ball.o: Ball.cpp Ball.hpp
	$(CC) $(CFLAGS) Ball.cpp -c -o Ball.o

Actor.o: Actor.cpp Actor.hpp
	$(CC) $(CFLAGS) Actor.cpp -c -o Actor.o

Scene.o: Scene.cpp Scene.hpp
	$(CC) $(CFLAGS) Scene.cpp -c -o Scene.o

Mesh.o: Mesh.cpp Mesh.hpp
	$(CC) $(CFLAGS) Mesh.cpp -c -o Mesh.o

Controls.o: Controls.cpp Controls.hpp
	$(CC) $(CFLAGS) Controls.cpp -c -o Controls.o

Global.o: Global.cpp Global.hpp
	$(CC) $(CFLAGS) Global.cpp -c -o Global.o

main.o: main.cpp Global.cpp Global.hpp Controls.cpp Controls.hpp Mesh.cpp Mesh.hpp Scene.cpp Scene.hpp Actor.cpp Actor.hpp Ball.cpp Ball.hpp Component.cpp Component.hpp MeshComponent.cpp MeshComponent.hpp Object.cpp Object.hpp
	$(CC) $(GLFLAGS) main.cpp -c -o main.o -I/Desktop/include/ -I.

%.out: %.o
	g++ $<  -o  $@   -lGLEW -lGL -lglfw   -L/Desktop/lib common/shader.cpp common/texture.cpp common/objloader.cpp common/vboindexer.cpp

clean:
	rm *.o *.out *~
