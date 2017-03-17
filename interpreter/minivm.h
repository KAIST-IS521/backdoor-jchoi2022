//-----------------------------------------------------------------------------
// minivm.h -- Prototypes and definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

#ifndef MINIVM_H
#define MINIVM_H


//---------------------------------------------------------
// MACRO DEFINITIONS:


// Size of the global function pointer table
#define MVM_NUM_FUNS 256

#define MVM_NUM_REGISTERS 16 // Default


//---------------------------------------------------------
// DATA STRUCTURES & TYPEDEFS:

struct VMContext;

typedef bool (*FunPtr)(struct VMContext* ctx, const uint32_t);

typedef uint32_t Reg;
//CHANGE THE INTERNALS OF THIS FOR YOUR OWN VM!
//typedef struct Reg {
//    uint32_t type;
//    uint32_t value;
//} Reg;

typedef struct VMContext {
    uint32_t pc;
    uint32_t numRegs;
    uint32_t numFuns;
    Reg* r;           // Ptr to register array.
    FunPtr* funtable; // Ptr to a funptr table.
    uint32_t numInstrs;
    uint32_t * code;
    uint32_t heapSz;
    uint8_t * heap;
} VMContext;


//---------------------------------------------------------
// ESOTERIC ITEMS:


#ifdef MVM_GLOBAL_FUNTABLE
// The global function pointer table.
static FunPtr mvm_function_table[MVM_NUM_FUNS];
#endif

// Byte extraction macros.
#define EXTRACT_B0(i) (i & 0xFF)
#define EXTRACT_B1(i) ((i >> 8) & 0xFF)
#define EXTRACT_B2(i) ((i >> 16) & 0xFF)
#define EXTRACT_B3(i) ((i >> 24) & 0xFF)


//---------------------------------------------------------
// FUNCTIONS:

// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                                Reg* registers,
                             FunPtr* funtable,
                            uint32_t num_instrs,
                          uint32_t * code,
                            uint32_t heapSz,
                           uint8_t * heap);

// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> bool
bool stepVMContext(struct VMContext* ctx);


//---------------------------------------------------------
#endif //ifndef MINIVM_H

bool halt(struct VMContext* ctx, const uint32_t);
bool load(struct VMContext* ctx, const uint32_t);
bool store(struct VMContext* ctx, const uint32_t);
bool move(struct VMContext* ctx, const uint32_t);
bool puti(struct VMContext* ctx, const uint32_t);
bool add(struct VMContext* ctx, const uint32_t);
bool sub(struct VMContext* ctx, const uint32_t);
bool gt(struct VMContext* ctx, const uint32_t);
bool ge(struct VMContext* ctx, const uint32_t);
bool eq(struct VMContext* ctx, const uint32_t);
bool ite(struct VMContext* ctx, const uint32_t);
bool jump(struct VMContext* ctx, const uint32_t);
