[BITS 16]

[ORG 0x0000]

cli

;STACK SETUP
xor ax, ax
mov ss, ax
mov sp, 0x7C00

;DATA SETUP
mov ax, 0x7C0
mov ds, ax

mov ah, 0x0E
mov bx, msg
hello_loop:
  mov al, byte [bx]
  test al, al
  je end
  inc bx
  int 0x10
jmp hello_loop
end:

sti
infloop:
  jmp infloop

msg: db "hello bios", 0x0A, 0x0D, 0

times 510-($-$$) db 0
dw 0xAA55
