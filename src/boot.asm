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

mov bx, 0     ; start
mov si, N     ; counter

mov ch, 0x00  ; cylinder 0
mov dh, 0x00  ; head 0
mov cl, 0x02  ; sector 2
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

      mov di, 5       ; error counter
      again:
        mov ah, 0x02  ; read sector
        mov al, 0x01  ; read 1 sector
        int 0x13
        jnc success
          dec di
          jnz fail
          jmp again
      success:

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

push suc_msg
call print

end:
sti
infloop:
  jmp infloop

fail:
  push err_msg
  call print
  jmp end 

print:
  pop ax
  pop si 
  push ax
  mov ah, 0x0E
  print_loop:
    mov al, byte [si]
    test al, al
    je print_end
    inc si
    int 0x10
    jmp print_loop
  print_end:
  ret

err_msg: db "somthing went wrong", 0x0A, 0x0D, 0
suc_msg: db "kernel loaded succes", 0x0A, 0x0D, 0

times 510-($-$$) db 0
dw 0xAA55
