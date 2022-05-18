#include "Phase2.h"
#include <stdlib.h>
#include "Instruction.h"
#include <stdio.h>
#include "ArrayList.h"
#include <string.h>

void resolve_addresses(struct ArrayList *unresolved, uint32_t first_pc, struct ArrayList *resolved) {
    int MAX_LABEL_LEN = 64;
    char **labels = malloc(size(unresolved) * sizeof(char*));
    // each label will be at index i -> at address first_pc + i * 4
    for(int i = 0; i < size(unresolved); i++) {
        labels[i] = malloc(sizeof(char) * MAX_LABEL_LEN);
        strncpy(labels[i], get(unresolved,i).label,MAX_LABEL_LEN);
    }
    uint32_t instrAddr = first_pc;
    for(int i = 0; i < size(unresolved); i++) {
        struct Instruction curr = get(unresolved, i);
        if(strcmp(curr.branch_label, "") == 0){
            addLast(resolved, curr);
            instrAddr += 4;
        }
        else{
            int j = 0;
            char *curr_lab = labels[j];
            // get the index of the label curr is branching to
            while(strcmp(curr_lab, curr.branch_label) != 0){
                j++;
                curr_lab = labels[j];
            }
            uint32_t labAddr = first_pc + (j * 4);
            
            instrAddr += 4;
            
            signed int tempLabAddr = labAddr;
            signed int tempInstrAddr = instrAddr;
            
            signed int imm = (tempLabAddr - tempInstrAddr)>>2;
            
            int32_t realImm = imm;
            
            addLast(resolved, newInstruction(curr.instruction_id, curr.rd, curr.rs, curr.rt, realImm,
                                             curr.jump_address, curr.shift_amount, curr.label, ""));
        }
    }
    for(int i = 0; i < size(unresolved); i++){
        free(labels[i]);
    }
    free(labels);
}

