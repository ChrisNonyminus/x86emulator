#pragma once

#include <cstdint>
#include <vector>
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

struct FlagBitfield32 { // this sucks
  uint8_t
      CF : 1; // Carry flag; on if carry out of most significant bit of result
  uint8_t b1 : 1; // always 1
  uint8_t PF : 1; // parity flag; on if low byte result has even parity (even
                  // number of set bits)
  uint8_t b3 : 1; // always 0
  uint8_t AF : 1; // Auxillary carry; on if carry out (???) of bit position 3
                  // (used for BCD)
  uint8_t b5 : 1; // always 0
  uint8_t ZF : 1; // zero flag; on if result is zero
  uint8_t SF : 1; // sign flag; on if result is negative
  uint8_t TF : 1; // trap flag
  uint8_t IF : 1; // interrupt enable
  uint8_t DF : 1; // direction flag; setting the DF flag causes string
                  // instructions to auto-decrement, or to process strings from
                  // high addresses to low addresses. Clearing the flag causes
                  // string instructions to auto-increment, or to process
                  // strings from low addresses to high addresses.
  uint8_t OF : 1; // overflow flag; on if result exceeds positive or negative
                  // limit of number range
  uint8_t IOPL : 1; // I/O privilege level
  uint8_t NT : 1;   // nested task flag
  uint8_t b14 : 1;  // always 0
  uint8_t RF : 1;   // resume flag
  uint8_t VM : 1;   // virtual 8086 mode
  uint8_t b17 : 1;
  uint8_t b18 : 1;
  uint8_t b19 : 1;
  uint8_t b20 : 1;
  uint8_t b21 : 1;
  uint8_t b22 : 1;
  uint8_t b23 : 1;
  uint8_t b24 : 1;
  uint8_t b25 : 1;
  uint8_t b26 : 1;
  uint8_t b27 : 1;
  uint8_t b28 : 1;
  uint8_t b29 : 1;
  uint8_t b30 : 1;
  uint8_t b31 : 1;
};

struct Registers {
  RegisterUnion gpr[8];
  uint32_t EIP;
  uint16_t CS, SS, DS, ES, FS, GS;
  union {
    FlagBitfield32 bf;
    uint32_t raw;
  } EFLAGS;
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

constexpr uint8_t I8_SIGNBIT(uint8_t v) { return (v & 0b1000'0000) >> 7; }
constexpr uint8_t I16_SIGNBIT(uint8_t v) {
  return (v & 0b1000'0000'0000'0000) >> 15;
}
constexpr uint8_t I32_SIGNBIT(uint8_t v) {
  return (v & 0b1000'0000'0000'0000'0000'0000'0000'0000) >> 31;
}

constexpr uint32_t SEGOFF(uint32_t selector, uint32_t offset) {
  return ((selector << 4) + offset);
}

enum class InstrPrefix : uint8_t {
    OperandSize16 = 0x67,
};

struct Instruction {
  // Prefixes: one or more bytes preceding an instruction which modify the
  // operation of the instruction. The following prefixes can be used by
  // application programs:
  // - segment override - explicitly specifies which segment register an
  // instruction should use, instead of the default segment register.
  // - address size - switches between 16 and 32 bit addressing. either size can
  // be the default: this prefix selects the non-default size.
  // - operand size - switches between 16 and 32 bit data size. either size can
  // be the default: this prefix selects the non-default size.
  // - repeat - used with a string instruction to cause the instruction to be
  // repeated for each element of the string. (ew.)
  std::vector<InstrPrefix> prefixes;
  // Opcode: specifies the operation performed by the instruction. Some
  // operations have several different opcodes, each specifying a different form
  // of the operation.
  uint8_t op;
  // Register specifier: an instruction may specify one or two register
  // operands. Register specifiers occur either in the same byte as the opcode
  // or in the same byte as the addressing mode specifier.
  uint8_t register_spec;
  // addressing mode specifier: when present, specifies whether an operand is a
  // register or memory location; if in memory, specifies whether a
  // displacement, a base register, an index register, and scaling are to be
  // used.
  uint8_t addressing_mode;
  // SIB (scale, index, base) byte: When the addressing mode specifier indicates
  // an index register will be used to calculate the address of an operand, a
  // SIB byte is included in the instruction to encode the base register, the
  // index register, and a scaling factor.
  uint8_t sib;
  // displacement: when the addressing mode specifier indicates a displacement
  // will be used to compute the address of an operand, the displacement is
  // encoded in the instruction. a displacement is a signed integer of 32, 16,
  // or 8 bits. the 8-bit form is used in the common case when the displacement
  // is sufficiently small. the processor extends an 8-bit displacement to 16 or
  // 32 bits, taking into account the sign.
  union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
  } displacement;
  // Immediate operand: when present, provides the value of an operand. Immediate operands may be bytes, words, or dwords. In cases where an 8-bit immediate operand is used with a 16bit or 32bit operand, the processor extends the 8-bit operand to an integer of the same sign and magnitude in the larger size. in the same way, a 16 bit operand is extended to 32 bits.
  union {
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
  } immediate;
};

struct CPU {};
} // namespace x86emul
