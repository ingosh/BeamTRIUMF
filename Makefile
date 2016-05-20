# Makefile

CXXFLAGS = -g -O2 -Wall -Wuninitialized

# optional ZLIB library

CXXFLAGS += -DHAVE_ZLIB

# ROOT libraries

ifdef ROOTSYS
ROOTGLIBS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lThread
HAVE_RHTTP = $(shell $(ROOTSYS)/bin/root-config --has-http)
ifeq ($(HAVE_RHTTP),yes)
ROOTGLIBS += -lRHTTP
endif
CXXFLAGS += -DHAVE_ROOT $(shell $(ROOTSYS)/bin/root-config --cflags)
endif

ifdef MIDASSYS
MIDASLIBS = $(MIDASSYS)/linux/lib/libmidas.a -lutil -lrt
CXXFLAGS += -DHAVE_MIDAS -DOS_LINUX -Dextname -I$(MIDASSYS)/include

UNAME=$(shell uname)
RTLIB=
ifeq ($(UNAME),Darwin)
#CXXFLAGS += -DOS_LINUX -DOS_DARWIN
MIDASLIBS = $(MIDASSYS)/darwin/lib/libmidas.a
#RPATH=
else
RTLIB=-lrt
endif

endif

ifdef ROOTSYS
CXXFLAGS += -DHAVE_LIBNETDIRECTORY -IlibNetDirectory
endif


# Check for ROOTANASYS env variable.
ifdef ROOTANASYS
ROOTANAINC = -I$(ROOTANASYS)/include
ROOTANALIBS = $(ROOTANASYS)/lib/librootana.a
else
ROOTANAINC = -I../include
ROOTANALIBS = ../lib/librootana.a
endif

all: accessMidasFiles.exe

accessMidasFiles.exe: accessMidasFiles.cxx
	$(CXX) -o $@ $(CXXFLAGS) $(ROOTANAINC) $^ $(ROOTANALIBS) $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH) -lssl $(RTLIB) -lutil

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(ROOTANAINC) -c $<

dox:
	doxygen

clean::
	-rm -f *.o *.a *.exe 

# end

