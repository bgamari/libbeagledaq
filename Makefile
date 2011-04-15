CXXFLAGS=-I. -std=c++0x -fPIC

all : libbeagledaq.so tools/dac-output

libbeagledaq.so : beagledaq.o spi_device.o
	${CXX} -shared ${LDFLAGS} -o $@ $+

clean :
	rm *.o *.so
