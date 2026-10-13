.DEFAULT_GOAL := build

KERNEL_C_SOURCES := $(shell find src -type f -name '*.c' | sort)
KERNEL_ASM_SOURCES := $(shell find src -type f -name '*.asm' ! -path 'src/boot.asm' | sort)
KERNEL_HEADERS := $(shell find include -type f -name '*.h' | sort)
KERNEL_OBJECTS := $(patsubst src/%.c,build/%.o,$(KERNEL_C_SOURCES)) \
	$(patsubst src/%.asm,build/%.o,$(KERNEL_ASM_SOURCES))

build/%.o: src/%.c $(KERNEL_HEADERS)
	mkdir -p $(@D)
	gcc -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector -Iinclude -c $< -o $@

build/%.o: src/%.asm
	mkdir -p $(@D)
	nasm -f elf32 $< -o $@

build/kernel_probe.elf: $(KERNEL_OBJECTS)
	ld -m elf_i386 -Ttext 0x7E00 --unresolved-symbols=ignore-all --entry=0 $^ -o $@

build/kernel_probe.bin: build/kernel_probe.elf
	objcopy -O binary $< $@
	@printf 'Kernel size: %s bytes\n' "$$(stat -c %s $@)"

build/boot.o: src/boot.asm build/kernel_probe.bin
	nasm -f elf32 -dSECTORS=$$(( ($$(stat -c %s build/kernel_probe.bin) + 511) / 512 )) src/boot.asm -o build/boot.o

build/os.bin: $(KERNEL_OBJECTS) build/boot.o src/link.ld
	ld -m elf_i386 build/boot.o $(KERNEL_OBJECTS) -T src/link.ld -o build/os.elf
	objcopy -I elf32-i386 -O binary build/os.elf build/os.bin

build/disk.img: build/os.bin
	dd if=/dev/zero of=build/disk.img bs=512 count=2880
	dd if=build/os.bin of=build/disk.img conv=notrunc

build: build/disk.img

run: build
	qemu-system-i386 -cpu pentium2 -m 1g -drive file=build/disk.img,format=raw,if=ide -device VGA

clean:
	rm -rf build