typedef unsigned int u32;
_Static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
typedef short unsigned int u16;
_Static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
typedef char u8;
_Static_assert(sizeof(u8) == 1, "u8 must be 1 bytes");