# Very Simple BootLoader

It is a very simple bootlader that can load kernel written in C.

Kernel is loaded in protected 32-bit mode.

Entry point in C code is  `void kmain()` function in `src/kernel.c` 

# Build

## Requirements

`yasm`, `gcc`, `ld` and `objcopy`

## Compilation

```bash
./build.sh
```

## For test in Virtualbox VM

### For the first time:
```bash
./init.sh
```

### For every test:
```bash
./test.sh
```