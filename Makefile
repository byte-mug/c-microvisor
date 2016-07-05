SHELL := bash

.PHONY: clean all

all: c-microvisor

c-microvisor: main.o
	$(CC) $(LDFLAGS) $? -o $@


