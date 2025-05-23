#pragma once  
#include <vector>  
#include "Vrchol.h"  
#include "libds/amt/explicit_hierarchy.h"  
#include "Zastavka.h"
#include "libds/adt/table.h"

#include <limits>
#define NOMINMAX 
#include "libds/adt/list.h"
#include "libds/adt/sorts.h"

using ViacCestnaHierarchia = ds::amt::MultiWayExplicitHierarchy<Vrchol>;  
using BlokHierarchie = ds::amt::MultiWayExplicitHierarchyBlock<Vrchol>;
using IteratorHierarchie = ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<Vrchol>>::PreOrderHierarchyIterator;
using ZastavkaTable = ds::adt::HashTable<int, Zastavka*>;

class Semestralka  
{  
   ViacCestnaHierarchia hierarchiaZastavok;  
   BlokHierarchie* koren_;  
   std::vector<Zastavka> zastavky;  
   ZastavkaTable* tabulkaZastavok;
   //ZastavkaTable* tabulkaZastavok;
	

public:  
   Semestralka(std::vector<Zastavka>& zastavky)
       : zastavky(zastavky), koren_(nullptr), tabulkaZastavok(nullptr)
   {
   }  

	~Semestralka()
   {
		delete tabulkaZastavok;
   }
   void nacitajVsetkyZastavky(std::string nazovSuboru);
   Zastavka* najdiZastavkuPodlaID(int& id);
   void zobrazMenuPrvaCast();  
   void zobrazMenuDruhaCast();
   void zobrazMenuTretiaCast();
   void zobrazMenuStvrtaCast(ds::amt::ImplicitSequence<Zastavka>& zastavky);
   void vypisZastavky(ds::amt::ImplicitSequence<Zastavka>& zastavkyNaVypis);
   void vypisVsetkyZastavky();
   void vypisSynov(BlokHierarchie& vrchol);
};