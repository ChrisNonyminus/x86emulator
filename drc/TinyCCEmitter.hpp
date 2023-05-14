#pragma once
#include "IEmitter.hpp"
#include <map>
#include <string>


struct TinyCCEmitter : IEmitter {
  void EmitByte(EmittedSegment seg, uint8_t val) {}
  void EmitWord(EmittedSegment seg, uint16_t val) {}
  void EmitDword(EmittedSegment seg, uint32_t val) {}

  std::map<EmittedSegment, std::string *> currentSegments;

  uint8_t *GetSegmentContents(EmittedSegment seg, std::size_t *size) {
    if (currentSegments.count(seg)) {
      *size = currentSegments[seg]->size();
      return reinterpret_cast<uint8_t *>(currentSegments[seg]->data());
    }
    return nullptr;
  }

  TinyCCEmitter() { currentSegments[EmittedSegment::CODE] = new std::string(); }
  ~TinyCCEmitter() {
    for (auto &seg : currentSegments) {
      delete seg.second;
    }
    currentSegments.clear();
  }

  void AddRegToReg(HostRegister lhs, HostRegister rhs, HostRegister dst,
                   size_t opsize) {
    char buf[512];
    sprintf(buf, R"(
        *(vars->%s) = *(vars->%s) + *(vars->%s);
    )",
            EnumToString(dst).c_str(), EnumToString(lhs).c_str(),
            EnumToString(rhs).c_str());
    currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
  }

  void MoveRegToMemory(HostRegister src, MemoryLocation &dst, size_t opsize) {
    char buf[512];
    switch (opsize) {
    case 1: {
      sprintf(buf, R"(
        *(uint8_t*)Var_%X = *(uint8_t*)(vars->%s);
    )",
              dst.guestAddr, EnumToString(src).c_str());
      currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
    } break;
    case 2: {
      sprintf(buf, R"(
        *(uint16_t*)Var_%X = *(uint16_t*)(vars->%s);
    )",
              dst.guestAddr, EnumToString(src).c_str());
      currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
    } break;
    case 4: {
      sprintf(buf, R"(
        *(uint32_t*)Var_%X = *(uint32_t*)(vars->%s);
    )",
              dst.guestAddr, EnumToString(src).c_str());
      currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
    } break;
    default:
      break;
    }
  }

  void Nop() {}

  void MoveMemoryToReg(HostRegister dst, MemoryLocation &src, size_t opsize) {
    char buf[512];
    switch (opsize) {
    case 1: {
      sprintf(buf, R"(
        *(uint8_t*)(vars->%s) = *(uint8_t*)Var_%X;
    )",
              EnumToString(dst).c_str(), src.guestAddr);
      currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
    } break;
    case 2: {
      sprintf(buf, R"(
        *(uint16_t*)(vars->%s) = *(uint16_t*)Var_%X;
    )",
              EnumToString(dst).c_str(), src.guestAddr);
      currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
    } break;
    case 4: {
      sprintf(buf, R"(
        *(uint32_t*)(vars->%s) = *(uint32_t*)Var_%X;
    )",
              EnumToString(dst).c_str(), src.guestAddr);
      currentSegments[IEmitter::EmittedSegment::CODE]->append(buf);
    } break;
    default:
      break;
    }
  }

  Function* ScanFunction(uint8_t *start, size_t maxScanLen);
  int RecompileFunction(Function* func);
  void* ExecuteFunction(Function* func, void* userdata);
};
