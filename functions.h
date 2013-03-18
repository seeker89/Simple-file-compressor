	/** @file functions.h
		* Header pour functions.c
	*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////// DATATYPES

	/**
		* Structure d'un noeud d'arbre.
	*/
typedef struct Noeud {
	int pere, /**< indice du pere*/ fg, /**< indice du fils gauche*/ fd; /**< indice du fils droite */
	double freq; /**< frequence d'un caractere dans le fichier source */
	char code; /**< un bit, 0 ou 1 */
} noeud;

	/**
		* Structure qui stocke un code.
	*/
typedef struct Code {
	char code[256]; /**< tableau des chars pour stocker le code. 256 chars maximum. */
	char num; /**<nombre des bits outilises pour stocker ce code binaire */
} stamp;

//////////////////////////////////////////////////////////////////////////////////////////////////////////// FONCTIONS

void initArbre(noeud arbre[], int total);
/* Initiation d'un arbre */

void initCode(stamp codes[], int total);
/* Initiation d'un tableau des codes */

void occur2Freq(noeud arbre[], int total);
/* Divise nombres d'occurences dans un tableau des structures noeuds par nombre total des characteres - pour avoir des freqs */

int minimum(noeud arbre[], int max);
/* Cherche un noeud dans un tableau des structures noeuds, d'une frequence minimale, renovoie son indice */

int consArbre(noeud arbre[], int indice_nn);
/* Construit une arbre - des liaisons pere - fils et retourne l'indice du noeud racine dans le tableau */

void genereCodes(noeud arbre[], stamp codes[], int indice, int profondeur);
/* Genere codes pour chaque feuille d'arbres */

void sauvegardeCode(noeud arbre[], stamp codes[], int indice, int profondeur);
/* Stocke un code, et un nombre des bits utilise, dans un tableau des codes */

void ecritUnBit(FILE *file_output, unsigned char *glob_buff, int *glob_counter, int x);
/* Accumule et puis ecrit des bits dans un fichier */

int encode(FILE * file_input, FILE * file_output, stamp codes[], unsigned char *glob_buff, int *glob_counter);
/* Encode le contenu deu fichier file_input et stocke le code dans file_output, retourne enombre des octets ecrits */

int sauvegardeHeader(FILE * file_output, stamp codes[], unsigned char *glob_buff, int *glob_counter);
/* Sauvegarde dans le fichier le HEADER - la partie avec des codes, renovoie lenombre des octets ecrits */

int compteCodes(stamp codes[]);
/* Calcule le nombre des codes generes */

float moyCodes(stamp codes[]);
/* Compte la longeur moyenne des codes */

void lireHeader(FILE * file_input, stamp codes[]);
/* Lit le header d'un fichier code et stocke des codes trouves dans le tableau des codes */

int trouveCode(stamp codes[], unsigned char buffer[], int *code, int *deb, int *fin);
/* Cherche un code dans le tableau des codes, renvoie son indice */

int decode(FILE * file_input, FILE * file_output, stamp codes[], int num, int printit);
/* Decode le message encode, en utilisant le tableau de codes, renvoie le nombre des octets ecrits. */


void printTArbre(noeud arbre[], int max);
/* Affiche le contenu d'un tableau des structures noeuds d'un facon comprehensible - debug */

void printTCodes(stamp codes[]);
/* Affiche le contenu d'un tableau des codes d'un facon comprehensible - debug */
