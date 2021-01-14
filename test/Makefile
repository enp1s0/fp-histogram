CXX=g++
CXXFLAGS=-std=c++11 -I../include -Wall
TARGET=fphistogram.test

$(TARGET):main.cpp ../include/fphistogram/fphistogram.hpp
	$(CXX) $< $(CXXFLAGS) -o $@

clean:
	rm -f $(TARGET)
