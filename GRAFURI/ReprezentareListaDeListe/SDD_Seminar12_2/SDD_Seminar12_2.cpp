#include <stdio.h>
#include<string.h>
#include<malloc.h>
#include <iostream>
#include <fstream>

using namespace std;

struct oras
{
	int id;
	char* denumire;
};

struct nodlp;

struct nodls
{
	nodls* next;
	nodlp* info;
};

struct nodlp
{
	nodls* vecini;//lista de vecini
	oras info;
	nodlp* next;
};

nodlp* inserareLP(nodlp* cap, oras s)
{
	nodlp* nou = (nodlp*)malloc(sizeof(nodlp));
	nou->info.id = s.id;
	nou->info.denumire = (char*)malloc((strlen(s.denumire) + 1) * sizeof(char));
	strcpy_s(nou->info.denumire, strlen(s.denumire)+1, s.denumire);

	nou->vecini = NULL;
	nou->next = NULL;

	if (cap)
	{
		nodlp* temp = cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
		return cap;
	}
	else
	{
		return nou;
	}
}

nodls* inserareLS(nodls* cap, nodlp* info)
{
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->info = info;
	nou->next = NULL;
	if (cap)
	{
		nodls* temp = cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
		return cap;
	}
	else
	{
		return nou;
	}
}

nodlp* cautareDupaId(nodlp* cap, int id)
{
	nodlp* temp = cap;
	while (temp && temp->info.id != id)
	{
		temp = temp->next;
	}
	return temp;
}

void cautareDupaDenumire(nodlp* cap, char* denumire)
{
	nodlp* temp = cap;
	while (temp && strcmp(temp->info.denumire,denumire)!=0)
	{
		temp = temp->next;
	}
	if(temp==nullptr) cout << "\nOrasul " << denumire << " NU a fost gasit!";
	else cout << "\nOrasul " << denumire << " a fost gasit!";
}

void inserareArcInGraf(nodlp* graf, int idStart, int idStop)
{
	if (graf)
	{
		nodlp* nodStart = cautareDupaId(graf, idStart);
		nodlp* nodStop = cautareDupaId(graf, idStop);
		if (nodStart && nodStop)
		{
			nodStart->vecini = inserareLS(nodStart->vecini, nodStop);//daca este orientat
			nodStop->vecini = inserareLS(nodStop->vecini, nodStart);
		}
	}
}

void traversareLS(nodls* cap)
{
	nodls* temp = cap;
	while (temp)
	{
		printf("    %d.%s\n", temp->info->info.id + 1, temp->info->info.denumire);
		temp = temp->next;
	}
}

void traversareLP(nodlp* cap)
{
	nodlp* temp = cap;
	while (temp)
	{
		printf("%d.%s are urmatorii vecini: \n", temp->info.id + 1, temp->info.denumire);
		traversareLS(temp->vecini);
		temp = temp->next;
	}
}

//pt parcurgere in adancime
struct NodStiva
{
	int id;
	NodStiva* next;
};

NodStiva* push(NodStiva* varf, int id)
{
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->next = NULL;
	nou->id = id;
	if (varf == NULL)
		return nou;
	else
	{
		nou->next = varf;
		varf = nou;
		return varf;
	}
}

int pop(NodStiva*& varf)
{
	if (varf)
	{
		int id = varf->id;
		NodStiva* temp = varf;
		varf = varf->next;
		free(temp);
		return id;
	}
	else
	{
		return -1;
	}
}



int calculareNumarNoduri(nodlp* graf)
{
	int contor = 0;
	nodlp* temp = graf;
	while (temp)
	{
		contor++;
		temp = temp->next;
	}
	return contor;
}

//parcurgere in adancime
void parcurgereAdancime(nodlp* graf, int idNodStart)
{
	if (graf)
	{
		NodStiva* stiva = NULL;
		int nrNoduri = calculareNumarNoduri(graf);
		int* vizitate = (int*)malloc(sizeof(int) * nrNoduri);
		for (int i = 0; i < nrNoduri; i++)
		{
			vizitate[i] = 0;
		}

		stiva = push(stiva, idNodStart);
		vizitate[idNodStart] = 1;

		while (stiva)
		{
			int idNodCurent = pop(stiva);

			nodlp* nodCurent = cautareDupaId(graf, idNodCurent);
			printf("%d.%s\n", nodCurent->info.id + 1, nodCurent->info.denumire);

			nodls* temp = nodCurent->vecini;
			while (temp)
			{
				if (vizitate[temp->info->info.id] == 0)
				{
					stiva = push(stiva, temp->info->info.id);
					vizitate[temp->info->info.id] = 1;
				}
				temp = temp->next;
			}
		}
	}
}

void dezalocareLS(nodls* cap) {
	nodls* temp = cap;
	while (temp) {
		nodls* temp2 = temp->next;
		free(temp);
		temp = temp2;
	}
}

void dezalocareLP(nodlp* cap) {
	nodlp* temp = cap;
	while (temp) {
		nodlp* temp2 = temp->next;
		if (temp->vecini)
			dezalocareLS(temp->vecini);
		printf("\nAm sters %s", temp->info.denumire);
		free(temp->info.denumire);
		free(temp);
		temp = temp2;
	}
}

struct nodCoada
{
	int id;
	nodCoada* next;
};

void put(nodCoada** prim, nodCoada** ultim, int o)
{
	nodCoada* nou = new nodCoada;
	nou->id = o;
	nou->next = nullptr;

	if (*prim == nullptr)
	{
		*prim = nou;
		*ultim = nou;
	}
	else
	{
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim)
{
	if (*prim != nullptr && *ultim != nullptr)
	{	
		int o = (*prim)->id;
		nodCoada* aux = *prim;
		*prim = (*prim)->next;
		return o;
	}
	else
	{
		if (*prim == nullptr)
			*ultim = nullptr;
		return -1;
	}
}

void parcurgereLatime(nodlp* graf, int idNodStart)
{
	if (graf)
	{
		//NodStiva* stiva = NULL;
		nodCoada* prim = nullptr, * ultim = nullptr;
		int nrNoduri = calculareNumarNoduri(graf);
		int* vizitate = new int[nrNoduri];
		for (int i = 0; i < nrNoduri; i++)
		{
			vizitate[i] = 0;
		}

		put(&prim, &ultim, idNodStart);
		vizitate[idNodStart] = 1;

		while (prim)
		{
			int idNodCurent=get(&prim, &ultim);
			nodlp* nodCurent = cautareDupaId(graf, idNodCurent);
			printf("%d.%s\n", nodCurent->info.id + 1, nodCurent->info.denumire);

			nodls* temp = nodCurent->vecini;
			while (temp)
			{
				if (vizitate[temp->info->info.id] == 0)
				{
					//stiva = push(stiva, temp->info->info.id);
					put(&prim, &ultim, temp->info->info.id);
					vizitate[temp->info->info.id] = 1;
				}
				temp = temp->next;
			}
		}
	}
}

void main()
{
	nodlp* graf = NULL;
	ifstream fin("fisier.txt");
	int nrNoduri;
	fin >> nrNoduri;
	for (int i = 0; i < nrNoduri; i++)
	{
		//id-ul va fi i-ul, denumirele citit de la tastatura
		oras s;
		s.id = i;
		char buffer[20];
		fin >> buffer;
		s.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy_s(s.denumire,strlen(buffer)+1, buffer);
		graf = inserareLP(graf, s);
	}

	//inserare arce(vecinii pt fiecare nod)
	int nrArce = 0;
	fin >> nrArce;

	for (int i = 0; i < nrArce; i++)
	{
		int idStart;
		fin >> idStart;
		int idStop;
		fin >> idStop;
		inserareArcInGraf(graf, idStart-1, idStop-1);
	}

	//afisare
	traversareLP(graf);
	//parcurgere in adancime
	printf("\nParcurgere in adancime de la nodul 1:\n");
	parcurgereAdancime(graf, 0);

	printf("\nParcurgere in latime de la nodul 1:\n");
	parcurgereLatime(graf, 0);
	//dezalocareLP(graf);

	//cauta un nod dupa denumire, similar cu cautare dupa id
	//pornind de la functia respectiva, vecinii directi ai lui (traversare lista de vecini-traversare ls sau ne pozitionam cu nod curent 
	/*char* denumireCautate = new char[strlen("Bucuresti") + 1];
	strcpy_s(denumireCautate, strlen("Bucuresti") + 1, "Bucuresti");
	cautareDupaDenumire(graf,denumireCautate);*/
	
}
