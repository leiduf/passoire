#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream> //ajout� pour cout 
#include <cwchar> //ajout� pour l'unicode

using namespace std;

int main(int argc, char* argv[])
{
	/* Vuln�rabilit� #1 : Nombre d'arguments non v�rifi�s
	* Exploit: Faire planter le programme
	* Correction : V�rifier si le nombre d'arguments est valide
	*/
	if (argc < 5)
	{
		cout << "Nombre d'arguments insuffisant, besoin de 5 arguments. ./passoire <pans de cloture (int)> < montant a payer (float)> <nom du fichier qui sera cree.exe (string)> <contenu du fichier.exe> <commande syst�me>";
		return -1;
	}

	/*Vuln�rabilit� #2
	Exploit: entrer du texte au lieu de chiffre
	Correction: v�rifier si stoi ou stof fonctionne
	*/

	/*int pansDeCloture{ stoi(argv[1]) };
	float montantAPayer{ stof(argv[2]) };*/

	int pansDeCloture{ 0 };
	float montantAPayer{ 0.0f };

	try {
		pansDeCloture = stoi(argv[1]);
		montantAPayer = stof(argv[2]);
		/*
		Vuln�rabilit� #3
		Exploit: faire planter le programme
		Correction: emp�cher des r�sultats illogiques ex: crash si 0 pans de cloture*/
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


	// Poteaux n�cessaires pour faire une cl�ture en ligne droite
	int poteaux{ pansDeCloture };
	montantAPayer += poteaux * 5;


	/* Vuln�rabilit� #4:
	Exploit: planter le programme
	Correction: v�rifier les caracteres invalides en nom de fichier windows*/
	const char* invalidChars = "\\/:*?\"<>|";
	for (size_t i = 0; i < strlen(argv[3]); ++i) {
		if (strchr(invalidChars, argv[3][i]) != nullptr) {
			cout << "Nom de fichier avec caracterse invalidses." << endl;
			return -1;
		}
	}

	/* Vuln�rabilit� #5:
* Exploit: Crash si le nom du fichier est trop long
* Correction: utiliser un tableau plus grand et v�rifier
*/


	/*char nomFichier[10];
	strcpy(nomFichier, argv[3]);
	strcat(nomFichier, ".exe");*/
	if (strlen(argv[3]) > 256)
	{
		cout << "Nombre de caract�res trop longs" << endl;
		return -1;
	}
	char nomFichier[256];
	strcpy(nomFichier, argv[3]);
	strcat(nomFichier, ".exe");


	ofstream fichier{ nomFichier };
	fichier << argv[4];
	//fermer le fichier
	fichier.close();

	/* Vuln�rabilit� #6:
	* Exploit:printf interp�tre le msg entr�
	* Correction: ajouter "%s\n"
	*/
	/*const char* msg{ argv[4] };
	printf(msg, pansDeCloture, coutParPan);*/
	const char* msg{ argv[4] };
	printf("%s\nPans de cloture: %d\nCo�t par pan: %.2f\n", msg, pansDeCloture, coutParPan);

	/* Vuln�rabilit� #7:
	* Exploit: Ex�cution de n'importe quelle commande entr�e par l'utilisateur sans v�rification
	Correction: V�rifier certains commandes ou compl�tement enlever ceci
	*/
	const char* commande{ argv[5] };
	system(commande);
	//Autre: Voir si d'autres vuln�rabilit�s pourraient se pr�senter sous d'autre OS
	return 0;
}