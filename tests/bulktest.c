#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bulk_rdp8_decompress.h"

/*****************************************************************************/
/* produce a hex dump */
void
hexdump(char *p, int len)
{
    unsigned char *line;
    int i;
    int thisline;
    int offset;

    line = (unsigned char *)p;
    offset = 0;

    while (offset < len)
    {
        printf("%04x ", offset);
        thisline = len - offset;

        if (thisline > 16)
        {
            thisline = 16;
        }

        for (i = 0; i < thisline; i++)
        {
            printf("%02x ", line[i]);
        }

        for (; i < 16; i++)
        {
            printf("   ");
        }

        for (i = 0; i < thisline; i++)
        {
            printf("%c", (line[i] >= 0x20 && line[i] < 0x7f) ? line[i] : '.');
        }

        printf("\n");
        offset += thisline;
        line += thisline;
    }
}

/*****************************************************************************/
int
rdp8_decompress_test (char *compressed_bytes, int compressed_len)
{
  char *decompressed_bytes = NULL;
  void *decompressor = NULL;
  int decomperssed_len = 0;

  // use rdp8 decompressor
  decompressor = rdp8_decompress_create (NL_RDP8_FLAGS_RDP80);
  if (decompressor == NULL)
  {
    return 1;
  }

  rdp8_decompress (decompressor, compressed_bytes, compressed_len,
			NL_PACKET_COMPR_TYPE_RDP8, &decompressed_bytes,
			&decomperssed_len);

  // dump out decompressed data
  hexdump(decompressed_bytes, decomperssed_len);
  printf("\n");

  // free out
  if (decompressed_bytes)
  {
    free (decompressed_bytes);
  }

  rdp8_decompress_destroy (decompressor);

  return 0;
}

/*****************************************************************************/
int
main (int argc, char **argv)
{
  char sample1[] =
    { 0xe0, 0x24, 0xCE, 0x9B, 0x19, 0x62, 0x18, 0x00 };

  char sample2[] =
    { 0xE0, 0x04, 0x54, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6B, 0x20,
	0x62, 0x72, 0x6F, 0x77, 0x6E, 0x20, 0x66, 0x6F, 0x78, 0x20, 0x6A, 0x75,
	0x6D, 0x70, 0x73, 0x20, 0x6F, 0x76, 0x65, 0x72, 0x20, 0x74, 0x68, 0x65,
	0x20, 0x6C, 0x61, 0x7A, 0x79, 0x20, 0x64, 0x6F, 0x67 };

  char sample3[] =
    { 0xE0, 0x24, 0x20, 0x90, 0x88, 0x71, 0x1F, 0xB2, 0x01 };

  char sample4[] =
    { 0xE1, 0x03, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x04,
	0x54, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6B, 0x20, 0x62, 0x72,
	0x6F, 0x77, 0x6E, 0x20, 0x0E, 0x00, 0x00, 0x00, 0x04, 0x66, 0x6F, 0x78,
	0x20, 0x6A, 0x75, 0x6D, 0x70, 0x73, 0x20, 0x6F, 0x76, 0x65, 0x10, 0x00,
	0x00, 0x00, 0x24, 0x39, 0x08, 0x0E, 0x91, 0xF8, 0xD8, 0x61, 0x3D, 0x1E,
	0x44, 0x06, 0x43, 0x79, 0x9C, 0x02, };

  /* decompress samples */
  rdp8_decompress_test (sample1, sizeof(sample1));
  rdp8_decompress_test (sample2, sizeof(sample2));
  rdp8_decompress_test (sample3, sizeof(sample3));
  rdp8_decompress_test (sample4, sizeof(sample4));

  return 0;
}

