#ifndef SECTOR_H
#define SECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Directory definitions constants
#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20

typedef struct directory_entry DIR;
typedef struct Buffer BUFFER;




// FAT16 STRUCTURES
// --------------------------------------------------------------------------------------

struct directory_entry{
	char DIR_Name[11]; //“Short”  file  name  limited  to  11  characters
	char DIR_Attr;
	char DIR_NTRes; //Reserved. Must be set to 0.
	char DIR_CrtTimeTenth;
	char DIR_CrtTime[2]; //Creation time
	char DIR_CrtDate[2];
	char DIR_LstAccDat[2]; //Last access date.
	char DIR_FstClusH[2];
	char DIR_WrtTime [2];
	char DIR_WrtDate[2];
	size_t DIR_FstClusL;
	char DIR_FileSize[4]; //32-bit  quantity  containing  size  in  bytes  of file/directory described by this entry.
};

typedef struct {
	char fat_name[8]; 
	size_t bytes_per_sector;
    size_t sectors_per_cluster;
    size_t reserved_sectors;
    size_t number_of_fats;
    size_t root_entries;
    size_t sectors_per_fat;
	size_t fat_sz;
} boot_sector_t;


// --------------------------------------------------------------------------------------

struct Buffer{
	char data[2048];
};

// FUNCTIONS DEFINITIONS
// --------------------------------------------------------------------------------------

void read_boot_sector(int fd, boot_sector_t * bsector);
void read_data(int fd, DIR *diretorio,int pos);
void sector_read(int fd, unsigned int secnum, BUFFER *buffer,int buffer_sz);
void copy_file(int fd,int cluster);
void open_diretory(int fd, char *path, int nextpath, int sector);

#endif
