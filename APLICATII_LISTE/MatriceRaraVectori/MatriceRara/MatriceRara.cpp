#include <iostream>
#include <fstream>

using namespace std;



//definire structura matrice rara, reprezentata prin vectori
struct MatriceRara
{
	int* val;
	int* lin;
	int* col;
	int m;
	int n;
	int nrnenule;
};

struct nod
{
	int valoare, linie, coloana;
	struct nod* next;
};

int** citireMatrice(int& nrlin, int& nrcol,string fisier)
{
	ifstream fin(fisier);
	fin >> nrlin;
	fin >> nrcol;
	int** a = new int* [nrlin];
	for (int i = 0; i < nrlin; i++)
		a[i] = new int[nrcol];
	for (int i = 0; i < nrlin; i++)
		for (int j = 0; j < nrcol; j++)
			fin >> a[i][j];
	return a;
}

void afisareMatrice(int** a, int m, int n)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			cout << a[i][j] << " ";
		cout << endl;
	}
}

int rara(int** a, int nrlin, int nrcol)
{
	float grad = 0;
	for (int i = 0; i < nrlin; i++)
		for (int j = 0; j < nrcol; j++)
			if (a[i][j] != 0)
				grad++;
	if (grad / (float)(nrlin * nrcol) < 0.3)
		return grad;
	else
		return 0;
}

MatriceRara transformare(int** a, int nrlin, int nrcol)
{
	MatriceRara mat;
	mat.m = mat.n=mat.nrnenule=0;
	mat.col = mat.lin = mat.val = NULL;
	int k = 0, ok=rara(a,nrlin,nrcol);
	if (ok)
	{
		mat.val = new int[ok];
		mat.lin = new int[ok];
		mat.col = new int[ok];
		mat.m = nrlin;
		mat.n = nrcol;
		mat.nrnenule = ok;
		for (int i = 0; i < nrlin; i++)
			for (int j = 0; j < nrcol; j++)
			{
				if (a[i][j] != 0)
				{
					mat.val[k] = a[i][j];
					mat.lin[k] = i;
					mat.col[k] = j;
					k++;
				}
			}
	}
	else
		cout << "Matricea nu este rara\n";
	return mat;
}

int** transformare_invers(MatriceRara a)
{
	int** b = new int* [a.m];
	for (int i = 0; i < a.m; i++)
		b[i] = new int[a.n];
	int k = 0, i, j;
	for (i = 0; i < a.m; i++)
		for (j = 0; j < a.n; j++)
			if ((a.lin[k] == i) && (a.col[k] == j))
			{
				b[i][j] = a.val[k];
				k++;
			}
			else
				b[i][j] = 0;
	return b;
}

void afisareRara(MatriceRara mat)
{
	printf("\n");
	for (int i = 0; i < mat.nrnenule; i++)
		printf("%d (%d,%d) ", mat.val[i], mat.lin[i], mat.col[i]);
}

int extrag(MatriceRara a, int i, int j)
{
	for (int k = 0; k < a.nrnenule; k++)
		if ((a.lin[k] == i) && (a.col[k] == j))
			return a.val[k];
	return 0;
}

void transp(MatriceRara a)
{
	int aux;
	for (int i = 0; i < a.nrnenule; i++)
	{
		aux = a.col[i];
		a.col[i] = a.lin[i];
		a.lin[i] = aux;
	}
	for (int i = 0; i < a.nrnenule - 1; i++)
		for (int j = i + 1; j < a.nrnenule; j++)
		{
			if ((a.lin[i] > a.lin[j]) && (a.col[i] > a.col[j]))
			{
				aux = a.col[i];
				a.col[i] = a.col[j];
				a.col[j] = aux;
				aux = a.lin[i];
				a.lin[i] = a.lin[j];
				a.lin[j] = aux;
				aux = a.val[i];
				a.val[i] = a.val[j];
				a.val[j] = aux;
			}
		}
	aux = a.m;
	a.m = a.n;
	a.n = aux;
}

nod* transform_lista(nod* cap, int** a, int nrlin, int nrcol)
{
	nod* p;
	for (int i = 0; i < nrlin; i++)
		for (int j = 0; j < nrcol; j++)
		{
			if (a[i][j] && rara(a, nrlin, nrcol))
			{
				nod* nou = (nod*)malloc(sizeof(nod));
				nou->valoare = a[i][j];
				nou->linie = i;
				nou->coloana = j;
				if (cap == NULL)
				{
					nou->next = NULL;
					cap = nou;
				}
				else
				{
					for (p = cap; p->next; p = p->next);
					p->next = nou;
					nou->next = NULL;
					p = nou;
				}
			}
		}
	return cap;
}

//dezalocare matrice
void dezaloc_matrice(int** a, int nrlin, int nrcol)
{
	for (int i = 0; i < nrlin; i++)
		delete[] a[i];
	delete[] a;
}

//dezalocare vectori
void dezaloc_vectori(MatriceRara mat)
{
	free(mat.lin);
	free(mat.col);
	free(mat.val);
}

void dezaloc_lista(nod* cap)
{
	nod* p = cap, * q;
	while (p)
	{
		q = p->next;
		delete p;
		p = q;
	}
}
MatriceRara adunare(MatriceRara a, MatriceRara b) {
	MatriceRara suma;
	if (a.n != b.n || a.m != b.m) cout << "Matricele au dimensiuni diferite si nu pot fi adunate";
	else
	{
		suma.m = a.m;
		suma.n = a.n;
		suma.nrnenule = 0;
		suma.col = new int[a.nrnenule + b.nrnenule];
		suma.lin = new int[a.nrnenule + b.nrnenule];
		suma.val= new int[a.nrnenule + b.nrnenule];
		int i=0, j=0,k=0;
		while(i<a.nrnenule && j<b.nrnenule)
		{
			if (a.col[i] == b.col[j] && a.lin[i] == b.lin[j])
			{
				suma.val[k] = a.val[i] + b.val[j];
				suma.col[k] = a.col[i];
				suma.lin[k] = a.lin[i];
				i++;
				j++;
			}
			else if ((a.col[i] <= b.col[j] && a.lin[i] <= b.lin[j]) || (a.lin[i] < b.lin[j] && a.col[i] >= b.col[j]))
			{
				suma.val[k] = a.val[i];
				suma.col[k] = a.col[i];
				suma.lin[k] = a.lin[i];
				i++;
			}
			else if ((a.col[i] >= b.col[j] && a.lin[i] >= b.lin[j])||(a.lin[i]>b.lin[j]&&a.col[i]<=b.col[j]))
			{
				suma.val[k] = b.val[j];
				suma.col[k] = b.col[j];
				suma.lin[k] = b.lin[j];
				j++;
			}
			k++;
		}
		while (i < a.nrnenule)
		{
			suma.val[k] = a.val[i];
			suma.col[k] = a.col[i];
			suma.lin[k] = a.lin[i];
			i++;
			k++;
		}
		while (j < a.nrnenule)
		{
			suma.val[k] = b.val[j];
			suma.col[k] = b.col[j];
			suma.lin[k] = b.lin[j];
			j++;
			k++;
		}
		suma.nrnenule = k-1;
	}
	
	return suma;
}

void main()
{
	int** mat1;
	int** mat2;
	int** mat3;
	int m, n;
	mat1 = citireMatrice(n, m,"fisier.txt");
	afisareMatrice(mat1, m, n);
	MatriceRara a;
	a = transformare(mat1, m, n);
	cout << "\nMatricea rara:" << endl;
	afisareRara(a);
	mat2 = transformare_invers(a);
	cout << "\nMatricea normala:" << endl;
	for (int i = 0; i < a.m; i++)
	{
		for (int j = 0; j < a.n; j++) cout << mat2[i][j] << " ";
		cout << endl;
	}
	
	/*cout << "Matrice transpusa: \n";
	transp(a);
	afisareRara(a);*/

	nod* cap = NULL;
	cap = transform_lista(cap, mat1, m, n);
	nod* p = cap;
	cout << "\n\nMatricea rara reprezentata prin lista simpla:" << endl;
	while (p)
	{
		printf("%d (%d,%d) ", p->valoare, p->linie, p->coloana);
		p = p->next;
	}

	cout << "\n\nAdunarea a 2 matrice:";
	mat3 = citireMatrice(n, m,"fisier2.txt");
	MatriceRara b;
	b = transformare(mat3, m, n);
	afisareRara(a);
	cout << "\n+";
	afisareRara(b);
	cout << "\n=";
	MatriceRara rezultat = adunare(a, b);
	afisareRara(rezultat);

	//dezalocari
	dezaloc_matrice(mat1, m, n);
	dezaloc_matrice(mat2, m, n);
	dezaloc_vectori(a);

}
