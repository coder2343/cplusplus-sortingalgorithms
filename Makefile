# Makefile for CSC-301 Project #1
# Eric A. Autry, 09/08/22
# 
# As usual, you can build this software by typing:
#
#     make


# ----- Make Macros -----

CXXFLAGS =	-O3
CXX	=	gcc

TARGETS =	project1tests
OBJECTS =	project1tests.cpp project1.cpp
REMOVE =	project1tests.o project1.o
LINKS =	-std=c++20 -lstdc++ -lm -llapack -lblas
INCLUDE = 

# ----- Make Rules -----

all:	$(TARGETS)

project1tests:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o project1tests $(OBJECTS) $(LINKS)

clean:
	rm -f $(TARGETS) $(REMOVE)