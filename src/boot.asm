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

;EXTRA SETUP
mov ax, 0x7E0
mov es, ax

mov ah, 0x02    ; read sector
mov al, 0x01    ; read 1 sector
mov bx, 0       ; start
mov si, N       ; counter

mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
jmp init

cylinder_loop:
  cmp ch, 80
  je cylinder_end

  mov dh, 0x00
  head_loop:
    cmp dh, 2
    je head_end
      
    mov cl, 0x01
    init:
    sector_loop:
      cmp cl, 19
      je sector_end

      cmp si, 0
      je cylinder_end

      int 0x13
      mov ah, 0x02
      mov al, 0x01

      add bx, 512
      dec si

      inc cl
      jmp sector_loop
    sector_end:

    inc dh 
    jmp head_loop
  head_end:

  inc ch
  jmp cylinder_loop
cylinder_end:


sti
infloop:
  jmp infloop

times 510-($-$$) db 0
dw 0xAA55
