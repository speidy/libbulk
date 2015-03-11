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

#ifndef __BULK_RDP8_COMPRESS_H
#define __BULK_RDP8_COMPRESS_H

typedef unsigned char byte;
typedef unsigned short uint16;
typedef unsigned int uint32;

/* flags for rdp8_compress_create */
#define NL_RDP8_FLAGS_RDP80 0x04

/* flags for rdp8 bulk compression */
#define NL_PACKET_COMPRESSED       0x20
#define NL_PACKET_COMPR_TYPE_RDP8  0x04
#define NL_COMPRESSION_TYPE_MASK   0x0F

/* descriptor values */
#define SEGMENTED_SINGLE        0xE0
#define SEGMENTED_MULTIPART     0xE1

/* token assignments from the spec, sorted by prefixLength */
typedef struct _Token
{
    int prefixLength;     /* number of bits in the prefix */
    int prefixCode;       /* bit pattern of this prefix */
    int valueBits;        /* number of value bits to read */
    int tokenType;        /* 0=literal, 1=match */
    uint32 valueBase;     /* added to the value bits */
} Token;

void *
rdp8_compress_create(int flags);
int
rdp8_compress_destroy(void *handle);
int
rdp8_compress(void *handle, char **cdata, int *cdata_bytes, int *flags,
              const char *data, int data_bytes);

#endif

