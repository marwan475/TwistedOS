#ifndef KERNEL_H
#define KERNEL_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI 0x20

#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10

#define HEAP_MEMORY 0x100000

static inline void panic() {
    asm volatile (
        "cli\n"   // Clear Interrupt Flag (disable interrupts)
        "hlt"     // Halt the CPU
    );
}

static inline void enableinterrupts() {
    asm volatile (
        "sti"  // Set Interrupt Flag (enable interrupts)
    );
}

#endif
