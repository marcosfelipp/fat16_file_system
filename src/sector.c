#include "sector.h"

char output_file_dir[] = "../../new_file.txt"; //Output to copy file
int copyVar =0;

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

void sector_read(int fd, unsigned int secnum, BUFFER *buffer,int buffer_sz)
{
  int sector = secnum * buffer_sz;
  lseek(fd,secnum, SEEK_SET);
  read(fd, &buffer->data,buffer_sz);
}


// Function receive the first cluster of file and copy the data to external diretory of FAT
void copy_file(int fd,int cluster){
	// Open output file to record data:
	BUFFER *buffer = (BUFFER*)calloc(1,sizeof(BUFFER));
	int buffer_sz = 2048;
	int output = open(output_file_dir,O_WRONLY);
	printf("Primeiro CLuster data: %d\n",cluster);
	// Read cluster adress while file 
	while(cluster != 0xFFFF){
		printf("lendo cluster\n");
		sector_read(fd,(153+4*(cluster-2))*512,buffer,buffer_sz);
		write(output,&buffer->data,sizeof(buffer->data));
		printf("%s\n",buffer->data);
		cluster = 512 + cluster*2;
		lseek(fd,cluster, SEEK_SET);
		read(fd, &cluster,2);
    	printf("CLuster:%d\n",cluster);
	}
	free(buffer);

}

// Function receive the path and find the archive/directory
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
				printf("Arquivo encontrado\n");
				printf("CLuster: %d\n",diretorio->DIR_FstClusL);
				// Se usuario definiu que quer copiar o arquivo:
				if(copyVar == 1)
					copy_file(fd,diretorio->DIR_FstClusL);
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
			printf("CLuster: %d\n",diretorio->DIR_FstClusL);
			free(diretorio);
			return;
		}

	}
			
}