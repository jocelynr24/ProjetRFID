/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier Arduino G�n�ral .ino
ROUTIN Jocelyn, GERARDI Marcelin
Derni�re modification : 09/06/2016

*/

// On inclut les fichiers essentiels au projet
#include "ProjetER_RFID.h"
#include "Fonctions.h"
#include "AccesPort.h"
#include "AccesADC.h"


// Variable permettant de stocker l'identifiant de la carte (premi�re valeur du tableau iTabMemoire, nombre de lignes de 0 � NB_CARTES-1)
int iIdentifiant = 0;

// Structure de la variable que l'on utilise pour m�moriser la donn�e stock�e (tableau de double dimensions de NB_CARTES lignes et de 11 valeurs pour 11 octets pour chaque ligne)
unsigned int iTabMemoire[NB_CARTES][11];

// Variables globales permettant de m�moriser le temps depuis lequel le programme est lanc� (�viter les attentes bloquantes)
unsigned long lTemps5sValid[NB_CARTES];

// Tableau qui permet de stocker la carte actuellement pos�e sur le lecteur (variable tampon)
unsigned int iTabLecture[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Variable temporaire qui sera utilis�e dans les boucles for
int iBcl;

// Type d'�tat du programme
typedef enum{
	ATTENTE,
	LECTURE,
	IDENTIFICATION,
	VALIDE,
	AJOUTER,
	SUPPRIMER
}TEtat;
TEtat Etat;
TEtat EtatOld;

void setup(){
	Serial.begin(9600); // Initialisation du port s�rie
	InitPortCarteFille(); // Initialisation de la carte fille
	Etat = ATTENTE;
	EtatOld = ATTENTE;
	InitCartes(); // Permet d'initialiser les cases m�moire en fonction du nombre maximal de cartes d�fini par l'utilisateur dans le #define (Fonctions.h)
}

void loop(){
	bool bCarteValide = 0; // Variable permettant de d�terminer si la carte est pr�sente ou non dans la m�moire
	bool bMemoireDispo = 0; // Variable permettant de d�terminer si de la m�moire est disponible
	bool bBP1 = BP1(); // Variable permettant de d�terminer l'appui sur le BP1
	bool bBP2 = BP2(); // Variable permettant de d�terminer l'appui sur le BP2

	// �volution du graphe d'�tats
	switch (Etat){
	case ATTENTE: // Si on est dans l'�tat attente d'une carte
		Serial.println("Attente d'une carte...");
		if (Serial.available() == 11){
			Etat = LECTURE; // Et si 11 octets sont pr�sent dans le buffer, on passe dans l'�tat lecture
		}
		else if (Serial.available() > 11){
			SerialClear(); // Et si une personne appuie trop longtemps sur le bouton d'ajout de carte et qu'on a atteint la limite de cartes, on vide le buffer pour �viter de le bloquer (s�curit�)
		}
		break;
	case LECTURE: // Si on est dans l'�tat lecture
		Etat = IDENTIFICATION; // On passe directement � l'�tat identification au prochain passage du loop()
		break;
	case IDENTIFICATION: // On identifie la carte et la fonction qui lui est demand�e (lecture, ajouter ou supprimer)
		for (iBcl = 0; iBcl < NB_CARTES; iBcl++){ // On cherche � savoir si la carte est pr�sente dans la m�moire
			if ((iTabLecture[0] == iTabMemoire[iBcl][0]) && (iTabLecture[1] == iTabMemoire[iBcl][1]) 
				&& (iTabLecture[2] == iTabMemoire[iBcl][2]) && (iTabLecture[3] == iTabMemoire[iBcl][3])
				&& (iTabLecture[4] == iTabMemoire[iBcl][4]) && (iTabLecture[5] == iTabMemoire[iBcl][5])
				&& (iTabLecture[6] == iTabMemoire[iBcl][6]) && (iTabLecture[7] == iTabMemoire[iBcl][7])
				&& (iTabLecture[8] == iTabMemoire[iBcl][8]) && (iTabLecture[9] == iTabMemoire[iBcl][9])
				&& (iTabLecture[10] == iTabMemoire[iBcl][10])){ // � l'aide de tous les octets
				Serial.println("La carte est existante");
				iIdentifiant = iBcl; // Si on l'a trouv�, son identifiant sera �gal � la valeur de iBcl � cet instant
				if (bBP2){ // Appui sur BP2
					Serial.println("Le BP2 est appuye, on supprime cette carte de la memoire");
					Etat = SUPPRIMER; // Permet de supprimer la carte de la m�moire
				}
				else if ((lTemps5sValid[iBcl] + 5000) <= millis()) { // Si la carte est valide et qu'elle a �t� activ�e il y a plus de 5 secondes
					Serial.println("La carte est valide");
					Etat = VALIDE; // Si la carte est dans la m�moire et qu'elle n'a pas �t� activ�e lors des 5 derni�res secondes, elle est valide
				}
				else{
					Serial.println("La carte a deja ete activee durant les 5 dernieres secondes");
					Etat = ATTENTE; // Si la carte est valide mais qu'elle a �t� activ�e lors des 5 derni�res secondes
				}
				bCarteValide = 1; // On passe un bool�en � 1 afin d'indiquer que la carte �tait bien pr�sente dans la m�moire
			}
		}
		if (bCarteValide == 0){ // Si la carte n'�tait pas pr�sente dans la m�moire 
			if (bBP1){ // Appui sur BP1
				for (iBcl = 0; iBcl < NB_CARTES; iBcl++){ // On cherche � d�terminer le premier identifiant disponible pour stocker la valeur de la carte en m�moire
					if (iTabMemoire[iBcl][0] == 0){ // Si une ligne de la m�moire est libre (le premier octet est forc�ment �gal � 1, donc s'il vaut 0, c'est qu'il n'y a pas de carte)
						iIdentifiant = iBcl; // On stocke la valeur de cet identifiant dans la variable iIdentifiant
						bMemoireDispo = 1; // Et on indique que la m�moire est disponible pour stocker une carte
					}
					else { // Si le nombre maximal de cartes a �t� atteint
						Serial.println("Le nombre maximal de cartes a ete atteint");
						Etat = ATTENTE; // On abandonne la lecture et on attend de nouveau une carte
					}
				}
				if (bMemoireDispo == 1){ // S'il reste de la m�moire libre et que l'on ajoute une carte
					Serial.println("Le BP1 est appuye, on ajoute cette carte a la memoire");
					Etat = AJOUTER; // On passe dans l'�tat ajouter
				}
			}
			else{
				Serial.println("Cette carte n'est pas valide");
				Etat = ATTENTE; // Si la carte n'est pas dans la m�moire, on retourne en attente de lecture d'une carte
			}
			
		}
		SerialClear(); // On vide le buffer
		break;
	case VALIDE: // On retourne dans l'�tat d'attente des donn�es
		Etat = ATTENTE;
		break;
	case AJOUTER: // On retourne dans l'�tat d'attente des donn�es
		Etat = ATTENTE;
		break;
	case SUPPRIMER: // On retourne dans l'�tat d'attente des donn�es
		Etat = ATTENTE;
		break;
	}

	// Sorties
	if (Etat != EtatOld){
		switch (Etat){
		case LECTURE: // On stocke dans la variable tampon les 11 octets lus de la carte
			for (iBcl = 0; iBcl < 11; iBcl++){
				iTabLecture[iBcl] = Serial.read();
			}
			break;
		case VALIDE: // Gestion de la carte valide
			CarteValide(); // On envoie un niveau logique haut en sortie de la carte (sur la base et la DEL)
			lTemps5sValid[iIdentifiant] = millis(); // Permet de d�terminer l'intervalle de 5 secondes sans bloquer le programme
			SerialClear(); // On vide le buffer
			break;
		case AJOUTER: // Ajout d'une carte � la m�moire (avec l'identifiant trouv� pr�c�demment)
			for (iBcl = 0; iBcl < 11; iBcl++){
				iTabMemoire[iIdentifiant][iBcl] = iTabLecture[iBcl];
			}
			break;
		case SUPPRIMER: // Supression d'une carte de la m�moire (avec l'identifiant trouv� pr�c�demment)
			for (iBcl = 0; iBcl < 11; iBcl++){
				iTabMemoire[iIdentifiant][iBcl] = 0;
			}
			break;
		}
	}
}