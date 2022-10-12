/* Copyright (c) 2012, Bastien Dejean
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>

#include <err.h>
#include "../util.h"

/* Reference: http://www.sensi.org/~alec/locale/other/ctext.html */
#define CT_UTF8_BEGIN   "\x1b\x25\x47"
#define CT_UTF8_END     "\x1b\x25\x40"

#define MIN(a,b) (((a)<(b))?(a):(b))

#ifndef BSPWM_COMMON_H
#define BSPWM_COMMON_H

#include <stdarg.h>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#define SOCKET_PATH_TPL  "/tmp/bspwm%s_%i_%i-socket"
#define SOCKET_ENV_VAR   "BSPWM_SOCKET"

#define FAILURE_MESSAGE  "\x07"

__attribute__((noreturn))
void erro(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

#endif

void jqSTR(const char *json, const char* field, char *out)
{
    int leng = strlen(field);
    char *pch, *sch;

    pch=strchr(json,':');
    while (pch!=NULL)
    {
        if (strncmp (pch-leng-1,field,leng) == 0)
        {
            pch+=2;
            sch=strchr(pch,'"');
            strncpy(out,pch,sch-pch);
            break;
        }
        pch=strchr(pch+1,':');
    }
}

long int jqINT(const char *json, const char* field)
{
    int leng = strlen(field);
    char *pch, *sch;
    char out[64] = "";

    pch=strchr(json,':');
    while (pch!=NULL)
    {
        if (strncmp (pch-leng-1,field,leng) == 0)
        {
            pch+=1;
            sch=strchr(pch,',');
            strncpy(out,pch,sch-pch);
            break;
        }
        pch=strchr(pch+1,':');
    }
    return atol(out);
}

wchar_t * applications[] = {L"Brave",L"Mozilla Firefox", L"Chromium", L"VIM", L"st", L"Vim", L"newsboat", L"neomutt", L"mocp",L"mpv",L"Telegram", NULL};
char *icons[] = {"","","", "", "", "", "", "", "", "", "",""};

int gimmeICONS(wchar_t *title)
{
    wchar_t * pwc;
    wchar_t * pwm;

    //Cuts everything before the last bar on longer titles this is where the window title usualy lives on long titles
    wchar_t bar = L'-';
    pwm = wcsrchr(title,bar);
    if (pwm == NULL)
        pwm = title;

    int i= 0;
    size_t leng = 0;
    while (applications[i] != NULL)
    {
        leng = wcslen(applications[i]);
        pwc = wcsrchr (pwm,applications[i][0]);
        if (pwc != NULL)
        {
            if (leng >= wcslen(pwc) && wcsncmp (pwc,applications[i],leng) == 0)
            {
                break;
            }
        }
        i++;
    }
    return i;
}

xcb_ewmh_connection_t *ewmh = NULL;
xcb_connection_t *dpy = NULL;
int default_screen = 0;
xcb_window_t root = XCB_NONE;

bool setup()
{
	dpy = xcb_connect(NULL, &default_screen);
	if (xcb_connection_has_error(dpy)) {
		warnx("can't open display.");
		return false;
	}
	xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
	if (screen == NULL) {
		warnx("can't acquire screen.");
		return false;
	}
	root = screen->root;
	ewmh = malloc(sizeof(xcb_ewmh_connection_t));
	if (xcb_ewmh_init_atoms_replies(ewmh, xcb_ewmh_init_atoms(dpy, ewmh), NULL) == 0) {
		warnx("can't initialize EWMH atoms.");
		return false;
	}
	return true;
}

void cleanup()
{
	if (ewmh != NULL) {
		xcb_ewmh_connection_wipe(ewmh);
	}
	if (dpy != NULL) {
		xcb_disconnect(dpy);
	}
	free(ewmh);
}

wchar_t *get_window_title(xcb_window_t win)
{
    bool visible = false;
	xcb_ewmh_get_utf8_strings_reply_t ewmh_txt_prop;
	xcb_icccm_get_text_property_reply_t icccm_txt_prop;
	ewmh_txt_prop.strings = icccm_txt_prop.name = NULL;
	wchar_t *title = NULL;
	if (win != XCB_NONE && ((visible && xcb_ewmh_get_wm_visible_name_reply(ewmh, xcb_ewmh_get_wm_visible_name(ewmh, win), &ewmh_txt_prop, NULL) == 1) || xcb_ewmh_get_wm_name_reply(ewmh, xcb_ewmh_get_wm_name(ewmh, win), &ewmh_txt_prop, NULL) == 1 || xcb_icccm_get_wm_name_reply(dpy, xcb_icccm_get_wm_name(dpy, win), &icccm_txt_prop, NULL) == 1)) {
		char *src = NULL;
		size_t title_len = 0;
		if (ewmh_txt_prop.strings != NULL) {
			src = ewmh_txt_prop.strings;
			title_len = ewmh_txt_prop.strings_len;
		} else if (icccm_txt_prop.name != NULL) {
			src = icccm_txt_prop.name;
			title_len = icccm_txt_prop.name_len;
			/* Extract UTF-8 embedded in Compound Text */
			if (title_len > strlen(CT_UTF8_BEGIN CT_UTF8_END) &&
			    memcmp(src, CT_UTF8_BEGIN, strlen(CT_UTF8_BEGIN)) == 0 &&
			    memcmp(src + title_len - strlen(CT_UTF8_END), CT_UTF8_END, strlen(CT_UTF8_END)) == 0) {
				src += strlen(CT_UTF8_BEGIN);
				title_len -= strlen(CT_UTF8_BEGIN CT_UTF8_END);
			}
		}
		if (src != NULL) {
			title_len = mbsnrtowcs(NULL, (const char**)&src, title_len, 0, NULL);
			if (title_len == (size_t)-1) {
				warnx("can't decode the title of 0x%08lX.", (unsigned long)win);
			} else {
				title = calloc((title_len + 1), sizeof(wchar_t));
				if (title != NULL) {
					mbsrtowcs(title, (const char**)&src, title_len, NULL);
					title[title_len] = L'\0';
				}
			}
		}
	}
	if (ewmh_txt_prop.strings != NULL) {
		xcb_ewmh_get_utf8_strings_reply_wipe(&ewmh_txt_prop);
	}
	if (icccm_txt_prop.name != NULL) {
		xcb_icccm_get_text_property_reply_wipe(&icccm_txt_prop);
	}
	return title;
}

bool get_active_window(xcb_window_t *win)
{
	return (xcb_ewmh_get_active_window_reply(ewmh, xcb_ewmh_get_active_window(ewmh, default_screen), win, NULL) == 1);
}

int bspwmSocket(int argc, char *argv[], char *json)
{
	int sock_fd;
	struct sockaddr_un sock_address;
	char msg[BUFSIZ], rsp[BUFSIZ];

	if (argc < 2) {
		erro("No arguments given.\n");
	}

	sock_address.sun_family = AF_UNIX;
	char *sp;

	if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		erro("Failed to create the socket.\n");
	}

	sp = getenv(SOCKET_ENV_VAR);
	if (sp != NULL) {
		snprintf(sock_address.sun_path, sizeof(sock_address.sun_path), "%s", sp);
	} else {
		char *host = NULL;
		int dn = 0, sn = 0;
		if (xcb_parse_display(NULL, &host, &dn, &sn) != 0) {
			snprintf(sock_address.sun_path, sizeof(sock_address.sun_path), SOCKET_PATH_TPL, host, dn, sn);
		}
		free(host);
	}

	if (connect(sock_fd, (struct sockaddr *) &sock_address, sizeof(sock_address)) == -1) {
		erro("Failed to connect to the socket.\n");
	}

	//argc--, argv++;
	int msg_len = 0;

	for (int offset = 0, rem = sizeof(msg), n = 0; argc > 0 && rem > 0; offset += n, rem -= n, argc--, argv++) {
		n = snprintf(msg + offset, rem, "%s%c", *argv, 0);
		msg_len += n;
	}

	if (send(sock_fd, msg, msg_len, 0) == -1) {
		erro("Failed to send the data.\n");
	}

	int ret = EXIT_SUCCESS, nb;

	struct pollfd fds[] = {
		{sock_fd, POLLIN, 0},
		{STDOUT_FILENO, POLLHUP, 0},
	};

	while (poll(fds, 2, -1) > 0) {
		if (fds[1].revents & (POLLERR | POLLHUP)) {
			break;
		}
		if (fds[0].revents & POLLIN) {
			if ((nb = recv(sock_fd, rsp, sizeof(rsp)-1, 0)) > 0) {
				rsp[nb] = '\0';
				if (rsp[0] == FAILURE_MESSAGE[0]) {
					ret = EXIT_FAILURE;
					fprintf(stderr, "%s", rsp + 1);
					fflush(stderr);
				} else {
                    strcpy(json,rsp);
					//fprintf(stderr, "%s", out);
					//fflush(stderr);
				}
			} else {
				break;
			}
		}
	}

	close(sock_fd);
	return ret;
}

const char *bspDesktops(void)
{
	setlocale(LC_CTYPE, "");

    char desktop[BUFSIZ] = "";
    char *argus[4];
    char activeDekstop[10] = "";
    char currLayout[20] = "";
    char monitor[BUFSIZ] = "";
    if (! setup())
        warnx("Setup error.");
    xcb_window_t win_id;
	wchar_t *title = NULL;

    argus[0] = "query";
    argus[1] = "-T";
    argus[2] = "-d";
    bspwmSocket(3, argus,desktop);
    jqSTR(desktop, "name", activeDekstop);
    jqSTR(desktop, "layout", currLayout);
    argus[1] = "-D";
    argus[2] = "--names";
    bspwmSocket(3, argus,monitor);

    char *split = strtok(monitor,"\n");
    int iconID = 0;
    char status[512] = "";
    bool space = true;

    argus[1] = "-T";
    argus[2] = "-d";
    while (split != NULL)
    {
        argus[3] = split;
        memset(desktop,0,strlen(desktop));
        bspwmSocket(4, argus, desktop);
        win_id = XCB_NONE;
        win_id = jqINT(desktop, "focusedNodeId");
        if (win_id != XCB_NONE && (title = get_window_title(win_id)) != NULL)
        {

            iconID = gimmeICONS(title);
            free(title);
            if (strcmp(activeDekstop, split) == 0)
            {
                strcat(status,"%{+u} ");
                strcat(status, icons[iconID]);
                strcat(status," %{-u}");
                space = false;
            }
            else
            {
                if (space)
                    strcat(status," ");
                else
                    space = true;
                strcat(status, icons[iconID]);
            }
            //printf("\n");
        }
        else
        {
            if (strcmp(activeDekstop, split) == 0)
            {
                strcat(status,"%{+u} ");
                strcat(status, split);
                strcat(status," %{-u}");
                space = false;
            }
            else
            {
                if (space)
                    strcat(status," ");
                else
                    space = true;
                strcat(status, split);
            }
        }

        split = strtok(NULL,"\n");
    }
    if (strcmp(currLayout, "tiled")==0)
        strcat(status, " |[]= ");
    else {if (strcmp(currLayout, "monocle")==0)
        strcat(status, " |[M] ");
        else
            strcat(status, " |<>< ");
    }
    win_id = XCB_NONE;
    strcat(status," %{B#555803} ");
    if (get_active_window(&win_id)) {
        if (win_id != XCB_NONE && (title = get_window_title(win_id)) != NULL) {
            strcat(status, (char*)title);
            free(title);
        }
    } else
        strcat(status,"    ");
    strcat(status," %{r B-}");

    cleanup();
    return bprintf("%s", status);
}
