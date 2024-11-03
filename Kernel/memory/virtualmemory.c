#include "virtualmemory.h"
#include "physicalmemory.h"

// adds flag to page dir entry
void pd_setflag(pd_entry* entry, uint32 flag){*entry |= flag;}
// clear flag in page dir entry
void pd_delflag(pd_entry* entry, uint32 flag){*entry &= ~flag;}
// sets a frame to page dir entry
void pd_setframe(pd_entry* entry, physicaladdr addr){*entry = (*entry & ~TFRAME) | addr;}
// test if table is present in memory
int pd_present(pd_entry entry){return entry & TPRESENT;}
// test if table is in usermode
int pd_user(pd_entry entry){return entry & TUSER;}
// test if table is 4mb
int pd_4mb(pd_entry entry){return entry & TMB;}
// test if table is writable
int pd_write(pd_entry entry){return entry & TWRITABLE;}
// gets the table addr
physicaladdr pd_frame(pd_entry entry){return entry & TFRAME;}

// adds flag to page table enty
void pt_setflag(pt_entry* entry, uint32 flag){*entry |= flag;}
// clears flag in page table entry
void pt_delflag(pt_entry* entry, uint32 flag){*entry &= ~flag;}
// sets frame to page entry
void pt_setframe(pt_entry* entry, physicaladdr addr){*entry = (*entry & ~PFRAME) | addr;}
// test if page is present
int pt_present(pt_entry entry){return entry & PPRESENT;}
// test if page is writable
int pt_write(pt_entry entry){return entry & PWRITE;}
// get page frame
physicaladdr pt_frame(pd_entry entry){return entry & PFRAME;}

struct pagedir *curdir = 0;

// Allocates memory for one page and sets the entry frame
int pagealloc(pt_entry* entry){  
  void * p = (void*)kernelmalloc(PAGE_SIZE);
  pt_setframe(entry,(physicaladdr)p);
  pt_setflag(entry,TPRESENT);

  return 1;
}

// frees page frome entry and sets to not present
void pagefree(pt_entry* entry){
  uint8* p = (uint8*) pt_frame(*entry);
  kernelfree(p,PAGE_SIZE);
  pt_delflag(entry,TPRESENT);
}

// returns the entry in pagetable give the virtual address
pt_entry* ptablelookup(struct pagetable* p, virtualaddr addr){
  return &(p->entries[TABLE_INDEX(addr)]);
}

// returns the entry in page dir given the virtual address
pd_entry* pagedirlookup(struct pagedir* p, virtualaddr addr){
  return &(p->entries[DIR_INDEX(addr)]);
}

int setpdir(physicaladdr p){
  asm volatile (
        "mov %0, %%cr3" // Load the page directory address into CR3
        :                      
        : "r" (p) // Input operand: page_directory
        : "memory"             // Clobber memory to avoid reordering issues
    );

  curdir = (struct pagedir*) p;
}

void flush(virtualaddr addr){
    asm volatile (
        "cli;"                  // Clear interrupts (disable interrupts)
        "invlpg (%0);"         // Invalidate the TLB entry for the specified address
        "sti;"                  // Set interrupts (enable interrupts)
        :
        : "r" (addr)           // Input operand: addr
        : "memory"             // Clobber memory to avoid reordering
    );
}

void mapPage(void* physical, void* virtuala){
  struct pagedir* p = curdir;

  // get page table
  pd_entry* entry = &(p->entries[DIR_INDEX((virtualaddr) virtuala)]);

  // if table not present we need to allocate it
  if ((*entry & TPRESENT) != TPRESENT){
    struct pagetable* t = (struct pagetable*) kernelmalloc(PAGE_SIZE);

    kernelmemset((uint8*)t,PAGE_SIZE,0);

    pd_entry* entryd = &(p->entries[DIR_INDEX((virtualaddr) virtuala)]);

    pd_setflag(entryd, TPRESENT);
    pd_setflag(entryd, TWRITABLE);
    pd_setframe(entryd, (physicaladdr)t);

  }

  struct pagetable* table = (struct pagetable*) PHYS_ADDR(entry);

  pt_entry* page =  &(table->entries[TABLE_INDEX((virtualaddr)virtuala)]);

  pt_setframe(page, (physicaladdr) physical);
  pt_setflag(page,PPRESENT);

}

void enablepaging() {
    unsigned int cr0;

    // Read CR0 into a variable
    asm volatile ("mov %%cr0, %0" : "=r" (cr0));

    // Set bit 31 to enable paging
    cr0 |= 0x80000000;

    // Write the modified CR0 back
    asm volatile ("mov %0, %%cr0" : : "r" (cr0) : "memory");
}

void virtualinit(){
  
  //0-4mb
  struct pagetable* table4mb = (struct pagetable*) kernelmalloc(PAGE_SIZE);

  kernelmemset((uint8*)table4mb,PAGE_SIZE,0);

  //4-8mb
  struct pagetable* table8mb = (struct pagetable*) kernelmalloc(PAGE_SIZE);
 
  kernelmemset((uint8*)table8mb,PAGE_SIZE,0);

  //8-12mb
  struct pagetable* table12mb = (struct pagetable*) kernelmalloc(PAGE_SIZE);
  
  kernelmemset((uint8*)table12mb,PAGE_SIZE,0);

  //12-16mb
  struct pagetable* table16mb = (struct pagetable*) kernelmalloc(PAGE_SIZE);

  kernelmemset((uint8*)table16mb,PAGE_SIZE,0);

  //Identity maping first 16mb
  
  for (int i = 0, phys = 0x0, virt = 0x00000000;i<1024; i++, phys+=4096, virt+= 4096){
    pt_entry page = 0;
    pt_setflag(&page,PPRESENT);
    pt_setframe(&page, (physicaladdr) phys);

    table4mb->entries[TABLE_INDEX(virt)] = page;
  }

  for (int i = 0, phys = 0x400000, virt = 0x00400000;i<1024; i++, phys+=4096, virt+= 4096){
    pt_entry page = 0;
    pt_setflag(&page,PPRESENT);
    pt_setframe(&page, (physicaladdr) phys);

    table8mb->entries[TABLE_INDEX(virt)] = page;
  }

  for (int i = 0, phys = 0x800000, virt = 0x00800000;i<1024; i++, phys+=4096, virt+= 4096){
    pt_entry page = 0;
    pt_setflag(&page,PPRESENT);
    pt_setframe(&page, (physicaladdr) phys);

    table12mb->entries[TABLE_INDEX(virt)] = page;
  }

  for (int i = 0, phys = 0xC00000, virt = 0x00C00000;i<1024; i++, phys+=4096, virt+= 4096){
    pt_entry page = 0;
    pt_setflag(&page,PPRESENT);
    pt_setframe(&page, (physicaladdr) phys);

    table16mb->entries[TABLE_INDEX(virt)] = page;
  }

  // page dir
  struct pagedir* dir = (struct pagedir*) kernelmalloc(PAGE_SIZE);

  kernelmemset((uint8*)dir,PAGE_SIZE,0);

  pd_entry* entry4mb = &dir->entries[DIR_INDEX(0x00000000)];
  pd_setflag(entry4mb,TPRESENT);
  pd_setflag(entry4mb,TWRITABLE);
  pd_setframe(entry4mb,(physicaladdr)table4mb);

  pd_entry* entry8mb = &dir->entries[DIR_INDEX(0x00400000)];
  pd_setflag(entry8mb,TPRESENT);
  pd_setflag(entry8mb,TWRITABLE);
  pd_setframe(entry8mb,(physicaladdr)table8mb);

  pd_entry* entry12mb = &dir->entries[DIR_INDEX(0x00800000)];
  pd_setflag(entry12mb,TPRESENT);
  pd_setflag(entry12mb,TWRITABLE);
  pd_setframe(entry12mb,(physicaladdr)table12mb);

  pd_entry* entry16mb = &dir->entries[DIR_INDEX(0x00C00000)];
  pd_setflag(entry16mb,TPRESENT);
  pd_setflag(entry16mb,TWRITABLE);
  pd_setframe(entry16mb,(physicaladdr)table16mb);

  // set the page dir
  setpdir((physicaladdr)(&dir->entries));

  enablepaging();
}

