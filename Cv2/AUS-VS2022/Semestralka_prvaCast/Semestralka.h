#pragma once  
#include <vector>  
#include "Vrchol.h"  
#include "libds/amt/explicit_hierarchy.h"  
#include "Zastavka.h"  

using ViacCestnaHierarchia = ds::amt::MultiWayExplicitHierarchy<Vrchol>;  
using BlokHierarchie = ds::amt::MultiWayExplicitHierarchyBlock<Vrchol>;
using IteratorHierarchie = ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<Vrchol>>::PreOrderHierarchyIterator;

class Semestralka  
{  
   ViacCestnaHierarchia hierarchiaZastavok;  
   BlokHierarchie* koren_;  
   std::vector<Zastavka> zastavky;  
   std::vector<Zastavka> filtrovaneZastavky;
	

public:  
   Semestralka(std::vector<Zastavka>& zastavky, std::vector<Zastavka>& filtrovaneZastavky)  
       : zastavky(zastavky), filtrovaneZastavky(filtrovaneZastavky), koren_(nullptr) {}  

   void nacitajVsetkyZastavky(std::string nazovSuboru);  
   void zobrazMenuPrvaCast();  
   void zobrazMenuDruhaCast();
   void vypisVsetkyZastavky(const std::vector<Zastavka>& zastavkyNaVypis);
   void vypisSynov(BlokHierarchie& vrchol);
   void vypisZastavkuSNazvom(std::string nazov);
};