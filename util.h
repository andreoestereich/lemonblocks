/* See LICENSE file for copyright and license details. */
#include <stdint.h>

#define LEN(X)             (int) (sizeof(X) / sizeof (X[0]))



char buf[1024];

extern char *argv0;

void warn(const char *, ...);
void die(const char *, ...);

int esnprintf(char *str, size_t size, const char *fmt, ...);
const char *bprintf(const char *fmt, ...);
const char *fmt_human(uintmax_t num, int base);
const char * fmt_bytes(uintmax_t num, char* unit);
int pscanf(const char *path, const char *fmt, ...);
