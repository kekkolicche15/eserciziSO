import subprocess, sys

sc = subprocess.run(["gcc", "-dD", "-E", "-"], input=b"#include <sys/syscall.h>", stdout=subprocess.PIPE).stdout.decode().split("\n# 21 \"/usr/include/asm/unistd.h\" 2 3 4")[0].split("_ASM_UNISTD_64_H")[1].strip()
if len(sys.argv)<2:
    exit(0)
try:
    num = int(sys.argv[1])
    for line in sc.splitlines():
        if int(line.split(" ")[2]) == num:
            print(line.split("R_")[1].split(" ")[0])
except ValueError:
    name = sys.argv[1]
    for line in sc.splitlines():
        if name==line.split("R_")[1].split(" ")[0]:
            print(line.split(" ")[-1])