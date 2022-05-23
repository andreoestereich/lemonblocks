/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <string.h>

#include "../util.h"

#include <limits.h>
#include <stdint.h>
#include <unistd.h>

const char *
cat_file(const char *path)
{
    char state[50];
    FILE *fp;
    fp = fopen(path, "r");

    if (fgets(state, 20, (FILE*)fp) == NULL ) {
        fclose(fp);
        return NULL;
    }
    char *pos;
    if ((pos=strchr(state, '\n')) != NULL)
        *pos = '\0';
    else

    fclose(fp);

    return bprintf("%s", state);
}
