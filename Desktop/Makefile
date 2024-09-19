TARGET := ardusdl
VPATH = ..
PKGCONFIG := $(shell which pkg-config)
CXXFLAGS := $(shell $(PKGCONFIG) --cflags sdl2)
LDLIBS := $(shell $(PKGCONFIG) --libs sdl2)
SOURCES := $(wildcard $(VPATH)/*.cpp *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)
INCLUDES := $(wildcard $(VPATH)/*.h *.h)

all: $(TARGET)

%.o : *.cpp $(INCLUDES)
	$(CXX) -c -o $(@F) $(CXXFLAGS) $<

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDLIBS)


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)
