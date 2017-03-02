CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: main

main: main.cpp ppm.c log.c estebanL.cpp anaB.cpp markF.cpp adamC.cpp analyV.cpp
	g++ $(CFLAGS) main.cpp log.c libggfonts.a estebanL.cpp anaB.cpp markF.cpp adamC.cpp analyV.cpp -Wall -Wextra $(LFLAGS) -o main

clean:
	rm -f main
	rm -f *.o

