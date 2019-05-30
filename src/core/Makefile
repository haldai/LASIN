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
	$(CXX) $(LDFLAGS) test_abduce.o $(LDFLAGS_SWI) $(LDFLAGS_MLPACK) -o test

<<<<<<< HEAD
test_abduce.o:test_abduce.cpp utils.hpp
=======
test_abduce.o:test_abduce.cpp
>>>>>>> cdbedcaa75ac82c9cf2c92ecd0e5040ca1978839
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) $(CXXFLAGS_MLPACK) -c test_abduce.cpp

pl_data.so:pl_data.o
	$(CXX) -shared $(LDFLAGS) pl_data.o $(LDFLAGS_MLPACK) -o pl_data.so

<<<<<<< HEAD
pl_data.o:pl_data.cpp utils.hpp
=======
pl_data.o:pl_data.cpp
>>>>>>> cdbedcaa75ac82c9cf2c92ecd0e5040ca1978839
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_data.cpp

pl_models.so:pl_models.o
	$(CXX) -shared $(LDFLAGS) pl_models.o $(LDFLAGS_MLPACK) -o pl_models.so

<<<<<<< HEAD
pl_models.o:pl_models.cpp utils.hpp
=======
pl_models.o:pl_models.cpp
>>>>>>> cdbedcaa75ac82c9cf2c92ecd0e5040ca1978839
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_models.cpp

pl_cluster.so:pl_cluster.o
	$(CXX) -shared $(LDFLAGS) pl_cluster.o $(LDFLAGS_MLPACK) -o pl_cluster.so

<<<<<<< HEAD
pl_cluster.o:pl_cluster.cpp utils.hpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_cluster.cpp

pl_mnist.so:pl_mnist.o get_strokes_impl.o
	$(CXX) -shared $(LDFLAGS) pl_mnist.o get_strokes_impl.o $(LDFLAGS_MLPACK) -o pl_mnist.so

pl_mnist.o:pl_mnist.cpp utils.hpp
=======
pl_cluster.o:pl_cluster.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_cluster.cpp

pl_mnist.so:pl_mnist.o
	$(CXX) -shared $(LDFLAGS) pl_mnist.o $(LDFLAGS_MLPACK) -o pl_mnist.so

pl_mnist.o:pl_mnist.cpp
>>>>>>> cdbedcaa75ac82c9cf2c92ecd0e5040ca1978839
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c pl_mnist.cpp

get_strokes_impl.o: get_strokes_impl.cpp get_strokes.h
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SWI) -c get_strokes_impl.cpp

#test_lib.so:test_lib.o
#       $(CXX) -shared $(LDFLAGS) $(LD_SHARED) ../arff/*.o test_lib.o -o test_lib.so

#test_lib.o:
#       $(CXX) $(CXXFLAGS) $(CXXFLAGS_SHARK) $(CXXFLAGS_SWI) -c test_lib.cpp

arff:
	cd $(ARFFDIR) && $(MAKE)

### mostly generic ###
#%.o: %.cpp
#       $(CXX) $(CXXFLAGS) $(CXXFLAGS_SHARK) $(CXXFLAGS_SWI) -c $<

clean:
	cd $(ARFFDIR); $(MAKE) clean
	rm -rf *.o *.so *.a symblearn test
