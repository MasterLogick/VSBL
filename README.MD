# VSBL - Very Simple BootLoader

VSBL is a very simple bootloader and a set of fundamental modules that can load kernel written in C++.

Kernel is loaded in protected 32-bit mode.

Entry point for C++ code is  `void kmain()` function in `src/Kernel.cpp`

# Build

## Requirements

`make`, `yasm`, `g++`, `ld` and `objcopy`

## Compilation

```bash
make
```

There are two versions of compiled kernel image: `./out/boot.img` (Version for VirtualBox) or `./out/boot/img.old` (
Version for booting on real machine)

## VirtualBox VM

You can test your kernel in VirtualBox virtual machine.

### To create VM:

```bash
./init.sh
```

### To start VM:

```bash
make run_vm
```