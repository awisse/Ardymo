TARGET := tests
VPATH := ..
PKGCONFIG := $(shell which pkg-config)
INCLUDE := $(shell $(PKGCONFIG) --cflags gtest)
LIBS := $(shell $(PKGCONFIG) --libs gtest_main)
CXXFLAGS = -g $(INCLUDE)
ARDYMOSRC := objects.cpp intersection.cpp rotate.cpp vehicle.cpp
SOURCES := $(wildcard *.cpp) $(ARDYMOSRC)
OBJECTS := $(notdir $(SOURCES:%.cpp=%.o))
ARDYMOINC := objects.h intersection.h structs.h rotate.h
INCLUDES := $(wildcard *.h) $(ARDYMOINC)

all: $(TARGET)

%.o : %.cpp $(SOURCES) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $(@F) -iquote .. $<

$(TARGET) : $(OBJECTS)
	$(CXX) $(LIBS) -o $@ $^


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
