#pragma once

#include <cstdint>
namespace x86emul {
union RegisterUnion {
  uint32_t dword;
  struct {
    union {
      uint16_t loword;
      struct {
        uint8_t lobyte;
        uint8_t hibyte;
      };
    };
    uint16_t hiword;
  };
};
struct Registers {
  RegisterUnion gpr[8];
  uint32_t EIP;
} extern gRegisters;

enum class RegIndex {
  AX = 0,
  CX,
  DX,
  BX,
  SP,
  BP,
  SI,
  DI,

  EAX = AX,
  ECX = CX,
  EDX = DX,
  EBX = BX,
  ESP = SP,
  EBP = BP,
  ESI = SI,
  EDI = DI,

  AL = AX,
  CL = CX,
  DL = DX,
  BL = BX,
  AH = SP,
  CH = BP,
  DH = SI,
  BH = DI,

  ES = 0,
  CS,
  SS,
  DS
};

constexpr uint32_t &REG_EAX() {
  return gRegisters.gpr[int(RegIndex::EAX)].dword;
}

constexpr uint32_t &REG_EBX() {
  return gRegisters.gpr[int(RegIndex::EBX)].dword;
}

constexpr uint32_t &REG_ECX() {
  return gRegisters.gpr[int(RegIndex::ECX)].dword;
}

constexpr uint32_t &REG_EDX() {
  return gRegisters.gpr[int(RegIndex::EDX)].dword;
}

constexpr uint32_t &REG_ESP() {
  return gRegisters.gpr[int(RegIndex::ESP)].dword;
}

constexpr uint32_t &REG_EBP() {
  return gRegisters.gpr[int(RegIndex::EBP)].dword;
}

constexpr uint32_t &REG_ESI() {
  return gRegisters.gpr[int(RegIndex::ESI)].dword;
}

constexpr uint32_t &REG_EDI() {
  return gRegisters.gpr[int(RegIndex::EDI)].dword;
}

constexpr uint16_t &REG_AX() {
  return gRegisters.gpr[int(RegIndex::AX)].loword;
}

constexpr uint16_t &REG_BX() {
  return gRegisters.gpr[int(RegIndex::BX)].loword;
}

constexpr uint16_t &REG_CX() {
  return gRegisters.gpr[int(RegIndex::CX)].loword;
}

constexpr uint16_t &REG_DX() {
  return gRegisters.gpr[int(RegIndex::DX)].loword;
}

constexpr uint8_t &REG_AH() { return gRegisters.gpr[int(RegIndex::AX)].hibyte; }

constexpr uint8_t &REG_BH() { return gRegisters.gpr[int(RegIndex::BX)].hibyte; }

constexpr uint8_t &REG_CH() { return gRegisters.gpr[int(RegIndex::CX)].hibyte; }

constexpr uint8_t &REG_DH() { return gRegisters.gpr[int(RegIndex::DX)].hibyte; }

constexpr uint8_t &REG_AL() { return gRegisters.gpr[int(RegIndex::AX)].lobyte; }

constexpr uint8_t &REG_BL() { return gRegisters.gpr[int(RegIndex::BX)].lobyte; }

constexpr uint8_t &REG_CL() { return gRegisters.gpr[int(RegIndex::CX)].lobyte; }

constexpr uint8_t &REG_DL() { return gRegisters.gpr[int(RegIndex::DX)].lobyte; }

struct CPU {};
} // namespace x86emul
