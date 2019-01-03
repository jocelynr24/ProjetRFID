/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier Arduino Général .ino
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 09/06/2016

*/

// On inclut les fichiers essentiels au projet
#include "ProjetER_RFID.h"
#include "Fonctions.h"
#include "AccesPort.h"
#include "AccesADC.h"


// Variable permettant de stocker l'identifiant de la carte (première valeur du tableau iTabMemoire, nombre de lignes de 0 à NB_CARTES-1)
int iIdentifiant = 0;

// Structure de la variable que l'on utilise pour mémoriser la donnée stockée (tableau de double dimensions de NB_CARTES lignes et de 11 valeurs pour 11 octets pour chaque ligne)
unsigned int iTabMemoire[NB_CARTES][11];

// Variables globales permettant de mémoriser le temps depuis lequel le programme est lancé (éviter les attentes bloquantes)
unsigned long lTemps5sValid[NB_CARTES];

// Tableau qui permet de stocker la carte actuellement posée sur le lecteur (variable tampon)
unsigned int iTabLecture[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Variable temporaire qui sera utilisée dans les boucles for
int iBcl;

// Type d'état du programme
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
	Serial.begin(9600); // Initialisation du port série
	InitPortCarteFille(); // Initialisation de la carte fille
	Etat = ATTENTE;
	EtatOld = ATTENTE;
	InitCartes(); // Permet d'initialiser les cases mémoire en fonction du nombre maximal de cartes défini par l'utilisateur dans le #define (Fonctions.h)
}

void loop(){
	bool bCarteValide = 0; // Variable permettant de déterminer si la carte est présente ou non dans la mémoire
	bool bMemoireDispo = 0; // Variable permettant de déterminer si de la mémoire est disponible
	bool bBP1 = BP1(); // Variable permettant de déterminer l'appui sur le BP1
	bool bBP2 = BP2(); // Variable permettant de déterminer l'appui sur le BP2

	// Évolution du graphe d'états
	switch (Etat){
	case ATTENTE: // Si on est dans l'état attente d'une carte
		Serial.println("Attente d'une carte...");
		if (Serial.available() == 11){
			Etat = LECTURE; // Et si 11 octets sont présent dans le buffer, on passe dans l'état lecture
		}
		else if (Serial.available() > 11){
			SerialClear(); // Et si une personne appuie trop longtemps sur le bouton d'ajout de carte et qu'on a atteint la limite de cartes, on vide le buffer pour éviter de le bloquer (sécurité)
		}
		break;
	case LECTURE: // Si on est dans l'état lecture
		Etat = IDENTIFICATION; // On passe directement à l'état identification au prochain passage du loop()
		break;
	case IDENTIFICATION: // On identifie la carte et la fonction qui lui est demandée (lecture, ajouter ou supprimer)
		for (iBcl = 0; iBcl < NB_CARTES; iBcl++){ // On cherche à savoir si la carte est présente dans la mémoire
			if ((iTabLecture[0] == iTabMemoire[iBcl][0]) && (iTabLecture[1] == iTabMemoire[iBcl][1]) 
				&& (iTabLecture[2] == iTabMemoire[iBcl][2]) && (iTabLecture[3] == iTabMemoire[iBcl][3])
				&& (iTabLecture[4] == iTabMemoire[iBcl][4]) && (iTabLecture[5] == iTabMemoire[iBcl][5])
				&& (iTabLecture[6] == iTabMemoire[iBcl][6]) && (iTabLecture[7] == iTabMemoire[iBcl][7])
				&& (iTabLecture[8] == iTabMemoire[iBcl][8]) && (iTabLecture[9] == iTabMemoire[iBcl][9])
				&& (iTabLecture[10] == iTabMemoire[iBcl][10])){ // à l'aide de tous les octets
				Serial.println("La carte est existante");
				iIdentifiant = iBcl; // Si on l'a trouvé, son identifiant sera égal à la valeur de iBcl à cet instant
				if (bBP2){ // Appui sur BP2
					Serial.println("Le BP2 est appuye, on supprime cette carte de la memoire");
					Etat = SUPPRIMER; // Permet de supprimer la carte de la mémoire
				}
				else if ((lTemps5sValid[iBcl] + 5000) <= millis()) { // Si la carte est valide et qu'elle a été activée il y a plus de 5 secondes
					Serial.println("La carte est valide");
					Etat = VALIDE; // Si la carte est dans la mémoire et qu'elle n'a pas été activée lors des 5 dernières secondes, elle est valide
				}
				else{
					Serial.println("La carte a deja ete activee durant les 5 dernieres secondes");
					Etat = ATTENTE; // Si la carte est valide mais qu'elle a été activée lors des 5 dernières secondes
				}
				bCarteValide = 1; // On passe un booléen à 1 afin d'indiquer que la carte était bien présente dans la mémoire
			}
		}
		if (bCarteValide == 0){ // Si la carte n'était pas présente dans la mémoire 
			if (bBP1){ // Appui sur BP1
				for (iBcl = 0; iBcl < NB_CARTES; iBcl++){ // On cherche à déterminer le premier identifiant disponible pour stocker la valeur de la carte en mémoire
					if (iTabMemoire[iBcl][0] == 0){ // Si une ligne de la mémoire est libre (le premier octet est forcément égal à 1, donc s'il vaut 0, c'est qu'il n'y a pas de carte)
						iIdentifiant = iBcl; // On stocke la valeur de cet identifiant dans la variable iIdentifiant
						bMemoireDispo = 1; // Et on indique que la mémoire est disponible pour stocker une carte
					}
					else { // Si le nombre maximal de cartes a été atteint
						Serial.println("Le nombre maximal de cartes a ete atteint");
						Etat = ATTENTE; // On abandonne la lecture et on attend de nouveau une carte
					}
				}
				if (bMemoireDispo == 1){ // S'il reste de la mémoire libre et que l'on ajoute une carte
					Serial.println("Le BP1 est appuye, on ajoute cette carte a la memoire");
					Etat = AJOUTER; // On passe dans l'état ajouter
				}
			}
			else{
				Serial.println("Cette carte n'est pas valide");
				Etat = ATTENTE; // Si la carte n'est pas dans la mémoire, on retourne en attente de lecture d'une carte
			}
			
		}
		SerialClear(); // On vide le buffer
		break;
	case VALIDE: // On retourne dans l'état d'attente des données
		Etat = ATTENTE;
		break;
	case AJOUTER: // On retourne dans l'état d'attente des données
		Etat = ATTENTE;
		break;
	case SUPPRIMER: // On retourne dans l'état d'attente des données
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
			lTemps5sValid[iIdentifiant] = millis(); // Permet de déterminer l'intervalle de 5 secondes sans bloquer le programme
			SerialClear(); // On vide le buffer
			break;
		case AJOUTER: // Ajout d'une carte à la mémoire (avec l'identifiant trouvé précédemment)
			for (iBcl = 0; iBcl < 11; iBcl++){
				iTabMemoire[iIdentifiant][iBcl] = iTabLecture[iBcl];
			}
			break;
		case SUPPRIMER: // Supression d'une carte de la mémoire (avec l'identifiant trouvé précédemment)
			for (iBcl = 0; iBcl < 11; iBcl++){
				iTabMemoire[iIdentifiant][iBcl] = 0;
			}
			break;
		}
	}
}