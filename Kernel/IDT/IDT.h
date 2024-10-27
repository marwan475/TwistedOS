#ifndef IDT_H
#define IDT_H

#include "../headers/kernel.h"

// Structure for an IDT entry
typedef struct {
    uint16 baselow;        // Lower 16 bits of the base address
    uint16 segment;       // Segment selector
    uint8 reserved;       // Reserved, must be zero
    uint8 flags;          // Flags for the entry
    uint16 basehigh;      // Upper 16 bits of the base address
} __attribute__((packed)) IDTentry;

// Structure for the IDT description
typedef struct {
    uint16 limit;         // Size of the IDT - 1
    IDTentry* ptr;       // Pointer to the IDT entries
} __attribute__((packed)) IDTdescription;

// Enum for IDT entry flags
typedef enum {
    IDT_FLAG_GATE_TASK = 0x5,              // Task Gate
    IDT_FLAG_GATE_16BIT_INT = 0x6,        // 16-bit Interrupt Gate
    IDT_FLAG_GATE_16BIT_TRAP = 0x7,       // 16-bit Trap Gate
    IDT_FLAG_GATE_32BIT_INT = 0xE,        // 32-bit Interrupt Gate
    IDT_FLAG_GATE_32BIT_TRAP = 0xF,       // 32-bit Trap Gate

    IDT_FLAG_RING0 = (0 << 5),             // Ring 0 (highest privilege)
    IDT_FLAG_RING1 = (1 << 5),             // Ring 1
    IDT_FLAG_RING2 = (2 << 5),             // Ring 2
    IDT_FLAG_RING3 = (3 << 5),             // Ring 3 (lowest privilege)

    IDT_FLAG_PRESENT = 0x80,                // Present flag
} IDT_FLAGS;

IDTentry IDT[256];

IDTdescription IDTd = {sizeof(IDT) - 1, IDT};

// Inline assembly function to load the IDT
static inline void LoadIDT(IDTdescription* IDTd) {
    __asm__ __volatile__ (
        "push %%ebp\n\t"                // Save base pointer
        "mov %%esp, %%ebp\n\t"          // Set base pointer to stack pointer
        "mov %0, %%eax\n\t"             // Load idt_ptr into eax
        "lidt (%%eax)\n\t"              // Load IDT from the address in eax
        "mov %%ebp, %%esp\n\t"          // Restore stack pointer
        "pop %%ebp\n\t"                 // Restore base pointer
        :                               // No outputs
        : "r"(IDTd)                  // Input: idt_ptr as a general-purpose register
        : "eax", "memory"               // Clobbered registers
    );
}

void SetIDTEntry(int interupt, void* base,uint16 segmentd,uint8 flags){
  IDT[interupt].baselow = ((uint32)base) & 0xFFFF;
  IDT[interupt].segment = segmentd;
  IDT[interupt].reserved = 0;
  IDT[interupt].flags =flags;
  IDT[interupt].basehigh = ((uint32)base >> 16) & 0xFFFF;
}

void EnableIDTEntry(int interupt){
    FLAG_SET(IDT[interupt].flags,IDT_FLAG_PRESENT);
}

void DisableIDTEntry(int interupt){
    FLAG_UNSET(IDT[interupt].flags,IDT_FLAG_PRESENT);
}

void initIDT(){
    LoadIDT(&IDTd);
}

#endif