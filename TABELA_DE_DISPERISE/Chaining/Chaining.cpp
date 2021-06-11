#include <iostream>
#include <fstream>
using namespace std;

ifstream fin("fisier.txt");

struct student {
	int cod;
	char* nume;
	float medie;
};

struct nodLs {
	student inf;
	nodLs* next;
};

struct hashT {
	nodLs** vect;
	int size;
};

int functieHash(int cheie, hashT tabela)
{
	return cheie % tabela.size;
}

int functieHash2(char cheie[20], hashT tabela)
{
	return cheie[0] % tabela.size;
}

int inserare(hashT tabela, student s)
{
	int pozitie = 0;
	if (tabela.vect != NULL)
	{
		//apelam functia de hash pt aflarea pozitiei
		//pozitie = functieHash(s.cod, tabela);
		pozitie = functieHash2(s.nume, tabela);

		//initializam un nou nod pentru lista
		nodLs* nou = new nodLs;
		nou->inf.cod = s.cod;
		nou->inf.nume = new char[strlen(s.nume) + 1];
		strcpy_s(nou->inf.nume, strlen(s.nume) + 1, s.nume);
		nou->inf.medie = s.medie;
		nou->next = NULL;
		//daca pozitia nu e ocupata, o ocupam
		if (tabela.vect[pozitie] == NULL)
			tabela.vect[pozitie] = nou;
		else//altfel adaugam dupa ultimul elemente din lista de la acea pozitie
		{
			nodLs* temp = tabela.vect[pozitie];
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = nou;
		}

	}
	return pozitie;
}

void traversareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for(int i=0;i<tabela.size;i++)
			if (tabela.vect[i] != NULL)
			{
				cout << "\nPozitia " << i<<endl;
				nodLs* temp = tabela.vect[i];
				while (temp)
				{
					cout << "Cod=" << temp->inf.cod << "\tNume=" << temp->inf.nume << "\tMedie=" << temp->inf.medie << endl;
					temp = temp->next;
				}
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
				nodLs* temp = tabela.vect[i];
				while (temp)
				{
					nodLs* temp2 = temp->next;
					delete temp->inf.nume;
					delete temp;
					temp = temp2;
				}
			}
		delete tabela.vect;
	}
}
void stergereNod(nodLs* nod)
{
	delete[] nod->inf.nume;
	delete nod;
}

int stergereCod(hashT tabela, int cod)
{
	int pozitie = 0;
	//ii calculam pozitia cu functia hash
	if (tabela.vect != NULL)
	{
		pozitie = functieHash(cod, tabela);
		if (tabela.vect[pozitie] == NULL) return -1; //nu exista elemente pe acea pozitie
		else
		{
			if(tabela.vect[pozitie]->inf.cod==cod)//daca primul nod din lista are codul dorit
				if (tabela.vect[pozitie]->next == NULL)//daca nu mai are alt nod dupa el
				{
					stergereNod(tabela.vect[pozitie]);
					tabela.vect[pozitie] = NULL;
				}
				else //daca mai are noduri dupa el, le aducem cu o poziie mai in fata
				{
					nodLs* temp = tabela.vect[pozitie];
					tabela.vect[pozitie] = temp->next;
					stergereNod(temp);
				}
			else//daca nu este primul nod din lista, parcurgem nodurile din lista
			{
				nodLs* temp = tabela.vect[pozitie];
				while (temp->next != NULL && temp->next->inf.cod != cod)
					temp = temp->next;
				nodLs* p = temp->next;
				if (p->next == NULL)//daca nu mai are alt nod dupa el
				{
					temp->next = NULL;
					stergereNod(p);
				}
				else //daca mai are noduri dupa el, le aducem cu o pozitie mai in fata
				{
					temp->next = p->next;
					stergereNod(p);
				}
			}
		}
	}
	return pozitie;
}
int stergereCod2(hashT tabela, char nume[20])
{
	int pozitie = 0;
	//ii calculam pozitia cu functia hash
	if (tabela.vect != NULL)
	{
		pozitie = functieHash2(nume, tabela);
		if (tabela.vect[pozitie] == NULL) return -1; //nu exista elemente pe acea pozitie
		else
		{
			if(strcmp(tabela.vect[pozitie]->inf.nume,nume)==0)//daca primul nod din lista are codul dorit
				if (tabela.vect[pozitie]->next == NULL)//daca nu mai are alt nod dupa el
				{
					stergereNod(tabela.vect[pozitie]);
					tabela.vect[pozitie] = NULL;
				}
				else //daca mai are noduri dupa el, le aducem cu o poziie mai in fata
				{
					nodLs* temp = tabela.vect[pozitie];
					tabela.vect[pozitie] = temp->next;
					stergereNod(temp);
				}
			else//daca nu este primul nod din lista, parcurgem nodurile din lista
			{
				nodLs* temp = tabela.vect[pozitie];
				while (temp->next != NULL && strcmp(temp->next->inf.nume, nume)!=0)
					temp = temp->next;
				nodLs* p = temp->next;
				if (p->next == NULL)//daca nu mai are alt nod dupa el
				{
					temp->next = NULL;
					stergereNod(p);
				}
				else //daca mai are noduri dupa el, le aducem cu o poziie mai in fata
				{
					temp->next = p->next;
					stergereNod(p);
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
	tabela.vect = new nodLs * [tabela.size];
	for (int i = 0; i < tabela.size; i++)
		tabela.vect[i] = NULL;
	int n; //nr studenti
	fin >> n;
	student s;
	char buffer[20];
	for (int i = 0; i < n; i++)
	{
		fin >> s.cod;
		fin >> buffer;
		s.nume = new char[strlen(buffer) + 1];
		strcpy_s(s.nume, strlen(buffer) + 1, buffer);
		fin >> s.medie;
		int poz=inserare(tabela, s);
		cout << s.cod << " a fost inserat pe pozitia " << poz << endl;
	}
	traversareTabela(tabela);

	/*int cod,poz; 
	cout << "\nIntroduceti cod de sters: ";
	cin >> cod;
	poz = stergereCod(tabela, cod);
	cout << "Codul " << cod << " a fost sters de pe pozitia " << poz << endl<<endl;
	*/

	char nume[20];
	int poz;
	cout << "\nIntroduceti nume de sters: ";
	cin >> nume;
	poz = stergereCod2(tabela, nume);
	cout << "Numele " << nume << " a fost sters de pe pozitia " << poz << endl << endl;

	traversareTabela(tabela);

	dezalocareTabela(tabela);
	
}