#include <libds/mm/compact_memory_manager.h>

int main() {
	ds::mm::CompactMemoryManager<int> cmm;
	cmm.allocateMemory();
}