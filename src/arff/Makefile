SRCDIR = .
#SRCFILES := $(shell find $(SRCDIR) -name "*.cpp")
SRCFILES = $(shell find $(SRCDIR) -name "arff_*.cpp")
OBJFILES = $(patsubst %.cpp,%.o,$(SRCFILES))

CXX = g++
CXXFLAGS = -std=c++11 -Wall -fPIC -Winvalid-pch -g3 -O0
INCLUDE = -I$(SRCDIR) -I../../include
LIBS = -L../../libs

default:$(OBJFILES)

test:$(OBJFILES) test_arff.o
	$(CXX) -g $(OBJFILES) test_arff.o -o test
test.o:
	$(CXX) $(CXXFLAGS) -c test_arff.cpp

### mostly generic ###
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

clean:
	rm -f *.o *.a test
###
