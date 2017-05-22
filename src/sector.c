#include "sector.h"

/* Read the sector 'secnum' from the image to the buffer */
void sector_read(FILE *fd, unsigned int secnum, void *buffer)
{
  int sector = secnum * 512;
  lseek(fd,0, SEEK_SET);
  read(fd, &buffer,sector);  
}
