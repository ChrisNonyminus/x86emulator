#include "LLE.hpp"
#include "Mem.hpp"

#include <cstdint>
#include <cstdio>

namespace x86emul::LLE {
bool LoadBios(const char *path, uint32_t address, uint32_t size, int alignment,
              int padding) {
  FILE *f = fopen(path, "rb");

  if (!f) {
    printf("ERROR: Could not open file \"%s\"\n", path);
    return false;
  }
  fseek(f, 0, SEEK_END);
  int fsize = ftell(f);
  if (size / (padding + 1) != fsize) {
    printf("ERROR: bad fsize for bios\n");
    fclose(f);
    return false;
  }
  rewind(f);

  if (!gMemoryMap.IsAddressMapped(address)) {
    gMemoryMap.map.push_back({"BIOS", address, size, NULL});
    gMemoryMap.Init(); // ensure bios memory is initialized
  }

  MappedMemory *mappedBiosArea = gMemoryMap.Get(address);

  if (!mappedBiosArea) {
    printf("Error! Bios mapped to memory DOESN'T EXIST.\n");
    fclose(f);
    return false;
  }

  if (mappedBiosArea->size != size) {
    printf("ERROR: invalid bios size, mismatch\n");
    fclose(f);
    return false;
  }

  for (uint32_t a = alignment; a < size; a += (padding + 1)) {
    fread(mappedBiosArea->data + a, 1, 1, f);
  }

  fclose(f);

  return true;
}
} // namespace x86emul::LLE