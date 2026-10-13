[BITS 32]

[GLOBAL memzero] ;: (void* dst, size_t n) -> void* dst
memzeroe:
  push edi 
  mov edi, dword [esp + 8]
  mov ecx, dword [esp + 12]
  xor eax, eax
  rep stosb
  pop edi
  mov eax, dword [esp + 4]
  ret