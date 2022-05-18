#include "Phase1.h"
#include <stdio.h>
#include "ArrayList.h"
#include "Instruction.h"
#include "InstructionFactory.h"

void mal_to_tal(struct ArrayList *mals, struct ArrayList *tals) {
    for(int i = 0; i < size(mals); i++) {
        struct Instruction curr = get(mals,i);
        if(curr.instruction_id == addiu ) {
            if(curr.immediate > 32767 || curr.immediate < -32768) { // immediate value greater than 16 bits
                if(curr.instruction_id == addiu){ // add the upper 16 bits, then shift left, then add the lower 16 bits
                    unsigned int currImm = curr.immediate;
                    int32_t upper = currImm/0x10000;  // get the upper 2 bytes of the immediate
                    int32_t lower = currImm%0x10000;  // get the lower 2 bytes of the immediate
                    struct Instruction loadStep = CreateLui_lab(1, upper, curr.label);
                    struct Instruction oriStep = CreateOri(1, 1, lower);
                    struct Instruction addStep = CreateAddu(curr.rt, curr.rs, 1);
                    addLast(tals, loadStep);
                    addLast(tals, oriStep);
                    addLast(tals, addStep);
                }
            }
            else {
                addLast(tals, curr);
            }
        }
        else if(curr.instruction_id == ori){
            unsigned int currImm = curr.immediate;
            if(currImm > 65535) {
                int32_t upper = (currImm / 0x10000) & 0x0000FFFF;  // get the upper 2 bytes of the immediate
                int32_t lower = currImm % 0x10000;  // get the lower 2 bytes of the immediate
                struct Instruction loadStep = CreateLui_lab(1, upper, curr.label);
                struct Instruction oriStep = CreateOri(1, 1, lower);
                struct Instruction orStep = CreateOr(curr.rt, curr.rs, 1);
                addLast(tals, loadStep);
                addLast(tals, oriStep);
                addLast(tals, orStep);
            }
            else{
                addLast(tals, curr);
            }
        }
        else if(curr.instruction_id == mov || curr.instruction_id == blt || curr.instruction_id == bge){
            int8_t at = (int8_t) 1;
            int8_t zero = (int8_t) 0;
            //move is just adding with zero register
            if(curr.instruction_id == mov){
                struct Instruction realMoveStep = CreateAddu_lab(curr.rd, zero, curr.rt, curr.label);
                addLast(tals, realMoveStep);
            }
            //blt is doing slt and then comparing to zero
            else if(curr.instruction_id == blt) {
                // assumes this wants rt < rs
                struct Instruction sltStep = CreateSlt_lab(at, curr.rt, curr.rs, curr.label);
                struct Instruction branchStep = CreateBne(at, zero, curr.branch_label);
                addLast(tals, sltStep);
                addLast(tals, branchStep);
            }
            //bge is doing slt and then comparing to zero but the opposite comparison of blt
            else {
                // assumes this wants rt >= rs
                struct Instruction sltStep = CreateSlt_lab(at, curr.rt, curr.rs, curr.label);
                struct Instruction branchStep = CreateBeq(at, zero, curr.branch_label);
                addLast(tals, sltStep);
                addLast(tals, branchStep);
            }
            
        }
        else {
            addLast(tals, curr);
        }
    }
}


/*
 All instructions that are TAL only:
 sll
 addu
 or
 beq
 bne
 slt
 lui
 
 imm dependent:
 addiu
 ori
 
 MAL:
 move
 blt
 bge
 */
