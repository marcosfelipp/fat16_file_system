#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

typedef struct cadastro
{
	int idade;
	int cpf;
	int id;

} CAD;

void sector_read(unsigned int secnum, void *buffer,int buffer_sz)
{
  int fd = open("test.txt", O_RDWR);
  lseek(fd,secnum, SEEK_SET);
  read(fd,buffer,buffer_sz);
  close(fd);
}


void sector_write(void *buffer,int buffer_sz)
{
  int fd = open("test.txt", O_WRONLY);
  lseek(fd,1, SEEK_END);
  write(fd,buffer,buffer_sz);
  close(fd);
}





void main(){
	CAD *temp = malloc(sizeof(CAD));
	CAD *temp2 = malloc(sizeof(CAD));
	short short int aa;
	printf("a%d\n",sizeof(aa));
	temp->idade = 10;
	temp->cpf = 4;
	temp->id = 1996;

	sector_write(temp,sizeof(CAD));
	sector_read(1,temp2,sizeof(CAD));

	printf("%d\n",temp2->id);
}