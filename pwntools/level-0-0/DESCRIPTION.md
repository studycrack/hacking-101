This level is a tutorial and relatively simple. You can directly run /challenge/pwntools-tutorials-level0.0 in the terminal and then input a specific string (which you can find by reading the bypass_me function), but that is not the goal of this level.

This level will guide you on how to use pwntools to complete the challenge. Next, you need to use the `process`, `send`, `recv`, and other APIs in pwntools to write an exploit script, send a specific input to bypass the check, and read the /flag. Please refer to the following pwntools example code (hint: be sure to replace `FIXME` with the specific string mentioned above):

```
from pwn import *

# Set architecture, os and log level
context(arch="amd64", os="linux", log_level="info")

# Load the ELF file and execute it as a new process.
challenge_path = "/challenge/pwntools-tutorials-level0.0"
p = process(challenge_path)

payload = b'FIXME'
# Send the payload after the string ":)\n###\n" is found.
p.sendafter(":)\n###\n", payload)

# Receive flag from the process
flag = p.recvline()
print(f"flag is: {flag}")
```