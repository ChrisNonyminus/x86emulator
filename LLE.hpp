#pragma once

#include <cstdint>
#include <vector>
namespace x86emul::LLE {
bool LoadBios(const char *path, uint32_t address, uint32_t size, int alignment, int padding);
}
