OPT = -O3 -I.

subsetting_example: subsetting_example.cpp
	g++ $(OPT) subsetting_example.cpp -o subsetting_example

clean:
	rm -f subsetting_example subsetting_example.exe *.o
