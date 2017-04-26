#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct directory_entry DIR;

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


void main(){
	FILE *imagem = fopen("fat16.img", "rb");
	//char c;
	printf("iniciando...\n");
	DIR node;
	/*do{
		c = getc(imagem);
		printf("%c" , c);    
	}while (c != EOF);
	* 
	*/
	fread(&node,sizeof(DIR),1,imagem);
	for(int i=0;i<4;i++){
		//printf("%c",node.DIR_Attr[i]);
	}
	printf("%c",node.DIR_CrtTimeTenth);
	
	printf("\n");
	fclose(imagem);
}
