# VSBL - Very Simple BootLoader

VSBL is a Very Simple BootLoader and a set of fundamental modules that can load kernel written in C++.

Kernel is loaded in long 64-bit mode.

Entry point for C++ code is  `void kmain()` function in `src/Kernel.cpp`. You can find loaders for each stage(Real mode,
Protected mode and Long mode) [here](https://github.com/MasterLogick/VSBL/tree/master/src/bootloader).

## Supported features:

+ Physical memory mapping
+ Dynamic memory allocation based on AVL tree
+ C++ global constructors calling
+ PS/2 controller support
+ APIC and I/O APIC support
+ ACPI basic system tables parsing
+ PS/2 keyboard handling
+ Terminal printing like cout in C++
+ PCI management
+ AHCI setup
+ CMake build system

# Build

## Requirements

`cmake`, `yasm`, `g++`, `ld` and `objcopy`

## Compilation

```bash
mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target VSBL
```

There are two versions of compiled kernel image: `./out/boot.img` (Version for VirtualBox. Aligned to 1MB size)
or `./out/boot/img.old` (Version for booting on real machine)

## VirtualBox VM

You can test your kernel in VirtualBox virtual machine.

### To create VM:

```bash
./init.sh
```

### To start VM:

#### First run

```bash
cmake --build . --target VSBL_VBox_Medium_Create
```

#### Further runs

```bash
cmake --build . --target VSBL_VBox_Update
```