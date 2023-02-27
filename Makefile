SRCS := $(shell echo *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(shell echo *.h)

./pangram2: $(OBJS) Makefile
	cc $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c $(HDRS) Makefile
	cc $(CFLAGS) -c $< -o $@
