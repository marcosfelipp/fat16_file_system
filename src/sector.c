#include "sector.h"

char output_file_dir[] = "../../new_file.txt"; //Output to copy file
int copyVar =0;



void sector_read(int fd, unsigned int secnum, void *buffer,int buffer_sz)
{
  lseek(fd,secnum, SEEK_SET);
  read(fd,buffer,buffer_sz);
}


// Function receive the first cluster of file and copy the data to external diretory of FAT
void copy_file(int fd,int cluster){
	// Open output file to record data:
	BUFFER *buffer = (BUFFER*)calloc(1,sizeof(BUFFER));
	int buffer_sz = 2048;
	int output = open(output_file_dir,O_CREAT,S_IRUSR);
	// Read cluster adress while file 
	while(cluster != 0xFFFF){
		BUFFER *buffer = (BUFFER*)calloc(1,sizeof(BUFFER));
		sector_read(fd,(153+4*(cluster-2))*512,buffer,buffer_sz);
		write(output,&buffer->data,sizeof(buffer->data));
		cluster = 512 + cluster*2;
		lseek(fd,cluster, SEEK_SET);
		read(fd, &cluster,2);
    	free(buffer);
	}
	
	printf("FILE copied with sucess to ../../new_file.txt\n");
}

void list_diretory(int fd,int sector){
	printf("\n");
	DIR *diretorio = (DIR*) calloc(1,sizeof(DIR));
	sector_read(fd,512*sector,diretorio,sizeof(DIR));
	// read_data(fd,diretorio,512*sector);
	int proximo = 32;
	while(diretorio->DIR_Attr == ATTR_DIRECTORY || diretorio->DIR_Attr == ATTR_ARCHIVE){
		printf("%s   -    %d    -    %d  \n",diretorio->DIR_Name,diretorio->DIR_CrtDate,diretorio->DIR_CrtTime);
		sector_read(fd,512*sector+proximo,diretorio,sizeof(DIR));
		proximo+=32;
	}
	free(diretorio);
}

// Function receive the path and find the archive/directory
void open_diretory(int fd, char *path, int nextpath, int sector,int *comando){
	// Copy path name from folder path
	int i = 0;
	char path_name[11];
	while(path[nextpath]!='/' && path[nextpath]!='\0'){
		path_name[i] = path[nextpath];
		nextpath++;
		i++;
	}
	path_name[i] = '\n';

	// Aloca diretório:
	DIR *diretorio = (DIR*) calloc(1,sizeof(DIR));
	sector_read(fd,512*sector,diretorio,sizeof(DIR));

	int proximo = 32;
	while(diretorio->DIR_Attr == ATTR_DIRECTORY || diretorio->DIR_Attr == ATTR_ARCHIVE){
		if(strncmp(path_name,diretorio->DIR_Name,3) == 0){
			// verify if dir name is atual directory 
			if(diretorio->DIR_Attr == ATTR_DIRECTORY){
				// Verify if is the last path:
				if(path[nextpath] == '\0'){
					// Comand of copy diretory:
					if(*comando == 0){
						printf("NOME    CREATION DATE    CREATION TIME\n");
						list_diretory(fd,153+(4*(diretorio->DIR_FstClusL-2)));
						free(diretorio);
						return;
					}
				}
				open_diretory(fd,path,nextpath+1,153+(4*(diretorio->DIR_FstClusL-2)),comando);
				free(diretorio);
				return;
			}if(diretorio->DIR_Attr == ATTR_ARCHIVE){
				printf("Arquivo encontrado\n");
				printf("CLuster: %d\n",diretorio->DIR_FstClusL);
				// COPY file:
				if(*comando == 1)
					copy_file(fd,diretorio->DIR_FstClusL);
				free(diretorio);
				return;
			}
		}
		// If alhead have directory ou files in this diretory:
		if(diretorio->DIR_FstClusL != 0xFFFF){
			sector_read(fd,512*sector+proximo,diretorio,sizeof(DIR));
			proximo+=32;
		}else{
			printf("Arquivo nao encontrado2\n");
			printf("CLuster: %d\n",diretorio->DIR_FstClusL);
			free(diretorio);
			return;
		}


	}
			
}


