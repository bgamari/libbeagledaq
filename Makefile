DESTDIR ?= /usr/local
CXXFLAGS = -I. -std=c++0x -fPIC ${EXTRA_CXXFLAGS}

all : libbeagledaq.so libbeagledaq.a tools/dac-output tools/adc-acquire tools/benchmark

libbeagledaq.so : beagledaq.o spi_device.o
	${CXX} -shared ${LDFLAGS} -o $@ $+

libbeagledaq.a : beagledaq.o spi_device.o
	ar rcs $@ $+

tools/dac-output : spi_device.o
tools/adc-acquire : spi_device.o
tools/benchmark : EXTRA_CXXFLAGS=-lrt
tools/benchmark : spi_device.o

clean :
	rm -f *.o *.so tools/dac-output

install : libbeagledaq.so
	cp libbeagledaq.so ${DESTDIR}/lib/libbeagledaq.so
	mkdir -p ${DESTDIR}/include/beagledaq/
	cp beagledaq.h ${DESTDIR}/include/beagledaq/beagledaq.h
	cp spi_device.h ${DESTDIR}/include/beagledaq/spi_device.h

# For automatic header dependencies
.deps/%.d : %
	@mkdir -p .deps
	@cpp ${INCLUDES} -std=c++0x -MM $< > $@

SOURCES = $(wildcard *.cpp) $(wildcard *.c)
-include $(addprefix .deps/,$(addsuffix .d,$(SOURCES)))

