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

typedef uint32 pt_entry;

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
};

typedef uint32 pd_entry;

typedef uint32 physicaladdr;
typedef uint32 virtualaddr;

// Virtual Adress Translation
#define DIR_INDEX(x) (((x) >> 22) & 0x3ff)
#define TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PHYS_ADDR(x) (*x & ~0xfff)

#define PAGE_SIZE 4096

// number of pages per table and number of tables per dir
#define PAGES 1024
#define TABLES 1024

struct pagetable {pt_entry entries[PAGES];};
struct pagedir {pd_entry entries[TABLES];};

int pagealloc(pt_entry* entry);
void pagefree(pt_entry* entry);
pt_entry* ptablelookup(struct pagetable* p, virtualaddr addr);

pd_entry* pdirlookup(struct pagedir* p, virtualaddr addr);
struct pagedir* gettable();
void flush(virtualaddr addr);
void mapPage(void* physical, void* virtuala);
void virtualinit();

#endif
