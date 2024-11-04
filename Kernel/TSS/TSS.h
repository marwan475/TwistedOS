#ifndef TSS_H
#define TSS_H

// stores state of machine before task switch
struct tss_entry{
	uint32 prev;
	uint32 esp0;
	uint32 ss0;
	uint32 esp1;
	uint32 ss1;
	uint32 esp2;
	uint32 ss2;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint32 es;
	uint32 cs;
	uint32 ss;
	uint32 ds;
	uint32 fs;
	uint32 gs;
	uint32 ldt;
	uint16 trap;
	uint16 iomap;
};

void flush_tss(uint16 sel) {
    __asm__ __volatile__("ltr %0" : : "r"(sel));
}

void tss_set_stack () {
	
	uint32 base = (uint32) 0x79FF;

	struct tss_entry* TSS = (struct tss_entry*)base;

	uint32 stack;

	// Inline assembly to get the current value of ESP
	__asm__ volatile (
		"mov %%esp, %0"   // Move ESP into the variable 'stack'
		: "=r" (stack)    // Output operand
	);

	TSS->ss0 = 0x10;
	TSS->esp0 = stack;
}


void instalTSS(){
	uint32 base = (uint32) 0x79FF;

	kernelmemset((uint8*)base,sizeof(struct tss_entry),0);

	struct tss_entry* TSS = (struct tss_entry*)base;

	TSS->ss0 = 0x10;
	TSS->esp0 = 0xFFFF;

	TSS->cs = 0x0B;
	TSS->ss = 0x13;
	TSS->es = 0x13;
	TSS->ds = 0x13;
	TSS->fs = 0x13;
	TSS->gs = 0x13;

	flush_tss(0x3B); //7th gdt entry id * size of gdt discriptor

}

#endif
