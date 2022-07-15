#include "VolitionPackage.h"

uint32_t endianswap(uint32_t data) {
    return
        (data & 0xFF000000) >> 24u |
        (data & 0x00FF0000) >>  8u |
        (data & 0x0000FF00) <<  8u |
        (data & 0x000000FF) << 24u ;
}

void seek_data(FILE *fileptr) {
    // Read 4 bytes of data at a time until we reach a point where we aren't encountering all NULL values.
    uint32_t tmpbuf;

    do {
        fread(&tmpbuf, sizeof(uint32_t), 1, fileptr);
    } while (tmpbuf == 0);

    fseek(fileptr, ftell(fileptr) - sizeof(uint32_t), SEEK_SET);
}

vpp_file_t read_vpp(FILE *fileptr) {

    // Create a local variable to store the current error state.
    uint8_t err = 0;

    // Go to the end of the file stream.
    fseek(fileptr, 0, SEEK_END);

    // Store the current offset (EOF in this case).
    uint32_t _filesize = (uint32_t) ftell(fileptr);

    // Set the file position to the origin (byte 0).
    fseek(fileptr, 0, SEEK_SET);

    // Store enough for the opening header data of the file.
    vpp_header_t *header = (vpp_header_t *) malloc(sizeof(vpp_header_t));

    // Read the contents of the header into the buffer above.
    fread(header, sizeof(char), sizeof(vpp_header_t), fileptr);

    // Test to see if the magic number matches what it should be.
    if (magic != header->magic) {
        fprintf(stderr, "Invalid VPP_PC package file!\n");
        err = 1;

        goto freemem;
    }

    // Test to see if the VPP_PC package version is supported.
    if (header->version != 4) {
        fprintf(stderr, "Unsupported VPP_PC package file!\n");
        err = 1;

        goto freemem;
    }

    // Seek through the data until we hit something.
    seek_data(fileptr);

    // Store enough for the file data.
    vpp_data_t *data = (vpp_data_t *) malloc(sizeof(vpp_data_t));

    // Read the contents of the header into the buffer above.
    fread(data, sizeof(char), sizeof(vpp_data_t), fileptr);

    // Determine if the filesize of the VPP_PC package is correct.
    if (_filesize != data->filesize) {
        fprintf(
            stdout,
            "Filesize mismatch!\n\n"
            "Filesize of VPP_PC package reported by OS = %u bytes\n"
            "Filesize of VPP_PC package reported by file = %u bytes\n\n",
            _filesize, data->filesize
        );
    }

    // Seek through the data until we hit something.
    seek_data(fileptr);

    // Store enough for the file entry tables.
    vpp_fileentry_t *files = (vpp_fileentry_t *) malloc(sizeof(vpp_fileentry_t) * data->filecount);

    // Read the contents of the header into the buffer above.
    fread(files, sizeof(vpp_fileentry_t), data->filecount, fileptr);

    // Set up the return data that will be sent.
    vpp_file_t vppfile = (vpp_file_t) {
        .header = *header,
        .data = *data,
        .files = files
    };

    goto exitfunc;

freemem:
    free(header);
    free(data);
    free(files);

exitfunc:
    fclose(fileptr);

    // Close the program upon error.
    if (err) exit(-1);

    // Return the dereferenced pointer data (the actual data).
    return vppfile;
}