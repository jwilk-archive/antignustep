CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
CPPFLAGS = -D_GNU_SOURCE

.PHONY: all
all: libantignustep.so

.PHONY: clean
clean:
	rm -f *.so

lib%.so: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -shared -fPIC -ldl $(<) -o $(@)

# vim:ts=4 sts=4 sw=4 noet
