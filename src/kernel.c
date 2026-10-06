void infloop();

void kernel() {
    // *((short int*) 0xB8000) = 0;
    short int *vga = (short int*) 0xB8000 + 1000;
    vga[0] = 0x4F4B;
    vga[1] = 0x2F45;
    vga[2] = 0x1F52;
    vga[3] = 0x2F4E;
    vga[4] = 0x4F45;
    vga[5] = 0x2F4C;
    infloop();
}