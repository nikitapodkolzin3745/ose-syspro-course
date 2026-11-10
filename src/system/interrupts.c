#include <stddef.h>
#include "lib/allocator.h"
#include "lib/types.h"
#include "lib/kernel_panic.h"

#pragma pack(push, 1)
typedef struct {
  u16 limit_0_15   : 16;
  u32 base_0_23    : 24;
  u8  type         :  4;
  u8  S            :  1;
  u8  DPL          :  2;
  u8  P            :  1;
  u8  limit_16_19  :  4;
  u8  available_52 :  1;
  u8  L            :  1;
  u8  D_B          :  1;
  u8  G            :  1;
  u8  base_24_31   :  8;
} segment_descriptor;
#pragma pack(pop)
_Static_assert(sizeof(segment_descriptor) == 8, "segment_descriptor must be 8 bytes");

#pragma pack(push, 1)
typedef struct {
    u16 offset_0_15         : 16;
    u16 segment_selector    : 16;
    u8  reserved            :  5;
    u8  fixed_123           :  3;
    u8  Type                :  3;
    u8  D                   :  1;
    u8  fixed_4             :  1;
    u8  DPL                 :  2;
    u8  P                   :  1;
    u16 offset_16_31        : 16;
} interrupt_descriptor;
#pragma pack(pop)
_Static_assert(sizeof(interrupt_descriptor) == 8, "interrupt_descriptor must be 8 bytes");

#pragma pack(push, 1)
typedef struct {
    u16 limit;
    u32 base;
} pseudo_descriptor;
#pragma pack(pop)
_Static_assert(sizeof(pseudo_descriptor) == 6, "pseudo_descriptor must be 6 bytes");

typedef struct {
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 gs, fs, es, ds;
    u32 vector, error_code; 
    u32 eip, cs, eflags;
} context;

extern void collect_context(); 
void do_lidt(pseudo_descriptor*);

void init_interrupts() {
    interrupt_descriptor *IDT = malloc_immortal(256 * sizeof(interrupt_descriptor), 8);

    for (size_t i = 0; i < 256; i++) {
        char *trampoline = malloc_immortal(10, 1);

        // push imm32
        // 0x68 xx xx xx xx
        trampoline[0] = 0x68;
        *(u32*)(trampoline + 1) = (u32) i;

        // jmp rel32
        // 0xE9 xx xx xx xx
        trampoline[5] = 0xE9;
        *(u32*)(trampoline + 6) = (u32)collect_context - (u32)(trampoline + 10);

        IDT[i].offset_0_15 = (u16)((u32)trampoline);
        IDT[i].segment_selector = 0x08;
        IDT[i].fixed_123 = 0;
        IDT[i].Type = 0b110; // Interrupt Gate
        IDT[i].D = 1;
        IDT[i].fixed_4 = 0;
        IDT[i].DPL = 0b00;
        IDT[i].P = 1;
        IDT[i].offset_16_31 = (u16)((u32)trampoline >> 16);
    }

    pseudo_descriptor IDTR = {
        .limit = 256 * sizeof(interrupt_descriptor) - 1,
        .base = (u32)IDT,
    };
    do_lidt(&IDTR);
}

void universal_handler(context* ctx) {
    kernel_panic(
        "unhandled interrupt: #%d at 0x%x:0x%x\n"
        "\n"
        "Registers:\n"
        "\t EAX: 0x%x, ECX: 0x%x, EDX: 0x%x, EBX: 0x%x\n"
        "\t ESP: 0x%x, EBP: 0x%x, ESI: 0x%x, EDI: 0x%x\n"
        "\t DS : 0x%x, ES : 0x%x, FS : 0x%x, GS : 0x%x\n"
        "\n"
        "Error code: 0x%x\n"
        "\n"
        "EFLAGS: 0x%x", 
        ctx->vector, ctx->cs, ctx->eip, 
        ctx->eax, ctx->ecx, ctx->edx, ctx->ebx, 
        ctx->esp, ctx->ebp, ctx->esi, ctx->edi, 
        ctx->ds, ctx->es, ctx->fs, ctx->gs, 
        ctx->error_code, 
        ctx->eflags
    );
}