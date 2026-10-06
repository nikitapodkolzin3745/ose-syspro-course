[BITS 16]

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

mov [boot_drive], dl

.loop:
    mov di, 5
  .retry:
    mov si, DAP
    mov ah, 0x42
    mov dl, [boot_drive]

    int 0x13

    jnc .succes
    dec di
    jnz .retry
    jmp .error
  .succes:

    inc word [lba_address]
    add word [buffer_offset], 512

    jnc .no_overflow
      add word [buffer_segment], 0x1000
    .no_overflow:

    cmp word [lba_address], SECTORS
    jbe .loop

.error:
infloop:
  jmp infloop

boot_drive:
  db 0
align 4
DAP:
  db 0x10
  db 0
  dw 1
buffer_offset:
  dw 0
buffer_segment:
  dw 0x7E0
lba_address:
  dq 1

times 510-($-$$) db 0
dw 0xAA55
