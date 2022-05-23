//Modify this file to change what commands output to your statusbar, and recompile using the make command.

#define MAXLEN 2048

static const bBlock blocks[] = {
	/*Function*/	/*format*/	           /*argument*/ 	/*Update Interval in ds*/	/*Update Signal*/
	//{ bspDesktops,        "%%{U#888B36}%s",    NULL, 2, 10},
	//{ run_command,        " %%{B#555803} %s %%{B- r}",           "xtitle -t 80", 1, 10},
	{ run_command,        "%s|",           "sh /home/andrelo/.local/bin/wttbar", 0, 11},
	{ run_command,        "%s ",           "sh /home/andrelo/.local/bin/ctemp", 300, 0},
	{ run_command,        "%s|",           "sh /home/andrelo/repos/pom/moonphase", 36000, 0},
	{ battery_state,      "%s",           "BAT0",  1,  0 },
	{ battery_perc,       "%s",           "BAT0",  1,  0 },
	//{ battery_remaining,  "%%%s",           "BAT0",  1,  0 },
	//{ cpu_freq,           "|%s",           NULL,  1,  0  },
	{ cpu_perc,           "|⚙️ %s",         1,  1,  0  },
	{ ram_used,           "| %s",          'G',  1,  0  },
	{ wifi_perc_bar,          "| %s",         "wlp1s0",  1,  0 },
	//{ netspeed_rx,          "|🔻%s",         "wlp1s0",  1,  0 },
	//{ netspeed_tx,          "🔺%s",         "wlp1s0",  1,  0 },
	{ num_files,          "|📫id:%s",           "/home/andrelo/Mail/idUff/Inbox/new/",  0,  2},
	{ num_files,          ".uc:%s",           "/home/andrelo/Mail/uceff/Inbox/new/",  0,  2 },
	//{ temp,               "|%s°C",         "/sys/class/thermal/thermal_zone1/temp",  10,  0 },
	{ cat_file,           "|%s",           "/home/andrelo/.config/redshift/stat",   0,  3},
	{ run_command,        "|🔆%s%%",       "xbacklight -get | cut -d. -f1", 0, 5},
	{ vol_perc,           "|%s",           "default",   0,  4},
	//{ run_command,        "|%s",           "sh /home/andrelo/.local/bin/volume"},
	{ datetime,           "|%s  ",           "%d %b(%a)%R",   30,  0 },
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim = ' ';
