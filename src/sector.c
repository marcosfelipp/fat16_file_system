#include "sector.h"


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
	pos = pos * 512;
	lseek(fd,pos, SEEK_SET);
	read(fd, &diretorio->DIR_Name,11);
	lseek(fd,pos+11, SEEK_SET);
	read(fd, &diretorio->DIR_Attr,1);
	lseek(fd,pos+26, SEEK_SET);
	read(fd, &diretorio->DIR_FstClusL,2);
}

// Recursive function to walk in diretories
void go_to_path(int fd,char *nome,DIR *diretorio_atual){
	// Se o cluster atual for um diretorio:
	if(diretorio_atual->DIR_Attr == ATTR_DIRECTORY){
		printf("Name:%s   ",diretorio->DIR_Name);
		printf("First cluster:%d\n",diretorio->DIR_FstClusL);
		read_data(imagem,diretorio_atual,dir_raiz+proximo);

	}else
		// Se o cluster atual for um arquivo:
		if(diretorio_atual->DIR_Attr == ATTR_ARCHIVE){
			// Verificar se ja estamos no fim do path
		}

	read_data(imagem,diretorio,512*dir_raiz+proximo);

}

/* Read the sector 'secnum' from the image to the buffer */
void sector_read(int fd, unsigned int secnum, BUFFER *buffer,int buffer_sz)
{
  int sector = secnum * buffer_sz;
  lseek(fd,sector, SEEK_SET);
  read(fd, &buffer->data,buffer_sz);  
}




// FUNÇÃO PARA LER O BLOCO DE INICIALIZACAO, NAO USAR - MUITO TRABALHO DE IMPLEMENTACAO
// void read_boot_sector(int fd, BPB *bs){
// 	BUFFER *bf = (BUFFER*) calloc(1,sizeof(BUFFER));
// 	// Read buffer with boot sector data:
// 	sector_read(fd,1,bf,512);
// 	// Transform buffer in boot sector structure
// 	memcpy(bs->DIR_Name, const void *src, 11);
// 	atoi();
// }
