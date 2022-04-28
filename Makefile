# compiler
COMPILER = g++
CXXFLAGS = -O3 -std=c++11

# library
LDFLAGS = -lglfw -lGLEW -framework OpenGL
LIBS    = 

# include
INCLUDE = -I./inc

# binary
TARGETS   = sphswe
TARGETDIR = ./bin

# source
SRCROOT  = .
SRCDIRS := $(shell find $(SRCROOT) -type d)
SOURCES  = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

# object
OBJROOT = .
OBJECTS = $(addprefix $(OBJROOT)/, $(SOURCES:.cpp=.o))
OBJDIRS = $(addprefix $(OBJROOT)/, $(SRCDIRS))

$(TARGETS): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $(TARGETDIR)/$@ $^ $(LDFLAGS)

$(OBJROOT)/%.o: $(SRCROOT)/%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

run: $(TARGETS)
	cd $(TARGETDIR); ./$(TARGETS); cd -

clean: 
	rm -f $(OBJECTS) $(TARGETDIR)/$(TARGETS)