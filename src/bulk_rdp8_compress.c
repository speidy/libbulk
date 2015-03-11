/**
 * RDP8 bulk compressor
 *
 * Copyright 2015 Jay Sorg <jay.sorg@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bulk_rdp8_compress.h>
#include "getset.h"

struct bulk_rdp8
{
    int i1;
};

struct rdp8_bulk_encoded_data
{
    byte header;
    byte *data;
};

struct rdp_data_segment
{
    uint32 size;
    struct rdp8_bulk_encoded_data *bulk_data;
};

struct rdp_segmented_data
{
    byte descriptor; /* 0xE0 SINGLE */
                     /* 0xE1 MULTIPART */
    uint16 segment_count; /* only for MULTIPART */
    uint32 uncompressed_size; /* only for MULTIPART */
    struct rdp8_bulk_encoded_data *bulk_data; /* only for SINGLE */
    struct rdp_data_segment *segment_array; /* only for MULTIPART */
};


const Token g_tokenTable[] =
{
    /* len code vbits type  vbase */
    {  1,    0,   8,   0,           0 },    /* 0 */
    {  5,   17,   5,   1,           0 },    /* 10001 */
    {  5,   18,   7,   1,          32 },    /* 10010 */
    {  5,   19,   9,   1,         160 },    /* 10011 */
    {  5,   20,  10,   1,         672 },    /* 10100 */
    {  5,   21,  12,   1,        1696 },    /* 10101 */
    {  5,   24,   0,   0,  0x00       },    /* 11000 */
    {  5,   25,   0,   0,  0x01       },    /* 11001 */
    {  6,   44,  14,   1,        5792 },    /* 101100 */
    {  6,   45,  15,   1,       22176 },    /* 101101 */
    {  6,   52,   0,   0,  0x02       },    /* 110100 */
    {  6,   53,   0,   0,  0x03       },    /* 110101 */
    {  6,   54,   0,   0,  0xFF       },    /* 110110 */
    {  7,   92,  18,   1,       54944 },    /* 1011100 */
    {  7,   93,  20,   1,      317088 },    /* 1011101 */
    {  7,  110,   0,   0,  0x04       },    /* 1101110 */
    {  7,  111,   0,   0,  0x05       },    /* 1101111 */
    {  7,  112,   0,   0,  0x06       },    /* 1110000 */
    {  7,  113,   0,   0,  0x07       },    /* 1110001 */
    {  7,  114,   0,   0,  0x08       },    /* 1110010 */
    {  7,  115,   0,   0,  0x09       },    /* 1110011 */
    {  7,  116,   0,   0,  0x0A       },    /* 1110100 */
    {  7,  117,   0,   0,  0x0B       },    /* 1110101 */
    {  7,  118,   0,   0,  0x3A       },    /* 1110110 */
    {  7,  119,   0,   0,  0x3B       },    /* 1110111 */
    {  7,  120,   0,   0,  0x3C       },    /* 1111000 */
    {  7,  121,   0,   0,  0x3D       },    /* 1111001 */
    {  7,  122,   0,   0,  0x3E       },    /* 1111010 */
    {  7,  123,   0,   0,  0x3F       },    /* 1111011 */
    {  7,  124,   0,   0,  0x40       },    /* 1111100 */
    {  7,  125,   0,   0,  0x80       },    /* 1111101 */
    {  8,  188,  20,   1,     1365664 },    /* 10111100 */
    {  8,  189,  21,   1,     2414240 },    /* 10111101 */
    {  8,  252,   0,   0,  0x0C       },    /* 11111100 */
    {  8,  253,   0,   0,  0x38       },    /* 11111101 */
    {  8,  254,   0,   0,  0x39       },    /* 11111110 */
    {  8,  255,   0,   0,  0x66       },    /* 11111111 */
    {  9,  380,  22,   1,     4511392 },    /* 101111100 */
    {  9,  381,  23,   1,     8705696 },    /* 101111101 */
    {  9,  382,  24,   1,    17094304 },    /* 101111110 */
    { 0 }
};

/*****************************************************************************/
void *
rdp8_compress_create(int flags)
{
    struct bulk_rdp8 *bulk;

    if ((flags & NL_RDP8_FLAGS_RDP80) == 0)
    {
        return NULL;
    }
    bulk = (struct bulk_rdp8 *) malloc(sizeof(struct bulk_rdp8));
    if (bulk == NULL)
    {
        return NULL;
    }
    memset(bulk, 0, sizeof(struct bulk_rdp8));
    return bulk;
}

/*****************************************************************************/
int
rdp8_compress_destroy(void *handle)
{
    struct bulk_rdp8 *bulk;

    bulk = (struct bulk_rdp8 *) handle;
    if (bulk == NULL)
    {
        return 1;
    }
    free(bulk);
    return 0;
}

/*****************************************************************************/
static struct rdp_data_segment *
rdp8_compress_segment(void *handle, const char *data, int data_bytes)
{
    if (handle == NULL)
    {
        return 1;
    }

    if (data_bytes > 0xffff)
    {
        return 1;
    }

    struct rdp_data_segment *segment;
    int bytes_remaining = data_bytes;

    segment = (struct rdp_data_segment *) malloc (sizeof(struct rdp_data_segment));
    if (segment == NULL)
    {
        return 1;
    }




    return segment;
}

/*****************************************************************************/
int
rdp8_compress(void *handle, char **cdata, int *cdata_bytes, int *flags,
              const char *data, int data_bytes)
{

    if (handle == NULL )
    {
        return 1;
    }

    int segment_count = data_bytes / 0xffff;
    int bytes_remaining = data_bytes;

    if (data_bytes > 0xffff)
    {
        /* segmented */
    }
    else
    {
        /* single */

    }

    return 0;
}


