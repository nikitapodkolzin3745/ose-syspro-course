[GLOBAL do_cli]
do_cli:
  cli
  ret

[GLOBAL do_lidt]
do_lidt:
  mov eax, [esp+4]
  lidt [eax]
  ret

CODE equ 0x08
DATA equ 0x10

static_flags:
  db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 1
db 0
db 1
db 1
db 1
db 1
db 1
db 0
db 0
db 1
db 0
db 0
db 0
db 1
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 1
db 1
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0
db 0

[GLOBAL collect_context]
collect_context:
    push eax
    mov eax, [esp+4]
    cmp byte [static_flags + eax], 1
    pop eax
    je .has_error_code

.no_error_code:
  push dword [esp]
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