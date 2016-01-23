#Build tools
CC	    = gcc
CFLAGS     += -std=c99 -Wall -g -fPIE -D_FORTIFY_SOURCE=2
LDFLAGS	   += -pie -lm 

# Specify extensions of files to delete when cleaning
CLEANEXTS   = o a 

# Specify the target file and the install directory
OUTPUTFILE  = libqth.a
INSTALLDIR  = /usr/local/lib/

# Default target
.PHONY: all
all: $(OUTPUTFILE)

$(OUTPUTFILE): qth.o
	ar rcs $@ $^
	ranlib $@

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: uninstall
uninstall:
	rm -f $(INSTALLDIR)/$(OUTPUTFILE)

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done
	rm -f testlib

testlib: testmain.o $(OUTPUTFILE)
	$(CC) $(LDFLAGS) -o $@ $?
	./$@

# Indicate dependencies of .ccp files on .hpp files
qth.o: qth.h

