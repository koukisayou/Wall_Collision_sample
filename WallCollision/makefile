.PHONY: main
main:
	g++ -c Wall.cpp
	g++ -c Collision.cpp
	g++ -c Ball.cpp -framework OpenGL -framework GLUT -mmacosx-version-min=10.8
	g++ -o main main.cpp Wall.o Collision.o Ball.o -framework OpenGL -framework GLUT -mmacosx-version-min=10.8