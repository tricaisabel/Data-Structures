#include <iostream>
#include <fstream>
using namespace std;

struct Angajat
{
	int cod;
	char* nume;
	float salariu;
};

struct nodArb
{
	Angajat inf;
	nodArb* stanga, * dreapta;
};

nodArb* creareNod(Angajat a, nodArb* st, nodArb* dr)
{
	nodArb* nou = new nodArb;
	nou->inf.cod = a.cod;
	nou->inf.nume = new char[strlen(a.nume) + 1];
	strcpy_s(nou->inf.nume, strlen(a.nume) + 1, a.nume);
	nou->inf.salariu = a.salariu;
	nou->stanga = st;
	nou->dreapta = dr;
	return nou;
}

nodArb* inserareNod(Angajat a, nodArb* rad)
{
	if (rad != nullptr)
	{
		if (a.cod < rad->inf.cod)
		{
			rad->stanga = inserareNod(a, rad->stanga);
			return rad;
		}
		else
			if (a.cod > rad->inf.cod)
			{
				rad->dreapta = inserareNod(a, rad->dreapta);
				return rad;
			}
			else
				return rad;
	}
	else
		return creareNod(a, nullptr, nullptr);
}

//nodArb* inserareNod(Angajat a, nodArb* rad)
//{
//	nodArb* temp = rad;
//	if (rad == nullptr)
//	{
//		temp = creareNod(a, nullptr, nullptr);
//		return temp;
//	}
//	else
//	{
//		while (true)
//		{
//			if (a.cod < temp->inf.cod)
//			{
//				if (temp->stanga != nullptr)
//				{
//					temp = temp->stanga;
//				}
//				else
//				{
//					temp->stanga = creareNod(a, nullptr, nullptr);
//					return rad;
//				}
//			}
//			else
//				if (a.cod > temp->inf.cod)
//				{
//					if (temp->dreapta != nullptr)
//					{
//						temp = temp->dreapta;
//					}
//					else
//					{
//						temp->dreapta = creareNod(a, nullptr, nullptr);
//						return rad;
//					}
//				}
//				else
//					return rad;
//		}
//	}
//}

void traversarePreOrdine(nodArb* rad)
{
	if (rad != nullptr)
	{
		cout << "Codul " << rad->inf.cod << " numele " << rad->inf.nume << " salariul " << rad->inf.salariu << endl;
		traversarePreOrdine(rad->stanga);
		traversarePreOrdine(rad->dreapta);
	}
}

void traversareInOrdine(nodArb* rad)
{
	if (rad != nullptr)
	{
		traversareInOrdine(rad->stanga);
		cout << "Codul " << rad->inf.cod << " numele " << rad->inf.nume << " salariul " << rad->inf.salariu << endl;
		traversareInOrdine(rad->dreapta);
	}
}

void traversarePostOrdine(nodArb* rad)
{
	if (rad != nullptr)
	{
		traversarePostOrdine(rad->stanga);
		traversarePostOrdine(rad->dreapta);
		cout << "Codul " << rad->inf.cod << " numele " << rad->inf.nume << " salariul " << rad->inf.salariu << endl;
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != nullptr)
	{
		nodArb* st = rad->stanga;
		nodArb* dr = rad->dreapta;
		delete[] rad->inf.nume;
		delete rad;
		dezalocare(st);
		dezalocare(dr);
	}
}

void angajatiSalPesteVal(nodArb* rad, Angajat* vect, int* nr, int prag)
{
	if (rad != nullptr)
	{
		if (rad->inf.salariu > prag)
		{
			vect[*nr] = rad->inf;
			(*nr)++;
		}
		angajatiSalPesteVal(rad->stanga, vect, nr, prag);
		angajatiSalPesteVal(rad->dreapta, vect, nr, prag);
	}
}

nodArb* stergeRad(nodArb* rad)
{
	nodArb* aux = rad;
	if (aux->stanga != nullptr)
	{
		rad = aux->stanga;
		if (aux->dreapta != nullptr)
		{
			nodArb* temp = aux->stanga;
			while (temp->dreapta != nullptr)
				temp = temp->dreapta;
			temp->dreapta = aux->dreapta;
		}
	}
	else
		if (aux->dreapta != nullptr)
			rad = aux->dreapta;
		else
			rad = nullptr;
	delete[] aux->inf.nume;
	delete aux;
	return rad;
}

nodArb* stergeNod(nodArb* rad, int cod)
{
	if (rad == nullptr)
		return nullptr;
	else
		if (rad->inf.cod == cod)
		{
			rad = stergeRad(rad);
			return rad;
		}
		else
		{
			nodArb* aux = rad;
			while (true)
			{
				if (cod < aux->inf.cod)
					if (aux->stanga == nullptr)
						break;
					else
						if (aux->stanga->inf.cod == cod)
							aux->stanga = stergeRad(aux->stanga);
						else
							aux = aux->stanga;
				else
					if (cod > aux->inf.cod)
						if (aux->dreapta == nullptr)
							break;
						else
							if (aux->dreapta->inf.cod == cod)
								aux->dreapta = stergeRad(aux->dreapta);
							else
								aux = aux->dreapta;
			}
			return rad;
		}
}

nodArb* stergeNod_recursiv(int cod, nodArb* rad)
{
	if (rad != nullptr)
	{
		if (cod < rad->inf.cod)
		{
			rad->stanga = stergeNod_recursiv(cod, rad->stanga);
		}
		else
			if (cod > rad->inf.cod)
			{
				rad->dreapta = stergeNod_recursiv(cod, rad->dreapta);
			}
			else rad = stergeRad(rad);
		return rad;
	}
	else return nullptr;
}


int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int nrNiveluri(nodArb* rad)
{
	if (rad != nullptr)
	{
		int stanga = nrNiveluri(rad->stanga);
		int dreapta = nrNiveluri(rad->dreapta);
		return 1 + maxim(stanga, dreapta);
	}
	else
		return 0;
}

//sterge un angajat dupa nume
nodArb* stergeAngajatNume(nodArb* rad, char* nume)
{
	if (rad != nullptr)
	{
		if (strcmp(rad->inf.nume, nume) == 0)
		{
			rad = stergeNod(rad, rad->inf.cod);
			return rad;
		}
		else
		{
			rad->stanga = stergeAngajatNume(rad->stanga, nume);
			rad->dreapta = stergeAngajatNume(rad->dreapta, nume);
			return rad;
		}
	}
	else
		return nullptr;
}

void main()
{
	int n;
	char buffer[20];
	nodArb* rad = nullptr;
	Angajat a;
	ifstream f;
	f.open("fisier.txt");
	f >> n;
	for (int i = 0; i < n; i++)
	{
		f >> a.cod;
		f >> buffer;
		a.nume = new char[strlen(buffer) + 1];
		strcpy_s(a.nume, strlen(buffer) + 1, buffer);
		f >> a.salariu;
		rad = inserareNod(a, rad);
		delete[] a.nume;
	}
	f.close();

	traversareInOrdine(rad);
	cout << endl;
	traversarePostOrdine(rad);
	cout << endl;
	traversarePreOrdine(rad);
	cout << endl;

	/*Angajat* vect = new Angajat[n];
	int nr = 0;
	angajatiSalPesteVal(rad, vect, &nr, 1850);
	for (int i = 0; i < nr; i++)
	{
		cout << "Codul " << vect[i].cod << " numele " << vect[i].nume << " salariul " << vect[i].salariu << endl;
	}
	delete[] vect;*/
	rad = stergeRad(rad);
	//rad = stergeNod(rad, 4);
	cout << endl;
	traversarePreOrdine(rad);

	cout << "Inaltime arbore: " << nrNiveluri(rad) << endl;
	cout << "Inaltime subarbore stang: " << nrNiveluri(rad->stanga) << endl;
	cout << "Inaltime subarbore drept: " << nrNiveluri(rad->dreapta) << endl;

	int gradEchilibru = nrNiveluri(rad->dreapta) - nrNiveluri(rad->stanga);
	if (gradEchilibru >= -1 && gradEchilibru <= 1)
		cout << "Arbore echilibrat, grad echilibru: " << gradEchilibru << endl;
	else
		cout << "Arbore neechilibrat" << endl;

	char nume[20];
	cout << "Nume de sters: ";
	cin >> nume;
	rad = stergeAngajatNume(rad, nume);
	traversarePreOrdine(rad);

	dezalocare(rad);
}