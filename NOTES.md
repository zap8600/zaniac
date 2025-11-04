# Notes

* For x86_64, use `set $pc += 5` in GDB to get past an infinite loop.
* In RISC-V64, `csrc` clears the bits in a CSR. E.g. `csrc mstatus, t0` is like `mstatus &= ~(t0)`.
* In RISC-V64, `csrs` sets the bits in a CSR. E.g. `csrs mstatus, t0` is like `mstatus |= t0`.
