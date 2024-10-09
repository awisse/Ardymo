TARGET := rotate
VPATH := ../..
CXXFLAGS = -g
ARDYMOSRC := objects.cpp rotate.cpp
SOURCES := $(wildcard *.cpp) $(ARDYMOSRC)
OBJECTS := $(notdir $(SOURCES:%.cpp=%.o))
ARDYMOINC := objects.h rotate.h
INCLUDES := $(wildcard *.h) $(ARDYMOINC)

all: $(TARGET)

%.o : %.cpp $(SOURCES) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c -o $(@F) -iquote ../.. $<

$(TARGET) : $(OBJECTS)
	$(CXX) $(LIBS) -o $@ $^


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
