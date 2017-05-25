#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FUSE_USE_VERSION 26
#include <fuse.h>

#include "sector.h"
#include "log.h"

// -----------------------------------------------------------------------------




//------------------------------------------------------------------------------

void *fat16_init(struct fuse_conn_info *conn)
{
  log_msg("Chamando init\n");

  // Your code here

  return NULL;
}

void fat16_destroy(void *data)
{
  log_msg("Chamando destroy\n");

  // Your code here
}

//------------------------------------------------------------------------------

struct fuse_operations fat16_oper = {
  .init       = fat16_init,
  .destroy    = fat16_destroy
};


//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  int ret;
  char command[5];
  char path[20];

  log_open();
  // ret = fuse_main(argc, argv, &fat16_oper, NULL);
  log_msg("ret: %d\n", ret);

  // Open image:
  int imagem = open("../../fat16.img", O_RDWR);
  if(filedesc < 0){
    log_msg("Error: File not found\n EXITING...\n");
    exit(0);
  }
  //---------------
  // PREPARE FILE SYSTEM, READ BOOT SECTOR:
  BPB *bs = (boot_sector_t*) calloc(1,sizeof(boot_sector_t));
  read_boot_sector(imagem,bs);
	log_msg("Starting\n");
  
 	strcpy(command,argv[0]);
 	strcpy(path,argv[1]);
 	CONTROL *control = (CONTROL*) calloc(1,sizeof(CONTROL));

 	control->root_entry= ((bs->root_entries * 32) + (bs->bytes_per_sector -1))/bs->bytes_per_sector;
	control->first_data_sector = bs->reserved_sectors + (bs->number_of_fats * bs->fat_sz) + root;

	go_to_path();
  

  // return ret;
}
