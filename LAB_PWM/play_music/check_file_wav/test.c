#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

#define WAV_FILE "output.wav"

int main()
{
    SF_INFO sndInfo;
    SNDFILE *sndFile = sf_open(WAV_FILE, SFM_READ, &sndInfo);
    if (sndFile == NULL)
    {
        fprintf(stderr, "Error reading source file '%s': %s\n", WAV_FILE, sf_strerror(sndFile));
        return EXIT_FAILURE;
    }

    // Check format - we only handle 16 bit PCM.
    if (sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16))
    {
        fprintf(stderr, "Input should be 16bit Wav\n");
        sf_close(sndFile);
        return EXIT_FAILURE;
    }

    // Allocate memory for data.
    int num_channels = sndInfo.channels;
    int num_frames = sndInfo.frames;
    short *buffer = malloc(num_frames * num_channels * sizeof(short));

    // Check if memory allocation was successful
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        sf_close(sndFile);
        return EXIT_FAILURE;
    }

    // Load data.
    if (sf_readf_short(sndFile, buffer, num_frames) != num_frames)
    {
        puts(sf_strerror(sndFile));
        sf_close(sndFile);
        free(buffer);
        return EXIT_FAILURE;
    }

    // Print out the sample values
    for (int i = 0; i < num_frames; i++)
    {
        printf("Sample %d: %d\n", i, buffer[i]);
    }

    // Cleanup.
    sf_close(sndFile);
    free(buffer);

    return 0;
}