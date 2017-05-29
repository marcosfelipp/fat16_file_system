// #include <errno.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdint.h>

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <unistd.h>

// #define FUSE_USE_VERSION 26
// #include <fuse.h>

#include "sector.h"
// #include "log.h"

// // -----------------------------------------------------------------------------

// void *fat16_init(struct fuse_conn_info *conn)
// {
//   log_msg("Chamando init\n");

//   // Your code here

//   return NULL;
// }

// void fat16_destroy(void *data)
// {
//   log_msg("Chamando destroy\n");

//   // Your code here
// }

// //------------------------------------------------------------------------------

// struct fuse_operations fat16_oper = {
//   .init       = fat16_init,
//   .destroy    = fat16_destroy
// };


// //------------------------------------------------------------------------------





int main(int argc, char *argv[])
{
	char path[30]; 
	char command[10];
	int comando;
	printf("\e[H\e[2J");
	strcpy(command,argv[1]);
	strcpy(path,argv[2]);

	// Open disk image: (Write Only mode)
	int imagem = open("../../fat16.img", O_RDWR);

	
	// Read boot sector(First sector)
	boot_sector_t *bs = (boot_sector_t*) calloc(1,sizeof(boot_sector_t));
	read_boot_sector(imagem,bs);
	

	int dir_raiz = (bs->number_of_fats * bs->fat_sz) + 1;
	int root = ((bs->root_entries * 32) + (bs->bytes_per_sector -1))/bs->bytes_per_sector;
	int first_data_sector = bs->reserved_sectors + (bs->number_of_fats * bs->fat_sz) + root;
	
	
	// List diretory:
	if(strcmp(command,"list") == 0){
		comando = 0;
		open_diretory(imagem,path,0,dir_raiz,&comando);
	}else 
		//Copy file of path to other place:
		if(strcmp(command,"copy") == 0){
			comando = 1;
			open_diretory(imagem,path,0,dir_raiz,&comando);
	}	

  
}
