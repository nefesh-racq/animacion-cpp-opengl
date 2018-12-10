## compilando la animacion
SRC = animacion.cpp
LIB = -lGL -lGLU -lSDL2
BINARY = animacion.out

all: $(SRC)
	g++ $(LIB) $(SRC) -o $(BINARY)


clean:
	rm $(BINARY)
