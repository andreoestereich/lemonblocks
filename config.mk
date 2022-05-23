# slstatus version
VERSION = 0

# customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# flags
CPPFLAGS = -I$(X11INC) -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Os -DJSMN_STRICT
LDFLAGS  = -L$(X11LIB) -s -z muldefs
LDLIBS   = -lX11 -lasound -lm -lxcb -lxcb-icccm -lxcb-ewmh

# compiler and linker
CC = cc
