#pragma once  
#include <vector>  
#include "Vrchol.h"  
#include "libds/amt/explicit_hierarchy.h"  
#include "Zastavka.h"
#include "libds/adt/table.h"
#include "libds/adt/list.h"

using ViacCestnaHierarchia = ds::amt::MultiWayExplicitHierarchy<Vrchol>;  
using BlokHierarchie = ds::amt::MultiWayExplicitHierarchyBlock<Vrchol>;
using IteratorHierarchie = ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<Vrchol>>::PreOrderHierarchyIterator;
using ZastavkaTable = ds::adt::HashTable<int, Zastavka*>;

class Semestralka  
{  
   ViacCestnaHierarchia hierarchiaZastavok;  
   BlokHierarchie* koren_;  
   std::vector<Zastavka> zastavky;  
   std::vector<Zastavka> filtrovaneZastavky;
   ZastavkaTable* tabulkaZastavok;
   //ZastavkaTable* tabulkaZastavok;
	

public:  
   Semestralka(std::vector<Zastavka>& zastavky, std::vector<Zastavka>& filtrovaneZastavky)  
       : zastavky(zastavky), filtrovaneZastavky(filtrovaneZastavky), koren_(nullptr), tabulkaZastavok(nullptr) {}  

	~Semestralka()
   {
		delete tabulkaZastavok;
   }
   void nacitajVsetkyZastavky(std::string nazovSuboru);
   void vytvorTabulkuZastavok();
   Zastavka* najdiZastavkuPodlaID(int id);
   void zobrazMenuPrvaCast();  
   void zobrazMenuDruhaCast();
   void zobrazMenuTretiaCast();
   void vypisVsetkyZastavky(const std::vector<Zastavka>& zastavkyNaVypis);
   void vypisSynov(BlokHierarchie& vrchol);
   void vypisZastavkuSNazvom(std::string nazov);
};