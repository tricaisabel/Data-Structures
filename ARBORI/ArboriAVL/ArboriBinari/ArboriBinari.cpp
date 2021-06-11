#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

struct student
{
	int cod;
	char* nume;
	float medie;
};

struct nodarb
{
	student inf;
	nodarb* left, * right;
};

struct nodlista
{
	student inf;
	nodlista* next, * prev;
};

struct nodstiva
{
	student inf;
	nodstiva* next;
};

nodarb* creare(student s, nodarb* st, nodarb* dr)
{
	nodarb* nou = new nodarb;
	nou->inf.cod = s.cod;
	nou->inf.nume = new char[strlen(s.nume) + 1];
	strcpy_s(nou->inf.nume, strlen(s.nume) + 1, s.nume);
	nou->inf.medie = s.medie;
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodarb* inserare(nodarb* rad, student s)
{
	if (rad != NULL)
	{
		if (s.cod < rad->inf.cod)
		{
			rad->left = inserare(rad->left, s);
			return rad;
		}
		else
			if (s.cod > rad->inf.cod)
			{
				rad->right = inserare(rad->right, s);
				return rad;
			}
			else
				return rad;
	}
	else
		rad=creare(s, NULL, NULL);
	return rad;
}


void preordine(nodarb* rad)
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Nume=%s, Medie=%5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodarb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nCod=%d, Nume=%s, Medie=%5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
		inordine(rad->right);
	}
}

void postordine(nodarb* rad)
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\nCod=%d, Nume=%s, Medie=%5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
	}
}

void dezalocare(nodarb* rad)
{
	if (rad != NULL)
	{
		nodarb* st = rad->left;
		nodarb* dr = rad->right;
		free(rad->inf.nume);
		free(rad);
		dezalocare(st);
		dezalocare(dr);
	}
}

nodarb* cautare(nodarb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (cheie == rad->inf.cod)
			return rad;
		else
			if (cheie < rad->inf.cod)
				return cautare(rad->left, cheie);
			else
				return cautare(rad->right, cheie);
	}
	else
		return NULL;
}

int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int nrNiveluri(nodarb* rad)
{
	if (rad != NULL)
	{
		//return 1 + maxim(nrNiveluri(rad->left), nrNiveluri(rad->right));

		int st = nrNiveluri(rad->left);
		int dr = nrNiveluri(rad->right);
		return 1 + maxim(st, dr);
	}
	else
		return 0;
}

void conversieNoduriFrunzaVector(nodarb* rad, student* vect, int* nr)
{
	if (rad != NULL)
	{
		if (rad->left == NULL && rad->right == NULL)
		{
			vect[*nr].cod = rad->inf.cod;
			vect[*nr].nume = new char[strlen(rad->inf.nume) + 1];
			strcpy_s(vect[*nr].nume, strlen(rad->inf.nume) + 1, rad->inf.nume);
			vect[*nr].medie = rad->inf.medie;
			(*nr)++;
		}
		conversieNoduriFrunzaVector(rad->left, vect, nr);
		conversieNoduriFrunzaVector(rad->right, vect, nr);
	}
}

void conversieArboreVector(nodarb* rad, student* vect, int* nr)
{
	if (rad != NULL)
	{
		vect[*nr].cod = rad->inf.cod;
		vect[*nr].nume = new char[strlen(rad->inf.nume) + 1];
		strcpy_s(vect[*nr].nume, strlen(rad->inf.nume) + 1, rad->inf.nume);
		vect[*nr].medie = rad->inf.medie;
		(*nr)++;

		nodarb* st = rad->left;
		nodarb* dr = rad->right;
		free(rad->inf.nume);
		free(rad);
		conversieArboreVector(st, vect, nr);
		conversieArboreVector(dr, vect, nr);
	}
}

nodarb* stergeRad(nodarb* rad)
{
	nodarb* aux = rad;
	if (aux->left != NULL)
	{
		rad = aux->left;
		if (aux->right != NULL)
		{
			nodarb* temp = aux->left;
			while (temp->right)
				temp = temp->right;
			temp->right = aux->right;
		}
	}
	else
		if (aux->right != NULL)
			rad = aux->right;
		else
			rad = NULL;
	free(aux->inf.nume);
	free(aux);
	return rad;
}

nodarb* stergeNod(nodarb* rad, int cheie)
{
	if (rad == NULL)
		return NULL;
	else
		if (rad->inf.cod == cheie)
		{
			rad = stergeRad(rad);
			return rad;
		}
		else
			if (cheie < rad->inf.cod)
			{
				rad->left = stergeNod(rad->left, cheie);
				return rad;
			}
			else
			{
				rad->right = stergeNod(rad->right, cheie);
				return rad;
			}
}

nodlista* inserare(nodlista* cap, student s)
{
	nodlista* nou = (nodlista*)malloc(sizeof(nodlista));
	nou->inf.cod = s.cod;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy_s(nou->inf.nume, strlen(s.nume)+1,s.nume);
	nou->inf.medie = s.medie;
	nou->next = NULL;
	nou->prev = NULL;
	if (cap == NULL)
		cap = nou;
	else
	{
		nodlista* temp = cap;
		while (temp->next) temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
	}
	return cap;
}

void conversie_arbore_lista_simpla(nodarb* rad, nodlista** cap)
{
	if (rad != NULL)
	{
		*cap = inserare(*cap, rad->inf);
		conversie_arbore_lista_simpla(rad->left, cap);
		conversie_arbore_lista_simpla(rad->right, cap);
	}
}

void afisare_lista(nodlista* cap)
{
	nodlista* aux = cap;
	while (aux != NULL)
	{
		printf("\nStudentul %s are codul %d si media %5.2f", aux->inf.nume, aux->inf.cod, aux->inf.medie);
		aux = aux->next;
	}
}

void conversie_arbore_vector(nodarb* rad, student* vect, int* nr)
{
	if (rad != NULL)
	{
		vect[*nr] = rad->inf;
		(*nr)++;
		conversie_arbore_vector(rad->left, vect, nr);
		conversie_arbore_vector(rad->right, vect, nr);
	}
}

void inserare_stiva(nodstiva** varf, student s)
{
	nodstiva* nou = (nodstiva*)malloc(sizeof(nodstiva));
	nou->inf.cod = s.cod;
	nou->inf.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy_s(nou->inf.nume, strlen(s.nume)+1,s.nume);
	nou->inf.medie = s.medie;
	if (*varf == NULL)
	{
		nou->next = NULL;
		*varf = nou;
	}
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

void conversie_arbore_stiva(nodarb* rad, nodstiva** varf)
{
	if (rad != NULL)
	{
		inserare_stiva(varf, rad->inf);
		conversie_arbore_stiva(rad->left, varf);
		conversie_arbore_stiva(rad->right, varf);
	}
}

void afisare_stiva(nodstiva* varf)
{
	nodstiva* aux = varf;
	while (aux != NULL)
	{
		printf("\nStudentul %s are codul %d si media %5.2f", aux->inf.nume, aux->inf.cod, aux->inf.medie);
		aux = aux->next;
	}
}

void main()
{
	int n;
	ifstream fin("arboreBinar.txt");
	fin >> n;
	nodarb* rad = NULL;
	student s;
	char buffer[20];
	for (int i = 0; i < n; i++)
	{
		fin >> s.cod;
		fin >> buffer;
		s.nume = new char[strlen(buffer) + 1];
		strcpy_s(s.nume, strlen(buffer) + 1, buffer);
		fin >> s.medie;
		rad = inserare(rad,s);
	}
	cout << "Afisare in inordine";
	inordine(rad);
	printf("\n--------------------\n");

	cout << "Afisare in preordine";
	preordine(rad);
	printf("\n--------------------\n");

	cout << "Afisare in postordine";
	postordine(rad);
	printf("\n--------------------\n");

	nodarb* nodCautat = cautare(rad, 6);
	if (nodCautat != NULL)
		printf("\nStudentul cautat se numeste %s", nodCautat->inf.nume);
	else
		printf("\nNu exista!");

	printf("\nInaltime este %d: ", nrNiveluri(rad));
	printf("\nInaltime subarbore stang este %d: ", nrNiveluri(rad->left));
	printf("\nInaltime subarbore drept este %d: ", nrNiveluri(rad->right));

	int gradEchilibru = nrNiveluri(rad->right) - nrNiveluri(rad->left);
	if (gradEchilibru <= 1 && gradEchilibru >= -1) {
		cout << "\nArbore echilibrat, grad echilibru: " << gradEchilibru;
	}
	else {
		cout << "\nArborele nu este echilibrat, grad echilibru: " << gradEchilibru;;
	}

	//printf("\n----Vector--------------------\n");

	student* vect = (student*)malloc(n * sizeof(student));
	int nr = 0;
	//conversieArboreVector(rad, vect, &nr);
	conversieNoduriFrunzaVector(rad, vect, &nr);
	for (int i = 0; i < nr; i++)
		printf("\nCod=%d, Nume=%s, Medie=%5.2f", vect[i].cod, vect[i].nume, vect[i].medie);

	for (int i = 0; i < nr; i++)
		free(vect[i].nume);
	free(vect);

	dezalocare(rad);
}