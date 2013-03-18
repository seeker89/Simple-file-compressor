	/** @file dehuf.c
		* Decompression
	*/

#include <stdio.h>
#include "functions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////// MAIN

int main(int n, char **argv){

	stamp codes[256];

	unsigned long int total_chars = 0, octets_ecrits = 0;
	unsigned char buff[3];
	
	FILE* file_input, *file_output;

	int ver = 0, ver_pos2 = 8; 

	if(n >= 2){ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////// DECODAGE
			printf("\n\nBienvenue au Huffman Code v.%d.%d\n\nInitialisation du mode DEcodage.\n\n\n", ver, ver_pos2);
			
			if(file_input = fopen(argv[1], "r")){
	
				initCode(codes, 256);
				
				// calcule la taille du fichier...
				while(fread(buff, sizeof(char), 1, file_input) > 0){ total_chars++; }; rewind(file_input);
				
				printf("\nInitialisation des codes.... ");
				lireHeader(file_input, codes);
				printTCodes(codes);
				
				printf("\nDecodage....\n"); fflush(stdout);
				
				if(n>=3){
					
					if(file_output = fopen(argv[2], "w")){
					
						octets_ecrits += decode(file_input, file_output, codes, total_chars, 0);
						printf("\nOK\n\nTaille de fichier decode (%s): %d bytes (%f %%)\n\n", argv[2], (int)octets_ecrits, (octets_ecrits/(float)total_chars)*100);
						fclose(file_output);
					}else{
						printf("On n'a pas pu creer un fichier '%s'\n", argv[2]);
					}
				}else{
				
					octets_ecrits += decode(file_input, file_output, codes, total_chars, 1);
					printf("\nOK\n\nTaille de output: %d bytes (%f %%)\n\n", (int)octets_ecrits, (octets_ecrits/(float)total_chars)*100);
				}
			}else{
				printf("\nError. Le fichier '%s' ne peut pas etre ouvert.\n", argv[1]);
			}
//////////////////////////////////////////////////////////////////////////////////////////////////////////// DECODAGE - FIN
	}else{
		
		printf("Utilisation: %s <fichier source> [<fichier destination>]\n\n", argv[0]);
	}
	return 0;
}
