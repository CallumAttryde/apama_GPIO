#
# Makefile
#
# $Copyright (c) 2018 Software AG, Darmstadt, Germany and/or Software AG USA Inc., Reston, VA, USA, and/or its subsidiaries and/or its affiliates and/or their licensors.$
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
CXXFLAGS := -std=c++11 -w -fPIC $(DEFINES)
INCLUDES := "-I$(APAMA_HOME)/include" "-I./lib"
LDFLAGS := -shared -L"$(APAMA_HOME)/lib"

BINDIR := ./bin
OBJDIR := ./obj

# the set of object files for this project
SenseHATPLUGIN-OBJ := SenseHATPlugin

SenseHATPLUGIN-SRC-CPP := senseHATPlugin.cpp
# --------------------------------------------------------------------

# Toplevel target of this file
senseHATplugin: $(OBJDIR) $(BINDIR)/libSenseHATPlugin.so install

.PHONY: senseHATplugin clean

$(OBJDIR)/$(SenseHATPLUGIN-OBJ):$(SenseHATPLUGIN-SRC-CPP)
	$(CXX) -c $^ -o $(OBJDIR)/$(SenseHATPLUGIN-OBJ) $(CXXFLAGS) $(INCLUDES)

$(BINDIR)/libSenseHATPlugin.so: $(OBJDIR)/$(SenseHATPLUGIN-OBJ)
	mkdir -p $(BINDIR)
	$(CXX) -o $@ $(OBJDIR)/$(SenseHATPLUGIN-OBJ) $(LDFLAGS) -Llib -lsense-hat
	
install:
	mkdir -p $(APAMA_WORK)/lib
	mkdir -p $(APAMA_WORK)/monitors
	cp $(BINDIR)/libSenseHATPlugin.so $(APAMA_WORK)/lib
	cp lib/libsense-hat.so $(APAMA_WORK)/lib
	cp SenseHATPlugin.mon $(APAMA_WORK)/monitors

$(OBJDIR):
	mkdir -p $(OBJDIR)

# clean target for this module
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
