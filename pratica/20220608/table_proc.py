import os, subprocess

uid = os.getuid()
res = {}
maxnamelenght = 0
split = "\t\t"
for pid in os.listdir("/proc"):
    status = str(subprocess.run(["cat", "/proc/"+pid+"/status"], stderr=subprocess.DEVNULL, stdout=subprocess.PIPE).stdout).split("\\n")
    if len(status)>1 and int(status[8].split("\\t")[1]) == uid:
        name = status[0].split("\\t")[1]
        res[name] = status[17].split("\\t")[1].strip()
        maxnamelenght = maxnamelenght if len(name)<maxnamelenght else len(name)
print("Nome processo".ljust(maxnamelenght, " ")+split+"Occupazione in memoria")

for name, size in res.items():
    print(name.ljust(maxnamelenght, " ")+split+size)