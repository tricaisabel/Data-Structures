#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

struct NodAO;

struct NodAdr
{
	NodAO* adr;
	NodAdr* next;
};

struct NodAO
{
	int id;
	NodAdr* desc;
};

void cautaNod(NodAO* r, int idp, NodAO** t)
{
	if (r)
	{
		if (r->id == idp)
			*t = r;
		NodAdr* q = r->desc;
		while (q)
		{
			cautaNod(q->adr, idp, t);
			q = q->next;
		}
	}
}

NodAO* inserareNodAO(NodAO* r, int idp, int idins)
{
	if (!r)
	{
		//inserare nod radacina
		NodAO* nou = new NodAO;
		nou->id = idins;
		nou->desc = NULL;
		return nou;
	}
	else
	{
		NodAO* t = NULL;
		cautaNod(r, idp, &t);
		if (t)
		{
			//inserare nod in lista de descendenti ai nodului t
			NodAO* nou = new NodAO;
			nou->id = idins;
			nou->desc = NULL;
			NodAdr* q = t->desc;
			//inserare nod in lista de descendenti
			NodAdr* nouAdr = new NodAdr;
			nouAdr->adr = nou;
			nouAdr->next = NULL;
			if (q)
			{
				while (q->next) q = q->next;
				q->next = nouAdr;
			}
			else
			{
				t->desc = nouAdr;
			}
		}
		return r;
	}
}

void afis(NodAO* r)
{
	NodAO* p = r;
	while (p->desc != NULL)
	{
		printf("\n%d", p->id);
		NodAdr* t = p->desc;
		printf("\n");
		while (t != NULL)
		{
			printf("%d ", t->adr->id);
			t = t->next;
		}
		if (p->desc) p = p->desc->adr;
	}
}

void afisareAO(NodAO* r)
{
	NodAO* p = r;
	afis(p);

	NodAdr* q = r->desc->next;
	while (q != NULL)
	{
		afis(q->adr);
		q = q->next;
	}
}

void main()
{
	NodAO* rad = NULL;
	int idP, idIns;

	ifstream fin("arboreOarecare.txt");

	while (fin>>idP>>idIns)
	{
		rad = inserareNodAO(rad, idP, idIns);
	}

	//afisare structura arbore oarecare
	afisareAO(rad);
}