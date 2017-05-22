#ifndef SECTOR_H
#define SECTOR_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/* Read the sector 'secnum' from the image to the buffer */
void sector_read(FILE *fd, unsigned int secnum, void *buffer);

#endif
