#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include "../headers/kernel.h"
// Page table entry
enum PAGE_FLAGS {
	PPRESENT = 1,
	PWRITABLE = 2,
	PUSER = 4,
	PWRITE = 8,
	PNOTCACHE = 0x10,
	PACCESSED = 0x20,
	PDIRTY = 0x40,
	PPAT = 0x80,
	PCPU_GLOBAL = 0x100,
	PLV4_GLOBAL = 0x200,
	PFRAME = 0x7FFFF000
};

typedef uint32 pt_enrty;

// Page Directory entry
enum PAGE_DIR_FLAGS {
	TPRESENT = 1,
	TWRITABLE = 2,
	TUSER = 4,
	TPWT = 8,
	TPCD = 0x10,
	TACCESSED = 0x20,
	TDIRTY = 0x40,
	TMB = 0x80,
	TCPU_GLOBAL = 0x100,
	TLV4_GLOBAL = 0x200,
	TFRAME = 0x7FFFF000
}

typedef uint32 pd_entry;

typedef uint32 physicaladdr;
typedef uint32 virtualaddr;

#define PAGES 1024
#define TABLES 1024

// Virtual Adress Translation
#define DIR_INDEX(x) (((x) >> 22) & 0x3ff)
#define TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PHYS_ADDR(x) (*x & ~0xfff)

#define PTABLE_SPACE 0x400000
#define DTABLE_SPACE 0x100000000

#define PAGE_SIZE 4096

// number of pages per table and number of tables per dir
#define PAGES 1024
#define TABLES 1024

struct pagetable {pt_entry entries[PAGES]};
struct pagedir {pd_entry entries[TABLES]};

// adds flag to page dir entry
void pd_addflag(pd_entry* entry, uint32 flag){*entry |= flag;}
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
int pd_write(pd_entry entry){return entry & TWRITE;}
// gets the table addr
physicaladdr pd_frame(pd_entry entry){return entry & TFRAME;}

// adds flag to page table enty
void pt_addflag(pt_entry* entry, uint32 flag){*entry |= flag;}
// clears flag in page table entry
void pt_delflag(pt_entry* entry, uint32 flag){*entry &= ~flag;}
// sets frame to page entry
void pt_setframe(pt_entry* entry, physicaladdr addr){*entry = (*entry & ~PFRAME) | addr;}
// test if page is present
int pt_present(pt_entry entry){return entry & PPRESENT;}
// test if page is writable
int pt_write(pt_entry entry){return entry & PWRITE;}
// get page frame
physicaladdr pt_frame(pd_entry entry){return entry PFRAME;}

int pagealloc(pt_entry* entry);
void pagefree(pt_entry* entry);
pt_entry* ptablelookup(struct pagetable* p, virtualaddr addr);

pd_entry* pdirlookup(struct pagedir* p, virtualaddr addr);
int switchptable(struct pagedir* p);
struct pagedir* gettable();
void flush(virtualaddr addr);
void mapPage(void* physical, void* virtuala);
void virtualInit();

#endif
