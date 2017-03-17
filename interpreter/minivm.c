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
