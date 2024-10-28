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

ISRERROR 8
ISRERROR 10
ISRERROR 11
ISRERROR 12
ISRERROR 13
ISRERROR 14
ISRERROR 17
ISRERROR 21
ISRERROR 29
ISRERROR 30

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 9
SR 15
ISR 16
ISR 18
ISR 19
ISR 20
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 31
ISR 32
ISR 33
ISR 34
ISR 35
ISR 36
ISR 37
ISR 38
ISR 39
ISR 40
ISR 41
ISR 42
ISR 43
ISR 44
ISR 45
ISR 46
ISR 47
ISR 48
ISR 49
ISR 50
ISR 51
ISR 52
ISR 53
ISR 54
ISR 55
ISR 56
ISR 57
ISR 58
ISR 59
ISR 60
ISR 61
ISR 62
ISR 63
ISR 64
ISR 65
ISR 66
ISR 67
ISR 68
ISR 69
ISR 70
ISR 71
ISR 72
ISR 73
ISR 74
ISR 75
ISR 76
ISR 77
ISR 78
ISR 79
ISR 80
ISR 81
ISR 82
ISR 83
ISR 84
ISR 85
ISR 86
ISR 87
ISR 88
ISR 89
ISR 90
ISR 91
ISR 92
ISR 93
ISR 94
ISR 95
ISR 96
ISR 97
ISR 98
ISR 99
ISR 100
ISR 101
ISR 102
ISR 103
ISR 104
ISR 105
ISR 106
ISR 107
ISR 108
ISR 109
ISR 110
ISR 111
ISR 112
ISR 113
ISR 114
ISR 115
ISR 116
ISR 117
ISR 118
ISR 119
ISR 120
ISR 121
ISR 122
ISR 123
ISR 124
ISR 125
ISR 126
ISR 127
ISR 128
ISR 129
ISR 130
ISR 131
ISR 132
ISR 133
ISR 134
ISR 135
ISR 136
ISR 137
ISR 138
ISR 139
ISR 140
ISR 141
ISR 142
ISR 143
ISR 144
ISR 145
ISR 146
ISR 147
ISR 148
ISR 149
ISR 150
ISR 151
ISR 152
ISR 153
ISR 154
ISR 155
ISR 156
ISR 157
ISR 158
ISR 159
ISR 160
ISR 161
ISR 162
ISR 163
ISR 164
ISR 165
ISR 166
ISR 167
ISR 168
ISR 169
ISR 170
ISR 171
ISR 172
ISR 173
ISR 174
ISR 175
ISR 176
ISR 177
ISR 178
ISR 179
ISR 180
ISR 181
ISR 182
ISR 183
ISR 184
ISR 185
ISR 186
ISR 187
ISR 188
ISR 189
ISR 190
ISR 191
ISR 192
ISR 193
ISR 194
ISR 195
ISR 196
ISR 197
ISR 198
ISR 199
ISR 200
ISR 201
ISR 202
ISR 203
ISR 204
ISR 205
ISR 206
ISR 207
ISR 208
ISR 209
ISR 210
ISR 211
ISR 212
ISR 213
ISR 214
ISR 215
ISR 216
ISR 217
ISR 218
ISR 219
ISR 220
ISR 221
ISR 222
ISR 223
ISR 224
ISR 225
ISR 226
ISR 227
ISR 228
ISR 229
ISR 230
ISR 231
ISR 232
ISR 233
ISR 234
ISR 235
ISR 236
ISR 237
ISR 238
ISR 239
ISR 240
ISR 241
ISR 242
ISR 243
ISR 244
ISR 245
ISR 246
ISR 247
ISR 248
ISR 249
ISR 250
ISR 251
ISR 252
ISR 253
ISR 254
ISR 255


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


  

