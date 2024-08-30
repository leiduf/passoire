#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream> //ajouté pour cout 
#include <cwchar> //ajouté pour l'unicode

using namespace std;

int main(int argc, char* argv[])
{
	/* Vulnérabilité #1 : Nombre d'arguments non vérifiés
	* Exploit: Faire planter le programme
	* Correction : Vérifier si le nombre d'arguments est valide
	*/
	if (argc < 5)
	{
		cout << "Nombre d'arguments insuffisant, besoin de 5 arguments. ./passoire <pans de cloture (int)> < montant a payer (float)> <nom du fichier qui sera cree.exe (string)> <contenu du fichier.exe> <commande système>";
		return -1;
	}

	/*Vulnérabilité #2
	Exploit: entrer du texte au lieu de chiffre
	Correction: vérifier si stoi ou stof fonctionne
	*/

	/*int pansDeCloture{ stoi(argv[1]) };
	float montantAPayer{ stof(argv[2]) };*/

	int pansDeCloture{ 0 };
	float montantAPayer{ 0.0f };

	try {
		pansDeCloture = stoi(argv[1]);
		montantAPayer = stof(argv[2]);
		/*
		Vulnérabilité #3
		Exploit: faire planter le programme
		Correction: empêcher des résultats illogiques ex: crash si 0 pans de cloture*/
		if (pansDeCloture < 0) throw -1;
		if (montantAPayer < 0) throw -1;
	}
	catch (const exception&) {
		cout << "Les deux premiers arguments sont invalides." << endl;
		return -1;
	}

	/*float coutParPan{ montantAPayer / pansDeCloture };*/
	float coutParPan{ 0 };
	if (pansDeCloture != 0) {
		 coutParPan = montantAPayer / pansDeCloture;
	}


	// Poteaux nécessaires pour faire une clôture en ligne droite
	int poteaux{ pansDeCloture };
	montantAPayer += poteaux * 5;


	/* Vulnérabilité #4:
	Exploit: planter le programme
	Correction: vérifier les caracteres invalides en nom de fichier windows*/
	const char* invalidChars = "\\/:*?\"<>|";
	for (size_t i = 0; i < strlen(argv[3]); ++i) {
		if (strchr(invalidChars, argv[3][i]) != nullptr) {
			cout << "Nom de fichier avec caracterse invalidses." << endl;
			return -1;
		}
	}

	/* Vulnérabilité #5:
* Exploit: Crash si le nom du fichier est trop long
* Correction: utiliser un tableau plus grand et vérifier
*/


	/*char nomFichier[10];
	strcpy(nomFichier, argv[3]);
	strcat(nomFichier, ".exe");*/
	if (strlen(argv[3]) > 256)
	{
		cout << "Nombre de caractères trop longs" << endl;
		return -1;
	}
	char nomFichier[256];
	strcpy(nomFichier, argv[3]);
	strcat(nomFichier, ".exe");


	ofstream fichier{ nomFichier };
	fichier << argv[4];
	//fermer le fichier
	fichier.close();

	/* Vulnérabilité #6:
	* Exploit:printf interpètre le msg entré
	* Correction: ajouter "%s\n"
	*/
	/*const char* msg{ argv[4] };
	printf(msg, pansDeCloture, coutParPan);*/
	const char* msg{ argv[4] };
	printf("%s\nPans de cloture: %d\nCoût par pan: %.2f\n", msg, pansDeCloture, coutParPan);

	/* Vulnérabilité #7:
	* Exploit: Exécution de n'importe quelle commande entrée par l'utilisateur sans vérification
	Correction: Vérifier certains commandes ou complétement enlever ceci
	*/
	const char* commande{ argv[5] };
	system(commande);
	//Autre: Voir si d'autres vulnérabilités pourraient se présenter sous d'autre OS
	return 0;
}