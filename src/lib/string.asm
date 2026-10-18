[BITS 32]

[GLOBAL memmove] ;: (void* dst, void* src, size_t n) -> void* dst
memmove:
  push esi
  push edi 
  mov edi, dword [esp + 12]
  mov esi, dword [esp + 16] ; можно по несколько копировать 
  mov ecx, dword [esp + 20]
  cmp edi, esi
  je .done
  ja .backward
.forward:
  cld 
  rep movsb
  jmp .done
.backward:
  std 
  lea esi, [esi + ecx - 1]
  lea edi, [edi + ecx - 1]
  rep movsb
.done:
  cld
  pop edi 
  pop esi
  mov eax, dword [esp + 4]
  ret

[GLOBAL memzero] ;: (void* dst, size_t n) -> void* dst
memzero:
  push edi 
  mov edi, dword [esp + 8]
  mov ecx, dword [esp + 12]
  xor eax, eax
  rep stosb
  pop edi
  mov eax, dword [esp + 4]
  ret