

void show_menu(){
	while(!(strcmp(command,"exit"))){
	 		printf("\e[H\e[2J");
	 		printf("Welcome to MAR-FS16 File System\n");
	 		printf("Use \"help\" for help, "exit" to close\n");
	 		printf(">\n");
	 		scanf("%s",command);
	 		

	 		// List attributs of diretory or file:
	 		if(!(strcmp(command,"ls"))){
	 			scanf("%s",path);

	 		}
	 		// GO to diretory
	 		else
	 			if(!(strcmp(command,"copy"))){


	 			}else 
	 				//Open file
	 				if(!(strcmp(command,"./"))){

	 				}else 
	 					printf("Invalid command\n");

	 	}
}