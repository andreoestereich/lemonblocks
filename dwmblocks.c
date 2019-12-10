#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<X11/Xlib.h>
#define CMDLEN		512

#include "util.h"
#include "dwmblocks.h"

void sighandler(int num);
void replace(char *str, char old, char new);
void getcmds(int time);
void getsigcmds(int signal);
void setupsignals();
int getstatus(char *str, char *last);
void setroot();
void statusloop();
void sighandler(int signum);
void termhandler();


#include "blocks.h"

static Display *dpy;
static int screen;
static Window root;
static char statusbar[LEN(blocks)][CMDLEN] = {0};
static char statusstr[2][512];
static int statusContinue = 1;
static void (*writestatus) () = setroot;

void replace(char *str, char old, char new)
{
	int N = strlen(str);
	for(int i = 0; i < N; i++)
		if(str[i] == old)
			str[i] = new;
}

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output)
{
	strcpy(output, block->icon);
	char *cmd = block->command;
	FILE *cmdf = popen(cmd,"r");
	if (!cmdf)
		return;
	int i = strlen(block->icon);
	fgets(output+i, CMDLEN-i, cmdf);
	i = strlen(output);
	if (delim != '\0' && --i)
		output[i++] = delim;
	output[i++] = '\0';
	pclose(cmdf);
}

void getcmds(int time)
{
	//bBlock* current;
    int ret, len;
	const char *res;
	for(int i = 0; i < LEN(blocks); i++)
	{	
		;
		if ((blocks[i].interval != 0 && time % blocks[i].interval == 0) || time == -1)
        {
            len = 0;
			if (!(res = blocks[i].func(blocks[i].args))) {
				res = "?";
			}
			if ((ret = esnprintf(statusbar[i] + len, sizeof(statusbar[i]) - len, blocks[i].fmt, res)) < 0)
            {
				break;
			}
        }
			//getcmd(current,statusbar[i]);
	}
}

void getsigcmds(int signal)
{
    int ret, len;
	const char *res;
	for (int i = 0; i < LEN(blocks); i++)
	{
		if ((int) blocks[i].signal == signal)
        {
            len = 0;
            printf("1\n");
			if (!(res = blocks[i].func(blocks[i].args))) {
				res = "?";
			}
            printf("2\n");
			if ((ret = esnprintf(statusbar[i] + len, sizeof(statusbar[i]) - len,
			                    blocks[i].fmt, res)) < 0) {
				break;
			}
            printf("3\n");
        }
	}
}

void setupsignals()
{
	for(int i = 0; i < LEN(blocks); i++)
	{	  
		if (blocks[i].signal > 0)
			signal(SIGRTMIN+blocks[i].signal, sighandler);
	}

}

int getstatus(char *str, char *last)
{
	strcpy(last, str);
	str[0] = '\0';
	for(int i = 0; i < LEN(blocks); i++)
		strcat(str, statusbar[i]);
	str[strlen(str)-1] = '\0';
	return strcmp(str, last);//0 if they are the same
}

void setroot()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only set root if text has changed.
		return;
	Display *d = XOpenDisplay(NULL);
	if (d) {
		dpy = d;
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	XStoreName(dpy, root, statusstr[0]);
	XCloseDisplay(dpy);
}

void pstdout()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;
	printf("%s\n",statusstr[0]);
	fflush(stdout);
}

void statusloop()
{
	setupsignals();
	int i = 0;
	getcmds(-1);
	while(statusContinue)
	{
		getcmds(i);
		writestatus();
		sleep(1.0);
		i++;
	}
}

void sighandler(int signum)
{
	getsigcmds(signum-SIGRTMIN);
	writestatus();
}

void termhandler(int signum)
{
	statusContinue = 0;
	exit(signum);
}

int main(int argc, char** argv)
{
	for(int i = 0; i < argc; i++)
	{	
		if (!strcmp("-d",argv[i]))
			delim = argv[++i][0];
		else if(!strcmp("-p",argv[i]))
			writestatus = pstdout;
	}
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	statusloop();
}
