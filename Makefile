
# compiler
CC=gcc


SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
LIBFILE 	:= libxmljtest.a

CFLAGS		:= -Wall --ansi --pedantic -g3 -O1 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra
CFLAGSu		:= -Wall -g3 -O0
CFLAGSu		:= -O1
LIB			:= -lxml2
INC			:= -I$(INCDIR)


vpath %.h $(INCDIR)
vpath %.o $(BUILDDIR)
vpath %.c $(SRCDIR)


SOURCES     := $(shell find $(SRCDIR) -type f -name *.c)
OBJECTS 	:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))


all: libxmljtest.a


# compiling

parse_error.o: parse_error.c parse_error.h
xml_parse_helper.o: xml_parse_helper.c xml_parse_helper.h
xmltester.o: xmltester.c xmltester.h

$(OBJECTS):
	$(CC) $(INC) -c -o $@ $(filter %.c,$^) $(CFLAGS) $(LIB)


# archive creation

$(LIBFILE): $(patsubst $(BUILDDIR)/%.o, %.o, $(OBJECTS))
	ar -rv $@ $^
	ranlib $@


# cleaning
clean:
	rm $(BUILDDIR)/*.o
	rm $(LIBFILE)


# install: libxmljtest.a
# 	mkdir -p /usr/include/libxmljtest/
# 	cp -u -t /usr/include/libxmljtest/ ./inc/*
# 	mkdir -p /usr/lib/libxmljtest/
# 	cp -u -t /usr/lib/libxmljtest/ libxmljtest.a

# uninstall:
# 	rm /usr/include/libxmljtest/*.h
# 	rmdir /usr/include/libxmljtest/
# 	rm /usr/lib/libxmljtest/*.a
# 	rmdir /usr/lib/libxmljtest/

.PHONY : clean all install uninstall