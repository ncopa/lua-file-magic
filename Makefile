
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
LUAPC ?= lua5.2
LUA_CFLAGS ?= $(shell pkg-config --cflags $(LUAPC))

CFLAGS ?= -g
CFLAGS += -fPIC $(LUA_CFLAGS)

magic.so: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ -fPIC -shared $^ $(LIBS)

