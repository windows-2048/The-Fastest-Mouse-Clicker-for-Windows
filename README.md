This is the source-only repository for educational purposes only.

# ./TheFastestMouseClicker_classic/ is a classic Windows-only version of **The Fastest Mouse Clicker for Windows** ported from 32-bit to 64-bit version recently.

One can compile it on Windows via [msys2](https://www.msys2.org/) environment. Just clone the repo, cd ./TheFastestMouseClicker_classic/, and run ./build_by_gcc64.sh.

**The Fastest Mouse Clicker for Windows Classic** (version 2.1.7.0) is the original, battle-tested Windows-only edition of one of the most popular open-source auto-clickers in the world - with over **183 GitHub stars**. It is a single self-contained 64-bit native Win32 application, statically linked with zero external runtime dependencies, and engineered to push mouse-click automation to the absolute hardware limit.

The secret behind its record-breaking throughput is a carefully crafted use of the **Win32 `SendInput()` API** with the input event *arrays*. Rather than issuing one synthetic mouse event per system call, the application batches hundreds of `INPUT` structures into a single `SendInput()` call, saturating the OS input queue and achieving verified click rates of up to **100,000 clicks per second (CPS)** - a result no conventional auto-clicker can match.
