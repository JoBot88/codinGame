all:
	make generator
	make processor

generator: generator.cpp
	g++ -Wall -std=c++11 -pthread generator.cpp -o generator

processor: processor.cpp
	g++ -Wall -std=c++11 -fopenmp -pthread processor.cpp -o processor

clean:
	rm -f generator
	rm -f processor