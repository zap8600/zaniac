# zaniac

My operating system

## Requirements

* `clang`
* `lld`
* `make`
* `coreutils`
* `llvm`

## Building

Export your target architecture as `ARCH`. By default, the architecture is `x86_64`. If you are also building the bootloader, export the target bootloader platform as `PLATFORM`. By default, the platform is `efi`. To build the kernel, you can run something like:

```shell
ARCH=x86_64 make kernel
```

Replace `ARCH` with your selected target. To build the bootloader, you can run something like:

```shell
ARCH=x86_64 PLATFORM=efi make boot
```

Replace `ARCH` and `PLATFORM` with your selected targets.
