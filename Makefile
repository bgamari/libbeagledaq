CXXFLAGS=-I. -std=c++0x -fPIC

all : libbeagledaq.so libbeagledaq.a tools/dac-output tools/adc-acquire

libbeagledaq.so : beagledaq.o spi_device.o
	${CXX} -shared ${LDFLAGS} -o $@ $+

libbeagledaq.a : beagledaq.o spi_device.o
	ar rcs $@ $+

tools/dac-output : spi_device.o
tools/adc-acquire : spi_device.o

clean :
	rm -f *.o *.so tools/dac-output

# For automatic header dependencies
.deps/%.d : %
	@mkdir -p .deps
	@cpp ${INCLUDES} -std=c++0x -MM $< > $@

SOURCES = $(wildcard *.cpp) $(wildcard *.c)
-include $(addprefix .deps/,$(addsuffix .d,$(SOURCES)))

