#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
typedef struct directory_entry DIR;
typedef struct bpb BPB;


#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20

struct directory_entry{
	char DIR_Name[11]; //“Short”  file  name  limited  to  11  characters
	char DIR_Attr;
	char DIR_NTRes; //Reserved. Must be set to 0.
	char DIR_CrtTimeTenth;
	char DIR_CrtTime[2]; //Creation time. Granularity is 2 seconds.
	char DIR_CrtDate[2];
	char DIR_LstAccDat[2]; //Last access date.
	char DIR_FstClusH[2];
	char DIR_WrtTime [2];
	char DIR_WrtDate[2];
	char DIR_FstClusL[2];
	char DIR_FileSize[4]; //32-bit  quantity  containing  size  in  bytes  of file/directory described by this entry.
};
struct bpb{
	char BS_jmpBoot[3];
	char BS_OEMName[8];
	unsigned short int  BPB_BytsPerSec;
	char BPB_SecPerClus;
	short int BPB_RsvdSecCnt;
	char BPB_NumFATs;
	unsigned short int BPB_RootEntCnt;
	short int BPB_TotSec16;
	char BPB_Media;
	unsigned short int BPB_FATSz16;
	unsigned short int BPB_SecPerTrk;
	unsigned short int BPB_NumHeads;
	unsigned int BPB_HiddSec;
	unsigned int BPB_TotSec32;
	char BS_DrvNum;
	char BS_Reserved1;
	char BS_BootSig;
	unsigned int BS_VolID;
	char BS_VolLab[11];
	char BS_FilSysType[8];

};


// size_t -> unsigned int de 2 byetes
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

void read_boot_sector(int fd, boot_sector_t * bsector) {
		lseek(fd, 3, SEEK_SET);
		read(fd, &bsector->fat_name, 8);
		lseek(fd, 11, SEEK_SET);
    read(fd, &bsector->bytes_per_sector, 2);
    lseek(fd, 13, SEEK_SET);
    read(fd, &bsector->sectors_per_cluster, 1);
    lseek(fd, 14, SEEK_SET);
    read(fd, &bsector->reserved_sectors, 2);
    lseek(fd, 16, SEEK_SET);
    read(fd, &bsector->number_of_fats, 1);
    lseek(fd, 17, SEEK_SET);
    read(fd, &bsector->root_entries, 2);
    lseek(fd, 19, SEEK_SET);
    read(fd, &bsector->sectors_per_fat, 2);
		lseek(fd, 22, SEEK_SET);
    read(fd, &bsector->fat_sz, 2);

}

void read_data(int fd, DIR *diretorio,int pos){
	lseek(fd,pos, SEEK_SET);
	read(fd, &diretorio->DIR_Name,11);
	lseek(fd,pos+11, SEEK_SET);
	read(fd, &diretorio->DIR_Attr,1);

}


void main(){
	int imagem = open("../../fat16.img", O_RDWR);
	printf("iniciando...\n");
	boot_sector_t *bs = (boot_sector_t*) calloc(1,sizeof(boot_sector_t));
	DIR *diretorio = (DIR*) calloc(1,sizeof(DIR));
	read_boot_sector(imagem,bs);

	int dir_raiz = (bs->number_of_fats * bs->fat_sz) + 1;


	int root = ((bs->root_entries * 32) + (bs->bytes_per_sector -1))/bs->bytes_per_sector;
	int first_data_sector;
	printf("raiz: %d\n",dir_raiz);
	printf("Root: %d\n",root);
	// printf("data:%d\n",first_data_sector);
	// dir_record_t *diretorio = (dir_record_t*) calloc(1,sizeof(dir_record_t));
	printf("%d\n",bs->fat_sz);
	printf("%d\n",bs->number_of_fats);
	printf("BPS: %d\n",bs->bytes_per_sector);

	int proximo = 32;

	read_data(imagem,diretorio,512*dir_raiz);

	while(diretorio->DIR_Attr == ATTR_DIRECTORY || diretorio->DIR_Attr == ATTR_ARCHIVE){
		printf("name:%s\n",diretorio->DIR_Name);
		read_data(imagem,diretorio,512*dir_raiz+proximo);
		proximo+=32;
	}


}
