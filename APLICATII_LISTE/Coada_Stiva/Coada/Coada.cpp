#include <iostream>
#include <fstream>
using namespace std;

ifstream fin("fisier.txt");
struct pacient
{
    int cod;
    char* nume;
    float suma;
};

struct nodStiva
{
    pacient inf;
    nodStiva* next;
};

void put(nodStiva** varf, nodStiva** ultim, pacient p)
{
    nodStiva* nou = new nodStiva;
    nou->inf.cod = p.cod;
    nou->inf.suma = p.suma;
    nou->inf.nume = new char[strlen(p.nume) + 1];
    strcpy_s(nou->inf.nume, strlen(p.nume) + 1, p.nume);
    nou->next = nullptr;
    if (*varf == nullptr && *ultim == nullptr)
    {
        *varf = nou;
        *ultim = nou;
    }
    else
    {
        //nodul nou se leaga in continuarea nodurilor exsitente
        (*ultim)->next = nou;
        //ultimul nod pointeaza catre nodul nou
        *ultim = nou;
    }
}

void traversare(nodStiva* varf)
{
    nodStiva* temp = varf;
    while (temp->next != nullptr)
    {
        cout << "Cod " << temp->inf.cod << ", nume " << temp->inf.nume << ", suma de plata " << temp->inf.suma << endl;
        temp = temp->next;
    }
    cout << "Cod " << temp->inf.cod << ", nume " << temp->inf.nume << ", suma de plata " << temp->inf.suma << endl;

}

int get(nodStiva** varf, nodStiva** ultim, pacient* p)
{
    if (*varf != nullptr && *ultim != nullptr)
    {
        //copiem campurile in pacient
        (*p).cod = (*varf)->inf.cod;
        (*p).suma= (*varf)->inf.suma;
        (*p).nume = new char[strlen((*varf)->inf.nume)+1];
        strcpy_s((*p).nume, strlen((*varf)->inf.nume) + 1, (*varf)->inf.nume);

        //stergem nodul din coada
        //pastram o copie a adresei nodului de sters pentru a-l putea sterge
        nodStiva* temp = *varf;
        //elementul este inlocuit cu cel de dupa
        *varf = (*varf)->next;
        delete[] temp->inf.nume;
        delete temp;
        return 0;
    }
    else//daca nu mai exista elemente in coada
    {
        if ((*varf) == nullptr)
            *ultim = nullptr;
        return -1;
    }
}

void dezalocare(nodStiva* varf,nodStiva* ultim)
{
    pacient p2;
    while (get(&varf, &ultim, &p2)==0)
    {
        delete[] p2.nume;
    }
}

int main()
{
    nodStiva* varf=nullptr;
    nodStiva* ultim=nullptr;
    pacient p;
    char buffer[20];
    int n; fin >> n;
    for (int i = 0; i < n; i++)
    {
        fin >> p.cod;
        fin >> buffer;
        p.nume = new char[strlen(buffer) + 1];
        strcpy_s(p.nume, strlen(buffer) + 1, buffer);
        fin >> p.suma;
        put(&varf, &ultim, p);
    }
    traversare(varf);
    dezalocare(varf, ultim);
}