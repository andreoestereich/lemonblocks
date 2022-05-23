# lemonblocks
Modular status bar filler for window managers that use `WM_NAME` or stdin, e.g. [lemonbar](https://wiki.archlinux.org/index.php/Lemonbar) and [dwm](http://dwm.suckless.org/), written in c. This is project is the combination of [dwmblocks](https://github.com/torrinfail/dwmblocks) and [sslstatus](http://tools.suckless.org/slstatus).

Less is not always less. Several bar outputs are much faster if written in c instead of necessarily using external pipe commands. The implemented modules can be found in the `components/` folder.

Each block or component can have an individual update interval or an specific update signal. The `pkill -RTMIN+n lemonblocks` where n is the signal number specified in `blocks.h`. The whole configuration is made via the `blocks.h` file similarlly to dwmblocks, dwm and other suckless applications.

Use the flag `-p` in order to print to `stdout` otherwise the output is printed to `WM_NAME`.

![Screenshot](screenshots/bar.png)
