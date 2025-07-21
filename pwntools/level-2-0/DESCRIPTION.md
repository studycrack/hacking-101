In this level, you need to write an assembly code snippet to meet the following condition to bypass the check, and use the asm API from pwntools to compile the assembly code and complete the challenge. The condition for this challenge is:

```
rax = 0x12345678
```

Please refer to the following pwntools example code to complete the script (Hint: make sure to replace the NOP instruction with the specific assembly instruction):
```
from pwn import *

def print_lines(io):
    info("Printing io received lines")
    while True:
        try:
            line = io.recvline()
            success(line.decode())
        except EOFError:
            break

# Set architecture, os and log level
context(arch="amd64", os="linux", log_level="info")

# Load the ELF file and execute it as a new process.
challenge_path = "/challenge/pwntools-tutorials-level2.0"

p = process(challenge_path)

# Send the payload after the string "(up to 0x1000 bytes): \n" is found.
p.sendafter("Please give me your assembly in bytes", asm("NOP"))

print_lines(p)
```