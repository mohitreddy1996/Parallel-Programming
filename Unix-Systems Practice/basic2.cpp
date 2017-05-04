//
// Created by mohit on 4/5/17.
//

/**
 * Pipe to a subprocess
 *
 * using popen(const char *command, const char *mode)
 * executes the shell command as a subprocess.
 * instead of waiting for the command to complete, it creates a pipe to the subprocess
 * and returns a stream that corresponds to that pipe.
 *
 * "r" - read from the stream to retrieve data from the standard output channel of the subprocess.
 * Subprocess inherits its standard input channel from the parent process.
 *
 * "w" - write to the stream to send data to the standard input channel of the subprocess.
 * Subprocess inherits its standard output channel from the parent process.
 *
 * */

#include <cstdio>
#include <cstdlib>
#include "basic2.h"

void write_data (FILE *stream)
{
    int i;
    for (i = 0; i<100; i++)
    {
        fprintf (stream, "%d\n", i);
    }
    if (ferror (stream))
    {
        fprintf(stderr, "Output to stream failed\n");
        exit (EXIT_FAILURE);
    }
}

int main ()
{
    FILE *output;
    output = popen ("more", "r");

    if (!output)
    {
        fprintf(stderr, "Incorect parameters or too many files.");
        return EXIT_FAILURE;
    }
    write_data (output);
    if (pclose(output) != 0)
    {
        fprintf(stderr, "Could not run more.");
    }

    return EXIT_SUCCESS;

}