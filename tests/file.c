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
// size_t -> unsigned int de 2 bytes

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
	size_t DIR_FstClusL;
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

typedef struct {
    size_t offset_to_fat;
    size_t size_of_fat;
    size_t offset_to_root_dir;
    size_t size_of_root_dir;
    size_t offset_to_clusters;
    size_t size_of_cluster;
} fs_info_t;


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
	lseek(fd,pos+26, SEEK_SET);
	read(fd, &diretorio->DIR_FstClusL,2);
}


void main(){
	int imagem = open("../../fat16.img", O_RDWR);
	printf("iniciando...\n");
	boot_sector_t *bs = (boot_sector_t*) calloc(1,sizeof(boot_sector_t));
	DIR *diretorio = (DIR*) calloc(1,sizeof(DIR));
	read_boot_sector(imagem,bs);
	printf("Numero de entadas diretorio raiz: %d\n",bs->root_entries);
	int dir_raiz = (bs->number_of_fats * bs->fat_sz) + 1;
	printf("sectors_per_cluster%d\n",bs->sectors_per_cluster);


	int root = ((bs->root_entries * 32) + (bs->bytes_per_sector -1))/bs->bytes_per_sector;
	int first_data_sector = bs->reserved_sectors + (bs->number_of_fats * bs->fat_sz) + root;


	printf("raiz: %d\n",dir_raiz);
	printf("data:%d\n",first_data_sector);

	int proximo = 32;

	read_data(imagem,diretorio,512*dir_raiz);
	printf("name:%s   ",diretorio->DIR_Name);
	printf("First cluster:%d\n",diretorio->DIR_FstClusL);

	while(diretorio->DIR_Attr == ATTR_DIRECTORY || diretorio->DIR_Attr == ATTR_ARCHIVE){
		read_data(imagem,diretorio,512*dir_raiz+proximo);
		printf("Name:%s   ",diretorio->DIR_Name);
		printf("First cluster:%d\n",diretorio->DIR_FstClusL);
		proximo+=32;
	}


	int fl = 157 * 512;
	read_data(imagem,diretorio,fl+64);
	printf("Name:%s   ",diretorio->DIR_Name);
	printf("First cluster:%d\n",diretorio->DIR_FstClusL);

	

}
