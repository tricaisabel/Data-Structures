#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

ifstream fin("fisier.txt");

struct student
{
	int varsta;
	char* nume;
	float medie;
};

struct nodLS
{
	student inf;
	nodLS* next;
};

struct nodLP
{
	nodLS* inf;
	nodLP* next;
};

void inserareLS(nodLS** cap, student s) 
{
	nodLS* nou = new nodLS;
	nou->inf.medie = s.medie;
	nou->inf.varsta = s.varsta;
	nou->inf.nume = new char[strlen(s.nume) + 1];
	strcpy_s(nou->inf.nume, strlen(s.nume) + 1, s.nume);
	nou->next = NULL;
	if (*cap == NULL)
	{
		*cap = nou;
	}
	else
	{
		nodLS* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void inserareLP(nodLP** capLP, nodLS* capLS)
{
	nodLP* nou = new nodLP;
	nou->inf = capLS;
	nou->next = NULL;
	if (*capLP == NULL)
		*capLP = nou;
	else
	{
		nodLP* temp = *capLP;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void traversareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		printf("\nVarsta=%d, Nume=%s, Medie=%5.2f", temp->inf.varsta, temp->inf.nume, temp->inf.medie);
		temp = temp->next;
	}
}

void traversareLP(nodLP* cap)
{
	nodLP* temp = cap;
	while (temp)
	{
		printf("\nSublista: ");
		traversareLS(temp->inf);
		temp = temp->next;
	}
}

void dezalocareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->inf.nume);
		free(temp);
		temp = temp2;
	}
}

void dezalocareLP(nodLP* cap)
{
	nodLP* temp = cap;
	while (temp)
	{
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->inf);
		free(temp);
		temp = temp2;
	}
}
//parcurgere liste pentru cautare
void nrStudenti(nodLP* capLP, float medie, int* nr)
{
	nodLP* temp = capLP;
	while (temp)
	{
		nodLS* temp2 = temp->inf;
		while (temp2)
		{
			if (temp2->inf.medie >= medie)
				(*nr)++;
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
}

void main()
{
	int n;
	fin >> n;
	char buffer[20];
	nodLP* capLP = NULL;
	nodLS* capLSpar = NULL;
	nodLS* capLSimpar = NULL;
	student s;

	for (int i = 0; i < n; i++)
	{
		fin >> buffer;
		s.nume = new char[strlen(buffer) + 1];
		strcpy_s(s.nume, strlen(buffer) + 1, buffer);
		fin >> s.varsta;
		fin >> s.medie;

		if (s.varsta % 2 == 0)
			inserareLS(&capLSpar,s);
		else
			inserareLS(&capLSimpar,s);
		delete s.nume;
	}
	inserareLP(&capLP, capLSpar);
	inserareLP(&capLP, capLSimpar);

	traversareLP(capLP);

	float medie;
	int nr = 0;
	cout << "\n\nIntroduceti media minima: "; cin >> medie;
	nrStudenti(capLP, medie, &nr);
	cout << "\nSunt " << nr << " studenti cu media minima " << medie;
	dezalocareLP(capLP);

}