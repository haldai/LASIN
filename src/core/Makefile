# swipl: load_foreign_library(foreign('img_process.so')).

SRCDIR = .
ARFFDIR = ../arff
LIBSDIR = ../../libs
INCLUDEDIR = ../../include
SRCFILES = $(shell find $(SRCDIR) -name "*.cpp")
OBJFILES = $(patsubst %.cpp,%.o,$(SRCFILES))

CXX = g++
MAKE = make
CXXFLAGS = -Wall -std=c++11 -fPIC -fopenmp $(COFLAGS) $(INCLUDE)
COFLAGS = -DO_DEBUG -gdwarf-2 -g -O0
LDFLAGS = -Wall -Winvalid-pch -fPIC -fopenmp $(LIBS) $(COFLAGS)
INCLUDE = -I$(SRCDIR) -I$(ARFFDIR) -I$(INCLUDEDIR) -I$(HOME)/.local/include
LIBS = -L$(SRCDIR) -L$(ARFFDIR) -L$(LIBSDIR) -L$(HOME)/.local/lib -L$(HOME)/.local/lib64

# Library prefix
CXXFLAGS_MLPACK = `pkg-config --cflags mlpack`
LDFLAGS_MLPACK = `pkg-config --libs mlpack`

# SWI-prolog
CXXFLAGS_SWI = `pkg-config --cflags swipl`
LDFLAGS_SWI = `pkg-config --libs swipl`

all:test pl_data.so pl_models.so pl_mnist.so pl_cluster.so

test:test_abduce.o #arff ../arff/*.o
	$(CXX) $(LDFLAGS) $(LDFLAGS_SWI) $(LDFLAGS_MLPACK) test_abduce.o -o test

test_abduce.o:
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) $(CXXFLAGS_MLPACK) -c test_abduce.cpp

pl_data.so:pl_data.o
	$(CXX) -shared $(LDFLAGS) $(LDFLAGS_MLPACK) pl_data.o -o pl_data.so

pl_data.o:
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_data.cpp

pl_models.so:pl_models.o
	$(CXX) -shared $(LDFLAGS) $(LDFLAGS_MLPACK) pl_models.o -o pl_models.so

pl_models.o:
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_models.cpp

pl_cluster.so:pl_cluster.o
	$(CXX) -shared $(LDFLAGS) $(LDFLAGS_MLPACK) pl_cluster.o -o pl_cluster.so

pl_cluster.o:
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_cluster.cpp

pl_mnist.so:pl_mnist.o
	$(CXX) -shared $(LDFLAGS) $(LDFLAGS_MLPACK) pl_mnist.o -o pl_mnist.so

pl_mnist.o:
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_mnist.cpp


#test_lib.so:test_lib.o
#	$(CXX) -shared $(LDFLAGS) $(LD_SHARED) ../arff/*.o test_lib.o -o test_lib.so

#test_lib.o:
#	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SHARK) $(CXXFLAGS_SWI) -c test_lib.cpp

arff:
	cd $(ARFFDIR) && $(MAKE)

### mostly generic ###
#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SHARK) $(CXXFLAGS_SWI) -c $<

clean:
	cd $(ARFFDIR); $(MAKE) clean
	rm -rf *.o *.so *.a symblearn test
