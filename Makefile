CXXFLAGS=-I. -std=c++0x -fPIC

all : libbeagledaq.so tools/dac-output

libbeagledaq.so : beagledaq.o spi_device.o
	${CXX} -shared ${LDFLAGS} -o $@ $+

clean :
	rm -f *.o *.so tools/dac-output

# For automatic header dependencies
.deps/%.d : %
	@mkdir -p .deps
	@cpp ${INCLUDES} -std=c++0x -MM $< > $@

SOURCES = $(wildcard *.cpp) $(wildcard *.c)
-include $(addprefix .deps/,$(addsuffix .d,$(SOURCES)))

