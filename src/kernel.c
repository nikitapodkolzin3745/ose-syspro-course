void infloop();

void kernel() {
    *((short int*)0xb8000) = 0;
    infloop();
}