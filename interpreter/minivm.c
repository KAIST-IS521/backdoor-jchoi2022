//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"


//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns,
Reg* registers, FunPtr* funtable, uint32_t numInstrs, uint32_t * code, uint32_t heapSz, uint8_t * heap) {
    ctx->pc         = 0;
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->numInstrs  = numInstrs;
    ctx->code       = code;
    ctx->heapSz     = heapSz;
    ctx->heap       = heap;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> bool
bool stepVMContext(struct VMContext* ctx) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = ctx->code[ctx->pc];
    const uint8_t idx = EXTRACT_B0(instr);

    // Dispatch to an opcode-handler.
    if (ctx->funtable[idx] == NULL) {
      printf("Invalid opcode, aborts\n");
      return false;
    }

    return (ctx->funtable[idx])(ctx, instr);
}

bool halt(struct VMContext* ctx, const uint32_t instr) {
  return false; // stop running
}

bool load(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t addr_idx = EXTRACT_B2(instr);
  const uint32_t addr = ctx->r[addr_idx];
  if (addr >= ctx->heapSz) {
    printf("Segmentation fault in 'load', aborts\n");
    return false;
  }
  ctx->r[dst_idx] = (uint32_t) ctx->heap[addr]; // unsigned int, so extended with 0
  return true;
}

bool store(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t addr_idx = EXTRACT_B1(instr);
  const uint8_t src_idx = EXTRACT_B2(instr);
  const uint32_t addr = ctx->r[addr_idx];
  if (addr >= ctx->heapSz) {
    printf("Segmentation fault in 'store', aborts\n");
    return false;
  }
  ctx->heap[addr] = (uint8_t) ctx->r[src_idx]; // extract lower 8bit
  return true;
}

bool move(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t src_idx = EXTRACT_B2(instr);
  ctx->r[dst_idx] = ctx->r[src_idx];
  return true;
}

bool puti(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t imm = EXTRACT_B2(instr);
  ctx->r[dst_idx] = (uint32_t) imm; // unsigned int, so extended with 0
  return true;
}

bool add(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  ctx->r[dst_idx] = ctx->r[oprnd1_idx] + ctx->r[oprnd2_idx];
  return true;
}

bool sub(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  ctx->r[dst_idx] = ctx->r[oprnd1_idx] - ctx->r[oprnd2_idx];
  return true;
}
