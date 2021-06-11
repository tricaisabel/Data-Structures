#include <iostream>
#include <fstream>
using namespace std;

ifstream fin("fisier.txt");

typedef struct {
	int cod;
	char* den;
	float pret;
	float cant;
}produs;

struct nodLs{
	produs inf;
	nodLs* next;
};

nodLs* inserare(nodLs* cap, produs p)
{
	nodLs* nou = new nodLs;
	//copiem informatia utila in noul nod
	nou->inf.cant = p.cant;
	nou->inf.pret = p.pret;
	nou->inf.cod = p.cod;
	nou->inf.den = new char[strlen(p.den) + 1];
	strcpy_s(nou->inf.den, strlen(p.den) + 1, p.den);
	//daca lista este nula, nodul nou devine cap
	if (cap == nullptr)
	{
		cap = nou;
		nou->next = nullptr;
	}
	else
	{
		//daca mai exista noduri inainte, le parcurgem folosind o copie
		nodLs* temp = cap;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		temp->next = nou;
		nou->next = nullptr;
	}
	return cap;
}

nodLs* citireFisier(nodLs* cap)
{
	int n;
	char buffer[20];
	produs p;
	fin >> n;
	for (int i = 0; i < n; i++)
	{
		fin >> p.cod;
		fin >> buffer;
		p.den = new char[strlen(buffer) + 1];
		strcpy_s(p.den, strlen(buffer) + 1, buffer);
		fin >> p.pret;
		fin >> p.cant;
		cap = inserare(cap, p);
		delete[] p.den;
	}
	return cap;
}

void traversare(nodLs* cap)
{
	nodLs* temp = cap;
	while (temp->next != nullptr)
	{
		cout << "cod=" << temp->inf.cod << " denumire=" << temp->inf.den << " pret" << temp->inf.pret << " cantitate=" << temp->inf.cant << endl;
		temp = temp->next;
	}
	cout << "cod=" << temp->inf.cod << " denumire=" << temp->inf.den << " pret" << temp->inf.pret << " cantitate=" << temp->inf.cant << endl;
}

void conversieVector(nodLs* cap, produs* produse, int* nr)
{
	//parcurgem elementele folosind temp
	nodLs* temp = cap;
	while (temp->next != nullptr)
	{
		//copiem camp cu camp elementele din nod
		produse[*nr].cod = temp->inf.cod;
		produse[*nr].pret = temp->inf.pret;
		produse[*nr].cant = temp->inf.cant;
		produse[*nr].den = new char[strlen(temp->inf.den) + 1];
		strcpy_s(produse[*nr].den, strlen(temp->inf.den)+1, temp->inf.den);

		//marim contor vector
		(*nr)++;

		//copiem pointerul de next al lui temp
		nodLs* temp2 = temp->next;
		//stergem informatia din temp
		delete temp;
		//echivalent cu temp=temp->next
		temp = temp2;
	}
	produse[*nr].cod = temp->inf.cod;
	produse[*nr].pret = temp->inf.pret;
	produse[*nr].cant = temp->inf.cant;
	produse[*nr].den = new char[strlen(temp->inf.den) + 1];
	strcpy_s(produse[*nr].den, strlen(temp->inf.den) + 1, temp->inf.den);
	//marim contor vector
	(*nr)++;
	//stergem informatia din temp
	delete temp;
}

void dezalocareLista(nodLs* cap)
{
	nodLs* temp = cap;
	while (temp->next != nullptr)
	{
		nodLs* temp2 = temp->next;
		//stergem campul alocat dinamic din temp->inf
		if (temp->inf.den != nullptr)
			delete[] temp->inf.den;
		delete temp;
		temp = temp2;
	}
	if (temp->inf.den != nullptr)
		delete[] temp->inf.den;
	delete temp;
}

int main()
{
	nodLs* cap = nullptr;
	cap=citireFisier(cap);
	traversare(cap);

	produs* produse = new produs[10];
	int nr = 0;
	conversieVector(cap, produse, &nr);
	cout << "Conversie din lista in vector\n";
	for (int i = 0; i < nr; i++)
	{
		cout << "cod=" <<produse[i].cod << " denumire=" << produse[i].den << " pret" << produse[i].pret << " cantitate=" << produse[i].cant << endl;
	}

	for (int i = 0; i < nr; i++)
	{
		delete[] produse[i].den;
	}
	delete[] produse;

	//dezalocareLista(cap);
}