#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct nodStiva
{
	int inf;
	nodStiva* next;
};

struct nodCoada
{
	int inf;
	nodCoada* next;
};

struct nod
{
	int n;
	char* denumire;
};

struct nodLS
{
	nod inf;
	nodLS* next;
};

struct nodLP
{
	nodLS* inf;
	nodLP* next;
};

void push(nodStiva** varf, int val)
{
	nodStiva* nou = new nodStiva;
	nou->inf = val;
	nou->next = NULL;
	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

void pop(nodStiva** varf, int* val)
{
	if (*varf == NULL)
		return;
	else
	{
		*val = (*varf)->inf;
		nodStiva* aux = *varf;
		*varf = (*varf)->next;
		free(aux);
	}
}

void put(nodCoada** prim, nodCoada** ultim, int val)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->inf = val;
	nou->next = NULL;
	if (*prim == NULL && *ultim == NULL)
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

void get(nodCoada** prim, nodCoada** ultim, int* val)
{
	if (*prim != NULL && *ultim != NULL)
	{
		*val = (*prim)->inf;
		nodCoada* aux = *prim;
		*prim = (*prim)->next;
		free(aux);
	}
	if (*prim == NULL)
		*ultim = NULL;
}

void parcurgereAdancime(int** mat, int* vizitat, int nr, int nod,char** denumiri)
{
	for (int i = 0; i < nr; i++)
		vizitat[i] = 0;

	nodStiva* varf = NULL;

	push(&varf, nod);
	vizitat[nod] = 1;

	while (varf != NULL)
	{
		pop(&varf, &nod);
		printf("%d-%s->", nod + 1, denumiri[nod]);
		for (int k = 0; k < nr; k++)
			if (mat[nod][k] == 1 && vizitat[k] == 0)
			{
				push(&varf, k);
				vizitat[k] = 1;
			}
	}
}

void parcurgereLatime(int** mat, int* vizitat, int nr, int nod, char** denumiri)
{
	for (int i = 0; i < nr; i++)
		vizitat[i] = 0;

	nodCoada* prim = NULL, * ultim = NULL;

	put(&prim, &ultim, nod);
	vizitat[nod] = 1;

	while (prim != NULL)
	{
		get(&prim, &ultim, &nod);
		printf("%d-%s->", nod + 1,denumiri[nod]);
		for (int k = 0; k < nr; k++)
			if (mat[nod][k] == 1 && vizitat[k] == 0)
			{
				put(&prim, &ultim, k);
				vizitat[k] = 1;
			}
	}
}

void inserareLS(nodLS** cap, int n, char* denumire)
{
	nodLS* nou = new nodLS;
	nou->inf.n = n;
	nou->inf.denumire = new char[strlen(denumire) + 1];
	strcpy_s(nou->inf.denumire, strlen(denumire) + 1, denumire);
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
		printf("\nNod=%d, Denumire=%s", temp->inf.n,temp->inf.denumire);
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
		free(temp->inf.denumire);
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

void main()
{
	ifstream fin("fisier.txt");
	int nr;

	fin >> nr;

	int** mat = new int*[nr];
	for (int i = 0; i < nr; i++)
		mat[i] = new int[nr];

	for (int i = 0; i < nr; i++)
		for (int j = 0; j < nr; j++)
			mat[i][j] = 0;

	char** denumiri=new char*[nr];
	for (int i = 0; i < nr; i++)
		denumiri[i] = new char[20];
	int i = 0;
	for(int j=0;j<nr;j++)
	{
		fin >> i;
		fin >> denumiri[i-1];
	}

	cout << endl;
	int n, ii, jj;
	fin >> n;
	for (int i = 0; i < n; i++)
	{
		fin >> ii;
		fin >> jj;
		if (ii <= n && jj <= n)
		{
			mat[ii - 1][jj - 1] = 1;
			mat[jj - 1][ii - 1] = 1;
		}
	}



	int* vizitat = new int[nr];

	int nod;
	printf("\nParcurgere in adancime de la nodul: ");
	cin >> nod;
	parcurgereAdancime(mat, vizitat, nr, nod,denumiri);

	printf("\nParcurgere in latime de la nodul: ");
	cin >> nod;
	parcurgereLatime(mat, vizitat, nr, nod,denumiri);

	//numar componente conexe
	printf("\n----------------------\n");
	for (int i = 0; i < nr; i++)
		vizitat[i] = 0;

	int nrComponente = 0;
	for (int j = 0; j < nr; j++)
		if (vizitat[j] == 0)
		{
			nrComponente++;
			parcurgereLatime(mat, vizitat, nr, j,denumiri);
		}

	if (nrComponente == 1)
		printf("\nGraful este conex!");
	else
		printf("\nGraful nu este conex! El are %d componente conexe!", nrComponente);

	//Matrice in lista de liste
	nodLP* capLP = NULL;
	for (int i = 0; i < n; i++)
	{
		nodLS* capLS = NULL;
		for (int j = 0; j < n; j++)
			if(mat[i][j]==1)
				inserareLS(&capLS, j+1, denumiri[j]);
		inserareLP(&capLP, capLS);
	}
	traversareLP(capLP);
	//traversare in latime si adancime graf reprezentat prin liste de liste

}
