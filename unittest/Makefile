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

.PHONY:all clean objdir test
.SILENT:clean

all:objdir $(TARGETS)


objdir:
	@mkdir -p $(OBJDIR)

clean:
	$(RM) *.o a.out depend.d $(TARGETS) $(OBJDIR) -rf

$(TARGETS):
	@make objdir
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ $^ $(LDFLAGS)


# Test
json_parse_test: json_parse_test.o

# Unittest 
basic_get_test: basic_get_test.o
compose_url_test: compose_url_test.o
request_unittest: request_unittest.o

test:$(TARGETS)
	@chmod u+x $(SCRIPTS)/*.sh
	@make -C $(SCRIPTS)

depend.d:$(wildcard *.h *.c)
	$(CC) $(CFLAGS) -MM $^ > $@

-include depend.d
