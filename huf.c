	/** @file huf.c
		* Compression
	*/

#include <stdio.h>
#include "functions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////// MAIN

int main(int n, char **argv){

	// variables
	noeud arbre[512];	// tableau des noeds
	stamp codes[256];	// tableau des codes binaires pour des caracteres

	unsigned long int total_chars = 0, octets_ecrits = 0;
	
	FILE* file_input, *file_output;
	
	unsigned char buff[3], glob_buff = 0;
	int racine, glob_counter = 0;

	int ver = 0, ver_pos2 = 8; // pour faire jolie.
	
	if(n >= 3){
//////////////////////////////////////////////////////////////////////////////////////////////////////////// ENCODAGE

		printf("\n\nBienvenue au Huffman Code v.%d.%d\n\nInitialisation du mode ENcodage.\n\n\n", ver, ver_pos2);
		
		if(file_input = fopen(argv[1], "r")){
			
			// INITIALISATION DES STRUCTURES
			initArbre(arbre, 512);
			initCode(codes, 256);

			// lire des chars & compter nbs d'occurences
			while(fread(buff, sizeof(char), 1, file_input) > 0){
				arbre[(int)buff[0]].freq++;
				total_chars++;
			}
			
			// calculer la frequences a partir des nombres d'occurences
			occur2Freq(arbre, total_chars); 
			
			// CONTRUCTION DE L'ARBRE
			racine = consArbre(arbre, 256);
			
			
			// GENERATION DES CODES
			genereCodes(arbre, codes, racine, 0);

			printTArbre(arbre, racine); 
			printTCodes(codes);

			printf("\n\nLongeur moyenne de codage: %f\n", moyCodes(codes));
			printf("Encodage.... "); fflush(stdout);
			
			if(file_output = fopen(argv[2], "w")){
				
				octets_ecrits += sauvegardeHeader(file_output, codes, &glob_buff, &glob_counter);
				octets_ecrits += encode(file_input, file_output, codes, &glob_buff, &glob_counter);
				
					printf("\nTaille originelle : %d (%s); taille compressee : %d (%s); gain : %f%% !\n\n", (int)total_chars, argv[1], (int)octets_ecrits, argv[2], 100 - (octets_ecrits/(float)total_chars)*100);

				fclose(file_output);
			}else{
				printf("On n'a pas pu creer un fichier '%s'\n", argv[2]);
			}
			
			fclose(file_input);
		}else{
		
			printf("\nError. Le fichier '%s' ne peut pas etre ouvert.\n", argv[1]);
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////// ENCODAGE - FIN
	}else{
		
		printf("Utilisation: %s <fichier source> <fichier destination>\n\n", argv[0]);
	}
	return 0;
}
