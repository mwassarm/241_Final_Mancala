all: mancala

manacala: mancala.cpp
	g++ -o mancala mancala.cpp

clean:
	rm -f mancala
