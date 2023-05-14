#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>
#include <sndfile.h>

#define BUFFER_SIZE 8192

int main(int argc, char *argv[])
{
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int channels, encoding;
    long rate;

    // Initialize mpg123 library
    err = mpg123_init();
    if (err != MPG123_OK || (mh = mpg123_new(NULL, &err)) == NULL)
    {
        fprintf(stderr, "Error: %s\n", mpg123_plain_strerror(err));
        return EXIT_FAILURE;
    }

    // Open the MP3 file
    if (mpg123_open(mh, "music.mp3") != MPG123_OK ||
        mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK)
    {
        fprintf(stderr, "Trouble with mpg123: %s\n", mpg123_strerror(mh));
        return EXIT_FAILURE;
    }

    // Ensure that this output format will not change (it might, when we allow it)
    mpg123_format_none(mh);
    mpg123_format(mh, rate, channels, encoding);

    // Allocate buffer
    buffer_size = BUFFER_SIZE;
    buffer = (unsigned char *)malloc(buffer_size * sizeof(unsigned char));

    // Initialize libsndfile
    SF_INFO sfinfo;
    sfinfo.samplerate = rate;
    sfinfo.channels = channels;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *outfile = sf_open("output.wav", SFM_WRITE, &sfinfo);
    if (!outfile)
    {
        fprintf(stderr, "Error opening output.wav: %s\n", sf_strerror(outfile));
        return EXIT_FAILURE;
    }

    // Decode and write to WAV file
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
    {
        sf_writef_short(outfile, (short *)buffer, done / sizeof(short));
    }

    // Cleanup
    free(buffer);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    sf_close(outfile);
    return EXIT_SUCCESS;
}