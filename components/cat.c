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
    char state[12];

    if (pscanf(path, "%12s", state) != 1) {
        return NULL;
    }

    return bprintf("%s", state);
}

