#include "TinyCCEmitter.hpp"
#include "IEmitter.hpp"
#include "tinycc_local/libtcc.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>

extern "C" {
#include "./tinycc_local/libtcc.h"
}

char gCEntryPoint[] =
#include "tinycc_drc_start_c.inc"
    ;
static void tcc_error(void *opaque, const char *msg) {
  printf("[TCC:ERR] %s\n", msg);
}
IEmitter::Function *TinyCCEmitter::ScanFunction(uint8_t *start,
                                                size_t maxScanLen) {
  Function *result = ScanCodeInternal(start, maxScanLen);
  return result;
}

int TinyCCEmitter::RecompileFunction(IEmitter::Function *func) {
  char funcPrototype[512];
  currentSegments[IEmitter::EmittedSegment::CODE]->assign(
      "#include <stdint.h>\n");

  std::map<uintptr_t, bool> memoryLocationInitialized;

  for (int i = 0; i < func->opCount; i++) {
    char str[128];
    // sprintf(str, "void* Var_%X = (void*)%p", func->ops[i]->)
    switch (func->ops[i].operation) {
    case IEmitter::GenericOperation::MOV_MEM2REG: {
      if (!memoryLocationInitialized.count(
              func->ops[i].operands[0].mem.guestAddr)) {
        sprintf(str, "void* Var_%X = (void*)%p;\n",
                func->ops[i].operands[0].mem.guestAddr,
                func->ops[i].operands[0].mem.ptr);
        currentSegments[IEmitter::EmittedSegment::CODE]->append(str);
        memoryLocationInitialized[func->ops[i].operands[0].mem.guestAddr] =
            true;
      }
    } break;
    case IEmitter::GenericOperation::MOV_REG2MEM: {
      if (!memoryLocationInitialized.count(
              func->ops[i].operands[1].mem.guestAddr)) {
        sprintf(str, "void* Var_%X = (void*)%p;\n",
                func->ops[i].operands[1].mem.guestAddr,
                func->ops[i].operands[1].mem.ptr);
        currentSegments[IEmitter::EmittedSegment::CODE]->append(str);
        memoryLocationInitialized[func->ops[i].operands[1].mem.guestAddr] =
            true;
      }
    } break;
    case IEmitter::GenericOperation::ADD_REG2REG:
    case IEmitter::GenericOperation::NOP:
    default:
      break;
    }
  }

  sprintf(funcPrototype, "void* %s(void* args) {\n", func->name);

  currentSegments[IEmitter::EmittedSegment::CODE]->append(funcPrototype);
  currentSegments[IEmitter::EmittedSegment::CODE]->append(R"(
    typedef struct Vars {
        unsigned long* ReturnValue;
        unsigned long* Arg1;
        unsigned long* Arg2;
        unsigned long* Arg3;
        unsigned long* Arg4;
        unsigned long* Arg5;
        unsigned long* Temp1;
        unsigned long* Temp2;
        unsigned long* Temp3;
        unsigned long* Saved1;
        unsigned long* Saved2;
        unsigned long* Saved3;
        unsigned long* StackPointer;
    }Vars ;
    Vars* vars = (Vars*)args;
  )");

  for (int i = 0; i < func->opCount; i++) {
    switch (func->ops[i].operation) {
    case IEmitter::GenericOperation::ADD_REG2REG: {
      AddRegToReg(func->ops[i].operands[0].reg, func->ops[i].operands[1].reg,
                  func->ops[i].operands[2].reg, 4);
    } break;
    case IEmitter::GenericOperation::MOV_REG2MEM: {
      MoveRegToMemory(func->ops[i].operands[0].reg,
                      func->ops[i].operands[1].mem, 4);
    } break;
    case IEmitter::GenericOperation::MOV_MEM2REG: {
      MoveMemoryToReg(func->ops[i].operands[1].reg,
                      func->ops[i].operands[0].mem, 4);
    } break;
    case IEmitter::GenericOperation::NOP:
    default:
      break;
    }
  }

  currentSegments[IEmitter::EmittedSegment::CODE]->append(
      "\nreturn vars;\n}\n");

  // compile program
  TCCState *tcc = tcc_new();
  if (!tcc) {
    printf("[TCC:ERR] Failed to create tcc context!\n");
    return -1;
  }

  tcc_set_lib_path(tcc, "./drc/tinycc");
  tcc_add_include_path(tcc, "./drc/tinycc");

  tcc_set_error_func(tcc, 0x0, tcc_error);
  tcc_set_options(tcc, "-g");
  tcc_set_output_type(tcc, TCC_OUTPUT_MEMORY);

  char c_file_name[128];
  sprintf(c_file_name, "FuncCache_%s.c", func->name);

  FILE *c_file = fopen(c_file_name, "w");

  fwrite(currentSegments[IEmitter::EmittedSegment::CODE]->data(), 1,
         currentSegments[IEmitter::EmittedSegment::CODE]->size(), c_file);

  fclose(c_file);

  int ret = tcc_add_file(tcc, c_file_name);

  if (ret < 0) {
    printf("[TCC:ERR] Failed to add tcc file!\n");
    tcc_delete(tcc);
    return -1;
  }

  char *program = (char *)calloc(1, tcc_relocate(tcc, NULL));
  if (!program) {
    printf("[TCC:ERR] Failed to allocate memory for the program!\n");
    tcc_delete(tcc);
    return -1;
  }

  ret = tcc_relocate(tcc, program);
  if (ret < 0) {
    printf("[TCC:ERR] Failed to allocate memory for the program!\n");
    tcc_delete(tcc);
    return -1;
  }

  func->EntryPoint =
      reinterpret_cast<void *(*)(void *)>(tcc_get_symbol(tcc, func->name));
  return 0;
}

void *TinyCCEmitter::ExecuteFunction(IEmitter::Function *func, void *userdata) {
  if (!func->EntryPoint)
    return nullptr;
  return func->EntryPoint(userdata);
};
