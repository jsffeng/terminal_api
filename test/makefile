
CC = gcc
RM = rm -rf

DBG_ENABLE = 0

SRC_PATH := .

TARGET := term_api

TEST_TARGET := test_func

SRCS +=$(wildcard $(SRC_PATH)/*.c)

OBJS := $(SRCS:.c=.o)

TEST_SRCS + = test_func.c
TEST_OBJS + = test_func.o
TEST_FUNC + = ../req_handler.c

LIBS  += microhttpd

LIBS  += cjson

INCLUDE_PATH += /usr/include

INCLUDE_PATH += /usr/include/cjson

LIBRARY_PATH += /usr/lib

ifeq (1, ${DBG_ENABLE})
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
endif

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))

LDFLAGS += $(foreach lib, $(LIBRARY_PATH), -L$(lib))

LDFLAGS += $(foreach lib, $(LIBS), -l$(lib))


.PHONY: clean

all: test

test:
	$(CC) -c $(CFLAGS) ../req_handler.c
	ar rcs -o ../libfunc.a req_handler.o
	gcc $(CFLAGS) -I/usr/include/cjson -o test_func\
	 ./test_func.c ../libfunc.a -lcjson
