TARGET := tests
VPATH := ..
PKGCONFIG := $(shell which pkg-config)
INCLUDE := $(shell $(PKGCONFIG) --cflags gtest) 
LIBS := $(shell $(PKGCONFIG) --libs gtest_main)
CXXFLAGS = -g $(INCLUDE)
SOURCES := objects.cpp vehicle.cpp rotate.cpp intersection.cpp tests.cpp utils.cpp target.cpp
OBJECTS := $(SOURCES:%.cpp=%.o)
INCLUDES := $(wildcard *.h)

all: $(TARGET)

%.o : %.cpp | $(INCLUDES)
	$(CXX) -c -o $(@F) $(CXXFLAGS) $<

$(TARGET) : $(OBJECTS)
	$(CXX) $(LIBS) -o $@ $(^F)


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
