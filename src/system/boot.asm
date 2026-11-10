[BITS 16]

cli

;STACK SETUP
xor ax, ax
mov ss, ax
mov sp, 0x7C00

;DATA SETUP
; mov ax, 0x7C0
xor ax, ax
mov ds, ax

; mov [boot_drive], dl

loop:
    mov di, 5
  retry:
    mov si, dap
    mov ah, 0x42
    ; mov dl, [boot_drive]

    int 0x13

    jnc succes
    dec di
    jnz retry
    jmp error
  succes:

    inc word [lba_address]
    add word [buffer_offset], 512

    jnc no_overflow
      add word [buffer_segment], 0x1000
    no_overflow:

    cmp word [lba_address], SECTORS
    jbe loop

CODE equ 0x08
DATA equ 0x10

mov ax, 0x2401
int 0x15

cld

lgdt [gdt_descriptor]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp dword CODE:next
[BITS 32]
next:

cli

mov ax, DATA
mov ds, ax
mov ss, ax
mov es, ax 
mov fs, ax 
mov gs, ax

[EXTERN kernel]
call kernel

error:

[GLOBAL infloop]
infloop:
  jmp infloop

[GLOBAL do_cli]
do_cli:
  cli
  ret

[GLOBAL do_lidt]
do_lidt:
  mov eax, [esp+4]
  lidt [eax]
  ret

[GLOBAL collect_context]
collect_context:
  cmp dword [esp], 8
  je .has_error_code
  cmp dword [esp], 10
  jb .no_error_code
  cmp dword [esp], 14
  jbe .has_error_code
  cmp dword [esp], 17
  je .has_error_code
  cmp dword [esp], 21
  je .has_error_code
  cmp dword [esp], 29
  je .has_error_code
  cmp dword [esp], 30
  je .has_error_code
  
.no_error_code:
  push dword 0
  xchg eax, dword [esp]
  xchg eax, dword [esp+4]
  xchg eax, dword [esp]
.has_error_code:

  push ds
  push es
  push fs
  push gs
  pusha

  mov ebx, esp
  
  and esp, -16
  sub esp, 12

  push ebx

  cli
  mov ax, DATA
  mov ds, ax
  mov ss, ax
  mov es, ax 
  mov fs, ax 
  mov gs, ax

  [EXTERN universal_handler]
  call universal_handler

align 4
dap:
  db 0x10
  db 0
  dw 1
buffer_offset:
  dw 0
buffer_segment:
  dw 0x7E0
lba_address:
  dq 1

gdt_descriptor:
  dw 0x17
  dd gdt

align 8
gdt:
; EMPTY SEGMENT
  dq 0
; CODE SEGMENT
  dw 0xFFFF   ; limit
  dw 0x0000   ; base ---------------------------------------
  db 0x00     ; base | P |   DPL   |  S  | E | DC | RW | A |
  db 0x9A     ;      | 1 |  0  | 0 |  1  | 1 | 0  | 1  | 0 |
  db 0xCF     ;      | 1 |  1  | 0 |  0  | 1 | 1  | 1  | 1 |
  db 0x00     ;      | G | B/D | L | AVL |     LIMIT       |
; DATA SEGMET ;      ---------------------------------------
  dw 0xFFFF   ; limit
  dw 0x0000   ; base ---------------------------------------
  db 0x00     ; base | P |   DPL   |  S  | E | DC | RW | A |
  db 0x92     ;      | 1 |  0  | 0 |  1  | 0 | 0  | 1  | 0 |
  db 0xCF     ;      | 1 |  1  | 0 |  0  | 1 | 1  | 1  | 1 |
  db 0x00     ;      | G | B/D | L | AVL |     LIMIT       |
              ;      ---------------------------------------
times 510-($-$$) db 0
dw 0xAA55
