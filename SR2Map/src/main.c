#include "VolitionPackage.h"
#include <string.h>

void help(char *program) {

    // Windows pathname is delimited by a forward-slash character.
    char *tok = strtok(program, "\\");

    // Typically the maximum depth with older versions of NT was 260.
    char *parts[260];

    // The current depth index that we will initialize with 0.
    uint8_t index = 0;

    // Keep reading through the tokens until we hit NULL.
    while (tok != 0) {

        // Append the current token into the current array at the specified location.
        parts[index] = tok;

        // Read the next token based on the delimiter.
        tok = strtok(0, "\\");

        // Increment the current depth index by +1.
        ++index;
    }

    // Decrement the current depth index by -1 to go from NULL to the program name.
    --index;

    // Print out the help text and stop the program.
    fprintf(stdout, "Usage: %s [.vpp_pc file]\n", parts[index]);
    exit(-1);
}

int main(int argc, char **argv) {
    if (argc != 2)
        help(argv[0]);
    
    // Read the current file buffer in binary-safe mode.
    FILE *fileptr = fopen(argv[1], "rb");

    // Gather the VPP_PC package data.
    vpp_file_t vppfile = read_vpp(fileptr);

    // Read the contents of the VPP_PC package file.
    fprintf(
        stdout,
        "Compressed: %s\n"
        "Version: %u\n"
        "Checksum: %08X\n"
        "File Size: %u\n"
        "File Count: %u\n"
        "Directory Count: %u\n",
        vppfile.data.compressed == 2 ? "False" : "True",
        vppfile.header.version,
        vppfile.data.checksum,
        vppfile.data.filesize,
        vppfile.data.filecount,
        vppfile.data.dircount
    );

    return 0;
}