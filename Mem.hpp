#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
namespace x86emul {
struct MappedMemory {
  const char *name;
  uint32_t start;
  uint32_t size;
  uint8_t *data;

  void Init() {
    if (!data)
      data = new uint8_t[size];
  }

  ~MappedMemory() {
    if (data)
      delete[] data;
  }
};

struct MemoryMap {
  std::vector<MappedMemory> map;

  void Init() {
    for (auto &entry : map) {
      entry.Init();
    }
  }

  uint8_t Read8(uint32_t addr) {
    for (auto &entry : map) {
      if (addr >= entry.start && addr < entry.start + entry.size) {
        return entry.data[addr - entry.start];
      }
    }
    printf("ERROR! Unmapped Read8 from address %08Xh.\n", addr);
    exit(1);
    return 0;
  }

  void Write8(uint32_t addr, uint8_t val) {
    for (auto &entry : map) {
      if (addr >= entry.start && addr < entry.start + entry.size) {
        entry.data[addr - entry.start] = val;
      }
    }
    printf("ERROR! Unmapped Write8 to address %08Xh.\n", addr);
    exit(1);
  }

  uint16_t Read16(uint32_t addr) {
    return Read8(addr) | (Read8(addr + 1) << 8);
  }

  void Write16(uint32_t addr, uint16_t val) {
    uint8_t p1 = (val >> 0) & 0xFF;
    uint8_t p2 = (val >> 8) & 0xFF;

    Write8(addr, p1);
    Write8(addr + 1, p2);
  }

  uint32_t Read32(uint32_t addr) {
    return (Read16(addr)) | (Read16(addr + 2) << 16);
  }
  void Write32(uint32_t addr, uint32_t val) {
    uint16_t p1 = (val & 0xFFFF0000) >> 16;
    uint16_t p2 = (val & 0x0000FFFF);

    Write16(addr, p1);
    Write16(addr + 2, p2);
  }

  bool IsAddressMapped(uint32_t addr) {
    for (auto &entry : map) {
      if (addr >= entry.start && addr < entry.start + entry.size) {
        return true;
      }
    }
    return false;
  }

  MappedMemory *Get(uint32_t addr) {
    for (auto &entry : map) {
      if (addr >= entry.start && addr < entry.start + entry.size) {
        return &entry;
      }
    }
    return NULL;
  }
};
extern MemoryMap gMemoryMap;
} // namespace x86emul
