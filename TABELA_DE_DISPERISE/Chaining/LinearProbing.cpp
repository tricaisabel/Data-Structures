#include <iostream>
#include <fstream>
using namespace std;

ifstream fin("fisier.txt");

struct student {
	int cod;
	char* nume;
	float medie;
};

struct hashT {
	student** vect;
	int size;
};

int functieHash(int cheie, hashT tabela)
{
	return cheie % tabela.size;
}

int inserare(hashT tabela, student* s)
{
	int pozitie = 0;
	if (tabela.vect != NULL)
	{
		//apelam functia de hash pt aflarea pozitiei
		pozitie = functieHash((*s).cod, tabela);
		if (tabela.vect[pozitie] == NULL)
			tabela.vect[pozitie] = s;
		else//altfel ne mutam la urmatoarea pozitie disponibila
		{
			int index=1;
			while (pozitie + index < tabela.size)
			{
				if (tabela.vect[pozitie + index] == NULL)
				{
					tabela.vect[pozitie + index] = s;
					pozitie += index;
					break;
				}
				index++;
			}
		}

	}
	return pozitie;
}

void traversareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.size; i++)
			if (tabela.vect[i] != NULL)
			{
				cout << "\nPozitia " << i << endl;
				cout << "Cod=" << tabela.vect[i]->cod << "\tNume=" << tabela.vect[i]->nume << "\tMedie" << tabela.vect[i]->medie<<endl;
				cout << "-----------------------------------------------";
			}
	}
}

void dezalocareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.size; i++)
			if (tabela.vect[i] != NULL)
			{
				delete tabela.vect[i]->nume;
				delete tabela.vect[i];
			}
		delete tabela.vect;
	}
}

int stergereCod(hashT tabela, int cod)
{
	int pozitie = 0;
	if (tabela.vect != NULL)
	{
		//ii calculam pozitia cu functia hash
		pozitie = functieHash(cod, tabela);
		if (tabela.vect[pozitie] == NULL) return -1; //nu exista elemente pe acea pozitie
		else
		{
			if (tabela.vect[pozitie]->cod == cod)//daca nu s-a produs coliziune
			{
				delete tabela.vect[pozitie]->nume;
				delete tabela.vect[pozitie];
				tabela.vect[pozitie] = NULL;
			}
			else//daca s-a produs coliziune
			{
				int index=1;
				while (pozitie + index < tabela.size)
				{
					if (tabela.vect[pozitie + index]->cod==cod)
					{
						pozitie += index;
						delete tabela.vect[pozitie]->nume;
						delete tabela.vect[pozitie];
						tabela.vect[pozitie] = NULL;
						break;
					}
					index++;
				}
			}
		}
	}
	return pozitie;
}

int main()
{
	hashT tabela;
	tabela.size = 101;
	tabela.vect = new student * [tabela.size];
	for (int i = 0; i < tabela.size; i++)
		tabela.vect[i] = NULL;
	int n; //nr studenti
	fin >> n;
	student *s;
	char buffer[20];
	for (int i = 0; i < n; i++)
	{
		s = new student;
		fin >> s->cod;
		fin >> buffer;
		s->nume = new char[strlen(buffer) + 1];
		strcpy_s(s->nume, strlen(buffer) + 1, buffer);
		fin >> s->medie;
		int poz = inserare(tabela, s);
		cout << s->cod << " a fost inserat pe pozitia " << poz << endl;
	}
	traversareTabela(tabela);

	int cod,poz;
	cout << "\nIntroduceti cod de sters: ";
	cin >> cod;
	poz = stergereCod(tabela, cod);
	cout << "Codul " << cod << " a fost sters de pe pozitia " << poz << endl<<endl;
	

	traversareTabela(tabela);

	dezalocareTabela(tabela);

}