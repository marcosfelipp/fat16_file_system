#ifndef SECTOR_H
#define SECTOR_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef struct directory_entry DIR;
typedef struct boot_sector_t BPB;
typedef struct buffer BUFFER;
typedef struct controller CONTROL;

struct buffer{	
	char data[512];
};

// FAT16 STRUCTURES
// --------------------------------------------------------------------------------------

// Structure for store the root entry and first data sector
struct controller{
	int root_entry;
	int fist_data_sector;
};

struct directory_entry{
	char DIR_Name[11]; //“Short”  file  name  limited  to  11  characters
	char DIR_Attr;
	char DIR_NTRes; 
	char DIR_CrtTimeTenth;
	size_t DIR_CrtTime; //Creation time. Granularity is 2 seconds.
	size_t DIR_CrtDate;
	size_t DIR_LstAccDat; //Last access date.
	size_t DIR_FstClusH;
	size_t DIR_WrtTime;
	size_t DIR_WrtDate;
	size_t DIR_FstClusL;
	int DIR_FileSize; //32-bit  quantity  containing  size  in  bytes  of file/directory described by this entry.
};

// Structure for store the boot sector
typedef struct boot_sector_t{
	char fat_name[8];
	size_t bytes_per_sector;
    size_t sectors_per_cluster;
    size_t reserved_sectors;
    size_t number_of_fats;
    size_t root_entries;
    size_t sectors_per_fat;
	size_t fat_sz;
} ;


// --------------------------------------------------------------------------------------


void sector_read(int fd, unsigned int secnum, BUFFER *buffer,int buffer_sz);
void read_data(int fd, DIR *diretorio,int pos);
void read_boot_sector(int fd, boot_sector_t * bsector);

#endif
