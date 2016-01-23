
TORCH_PATH ?= $(HOME)/torch/install
CXX = gcc
CFLAGS=-std=c99 -Wall -pedantic -O2 -I$(TORCH_PATH)/include/TH -I$(TORCH_PATH)/include 
LDFLAGS = -L$(TORCH_PATH)/lib
LDLIBS =-lluajit -lm -l


%.so: %.c
	$(CXX) $(CFLAGS) -fpic -shared -o $@ $^


