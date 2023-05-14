#include "IEmitter.hpp"
#include "../CPU.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

unsigned long checksumCode(uint8_t* start, int len) {
    unsigned long checksum = 0;
    for (int i =0; i < len; i++) {
        checksum += start[i];
        checksum =  ((float)checksum / (float)len) * 100;
    }
    return checksum;
}

IEmitter::Function *IEmitter::ScanCodeInternal(uint8_t *start,
                                               size_t maxScanLen) {
  static uint8_t mem[4 * 1024 * 1024]; // temp

  int i = 0;
  Function *func = new Function(); // make sure to delete the Function after it
                                   // is no longer used
  func->ops = (Function::FuncOp *)malloc(sizeof(Function::FuncOp) * 1);
  func->opCount = 1;
  int opidx = 0;
  while (i < maxScanLen) {
    // TODO: actual code scanning for x86 functions (calls, jmps, etc)
    // for now, implement a rudimentary imaginary machine code language
    uint32_t tmpaddr;
    switch (start[i]) {
    case int(IEmitter::GenericOperation::NOP):
      func->ops[func->opCount - 1] = {IEmitter::GenericOperation::NOP, {}};
      i++;
      break;
    case int(IEmitter::GenericOperation::ADD_REG2REG):
      i++;
      func->ops[func->opCount - 1] = {IEmitter::GenericOperation::ADD_REG2REG,
                                      {}};
      func->ops[func->opCount - 1].operands[0].reg = HostRegister(start[i++]);
      func->ops[func->opCount - 1].operands[1].reg = HostRegister(start[i++]);
      func->ops[func->opCount - 1].operands[2].reg = HostRegister(start[i++]);
      break;
    case int(IEmitter::GenericOperation::MOV_REG2MEM):
      i++;
      func->ops[func->opCount - 1] = {IEmitter::GenericOperation::MOV_REG2MEM,
                                      {}};
      func->ops[func->opCount - 1].operands[0].reg = HostRegister(start[i++]);
      tmpaddr = *(uint32_t*)&start[i];
      i+=4;
      func->ops[func->opCount - 1].operands[1].mem = {&mem[tmpaddr], (uintptr_t)&mem[tmpaddr], tmpaddr};
      break;
    case int(IEmitter::GenericOperation::MOV_MEM2REG):
      i++;
      func->ops[func->opCount - 1] = {IEmitter::GenericOperation::MOV_MEM2REG,
                                      {}};
      tmpaddr = *(uint32_t*)&start[i];
      i+=4;
      func->ops[func->opCount - 1].operands[0].mem = {&mem[tmpaddr], (uintptr_t)&mem[tmpaddr], tmpaddr};
      func->ops[func->opCount - 1].operands[1].reg = HostRegister(start[i++]);
      break;
    default:
      break;
    }

    if (i < maxScanLen) {
      func->ops = (Function::FuncOp *)realloc(
          func->ops, sizeof(Function::FuncOp) * (func->opCount + 1));
      func->opCount++;
    }


  }

  char name[512];
  sprintf(name, "FUNC_%X", checksumCode(start, maxScanLen));
  func->name = ( char*)malloc(strlen(name));
  strcpy(func->name, name);

  return func;

}