#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct pacient {

	int cod;
	char* nume;
	char* descriere;
	float varsta;
	int prioritate;
};

//definire structura coadaPrioritati
struct coadaPrioritati {
	pacient* vect;
	int nr; //nr de pacienti
};

void filtrare(coadaPrioritati h, int index)
{
	int indexMax = index;//nod radacina
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS<h.nr && h.vect[indexS].prioritate>h.vect[indexMax].prioritate)
		indexMax = indexS;
	if (indexD<h.nr && h.vect[indexD].prioritate>h.vect[indexMax].prioritate)
		indexMax = indexD;

	if (index != indexMax)
	{
		pacient aux = h.vect[index];
		h.vect[index] = h.vect[indexMax];
		h.vect[indexMax] = aux;

		filtrare(h, indexMax);
	}

}

void inserare(coadaPrioritati* h, pacient elem, char* categorie)
{
	int prioritate;
	if (strcmp(categorie, "gravide si copii cu varsta sub 1 an") == 0)
		elem.prioritate = 5;
	else if (strcmp(categorie, "copii cu varsta peste 1 an") == 0)
		elem.prioritate = 4;
	else if (strcmp(categorie, "restul pacientilor") == 0)
		elem.prioritate = 1;

	pacient* vect1 = new pacient[(*h).nr + 1];
	for (int i = 0; i < (*h).nr; i++)
		vect1[i] = (*h).vect[i];

	(*h).nr++;
	delete[](*h).vect;
	(*h).vect = vect1;

	(*h).vect[(*h).nr - 1] = elem;

	for (int i = ((*h).nr - 1) / 2; i >= 0; i--) //filtrare de la ultimul element
		filtrare((*h), i);
}

void afisare(coadaPrioritati h)
{
	for (int i = 0; i < h.nr; i++)
	{
		cout << "\n\nPrioritatea este: " << h.vect[i].prioritate << endl;
		cout << "Codul: " << h.vect[i].cod << endl;
		cout << "Numele pacientului: " << h.vect[i].nume << endl;
		cout << "Descrierea problemei: " << h.vect[i].descriere << endl;
		cout << "Varsta: " << h.vect[i].varsta << endl;
		if (h.vect[i].prioritate == 5)
			cout << "Categoria 1: gravide si copii cu varsta sub 1 an" << endl;
		else if (h.vect[i].prioritate == 4)
			cout << "Categoria 2: copii cu varsta peste 1 an" << endl;
		else if (h.vect[i].prioritate >= 1 && h.vect[i].prioritate <= 3)
			cout << "Categoria 3: restul pacientilor " << endl;
		cout << "--------------------------------------------------\n";
	}
}

//void extragere(heap* h, apelUrgenta* elem)
//{
//	apelUrgenta* vect1 = new apelUrgenta[(*h).nrElem - 1];
//
//	apelUrgenta temp = (*h).vect[0];//interschimbam primul elem cu ultimul din vector
//	(*h).vect[0] = (*h).vect[(*h).nrElem - 1];
//	(*h).vect[(*h).nrElem - 1] = temp;
//
//	*elem = (*h).vect[(*h).nrElem - 1];//elementul maxim care se afla acum pe ultima pozitie in vector
//
//	for (int i = 0; i < (*h).nrElem - 1; i++)
//		vect1[i] = (*h).vect[i];
//
//	(*h).nrElem--;
//	delete[](*h).vect;
//	(*h).vect = vect1;//noul vector
//
//	for (int i = ((*h).nrElem - 1) / 2; i >= 0; i--)
//		filtrare((*h), i);//filtrare de jos in sus
//
//}

void dezalocare(coadaPrioritati h)
{
	for (int i = 0; i < h.nr; i++)
		delete[] h.vect[i].nume;
	delete[] h.vect;
}

int nrCategoria1(coadaPrioritati c)
{
	//gravidele si copii sub un an au prioritatea 5
	int nr = 0;
	for (int i = 0; i < c.nr; i++)
		if (c.vect[i].prioritate == 5)
			nr++;
	return nr;
}
int simptomulExista(string s[100], int nr, string simptom)
{
	for (int i = 0; i < nr; i++)
		if (s[i]==simptom)
			return i;
	return -1;
}

void determinareSimptome(coadaPrioritati c, string*& simptome, int*& frecventa,int *nr)
{
	for (int i = 0; i < c.nr; i++)
		if (simptomulExista(simptome, (*nr), c.vect[i].descriere)==-1)
		{
			string s = string(c.vect[i].descriere);
			simptome[(*nr)] =s;
			frecventa[(*nr)] = 1;
			nr++;
		}
		else
		{
			int index = simptomulExista(simptome, (*nr), c.vect[i].descriere);
			frecventa[index]++;
		}
}


void main()
{
	coadaPrioritati c;
	ifstream fin("fisier.txt");
	int n;
	fin >> c.nr;
	char buffer[20];
	char* categorie;
	string linie1,linie2;
	c.vect = new pacient[c.nr];
	for (int i = 0; i < c.nr; i++)
	{
		fin >> c.vect[i].cod;
		fin >> linie1;
		getline(fin, linie2, '\n');
		linie1 += linie2;
		c.vect[i].nume = new char[(linie1.length() + 1)];
		strcpy_s(c.vect[i].nume, linie1.length() + 1, linie1.c_str());
		fin >> linie1;
		getline(fin, linie2, '\n');
		linie1 += linie2;
		c.vect[i].descriere = new char[(linie1.length() + 1)];
		strcpy_s(c.vect[i].descriere, linie1.length() + 1, linie1.c_str());
		fin >> c.vect[i].varsta;
		fin >> linie1;
		getline(fin, linie2, '\n');
		linie1 += linie2;
		categorie = new char[(linie1.length() + 1)];
		strcpy_s(categorie, linie1.length() + 1, linie1.c_str());

		if (strcmp(categorie, "gravide si copii cu varsta sub 1 an") == 0)
			c.vect[i].prioritate = 5;
		else if (strcmp(categorie, "copii cu varsta peste 1 an") == 0)
			c.vect[i].prioritate = 4;
		else if (strcmp(categorie, "restul pacientilor") == 0)
			c.vect[i].prioritate = 1;
	}

	for (int i = (c.nr - 1) / 2; i >= 0; i--)
		filtrare(c, i);
	//insearea unui nou pacient
	pacient p;
	char nume[20], descriere[50];
	strcpy_s(nume, strlen("Ionescu Maria") + 1, "Ionescu Maria");
	p.cod = 100;
	strcpy_s(descriere, strlen("Dureri de cap") + 1, "Dureri de cap");
	p.varsta = 20;
	categorie = new char[100];
	strcpy_s(categorie, strlen("gravide si copii cu varsta sub 1 an") + 1, "gravide si copii cu varsta sub 1 an");	
	p.prioritate = 1;
	p.nume = new char[strlen(nume) + 1];
	strcpy_s(p.nume, strlen(nume) + 1, nume);
	p.descriere = new char[strlen(descriere) + 1];
	strcpy_s(p.descriere, strlen(descriere) + 1, descriere);

	inserare(&c, p,categorie);
	afisare(c);

	//nr de gravide si copii sub un an
	int nr = nrCategoria1(c);
	cout << "\n\nNumarul de persoane din categoria 1 este: " << nr << endl;

	//statistici simptome
	/*int nrSimptome = 0;
	string* simptome=new string[100];
	int* frecvente=new int[100];
	determinareSimptome(c,simptome,frecvente,&nrSimptome);
	for (int i = 0; i < nrSimptome; i++)
	{
		cout << "Sindrom: " << simptome[i] << endl;
		cout << "Aparitii: " << frecvente[i] << endl;
		cout << endl;
	}*/
}


