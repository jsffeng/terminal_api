CC = gcc
RM = rm -rf

DBG_ENABLE = 1

CMOCK_DIR = ../../Lib/cmock
CMOCK_SRC = $(CMOCK_DIR)/src
UNITY_SRC = $(CMOCK_DIR)/vendor/unity/src
MOCKS_DIR = mocks

# Mock file
NAME_DEP = to_do_func
H_DEP = $(NAME_DEP).h
C_MOCKS = $(MOCKS_DIR)/Mock$(NAME_DEP).c

SRC_PATH := .
TEST_DIR := ./test

TARGET := term_api
TEST_TARGET := UTfuncs

# UT code
C_UT = $(TEST_DIR)/$(TEST_TARGET)_ut.c
C_RUN = $(TEST_DIR)/$(TEST_TARGET)_runner.c

TEST_SRCS := $(UNITY_SRC)/unity.c $(CMOCK_SRC)/cmock.c
TEST_SRCS += $(C_MOCKS) $(C_UT) $(C_RUN)

SRCS = $(wildcard $(SRC_PATH)/*.c)
OBJS := $(SRCS:.c=.o)

TEST_OBJS := $(TEST_SRCS:.c=_test.o) 
TEST_OBJS += req_handler_test.o db_api_test.o

LIBS  += microhttpd
LIBS  += cjson

INCLUDE_PATH += /usr/include
INCLUDE_PATH += /usr/local/include/cjson
TEST_INCLUDE_PATH += $(CMOCK_SRC) 
TEST_INCLUDE_PATH += $(UNITY_SRC)
TEST_INCLUDE_PATH += $(MOCKS_DIR)
TEST_INCLUDE_PATH += .

LIBRARY_PATH += /usr/lib
LIBRARY_PATH += /usr/local/lib

ifeq (1, ${DBG_ENABLE})
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
endif

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
DUTFLAG += -DUTFLAG
TEST_CFLAGS += $(foreach dir, $(TEST_INCLUDE_PATH), -I$(dir))
LDFLAGS += $(foreach lib, $(LIBRARY_PATH), -L$(lib))
LIBFLAGS += $(foreach lib, $(LIBS), -l$(lib))

.PHONY: BLD UT mock clean_all clean_test clean_build

all: BLD UT

UT:  clean_test mock $(TEST_DIR)/$(TEST_TARGET)

BLD:  clean_build $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBFLAGS)

$(TEST_DIR)/$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(DUTFLAG) $(CFLAGS) $(TEST_CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBFLAGS)

%_test.o:%.c
	$(CC) $(DUTFLAG) $(CFLAGS) $(TEST_CFLAGS) -o $@ -c $<
%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

mock:
	mkdir -p $(MOCKS_DIR)
	ruby $(CMOCK_DIR)/lib/cmock.rb $(H_DEP)
	ruby $(CMOCK_DIR)/vendor/unity/auto/generate_test_runner.rb $(C_UT) $(C_RUN)

clean_all: clean_build clean_test

clean_test:
	$(RM) $(MOCKS_DIR)
	$(RM) $(TEST_OBJS) $(TEST_DIR)/$(TEST_TARGET) $(C_RUN) $(TEST_DIR)/*.log

clean_build: 
	$(RM) $(OBJS) $(TARGET) *.log

