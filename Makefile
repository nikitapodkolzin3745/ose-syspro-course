build/kernel.o: src/kernel.c
	mkdir -p build
	gcc -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector -c src/kernel.c -o build/kernel.o

build/kernel_probe.bin: build/kernel.o
	ld -m elf_i386 -Ttext 0x7E00 --unresolved-symbols=ignore-all --entry=0 build/kernel.o -o build/kernel_probe.elf
	objcopy -O binary build/kernel_probe.elf build/kernel_probe.bin

build/boot.o: src/boot.asm build/kernel_probe.bin
	nasm -f elf32 -dSECTORS=$$(( ($$(stat -c %s build/kernel_probe.bin) + 511) / 512 )) src/boot.asm -o build/boot.o

build/os.bin: build/kernel.o build/boot.o src/link.ld
	ld -m elf_i386 build/boot.o build/kernel.o -T src/link.ld -o build/os.elf
	objcopy -I elf32-i386 -O binary build/os.elf build/os.bin

build/disk.img: build/os.bin
	dd if=/dev/zero of=build/disk.img bs=512 count=2880
	dd if=build/os.bin of=build/disk.img conv=notrunc

build: build/disk.img

run: build
	qemu-system-i386 -cpu pentium2 -m 1g -drive file=build/disk.img,format=raw,if=ide -device VGA

clean:
	rm -rf build