#include <iostream>
#include <fstream>
using namespace std;

struct nodLS {
	int coloana, valoare;
	nodLS* next;
};

struct nodLP {
	nodLS* inf;
	nodLP* next;
};

nodLS* inserareLS(nodLS* cap,int coloana, int valoare)
{
	nodLS* nou = new nodLS;
	nou->coloana = coloana;
	nou->valoare = valoare;
	nou->next = NULL;
	if (cap == NULL)
	{
		cap = nou;
	}
	else
	{
		nodLS* temp = cap;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return cap;
}

nodLS* inserareLinie(nodLS* capLS, int** mat, int nrLinii, int nrColoane, int indiceLinie)
{
	nodLS* nou = nullptr;
	for (int j = 0; j < nrColoane; j++) {
		if (mat[indiceLinie][j] != 0) {
			nou = inserareLS(nou, j, mat[indiceLinie][j]);
		}

	}
	return nou;
}

nodLP* inserareLP(nodLP* capLP, nodLS* capLS)
{
	nodLP* nou = new nodLP;
	nou->inf = capLS;
	nou->next = nullptr;
	if (capLP == nullptr)
	{
		capLP = nou;
	}
	else
	{
		nodLP* temp = capLP;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
	return capLP;
}

void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		cout << "\tColoana: " << temp->coloana << " Valoare: " << temp->valoare << endl;
		temp = temp->next;
	}
}

void traversareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	int i = 0;
	while (temp)
	{
		cout << "Linia "<<i++;
		cout << endl;
		traversareLS(temp->inf);
		temp = temp->next;
	}
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		delete temp;
		temp = temp2;
	}
}

void dezalocareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	while (temp)
	{
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->inf);
		delete temp;
		temp = temp2;
	}
}

nodLP* sumaMatrice(nodLP* capLP1, nodLP* capLP2)
{
	nodLP* capLPsum = nullptr;
	nodLP* temp1 = capLP1;
	nodLP* temp2 = capLP2;
	int i;
	while (temp1 && temp2)
	{
		nodLS* temps1 = temp1->inf;
		nodLS* temps2 = temp2->inf;
		nodLS* capLSsum = nullptr;
		while (temps2 && temps1) 
		{
			if(temps1->coloana < temps2->coloana) {
				capLSsum = inserareLS(capLSsum, temps1->coloana, temps1->valoare);
				temps1 = temps1->next;
			}
			else if(temps1->coloana == temps2->coloana)
			{
				capLSsum = inserareLS(capLSsum, temps2->coloana, temps2->valoare+temps1->valoare);
				temps2 = temps2->next;
				temps1 = temps1->next;
			}
			else if(temps1->coloana > temps2->coloana) {
				capLSsum = inserareLS(capLSsum, temps2->coloana, temps2->valoare);
				temps2 = temps2->next;
			}
		}
		if(temps1)
			capLSsum = inserareLS(capLSsum, temps1->coloana, temps1->valoare);
		if (temps2)
			capLSsum = inserareLS(capLSsum, temps2->coloana, temps2->valoare);
		capLPsum = inserareLP(capLPsum, capLSsum);
		temp1 = temp1->next;
		temp2 = temp2->next;
	}
	return capLPsum;
}
//Pe diagonala principala i = j
//Sub diagonala principala : i > j
//Deasupra diagonalei principale : i<j
//Pe diagonala secundara j = n - i + 1
//Sub diagonala secundara : j>n - i + 1
//Deasupra diagonalei secundare : j < n - i + 1

void peDiagP(nodLP* cap) {
	cout << "\nElemente de pe diagonala principala:";
	nodLP* temp1 = cap;
	int linie=0;
	while (temp1) {
		nodLS* temp2 = temp1->inf;
		while (temp2)
		{
			if (temp2->coloana == linie)
				cout <<temp2->valoare<<"("<<linie<<")("<<temp2->coloana<< ") ";
			temp2 = temp2->next;
		}
		temp1 = temp1->next;
		linie++;
	}
}

int** citireFisier(int* nrLinii, int* nrColoane,string fisier)
{
	int** mat;
	ifstream fis(fisier);
	fis >> (*nrLinii);
	fis >> (*nrColoane);
	mat = new int* [(*nrLinii)];
	for (int i = 0; i < (*nrLinii); i++)
	{
		mat[i] = new int[(*nrColoane)];
	}
	for (int i = 0; i < (*nrLinii); i++) {
		for (int j = 0; j < (*nrColoane); j++) {
			fis >> mat[i][j];
		}
	}
	fis.close();
	return mat;
}

void dezalocareMatrice(int** mat,int nrLinii)
{
	for (int i = 0; i < nrLinii; i++) {
		delete[] mat[i];
	}
	delete[] mat;
}

void main()
{
	int nrLinii, nrColoane;
	int** mat1=citireFisier(&nrLinii,&nrColoane,"fisier2.txt");
	int** mat2=citireFisier(&nrLinii,&nrColoane,"fisier3.txt");


	nodLP* capLP1 = nullptr;
	for (int i = 0; i < nrLinii; i++) {
		nodLS* capLS1 = nullptr;
		capLS1 = inserareLinie(capLS1, mat1, nrLinii, nrColoane, i);
		capLP1 = inserareLP(capLP1, capLS1);
	}

	nodLP* capLP2 = nullptr;
	for (int i = 0; i < nrLinii; i++) {
		nodLS* capLS2 = nullptr;
		capLS2 = inserareLinie(capLS2, mat2, nrLinii, nrColoane, i);
		capLP2 = inserareLP(capLP2, capLS2);
	}
	
	cout << "Matricea 1:\n";
	traversareLP(capLP1);
	cout << "\nMatricea 2:\n";
	traversareLP(capLP2);
	cout << "\nMatricea suma:\n";
	nodLP* capLPsuma = sumaMatrice(capLP1, capLP2);
	traversareLP(capLPsuma);

	peDiagP(capLP1);

	dezalocareLP(capLP1);
	dezalocareLP(capLP2);

	dezalocareMatrice(mat1, nrLinii);
	dezalocareMatrice(mat2, nrLinii);

}