build: build/boot.img

N:=$(shell stat -c %s src/load)
SECTORS := $(shell echo $$(( ($(N) + 511) / 512 )))

build/hellobios/hellobios.img: src/hellobios/hellobios.asm
	mkdir -p build/hellobios
	nasm -fbin src/hellobios/hellobios.asm -o build/hellobios/hellobios.bin
	dd if=/dev/zero of=build/hellobios/hellobios.img bs=1024 count=1440
	dd if=build/hellobios/hellobios.bin of=build/hellobios/hellobios.img conv=notrunc

build/boot.img: src/boot.asm src/load
	mkdir -p build
	nasm -fbin src/boot.asm -o build/boot.bin -dSECTORS=$(SECTORS)
	dd if=/dev/zero of=build/boot.img bs=1024 count=1440
	dd if=build/boot.bin of=build/boot.img conv=notrunc
	dd if=src/load of=build/boot.img conv=notrunc seek=1

run: build/boot.img
	qemu-system-i386 \
		-cpu pentium2 \
		-m 1g \
		-drive file=build/boot.img,format=raw,if=ide \
		-device VGA

test: build/boot.img
	{ \
		sleep 1 ; \
		echo "pmemsave 0x7E00 $N build/load_dumped" ; \
		echo quit ; \
	} | qemu-system-i386 \
		-cpu pentium2 \
		-m 1g \
		-drive file=build/boot.img,format=raw,if=ide \
		-monitor stdio \
		-device VGA

	diff src/load build/load_dumped && echo succes

clean:
	rm -rf build