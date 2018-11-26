
CC = gcc
RM = rm -rf

DBG_ENABLE = 0

SRC_PATH := .

TARGET := term_api

TEST_TARGET := ./test/test_func

SRCS +=$(wildcard $(SRC_PATH)/*.c)

OBJS := $(SRCS:.c=.o)

TEST_SRCS += ./test/test_func.c
TEST_OBJS += ./test/test_func.o
TEST_OBJS += ./test/req_handler.o
TEST_FUNC += req_handler.c

LIBS  += microhttpd

LIBS  += cjson

INCLUDE_PATH += /usr/include

INCLUDE_PATH += /usr/local/include/cjson

LIBRARY_PATH += /usr/lib

LIBRARY_PATH += /usr/local/lib

ifeq (1, ${DBG_ENABLE})
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
endif

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))

LDFLAGS += $(foreach lib, $(LIBRARY_PATH), -L$(lib))

LDFLAGS += $(foreach lib, $(LIBS), -l$(lib))


.PHONY: clean

all: clean build test

build:
	$(CC) -c $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
clean:
	$(RM) $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET) libfunc.a
