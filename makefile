#
# Makefile
#
# $Copyright (c) 2016 Software AG, Darmstadt, Germany and/or Software AG USA Inc., Reston, VA, USA, and/or its subsidiaries and/or its affiliates and/or their licensors.$
# Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Software AG

# Make sure the required Apama envars have been defined
ifndef APAMA_HOME
$(warning Did you remember to source the apama_env script?)
$(error APAMA_HOME is not defined)
endif
ifndef APAMA_WORK
$(warning Did you remember to source the apama_env script?)
$(error APAMA_WORK is not defined)
endif

# C++ compiler
CXX := /usr/bin/g++ 
DEFINES := -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
# -D__arm__ -mfloat-abi=hard
CXXFLAGS := -std=c++11 -w -fPIC $(DEFINES)
INCLUDES := "-I$(APAMA_HOME)/include"
LDFLAGS := -shared -L"$(APAMA_HOME)/lib"

BINDIR := ./bin
OBJDIR := ./obj

# List each of the C++ source files
GPIOPLUGIN-SRC-CPP := \
	GPIOPlugin.cpp

# the set of object files for this project
GPIOPLUGIN-OBJ := GPIOPlugin

# the set of dependency (*.d) files for this project
GPIOPLUGIN-DEP := $(patsubst %.cpp, %.d, $(GPIOPLUGIN-SRC-CPP) )

# --------------------------------------------------------------------

# Toplevel target of this file
gpioplugin: $(OBJDIR) $(BINDIR)/libGPIOPlugin.so install

.PHONY: gpioplugin clean

$(OBJDIR)/$(GPIOPLUGIN-OBJ):$(GPIOPLUGIN-SRC-CPP)
	$(CXX) -c $^ -o $(OBJDIR)/$(GPIOPLUGIN-OBJ) $(CXXFLAGS) $(INCLUDES)

$(BINDIR)/libGPIOPlugin.so: $(OBJDIR)/$(GPIOPLUGIN-OBJ)
	mkdir -p $(BINDIR)
	$(CXX) -o $@ $(OBJDIR)/$(GPIOPLUGIN-OBJ) $(LDFLAGS) -lwiringPi
	
install:
	mkdir -p $(APAMA_WORK)/lib
	mkdir -p $(APAMA_WORK)/monitors
	cp $(BINDIR)/*.so $(APAMA_WORK)/lib
	cp *.mon $(APAMA_WORK)/monitors

$(OBJDIR):
	mkdir -p $(OBJDIR)

# clean target for this module
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
