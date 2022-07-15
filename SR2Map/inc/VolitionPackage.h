#ifndef __VOLITION_PACKAGE_FILE
#define __VOLITION_PACKAGE_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static const uint32_t magic = 0x51890ACEl; // Little-Endian, works for all VPP_PC files.

typedef struct {
    uint32_t magic;
    uint32_t version;
} vpp_header_t;

typedef struct {
    uint32_t compressed;
    uint32_t _unknown_b;
    uint32_t checksum;
    uint32_t filesize;
    uint32_t flags;
    uint32_t filecount;
    uint32_t dircount;
    uint32_t filenamelen;
    uint32_t filedatalen;
    uint32_t compressedsize;
    uint32_t _unknown_c;
} vpp_data_t;

typedef struct {
    uint32_t filename;
    uint32_t extension;
    uint32_t _unknown_a;
    uint32_t filedata;
    uint32_t filesize;
    uint32_t compressedsize;
    uint32_t _unknown_b;
} vpp_fileentry_t;

typedef struct {
    vpp_header_t header;
    vpp_data_t data;
    vpp_fileentry_t *files;
} vpp_file_t;

vpp_file_t read_vpp(FILE *);

#endif