#include <srl.hpp>

#include "../../i_system.h"

void* nMalloc(size_t size) {
	return SRL::Memory::Malloc(size, SRL::Memory::Zone::Default);
}	