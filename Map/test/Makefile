TARGET := tests
VPATH := ..:../src
PKGCONFIG := $(shell which pkg-config)
INCLUDE := $(shell $(PKGCONFIG) --cflags gtest)
LIBS := $(shell $(PKGCONFIG) --libs gtest_main)
CXXFLAGS = -g $(INCLUDE)
ARDYMOSRC := objects.cpp intersection.cpp rotate.cpp viewport.cpp
SOURCES := $(wildcard *.cpp) $(ARDYMOSRC)
OBJECTS := $(notdir $(SOURCES:%.cpp=%.o))

all: $(TARGET)

%.o : %.cpp $(SOURCES) %.h
	$(CXX) $(CXXFLAGS) -c -o $(@F) $<

$(TARGET) : $(OBJECTS)
	$(CXX) $(LIBS) -o $@ $^


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
