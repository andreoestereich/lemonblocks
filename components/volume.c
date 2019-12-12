#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>

#include "../util.h"

const char *
vol_perc(const char *card)
{
    int ret = 0;
    long failsafe = 0;
    long* outvol = &failsafe;
    snd_mixer_t* handle;
    snd_mixer_elem_t* elem;
    snd_mixer_selem_id_t* sid;

    static const char* mix_name = "Master";
    //static const char* card = "default";
    static int mix_index = 0;

    snd_mixer_selem_id_alloca(&sid);

    //sets simple-mixer index and name
    snd_mixer_selem_id_set_index(sid, mix_index);
    snd_mixer_selem_id_set_name(sid, mix_name);

    if ((snd_mixer_open(&handle, 0)) < 0) return NULL;

    if ((snd_mixer_attach(handle, card)) < 0) {
        snd_mixer_close(handle);
        return NULL;
    }
    if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
        snd_mixer_close(handle);
        return NULL;
    }
    ret = snd_mixer_load(handle);
    if (ret < 0) {
        snd_mixer_close(handle);
        return NULL;
    }
    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        snd_mixer_close(handle);
        return NULL;
    }

	if (snd_mixer_selem_has_playback_switch(elem)) {
		int psw = 0;
		snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_MONO, &psw);
		if(! psw)
        {
            snd_mixer_close(handle);
            return bprintf("");
        }
	}

    long minv, maxv;

    snd_mixer_selem_get_playback_volume_range (elem, &minv, &maxv);
    //fprintf(stderr, "Volume range <%i,%i>\n", minv, maxv);

    if(snd_mixer_selem_get_playback_volume(elem, 0, outvol) < 0) {
        snd_mixer_close(handle);
        return NULL;
    }

    fprintf(stderr, "Get volume %ld with status %i\n", *outvol, ret);
    /* make the value bound to 100 */
    *outvol -= minv;
    maxv -= minv;
    *outvol = 100 * (*outvol) / maxv; // make the value bound from 0 to 100

    snd_mixer_close(handle);
    failsafe = *outvol;
    if (*outvol > 70) return bprintf("%ld%%", *outvol);
    else if (*outvol < 30) return bprintf("%ld%%", *outvol);
    return bprintf("%ld%%", *outvol);
    }

