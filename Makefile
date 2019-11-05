AR		= $(CROSS_COMPILE)ar
CC		= $(CROSS_COMPILE)gcc
CXX		= $(CROSS_COMPILE)g++
CFLAGS		= -Wall -g -fPIC -D_DEBUG_JSON_PARSE_
CXXFLAGS	= -Wall -g -fPIC -D_DEBUG_JSON_PARSE_
LDSHFLAGS	= -rdynamic -shared -fPIC
ARFLAGS		= rcv
CODE_STYLE	= astyle --align-pointer=name --align-reference=name --suffix=none --break-blocks --pad-oper --pad-header --break-blocks --keep-one-line-blocks --indent-switches --indent=spaces

SOURCES	= $(wildcard src/*.c)
HEADERS	= $(wildcard src/*.h)
OBJECTS	= $(SOURCES:.c=.o) 
TARGETS = libiotexemb.a libiotexemb.so

SCRIPTS=scripts
UNITTEST=unittest

.PHONY:all clean distclean test install help style statistics
.SILENT: clean

all:$(TARGETS)

clean:
	find . -name "*.o" | xargs rm -f 
	$(RM) *.o *.so *~ a.out depend.d $(TARGETS) build -rf

distclean:
	make clean
	make clean -C $(UNITTEST)

test:libiotexemb.so
	make -C $(UNITTEST)

unittest:test
	make -C $(SCRIPTS)

style:
	@find -regex '.*/.*\.\(c\|cpp\|h\)$$' | xargs $(CODE_STYLE)

statistics:
	@printf "Header:"
	@find -regex '.*/.*\.\(h\)$$' -not -path "./unittest/*" | xargs wc -l | tail -n 1 | awk '{print $$1}'

	@printf "Source:"
	@find -regex '.*/.*\.\(c\|cpp\)$$' -not -path "./unittest/*" | xargs wc -l | tail -n 1 | awk '{print $$1}'

	@printf "Unittest:"
	@find ./unittest -regex '.*/.*\.\(c\|cpp\)$$' | xargs wc -l | tail -n 1 | awk '{print $$1}'

install:$(TARGETS)
	@echo "Install libiotexemb lib..."
	@mkdir -p $(LOCAL_PACK_PATH)/$(LIB_PATH)
	@cp libiotexemb.so $(LOCAL_PACK_PATH)/$(LIB_PATH)


libiotexemb.a:$(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

libiotexemb.so:$(OBJECTS)
	$(CC) $(LDSHFLAGS) -o $@ $^

depend.d:$(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -MM $^ > $@

-include depend.d
