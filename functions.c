	/** @file functions.c
		* Tous les fonctions outilises dans les programmes
	*/

#include <stdio.h>
#include "functions.h"

	/**
		* Initialisation d'un arbre.
		* Parcours de tableau de structures noeud, de 0 a stop - 1.
		* Les membres pere, fg et fd sont intialises a -1.
		* Le membre freq est initialise a 0.0.
		* @param arbre le tableau de structures noeud - l'arbre a traiter.
		* @param stop l'indice minimale qui ne va pas etre traite.
	*/
	
void initArbre(noeud arbre[], int stop){

	for(int i=0;i<stop;i++){
		arbre[i].freq = 0.0;
		arbre[i].pere = -1;
		arbre[i].fg = -1;
		arbre[i].fd = -1;
	}
}


	/**
		* Initialisation du tableau des codes.
		* Parcours de tableau de structures stamp, de 0 a stop - 1.
		* Le membre num est initialise a 0.
		* Le premier element du membre code est initialise a '0'.
		* @param codes le tableau de structures stamp a traiter.
		* @param stop l'indice minimale qui ne va pas etre traite.
	*/
void initCode(stamp codes[], int stop){

	for(int i=0;i<stop;i++){
		codes[i].num = 0;
		codes[i].code[0] = '0';
	}
}


	/**
		* Calcule les frequences a partir de nombre d'occurences.
		* Parcours de tableau de structures noeud, en divisant son nombre d'occurences (stocke dans membre freq), par nombre total de caracteres.
		* @param arbre le tableau de structures noeud - l'arbre.
		* @param total nombre total de caracteres.
	*/
void occur2Freq(noeud arbre[], int total){
	int i = 0;
	for(i=0;i<256;i++){
		if(arbre[i].freq != 0.0){
			arbre[i].freq = arbre[i].freq / total;
		}
	}
}


	/**
		* Recherche du noeud minimal dans l'arbre.
		* Parcours de tableau de structures noeud, en cherchant le noeud avec la frequence minimale.
		* @param arbre le tableau de structures noeud - l'arbre.
		* @param max indice maximale a regarder.
	*/
int minimum(noeud arbre[], int max){
	int indice = -1;
	float freq = 1.0;
	while(max>=0){
		if((arbre[max].pere == -1) && (arbre[max].freq != 0.0) && (arbre[max].freq <= freq)){
			indice = max;
			freq = arbre[max].freq;
		}
		max--;
	}
	return indice;
}


	/**
		* Construction de l'arbre.
		* Parcours de tableau de structures noeud et etablit des liaisons pere-fils. Renvoie l'indice root de l'arbre.
		* @param arbre le tableau de structures noeud - l'arbre.
		* @param indice_nn indice de tableau arbre ou placer le premier noeud genere.
	*/
int consArbre(noeud arbre[], int indice_nn){
	/* CONSTRUIT UNE ARBRE */
	
	int minG, minD;
	
	while((minimum(arbre, (indice_nn - 1)) != -1)){
		// FILS GAUCHE
		minG = minimum(arbre, (indice_nn - 1));
		arbre[minG].pere = indice_nn;
		arbre[indice_nn].fg = minG;

		// FILS DROITE
		minD = minimum(arbre, (indice_nn - 1));
		if(minD == -1){
			break;
		}
		arbre[minD].pere = indice_nn;
		arbre[indice_nn].fd = minD;

		// SOMME DES FREQS
		arbre[indice_nn].freq = arbre[minG].freq + arbre[minD].freq;
		
		indice_nn++;
	}
	
	return (indice_nn -1);
}


	/**
		* Construction recursive des codes.
		* Parcours de tableau de structures noeud en attribuant 0 a fils gauche et 1 a fils droit.
		* @param arbre le tableau de structures noeud - l'arbre.
		* @param codes le tableau de structures stamp pour stocker des codes.
		* @param indice indice root de l'arbre.
		* @param profondeur de la reccurence - pour savoir le longeur du code genere.
	*/
void genereCodes(noeud arbre[], stamp codes[], int indice, int profondeur){
	if(arbre[indice].fg == -1 || arbre[indice].fd == -1){ // feuille
		sauvegardeCode(arbre, codes, indice, profondeur);
		//printf("sauvegarde: indice %d, profondeur %d", indice, profondeur);
	}else{
		arbre[arbre[indice].fg].code = '0';
		arbre[arbre[indice].fd].code = '1';
		genereCodes(arbre, codes, arbre[indice].fg, profondeur + 1);
		genereCodes(arbre, codes, arbre[indice].fd, profondeur + 1);
	}
}


	/**
		* Sauvegarder un code.
		* Parcours de l'arbre dans le sens de feuille, en suvagardant des 0 et 1 dans le tableau des structures stamp.
		* @param arbre le tableau de structures noeud - l'arbre.
		* @param codes le tableau de structures stamp pour stocker des codes.
		* @param indice feuille de l'arbre.
		* @param profondeur de la reccurence - le longeur du code genere.
	*/
void sauvegardeCode(noeud arbre[], stamp codes[], int indice, int profondeur){

	int i = profondeur - 1, j = indice;
	
	// le cas d'un seul caractere:
	if(profondeur == 0){
		codes[arbre[indice + 1].fg].code[0] = '0';
		codes[arbre[indice + 1].fg].num = 1;
	}else{
	
		// suite des bits dans l'orde inverse
		while (arbre[j].pere != -1 && i >= 0)
		{
			codes[indice].code[i] = arbre[j].code;
			j = arbre[j].pere;
			i--;
		}
	
		// nombre des bits utilises - profondeur de la reccurence
		codes[indice].num = profondeur;
	}
}

	/**
		* Ecrire 0 ou 1 dans le buffeur.
		* Parcours de l'arbre dans le sens de feuille, en suvagardant des 0 et 1 dans le tableau des structures stamp.
		* @param file_output le pointeur vers un fichier ouvert pour l'ecriture.
		* @param glob_buff le buffeur a ecrire dans le fichier.
		* @param glob_counter stocke le nombre des "bits" ecrits dans buffeur.
		* @param x 0 ou 1 a ecrire dans le buffeur.
	*/
void ecritUnBit(FILE *file_output, unsigned char *glob_buff, int *glob_counter, int x){
	
	*glob_buff *= 2;
	if(x==1){ 
		*glob_buff += 1; 
	}
	*(glob_counter) += 1;
	//printf("%d",x);
	
	if(*(glob_counter) == 8){
		fputc(*glob_buff, file_output);
		*(glob_counter) = 0;
	}
}


	/**
		* Ecrit la partie d'Entete dans le fichier compresse.
		* On fait la chose suivante:
			*    - sur un char on stocke le nombre total de codes outilises,
			*    - pour chaque caractere qui apparait dans le fichier de source:
			*         -# sur un char on stocke le caractere lui meme,
			*         -# sur un char on stocke un certain nombre X des bits outilises pour stocker le code binaire du caractere,
			*         -# sur X chars on stocke le code binaire du caractere (on ajoute des zeros au debut).
		* @param file_output le pointeur vers un fichier ouvert pour l'ecriture.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
		* @param glob_buff le buffeur a ecrire dans le fichier.
		* @param glob_counter stocke le nombre des "bits" ecrits dans buffeur.
	*/
int sauvegardeHeader(FILE * file_output, stamp codes[], unsigned char *glob_buff, int *glob_counter){

	int reste = 0, ecrit = 8;
	
	printf("%d codes sauvegardes. \n", fputc(compteCodes(codes) - 1, file_output) + 1);  	// le premier octet stocke nombre des codes
	
	for(int j=0; j<256; j++){
		if(codes[j].num != 0){
		
			fputc(j, file_output); 		// stocke le caractere
			fputc(codes[j].num, file_output); // stocke nombre des bits pris par le code
			ecrit += 16;
			
			reste = 0;					
			while (reste < codes[j].num){ reste += 8; }	// compter combien de zeros ajouter au debut
			reste -= codes[j].num;
			
			//printf("%c[%d]: \t[", j, codes[j].num);
			
			for (int h = 0; h < reste; h++){
				ecritUnBit(file_output, glob_buff, glob_counter, 0);
				//printf("0");
				ecrit++;
			}
			
			//printf("]");

			for (int i = 0; i < codes[j].num; i++){
				ecritUnBit(file_output, glob_buff, glob_counter, (codes[j].code[i] - 48));
				//printf("%d", codes[j].code[i] - 48);
				ecrit++;
			}
			//printf("\n");
		}
	}
	return ecrit/8;
}

	/**
		* Ecrit la partie binaire du fichier compresse.
		* Parcours de fichier de source, et pour chaque caractere, on stocke le code correspandant dans le fichier destination
		* @param file_input le pointeur vers le fichier source.
		* @param file_output le pointeur vers le fichier destination.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
		* @param glob_buff le buffeur a ecrire dans le fichier.
		* @param glob_counter stocke le nombre des "bits" ecrits dans buffeur.
	*/
int encode(FILE * file_input, FILE * file_output, stamp codes[], unsigned char *glob_buff, int *glob_counter){
	/* lit le source, et encode dans la destination */
	
	int i, ecrit = 0;
	unsigned char reste = 0, buff[2], car;
	
	rewind(file_input);
	
	while(fread(buff, sizeof(char), 1, file_input) > 0){
		
		car = buff[0];
		
		for (i = 0; i < codes[car].num; i++)
		{
			ecritUnBit(file_output, glob_buff, glob_counter, (codes[car].code[i] - 48));
			//printf("%d", codes[car].code[i] - 48);
			ecrit++;
		}
	}
	
	// pour les bits qui ont restes
	if (*(glob_counter) > 0){
		reste = 8 - *(glob_counter);
		for(int i=0;i<reste;i++)
		{
			ecritUnBit(file_output, glob_buff, glob_counter, 0);
			//printf("0");
			ecrit++;
		}
	}
	// sauvegarder dans le dernier octet nombre des zeros ajoutes
	fputc(reste, file_output);
	
	return ecrit/8 + 1;
}


	/**
		* Compte des codes.
		* Parcours du tableau des structures stamp, en comptant des codes generes precedament.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
	*/

int compteCodes(stamp codes[]){
	int occur = 0;
	for(int i=0;i<256;i++){
		if(codes[i].num != 0){occur++;}
	}
	return occur;
}

	/**
		* Compte la longeur moyenne des codes.
		* Parcours du tableau des structures stamp, en comptant la moyenne de la longeur des codes.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
	*/

float moyCodes(stamp codes[]){
	int occur = 0;
	float moy = 0.0;
	for(int i=0;i<256;i++){
		if(codes[i].num != 0){
			moy += codes[i].num;
			occur++;
		}
	}
	return moy/occur;
}


	/**
		* Lit la partie d'Entete dans le fichier compresse.
		* Parcours d'un fichier source et reconstruction de tableau des codes binaires de caracteres ASCII.
		* @param file_input un pointeur vers le fichier compresse.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
	*/

void lireHeader(FILE * file_input, stamp codes[]){

	int nombreCodes = (unsigned char)fgetc(file_input);
	int nombreBits, nombreCars, car;
	unsigned char buffer[32];
	
	printf("%d codes trouves.\n", nombreCodes + 1);

	for (int i = 0; i <= nombreCodes; i++){

		nombreCars = 1;
		car = fgetc(file_input);		// lit le caractere
		nombreBits = fgetc(file_input);		// lit nbre des bits utilises pour l'encoder	
		codes[car].num = nombreBits;		// et stocke dans le tableau des codes

		while (nombreCars * 8 < nombreBits){	// trouve le nombre des chars utilises
			nombreCars += 1; 
		}
		
		for (int j = 0; j < nombreCars; j++){	// lit ces chars dans buffer
			buffer[j] = fgetc(file_input);
		}
		
		for (int h = nombreCars -1; h>=0 ;h--){	//pour tous les chars lit, dans l'ordre inverse
			
			for(int k=0; k<8; k++){

				codes[car].code[(k+(nombreCars - h - 1)*8)] = (char) (buffer[h] % 2 )+ 48;
			//printf("%c[%d] => %c\n", car,(k+(nombreCars - h - 1)*8),(buffer[h] % 2 ) + 48);
				buffer[h] /= 2;
			}
		}
	}

	// inverser l'ordre des codes
	char b[256];
	int in = 0;
	for(int i=0;i<256;i++){ 
		if(codes[i].num != 0){
			for(int j=0; j<codes[i].num; j++){
				b[j] = codes[i].code[codes[i].num - j -1];
			}
			for(int j=0; j<codes[i].num; j++){
				codes[i].code[j] = b[j];
			}
		}
	}
}

	/**
		* Decode la partie binaire d'un fichier compresse.
		* Parcours du tableau des structures stamp, en comptant des codes generes precedament.
		* @param file_input un pointeur vers le fichier compresse.
		* @param file_output un pointeur vers le fichier de destination.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
		* @param total_num nombre des caracteres, pour savoir si on est deja a l'avant dernier octet de fichier source.
		* @param printit pour choisir entre affichage a la sortie standard, ou dans un fichier
	*/
int decode(FILE * file_input, FILE * file_output, stamp codes[], int total_num, int printit){
	
	unsigned char buffer[264], bf[2];
	
	int deb = 0, fin = 0, octets_ecrits = 0, code = 0;

	while(fread(bf, sizeof(char), 1, file_input) > 0){
	

		for(int b = 7; b >= 0; b--){
			buffer[fin + b] = (bf[0] % 2) + 48;
			bf[0] /= 2;
		}
		fin += 8;
	
		if(ftell(file_input) == total_num - 1){		// si on vient de traiter l'avant dernier
			fread(bf, sizeof(char), 1, file_input);
			fin -= bf[0];				// on saute des zeros a la fin
		}
		
	
		while(trouveCode(codes, buffer, &code, &deb, &fin)){
			//printf("%c, deb:%d, buffer[deb]:%c %d/%d\n", code, deb, buffer[deb], (int)ftell(file_input), total_num);
			if(printit){
				printf("%c", code); fflush(stdout);
			}else{
				fputc(code, file_output);
			}
			octets_ecrits++;
		}
	
		//printf("debug... deb: %d, fin: %d, code: %d, buffer: \n[", deb, fin, code, buffer[0]); fflush(stdout); for(int x = 0; x< fin; x++){ printf("%c", buffer[x]); } printf("]\n");
		
		// decalage
		if(deb > 0){
			for(int i = deb; i < fin; i++){
				buffer[i - deb] = buffer[i];
			}
			fin -= deb;
			deb = 0;
		}
	}
	return octets_ecrits;
}

	/**
		* Cherche un code dans le buffeur.
		* Renvoie 1 et stocke le code ds &code si un code a ete trouve, renvoie 0 sinon.
		* @param codes tableau des codes.
		* @param buffer le buffeur.
		* @param code pointeur pour sauvegarder le caractere trouve .
		* @param deb indice debut de tableau buffeur.
		* @param fin indice fin de tableau buffeur.
	*/
int trouveCode(stamp codes[], unsigned char buffer[], int *code, int *deb, int *fin){

	int trouve, nbr;
	
	for(int i = 0; i < 256; i++){ 
	
		if(codes[i].num != 0){
		
			trouve = 1;
			nbr = i;
			
			for(int j = 0; j < codes[i].num; j++){
			
				if((*(deb) + j) == *(fin) || (codes[i].code[j] != buffer[*(deb) + j])){
					trouve = 0;
					break;
				}
			}
			
			if(trouve){ break; }
		}
	}

	if(trouve){
		*deb += codes[nbr].num;
		*code = nbr;
		return 1;
	}else{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////// DEBUG MODE - FONCTIONS
	/**
		* Affiche un arbre.
		* Affiche un tableau des structures noeud, de 0 a max, sur la sortie standard.
		* @param arbre le tableau des structures noeud a afficher.
		* @param max indice maximale a afficher.
	*/
void printTArbre(noeud arbre[], int max){
	printf("\nAffichage de l'arbre:\n\n");
	for(int i=0;i<=max;i++){ 
		if(arbre[i].freq != 0.0){
			if(i<=126){ printf("%d) [%c] pere:%d, fg:%d, fd:%d, frequence:%f, code: %c\n", i, i, arbre[i].pere, arbre[i].fg, arbre[i].fd, arbre[i].freq, arbre[i].code); }
			else{ printf("%d) pere:%d, fg:%d, fd:%d, frequence:%f, code: %c\n", i,  arbre[i].pere, arbre[i].fg, arbre[i].fd, arbre[i].freq, arbre[i].code); }
		}
	}
}


	/**
		* Affiche un tableau des codes.
		* Affiche un tableau des structures stamp sur la sortie standard.
		* @param codes le tableau des structures stamp ou sont stockes des codes.
	*/
void printTCodes(stamp codes[]){
	printf("\nAffichage des codes:\n\n");
	for(int i=0;i<256;i++){ 
		if(codes[i].num != 0){
			if(i<=126){ printf("%d) [%c] => ", i, i); }
			else{ printf("%d)     => ", i); }
			for(int j=0; j < codes[i].num; j++){
				printf("%c", codes[i].code[j], codes[i].code[j]);
			}
			printf(", code sur %d bits\n", codes[i].num);
		}
	}
}
