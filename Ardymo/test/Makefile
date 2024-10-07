TARGET := structs
CXXFLAGS = -g
VPATH = ..
PKGCONFIG := $(shell which pkg-config)
ARDYMOSRC := ../objects.cpp ../intersection.cpp
SOURCES := $(wildcard *.cpp) $(ARDYMOSRC)
OBJECTS := $(notdir $(SOURCES:%.cpp=%.o))
ARDYMOINC := ../objects.h ../intersection.h ../structs.h
INCLUDES := $(wildcard *.h) $(ARDYMOINC)

all: $(TARGET)

%.o : %.cpp $(SOURCES) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $(@F) -iquote .. $<

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
