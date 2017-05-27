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
	read(fd, &diretorio->DIR_Name,8);
	lseek(fd,pos+11, SEEK_SET);
	read(fd, &diretorio->DIR_Attr,1);
	lseek(fd,pos+26, SEEK_SET);
	read(fd, &diretorio->DIR_FstClusL,2);
}



void copy_file(){


}


void open_diretory(int fd, char *path, int nextpath, int sector){
	// Copy path name from folder path
	
	int i = 0;
	char path_name[11];
	while(path[nextpath]!='/' && path[nextpath]!='\0'){
		path_name[i] = path[nextpath];
		nextpath++;
		i++;
	}
	path_name[i] = '\n';
	printf("PATH SEARCH: %s\n",path_name);
	printf("DIRETORY LIST:\n");
	
	DIR *diretorio = (DIR*) calloc(1,sizeof(DIR));
	
	read_data(fd,diretorio,512*sector);
	printf("%s\n",diretorio->DIR_Name);
	
	printf("%d\n",diretorio->DIR_NTRes);

	int proximo = 32;
	
	while(diretorio->DIR_Attr == ATTR_DIRECTORY || diretorio->DIR_Attr == ATTR_ARCHIVE){
		if(strncmp(path_name,diretorio->DIR_Name,3) == 0){
			// verify if dir name is atual directory 
			if(diretorio->DIR_Attr == ATTR_DIRECTORY){
				open_diretory(fd,path,nextpath+1,153+(4*(diretorio->DIR_FstClusL-2)));
				free(diretorio);
				return;
			}if(diretorio->DIR_Attr == ATTR_ARCHIVE){
				copy_file();
				printf("Arquivo encontrado\n");
				free(diretorio);
				return;
			}
		}
		if(diretorio->DIR_FstClusL != 0xFFFF){
			read_data(fd,diretorio,512*sector+proximo);
			printf("%s\n",diretorio->DIR_Name);
			proximo+=32;
		}else{
			printf("Arquivo nao encontrado2\n");
			free(diretorio);
			return;
		}

	}
			
}


void main(int argc, char *argv[]){
	char path[30];
	printf("\e[H\e[2J");

	// Open disk image:
	int imagem = open("../../fat16.img", O_RDWR);
	printf("iniciando...\n");
	
	// Read boot sector:
	boot_sector_t *bs = (boot_sector_t*) calloc(1,sizeof(boot_sector_t));
	read_boot_sector(imagem,bs);
	int dir_raiz = (bs->number_of_fats * bs->fat_sz) + 1;
	int root = ((bs->root_entries * 32) + (bs->bytes_per_sector -1))/bs->bytes_per_sector;
	int first_data_sector = bs->reserved_sectors + (bs->number_of_fats * bs->fat_sz) + root;
	// Search file in path:
	
	strcpy(path,argv[1]);
	open_diretory(imagem,path,0,dir_raiz);

}

