Description
===========
Simplify listing files so we are able to diff with previous version without being flooded with address
changes.

Listing files were retrieved via:
~~~
> avr-objdump --no-show-raw-insn --headers --section=.text --demangle --source --wide ${elf_file} > ${lss_file}
~~~

Usage:
~~~
> asm_simplifier ${lss_file} > ${simplified_lss}
~~~
