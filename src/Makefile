EPOLL :=1 

CC ?= gcc
AR ?= ar
RM ?= rm -f
TAR ?= tar
FIND ?= find
INSTALL ?= install

EXTRA_WARNINGS += -Wbad-function-cast
EXTRA_WARNINGS += -Wcast-align
EXTRA_WARNINGS += -Wdeclaration-after-statement
EXTRA_WARNINGS += -Wformat
EXTRA_WARNINGS += -Wformat-security
EXTRA_WARNINGS += -Wformat-y2k
EXTRA_WARNINGS += -Wformat=2
EXTRA_WARNINGS += -Winit-self
EXTRA_WARNINGS += -Wmissing-declarations
EXTRA_WARNINGS += -Wmissing-prototypes
EXTRA_WARNINGS += -Wnested-externs
EXTRA_WARNINGS += -Wnested-externs
EXTRA_WARNINGS += -Wno-system-headers
EXTRA_WARNINGS += -Wold-style-definition
EXTRA_WARNINGS += -Wpacked
EXTRA_WARNINGS += -Wredundant-decls
EXTRA_WARNINGS += -Wshadow
EXTRA_WARNINGS += -Wsign-compare
EXTRA_WARNINGS += -Wstack-protector
EXTRA_WARNINGS += -Wstrict-aliasing=3
EXTRA_WARNINGS += -Wstrict-prototypes
EXTRA_WARNINGS += -Wswitch-default
EXTRA_WARNINGS += -Wswitch-enum
EXTRA_WARNINGS += -Wundef
EXTRA_WARNINGS += -Wunused-result
EXTRA_WARNINGS += -Wvolatile-register-var
EXTRA_WARNINGS += -Wwrite-strings

ifeq ("$(origin DEBUG)", "command line")
  LDNSD_DEBUG = $(DEBUG)
endif
ifndef LDNSD_DEBUG
  CFLAGS_OPTIMIZE = -O6
else
	CFLAGS_OPTIMIZE = -O0
endif

EXTRA_CFLAGS := -D_GNU_SOURCE
EXTRA_CFLAGS += -DDEBUG

# enable via "make MUDFLAP=1 all"
ifeq ("$(origin MUDFLAP)", "command line")
	EXTRA_CFLAGS += -fmudflap -lmudflap
endif

OBJ :=  \
								event.o \
								getopt.o \
								gl-cockpit.o \
								gl-display.o \
								gl-infobox.o \
								gl-model-loader.o \
								gl-node.o \
								gl-packet.o \
								gl-skybox.o \
								gl-terrain.o \
								gl-text.o \
								gl-texture.o \
								manet-visualizer.o \
								node.o \
								packet.o \
								scenario.o \
								time.o \
								trace-parser.o \
								trace-parser-ns2.o \
								util.o


CFLAGS += -ggdb3 -Wall -Wextra -std=gnu99 $(CFLAGS_OPTIMIZE) -D_FORTIFY_SOURCE=2 $(EXTRA_WARNINGS) $(EXTRA_CFLAGS)
EXTLIBS = -lglut -lX11 -lGL -lGLU -lXi -lXmu -lm
ALL_CFLAGS = $(CFLAGS)
ALL_LDFLAGS = $(LDFLAGS)

ifeq ($(shell sh -c "echo 'int foo(void) {char X[2]; return 3;}' | $(CC) -x c -c -Werror -fstack-protector-all - -o /dev/null "$(QUIET_STDERR)" && echo y"), y)
  CFLAGS := $(CFLAGS) -fstack-protector-all
endif

TARGET := manet-visualizer


.SUFFIXES:
.SUFFIXES: .c .o

all: $(TARGET)

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(EXTLIBS) -o $(TARGET) $(OBJ)

clean:
	$(RM) -f $(OBJ) $(TARGET) core vgcore.* *~

distclean: clean
	$(RM) -f cscope* tags

cscope:
	$(RM) -f cscope*
	$(FIND) . -name '*.[hcS]' -print0 | xargs -0 cscope -b

tags:
	$(RM) tags
	$(FIND) . -name '*.[hcS]' -print | xargs ctags -a

checkstyle:
	 /usr/src/linux/scripts/checkpatch.pl --file --terse $(shell ls *.c)

stackusage:
	objdump -d $(TARGET) | perl /usr/src/linux/scripts/checkstack.pl
