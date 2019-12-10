# See LICENSE file for copyright and license details
# slstatus - suckless status monitor
.POSIX:

#	components/netspeeds
include config.mk

REQ = util
COM =\
	components/battery\
	components/cpu\
	components/datetime\
	components/disk\
	components/entropy\
	components/hostname\
	components/ip\
	components/kernel_release\
	components/keyboard_indicators\
	components/keymap\
	components/load_avg\
	components/num_files\
	components/ram\
	components/run_command\
	components/swap\
	components/temperature\
	components/uptime\
	components/user\
	components/volume\
	components/cat\
	components/wifi

all: dwmblocks

$(COM:=.o): config.mk $(REQ:=.h)
dwmblocks.o: dwmblocks.c dwmblocks.h blocks.h config.mk $(REQ:=.h)

.c.o:
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

dwmblocks: dwmblocks.o $(COM:=.o) $(REQ:=.o) 
	$(CC) -o $@ $(LDFLAGS) $(COM:=.o) $(REQ:=.o) dwmblocks.o $(LDLIBS)

clean:
	rm -f dwmblocks dwmblocks.o $(COM:=.o) $(REQ:=.o)

