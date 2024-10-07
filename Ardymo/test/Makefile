TARGET := structs
CXXFLAGS = -g
VPATH = ..
PKGCONFIG := $(shell which pkg-config)
ARDYMOSRC := ../objects.cpp ../intersection.cpp
SOURCES := $(wildcard *.cpp) $(ARDYMOSRC)
OBJECTS := $(notdir $(SOURCES:%.cpp=%.o))
INCLUDES := $(wildcard *.h)

all: $(TARGET)

%.o : %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $(@F) -I.. $<

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
