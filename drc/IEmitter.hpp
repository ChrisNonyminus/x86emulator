#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
struct IEmitter {
  /*
  A defined segment in the emitted code.
  */
  enum class EmittedSegment { CODE, DATA, RODATA, BSS };

  /*
  Emit raw bytes/words/dwords to a segment.
  */
  virtual void EmitByte(EmittedSegment seg, uint8_t val) = 0;
  virtual void EmitWord(EmittedSegment seg, uint16_t val) = 0;
  virtual void EmitDword(EmittedSegment seg, uint32_t val) = 0;

  /*
  Get data from a segment.
  */
  virtual uint8_t *GetSegmentContents(EmittedSegment seg,
                                      std::size_t *size) = 0;

  /*
  Generic host register types.
  */
  enum class HostRegister {
    ReturnValue,
    Arg1,
    Arg2,
    Arg3,
    Arg4,
    Arg5,
    Temp1,
    Temp2,
    Temp3,
    Saved1,
    Saved2,
    Saved3,
    StackPointer,
  };

  // /*
  // Host register getters/setters.
  // */
  // virtual unsigned long GetReg(HostRegister type) = 0;
  // virtual void SetReg(HostRegister type, unsigned long val) = 0;

  struct MemoryLocation {
    void *ptr;
    uintptr_t hostAddr;
    uintptr_t guestAddr;
  };

  /*
  Generic operations.
  */
  enum class GenericOperation { NOP, ADD_REG2REG, MOV_REG2MEM, MOV_MEM2REG };
  //   virtual void AddRegToReg(HostRegister lhs, HostRegister rhs, HostRegister
  //   dst,
  //                            size_t opsize) = 0;
  //   virtual void MoveRegToMemory(HostRegister src, MemoryLocation dst,
  //                                size_t opsize) = 0;
  //   virtual void MoveMemoryToReg(HostRegister dst, MemoryLocation src,
  //                                size_t opsize) = 0;
  //   virtual void Nop() = 0;
  // TODO

  struct Function {
    struct FuncOp {
      GenericOperation operation;
      /*
      Usual operand order:
      [0] = src/lhs
      [1] = dst/rhs
      [3] = none/dst
      [4] = ???/???
      */
      union {
        MemoryLocation mem;
        HostRegister reg;
        uint32_t imm32;
      } operands[4];
    } * ops;
    size_t opCount;
    MemoryLocation location;
    void *emittedCode;
    size_t emittedCodeSize;
    void *(*EntryPoint)(void *);
    char *name;

    ~Function() {
      free(ops);
      free(name);
      free(emittedCode);
    }
  };

  /*
  Function manipulation and code generation/execution.
  */
  virtual Function *ScanFunction(uint8_t *start, size_t maxScanLen) = 0;
  virtual int RecompileFunction(Function *func) = 0;
  virtual void*  ExecuteFunction(Function *func, void* userdata) = 0;

protected:
  Function *ScanCodeInternal(uint8_t *start, size_t maxScanLen);

  std::string EnumToString(HostRegister val) {
    switch (val) {
    case HostRegister::ReturnValue:
      return "ReturnValue";
    case HostRegister::Arg1:
      return "Arg1";
    case HostRegister::Arg2:
      return "Arg2";
    case HostRegister::Arg3:
      return "Arg3";
    case HostRegister::Arg4:
      return "Arg4";
    case HostRegister::Arg5:
      return "Arg5";
    case HostRegister::Temp1:
      return "Temp1";
    case HostRegister::Temp2:
      return "Temp2";
    case HostRegister::Temp3:
      return "Temp3";
    case HostRegister::Saved1:
      return "Saved1";
    case HostRegister::Saved2:
      return "Saved2";
    case HostRegister::Saved3:
      return "Saved3";
    case HostRegister::StackPointer:
      return "StackPointer";
    default:
      return "UNK";
    }
  }
};
