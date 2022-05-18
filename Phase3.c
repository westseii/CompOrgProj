#include "Phase3.h"
#include <stdio.h>

/*
I-Types   op
addiu     9
beq       4
bne       5
lui       F
ori       D

R-Types   op    funct
addu      0     21
or        0     25
sll       0     00
slt       0     2A
 */

uint32_t translate_I(struct Instruction e);
uint32_t translate_R(struct Instruction e);

void translate_instructions(struct ArrayList *tals, uint32_t machineCode[]) {
  // printf("TEST\n");

  for (int i = 0; i < size(tals); i++) {
    struct Instruction tal = get(tals, i);

    // printf("scott %d: ", i + 1);

    // if I-Type instruction
    if (tal.instruction_id == addiu || tal.instruction_id == beq ||
        tal.instruction_id == bne || tal.instruction_id == lui ||
        tal.instruction_id == ori) {

      // translate/add I-Type instruction to output array
      machineCode[i] = translate_I(tal);
    }

    // if R-Type instruction
    else if (tal.instruction_id == addu || tal.instruction_id == or ||
             tal.instruction_id == sll || tal.instruction_id == slt) {

      // translate/add R-Type instruction to output array
      machineCode[i] = translate_R(tal);
    }
  }

  // printf("\n");
}

// translate I-Type
uint32_t translate_I(struct Instruction tal) {
  uint32_t machC = 0b0;

  // opcode
  if (tal.instruction_id == addiu)
    machC |= 0x9;
  else if (tal.instruction_id == beq)
    machC |= 0x4;
  else if (tal.instruction_id == bne)
    machC |= 0x5;
  else if (tal.instruction_id == lui)
    machC |= 0xF;
  else if (tal.instruction_id == ori)
    machC |= 0xD;

  // rs
  machC <<= 5;
  machC |= tal.rs;

  // rt
  machC <<= 5;
  machC |= tal.rt;

  // immediate/address
  machC <<= 16;

  if (tal.immediate >= 0x0) {
    machC |= tal.immediate;
  } else {
    // printf("has negative immediate ");
    tal.immediate ^= 0xFFFF0000;
    machC |= tal.immediate;
  }

  // printf("0x%08x\n", machC);
  return machC;
}

// translate R-Type
uint32_t translate_R(struct Instruction tal) {
  uint32_t machC = 0b0;

  // opcode
  machC |= 0b000000; // redundant, opcode always 0b000000. kept for clarity

  // rs
  machC <<= 5;
  machC |= tal.rs;
  // rt
  machC <<= 5;
  machC |= tal.rt;

  // rd
  machC <<= 5;
  machC |= tal.rd;

  // shamt
  machC <<= 5;
  machC |= tal.shift_amount;

  // funct
  machC <<= 6;
  if (tal.instruction_id == addu)
    machC |= 0x21;
  else if (tal.instruction_id == or)
    machC |= 0x25;
  else if (tal.instruction_id == sll)
    machC |= 0x00;
  else if (tal.instruction_id == slt)
    machC |= 0x2A;

  // printf("0x%08x\n", machC);
  return machC;
}
