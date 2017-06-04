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
typedef struct root boot_sector_t;



// FAT16 STRUCTURES
// --------------------------------------------------------------------------------------

struct directory_entry{
	char DIR_Name[11]; //“Short”  file  name  limited  to  11  characters
	char DIR_Attr;
	char DIR_NTRes; //Reserved. Must be set to 0.
	char DIR_CrtTimeTenth;
	short int DIR_CrtTime; //Creation time
	short int DIR_CrtDate;
	short int DIR_LstAccDat; //Last access date.
	short int DIR_FstClusH;
	short int DIR_WrtTime ;
	short int DIR_WrtDate;
	size_t DIR_FstClusL;
	char DIR_FileSize[4]; //32-bit  quantity  containing  size  in  bytes  of file/directory described by this entry.
}__attribute__((packed, aligned(1)));

struct root {
	char jmp_boot[3];
	char fat_name[8]; 
	unsigned short int bytes_per_sector;
    char sectors_per_cluster;
    unsigned short int reserved_sectors;
    char number_of_fats;
    unsigned short int root_entries;
    unsigned short sectors_per_fat;
	char media;
	unsigned short fat_sz;
}__attribute__((packed, aligned(1)));


// --------------------------------------------------------------------------------------

struct Buffer{
	char data[2048];
};

// FUNCTIONS DEFINITIONS
// --------------------------------------------------------------------------------------

void sector_read(int fd, unsigned int secnum, void *buffer,int buffer_sz);
void copy_file(int fd,int cluster);
void open_diretory(int fd, char *path, int nextpath, int sector,int *comando);

#endif
