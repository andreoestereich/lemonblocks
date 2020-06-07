//Modify this file to change what commands output to your statusbar, and recompile using the make command.

#define MAXLEN 2048

static const bBlock blocks[] = {
	/*Function*/	/*format*/	           /*argument*/ 	/*Update Interval in ds*/	/*Update Signal*/
	{ bspDesktops,        "%%{U#888B36}%s",    NULL, 2, 10},
	{ run_command,        " %%{B#555803} %s %%{B- r}",           "xtitle -t 80", 1, 10},
	{ run_command,        "%s|",           "curl -s 'wttr.in/Mondai?format=%t+%c+%p+%o+%m' | sed 's/+//'", 3600, 0},
	{ battery_state,      "%s",           "BAT0",  1,  0 },
	{ battery_perc,       " %s%%",           "BAT0",  1,  0 },
	//{ battery_remaining,  "%%%s",           "BAT0",  1,  0 },
	//{ cpu_freq,           "|%s",           NULL,  1,  0  },
	{ cpu_perc,           "| %s%%",         NULL,  1,  0  },
	{ ram_free,           "| %s",          NULL,  1,  0  },
	{ wifi_perc,          "|%s%%",         "wlp1s0",  1,  0 },
	{ num_files,          "|id:%s",           "/home/andrelo/Mail/idUff/Inbox/new/",  0,  2},
	{ num_files,          ".if:%s",           "/home/andrelo/Mail/ifmail/Inbox/new/",  0,  2 },
	{ temp,               "|%s°C",         "/sys/class/thermal/thermal_zone1/temp",  10,  0 },
	//{ cat_file,           "|%s",           "/home/andrelo/.config/redshift/stat",   0,  3},
	{ vol_perc,           "|%s",           "default",   0,  4},
	//{ run_command,        "|%s",           "sh /home/andrelo/.local/bin/volume"},
	{ datetime,           "|%s  ",           "%d %b(%a)%R",   30,  0 },
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim = ' ';
