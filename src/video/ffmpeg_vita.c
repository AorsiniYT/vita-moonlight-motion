//Have mercy
#include "Limelight.h"
#include "ffmpeg.h"
#include "../util.h"
#include "sys/unistd.h"


static void* ffmpeg_buffer = NULL;
static size_t ffmpeg_buffer_size = 0;

static int pipefd[2];

/*

-----------------------Setup-------------------------------

*/

int vita_software_setup(int videoFormat, int width, int height, int redrawRate, void* context, int drFlags) {
    if (ffmpeg_init(videoFormat, width, height, avc_flags, 2, SLICES_PER_FRAME) < 0) {
        fprintf(stderr, "Couldn't initialize video decoding\n");
        return -1;
    }
}

/*

-----------------------Decode------------------------------

*/

int vita_software_submit_decode_unit(PDECODE_UNIT decode_unit) {
    PLENTRY entry = decode_unit->bufferList;
    int length = 0;

    ensure_buf_size(&ffmpeg_buffer, &ffmpeg_buffer_size, decode_unit->fullLength + AV_INPUT_BUFFER_PADDING_SIZE);

    while (entry != NULL) {
        memcpy(ffmpeg_buffer+length, entry->data, entry->length);
        length += entry->length;
        entry = entry->next;
    }

    ffmpeg_decode(ffmpeg_buffer, length);

    AVFrame* frame = ffmpeg_get_frame(true);
    if (frame != NULL)
        write(pipefd[1], &frame, sizeof(void*));

    return DR_OK;
}



DECODER_RENDERER_CALLBACKS decoder_callbacks_software_vita = {
    .setup = vita_software_setup,
    .cleanup = vita_software_cleanup,
    .capabilities = CAPABILITY_DIRECT_SUBMIT,
    .submitDecodeUnit = vita_software_submit_decode_unit
};