#include <libds/mm/compact_memory_manager.h>

struct Person {
	int id;
	std::string name;
};

struct Student : Person{
	int groupId;
};


int main() {
	ds::mm::CompactMemoryManager<Student> cmm;
	Student* first = cmm.allocateMemory();
	Student* second = cmm.allocateMemory();
	
	first->id = 1;
	first->name = "Bendzo";

	first->groupId = 100;

	cmm.print(std::cout);
}