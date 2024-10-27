bits 32

; defining interupt service routines for each interupt in the interupt table

extern ISRHANDLER

%macro ISR 1

global ISR%1:
ISR%1:
  push 0
  push %1
  jmp ISRCOMMON

%endmacro

%macro ISRERROR 1

global ISR%1:
ISR%1:
  push %1
  jmp ISRCOMMON

%endmacro

; for ISRs we save current state, call the handler, then restore state and end interupt with iret
ISRCOMMON:
  pusha ; push all the use registers
  
  xor eax, eax ; push data segment
  mov ax, ds
  push eax

  mov ax, 0x10 ; use kernel data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  push esp ; pass stack pointer to C function

  call ISRHANDLER

  add esp, 4

  ; restore old registers and segments
  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa
  add esp, 8 ; remove error code and interupt number
  iret ; interupt is done


  

