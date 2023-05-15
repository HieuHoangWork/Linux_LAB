#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>
#include <sndfile.h>

#define BUFFER_SIZE 8192

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input.mp3> <output.wav>\n", argv[0]);
        return EXIT_FAILURE;
    }

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
    if (mpg123_open(mh, argv[1]) != MPG123_OK ||
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

    // Check if memory allocation was successful
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    // Initialize libsndfile
    SF_INFO sfinfo;
    sfinfo.samplerate = rate;
    sfinfo.channels = channels;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *outfile = sf_open(argv[2], SFM_WRITE, &sfinfo);
    if (!outfile)
    {
        fprintf(stderr, "Error opening output.wav: %s\n", sf_strerror(outfile));
        free(buffer);
        return EXIT_FAILURE;
    }

    // Decode and write to WAV file
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
    {
        if (sf_writef_short(outfile, (short *)buffer, done / sizeof(short)) != done / sizeof(short))
        {
            fprintf(stderr, "Error writing to output file\n");
            break;
        }
    }

    // Cleanup
    free(buffer);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    sf_close(outfile);

    return EXIT_SUCCESS;
}