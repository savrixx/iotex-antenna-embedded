AR		= $(CROSS_COMPILE)ar
CC		= $(CROSS_COMPILE)gcc
CXX		= $(CROSS_COMPILE)g++
LDSHFLAGS	= -rdynamic -shared 
ARFLAGS		= rcv
CFLAGS		= -Wall -g -fPIC -I../src
CXXFLAGS	= -Wall -g -fPIC -I../src
LDFLAGS		= -L.. -liotexemb -lcurl -Wl,-R -Wl,..

SRCS=$(wildcard *.c) $(wildcard *.cpp)
OBJDIR=../objs
SCRIPTS=../scripts
TARGETS=$(foreach src, $(SRCS), $(basename $(src)))

.PHONY:all clean objdir test no_int128
.SILENT:clean

all:objdir $(TARGETS)


objdir:
	@mkdir -p $(OBJDIR)

clean:
	$(RM) *.o a.out depend.d $(TARGETS) $(OBJDIR) -rf

$(TARGETS):
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ $^ $(LDFLAGS)

no_int128:
	@make clean && make CFLAGS="-Wall -g -fPIC -I../src -D_NO_128INT_"

# Test
json_parse_test: json_parse_test.o

# Unittest 
u128_unittest: u128_unittest.o
basic_get_test: basic_get_test.o
compose_url_test: compose_url_test.o
request_unittest: request_unittest.o

test:$(TARGETS)
	@chmod u+x $(SCRIPTS)/*.sh
	@make -C $(SCRIPTS)

depend.d:$(wildcard *.h *.c)
	$(CC) $(CFLAGS) -MM $^ > $@

-include depend.d
