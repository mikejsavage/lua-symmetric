all: symmetric.so

include Makefile.mess

SRCS += src/main.c

CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -O2 -fPIC -DNDEBUG
CFLAGS += `pkg-config --cflags libsodium`

LDFLAGS += `pkg-config --libs libsodium`

OBJS := $(patsubst %.c,%.o,$(SRCS))

debug: CFLAGS += -ggdb3 -UNDEBUG
debug: all

symmetric.so: $(OBJS)
	$(CC) -o symmetric.so $(OBJS) $(LDFLAGS)

clean:
	rm -f symmetric.so
	rm -f $(OBJS)
