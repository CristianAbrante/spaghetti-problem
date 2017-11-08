
# makefile used to compile project 10151 ICPC
# Authors: Carlos Domínguez García, Daute Rodríguez Rodríguez, Cristian Abrante Dorta

#Variables used for compilation.
CXX = g++
BINARY = spaghetti

IDIR = include
ODIR = obj

_DEPS = functions.hpp program-functions.hpp
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o functions.o program-functions.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

CXXFLAGS = -g --std=c++11 -I$(IDIR)

all: $(BINARY)

# Creation of objects under obj/ folder.
$(ODIR)/%.o: src/%.cpp $(DEPS)

		@if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi

		$(CXX) -c -o $@ $< $(CXXFLAGS)


# Compilation of the binary program.
$(BINARY): $(OBJ)
		$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(ODIR)/*.o $(BINARY)
	rm -r $(ODIR)
