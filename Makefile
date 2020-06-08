OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
CPPFLAGS += -pthread -fgnu-tm -std=c++14 -mrtm

all: $(OBJECTS)
%.o: %.cpp
	g++ -o $@ $< $(CPPFLAGS)

clean: 
	rm -f $(OBJECTS)
