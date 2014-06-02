#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char *name) {
  printf("Usage: %s <file>\n", name);
  exit(1);
}

// detect endianness 0 = big, 1 = little
// http://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
int endianness()
{
  unsigned int x = 1;
  return (int) (((char *)&x)[0]);
}

// swap little/big endian
// reference: http://forums.codeguru.com/showthread.php?292902-C-General-How-do-I-convert-between-big-endian-and-little-endian-values
long long endian_swap(unsigned long long x)
{
  x = (x>>56) |
    ((x<<40) & 0x00FF000000000000) |
    ((x<<24) & 0x0000FF0000000000) |
    ((x<<8)  & 0x000000FF00000000) |
    ((x>>8)  & 0x00000000FF000000) |
    ((x>>24) & 0x0000000000FF0000) |
    ((x>>40) & 0x000000000000FF00) |
    (x<<56);
  return x;
}

int main(int argc, char *argv[]) {
  unsigned long long buf[16384], c = 0;
  FILE *in;
  int i;
  int en = endianness();

  if (argc != 2) {
    usage(argv[0]);
  }
  in = fopen(argv[1], "rb");
  if (in == NULL) {
    usage(argv[0]);
  }

  fread(buf, 8192, 8, in);
  fseek(in, -65536, SEEK_END);
  fread(&buf[8192], 8192, 8, in);
  for (i = 0; i < 16384; i++) {
    c += en == 0 ? endian_swap(buf[i]) : buf[i];
  }
  c+= ftell(in);
  fclose(in);

  printf("%016llx\n", c);
  return 0;
}
