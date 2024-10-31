PKGCONFIG := $(shell which pkg-config)
CXXFLAGS := -g -O0 $(shell $(PKGCONFIG) --cflags sdl2) -pedantic-errors
LDLIBS := $(shell $(PKGCONFIG) --libs sdl2)

ardymodir := ../Ardymo
ardymosources = $(wildcard $(ardymodir)/*.cpp *.cpp)
ardymoobjects = $(ardymosources:%.cpp=%.o)

mapdir := ../Map
mapsources = $(wildcard $(mapdir)/src/*.cpp $(mapdir)/*.cpp *.cpp)
mapobjects = $(mapsources:%.cpp=%.o)

INCLUDES = $(wildcard $(mapdir)/*.h $(ardymodir)/*.h *.h)

all: ardymo map

%.o : %.cpp $(INCLUDES)
	$(CXX) -c -o $(@F) $(CXXFLAGS) $<

ardymo : $(ardymoobjects)
	$(CXX) -o $@ $(^F) $(LDLIBS)

map : $(mapobjects)
	$(CXX) -o $@ $(^F) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) map ardymo
	$(RM) *.o
