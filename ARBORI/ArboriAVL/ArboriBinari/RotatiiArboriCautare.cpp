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
	int BF;
	student inf;
	nodarb* left, * right;
};




nodarb* creare(student s, nodarb* st, nodarb* dr)
{
	//declarare nod nou
	nodarb* nou = new nodarb;
	//copiere informatii in nod
	nou->inf.cod = s.cod;
	nou->inf.nume = new char[strlen(s.nume) + 1];
	strcpy_s(nou->inf.nume, strlen(s.nume) + 1, s.nume);
	nou->inf.medie = s.medie;
	//copiere copil stanga si dreapta
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodarb* inserare(nodarb* rad, student s)
{
	if (rad != NULL)//daca exista o radacina
	{
		if (s.cod < rad->inf.cod)//daca codul nodului de introdus este mai mic ca cel al radacinii
		{
			rad->left = inserare(rad->left, s); //nodul nou devine copilul stanga
			return rad;
		}
		else
			if (s.cod > rad->inf.cod)//daca codul este mai mare
			{
				rad->right = inserare(rad->right, s);//nodul nou devine copilul dreapta al radacinii
				return rad;
			}
			else//daca nodul este egal cu radacina, nu se insereaza
				return rad;
	}
	else//daca nu exista inca o radacina
		//creem un nod fara fii
		rad = creare(s, NULL, NULL);
	return rad;
}


void preordine(nodarb* rad)//radacina-stanga-dreapta
{
	if (rad != NULL)
	{
		printf("\nCod=%d, Nume=%s, Medie=%5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodarb* rad)//stanga-radacina-dreapta
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nCod=%d, Nume=%s, Medie=%5.2f", rad->inf.cod, rad->inf.nume, rad->inf.medie);
		inordine(rad->right);
	}
}

void postordine(nodarb* rad)//stanga-dreapta-radacina
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

		int st = nrNiveluri(rad->left);//calcularea nr de niveluri din subarborele stang
		int dr = nrNiveluri(rad->right);//calcularea numarului de niveluri din subarborele drept
		return 1 + maxim(st, dr);//nivelul actual+nr maxim de noduri din subarbori
	}
	else
		return 0;
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

void calculBF(nodarb* rad)
{
	if (rad != NULL)
	{
		rad->BF = nrNiveluri(rad->right) - nrNiveluri(rad->left);
		calculBF(rad->left);
		calculBF(rad->right);
	}
}

nodarb* rotatie_dreapta(nodarb* rad)
{
	printf("\nRotatie dreapta\n");
	nodarb* nod1 = rad->left;
	rad->left = nod1->right;
	nod1->right = rad;
	rad = nod1;
	return rad;
}

nodarb* rotatie_stanga(nodarb* rad)
{
	printf("\nRotatie stanga\n");
	nodarb* nod1 = rad->right;
	rad->right = nod1->left;
	nod1->left = rad;
	rad = nod1;
	return rad;
}

nodarb* rotatie_dreapta_stanga(nodarb* rad)
{
	printf("\nRotatie dreapta-stanga\n");
	nodarb* nod1 = rad->right;
	nodarb* nod2 = nod1->left;
	nod1->left = nod2->right;
	nod2->right = nod1;
	rad->right = nod2->left;
	nod2->left = rad;
	rad = nod2;
	return rad;
}

nodarb* rotatie_stanga_dreapta(nodarb* rad)
{
	printf("\nRotatie stanga-dreapta\n");
	nodarb* nod1 = rad->left;
	nodarb* nod2 = nod1->right;
	nod1->right = nod2->left;
	nod2->left = nod1;
	rad->left = nod2->right;
	nod2->right = rad;
	rad = nod2;
	return rad;
}

nodarb* reechilibrare(nodarb* rad)
{
	calculBF(rad);
	if (rad->BF <= -2 && rad->left->BF <= -1)
	{
		rad = rotatie_dreapta(rad);
		calculBF(rad);
	}
	else
		if (rad->BF >= 2 && rad->right->BF >= 1)
		{
			rad = rotatie_stanga(rad);
			calculBF(rad);
		}
		else
			if (rad->BF >= 2 && rad->right->BF <= -1)
			{
				rad = rotatie_dreapta_stanga(rad);
				calculBF(rad);
			}
			else
				if (rad->BF <= -2 && rad->left->BF >= 1)
				{
					rad = rotatie_stanga_dreapta(rad);
					calculBF(rad);
				}
	return rad;
}

void afisareNivel(nodarb* rad, int level)
{
	if (rad == NULL)
		return;
	if (level == 1)
		printf("\nCod=%d, Nume=%s, Medie=%5.2f, BF=%d", rad->inf.cod, rad->inf.nume, rad->inf.medie, rad->BF);
	else if (level > 1)
	{
		afisareNivel(rad->left, level - 1);
		afisareNivel(rad->right, level - 1);
	}
}

bool existaDrum(nodarb* rad, int* vector, int* nr, int nod)
{
	if (!rad)
		return false;

	//se adauga nodul curent in vector
	vector[*nr] = rad->inf.cod;
	(*nr)++;

	//daca nodul primit ca parametru este egal cu radacina returnam true
	if (rad->inf.cod == nod)
		return true;

	//altfel verificam daca nodul cautat se afla in subarborele stang sau drept
	if (existaDrum(rad->left, vector, nr, nod) || existaDrum(rad->right, vector, nr, nod))
		return true;

	//daca nu se afla in subarbore stang sau drept se elimina din vector si returnam false
	(*nr)--;
	return false;
}

// functie care afiseaza drumul de la radacina la un anumit nod
void afiseazaDrum(nodarb* rad, int* vector, int nr, int nod)
{
	if (existaDrum(rad, vector, &nr, nod))
	{
		for (int i = 0; i < nr; i++)
			cout << vector[i] << "->";
	}
	else
		cout << "nu exista drum" << endl;
}


void traversareNiveluri(nodarb* rad)
{
	int h = nrNiveluri(rad);
	for (int i = 1; i <= h; i++)
		afisareNivel(rad, i);
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
		rad = inserare(rad, s);

		rad = reechilibrare(rad);
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

	cout << endl << "Noduri de pe nivelul 3: " << endl;
	afisareNivel(rad, 3);

	cout << endl << "Traversare pe niveluri: " << endl;
	traversareNiveluri(rad);

	cout << endl << "Drumul de la radacina la un anumit nod: " << endl;
	int* vector = new int[n];
	int nr = 0;
	afiseazaDrum(rad, vector, nr, 5);

	dezalocare(rad);
}