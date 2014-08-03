



all: makePictures localMaxima localMinima makePictures calculateMeasures 


makePictures: makePictures.cpp
	g++ -g -O3 -o makePictures makePictures.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -I../tools -lboost_program_options

localMaxima: localMaxima.cpp
	g++  -O2 -o localMaxima localMaxima.cpp -lboost_program_options

localMinima: localMinima.cpp
	g++  -g -O3 -o localMinima localMinima.cpp -lboost_program_options

calculateMeasures: calculateMeasures.cpp dir.h measures.o
	g++  -g -O3 -o calculateMeasures calculateMeasures.cpp measures.o -lboost_program_options

measures.o: measures.cpp measures.h
	g++  -g -O3 -c measures.cpp


install: all
	cp -f makePictures ~/bin
	cp -f calculateMeasures ~/bin
	cp -f localMaxima ~/bin
	cp -f localMinima ~/bin

clean:
	rm -f calculateMeasures makePictures *.o localMaxima localMinima
	






.PHONY: all clean install
