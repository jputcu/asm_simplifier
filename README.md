Description
===========
Simplify listing files so we are able to diff with previous version without being flooded with address
changes.

Pipe the output of `avr-objdump` into `avr_simplifier`:
~~~
> avr-objdump --no-show-raw-insn --headers --section=.text --demangle --source --wide ${elf_file} | asm_simplifier
~~~

