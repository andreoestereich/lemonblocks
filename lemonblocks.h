typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;

typedef struct {
	const char* (*func)();
	const char* fmt;
	const char* args;
	unsigned int interval;
	unsigned int signal;
} bBlock;
void sighandler(int num);
void replace(char *str, char old, char new);
void getcmds(int time);
void getsigcmds(int signal);
void setupsignals();
int getstatus(char *str, char *last);
void pstdout();
void statusloop();
void sighandler(int signum);
void termhandler(int signum);


const char *bspDesktops(void);

/* battery */
const char *battery_perc(const char *);
const char *battery_state(const char *);
const char *battery_remaining(const char *);
const char * battery_icon(const char *bat);

/* cpu */
const char *cpu_freq(void);
const char *cpu_perc(const char* bar);

/* datetime */
const char *datetime(const char *fmt);

/* disk */
const char *disk_free(const char *path);
const char *disk_perc(const char *path);
const char *disk_total(const char *path);
const char *disk_used(const char *path);

/* entropy */
const char *entropy(void);

/* hostname */
const char *hostname(void);

/* ip */
const char *ipv4(const char *interface);
const char *ipv6(const char *interface);

/* kernel_release */
const char *kernel_release(void);

/* keyboard_indicators */
const char *keyboard_indicators(void);

/* keymap */
const char *keymap(void);

/* load_avg */
const char *load_avg(void);

// netspeeds
const char *netspeed_rx(const char *interface);
const char *netspeed_tx(const char *interface);


/* num_files */
const char *num_files(const char *path);

/* ram */
const char *ram_free(const char *unit);
const char *ram_perc(void);
const char *ram_total(const char *unit);
const char *ram_used(const char *unit);

/* run_command */
const char *run_command(const char *cmd);

/* swap */
const char *swap_free(void);
const char *swap_perc(void);
const char *swap_total(void);
const char *swap_used(void);

/* temperature */
const char *temp(const char *);

/* uptime */
const char *uptime(void);

/* user */
const char *gid(void);
const char *username(void);
const char *uid(void);

/* volume */
const char *vol_perc(const char *card);

/* wifi */
const char *wifi_perc(const char *interface);
const char *wifi_icon(const char *interface);
const char *wifi_perc_bar(const char *interface);
const char *wifi_essid(const char *interface);

// get output of file
const char *cat_file(const char *path);
