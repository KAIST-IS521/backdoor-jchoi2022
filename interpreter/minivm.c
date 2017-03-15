//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"


//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    if (ctx->funtable[i] != NULL)
      (ctx->funtable[i])(ctx, instr);
}


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

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);

    return true;
}

