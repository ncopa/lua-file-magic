
VERSION = 0.1
GIT_REV		:= $(shell test -d .git && git describe || echo exported)
ifneq ($(GIT_REV), exported)
FULL_VERSION    := $(GIT_REV)
FULL_VERSION    := $(patsubst v%,%,$(FULL_VERSION))
else
FULL_VERSION    := $(VERSION)
endif


OBJS = magic.o
LIBS = -lmagic

CFLAGS ?= -g
CFLAGS += -fPIC

magic.so: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ -fPIC -shared $^ $(LIBS)

