

OBJECTS=obj/boot.o obj/kernel.o obj/gdt.o obj/port.o

obj/%.o : src/%.s
	mkdir -p $(@D)
	i686-elf-as $< -o $@

obj/%.o : src/%.c
	mkdir -p $(@D)
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -Wall -Wextra - Iinclude
	
obj/%.o : src/%.cpp
	mkdir -p $(@D)
	i686-elf-g++ -c $< -o $@ -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude
	
bin/kernel.bin : link.ld $(OBJECTS)
	mkdir -p $(@D)
	i686-elf-gcc -T $< -o $@ -ffreestanding -nostdlib $(OBJECTS) -lgcc
	
projectJupiter.iso : bin/kernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "project jupiter" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso
	rm -rf iso

run: projectJupiter.iso
	qemu-system-i386 -cdrom $<
	
.PHONY clean:
	rm -rf obj
	rm -rf bin
	rm -rf projectJupiter.iso
