import os, sys
path = "." if len(sys.argv)<2 else sys.argv[1]
flens = [{},{},{}]
rows=[0, 0, 0]
ext=[".c",".h","Makefile"]
for cur, _, files in os.walk(path):
    for file in files:
        if file=="Makefile":
            with open(os.path.join(cur, file), 'r') as f:
                flens[2][os.path.join(cur, file)] = sum([1 for _ in f])
        elif file.split(".")[-1]=="h":
            with open(os.path.join(cur, file), 'r') as f:
                flens[1][os.path.join(cur, file)] = sum([1 for _ in f])
        elif file.split(".")[-1]=="c":
            with open(os.path.join(cur, file), 'r') as f:
                flens[0][os.path.join(cur, file)] = sum([1 for _ in f])
for i in range(3):
    rows[i] = sum(flens[i].values())
maxnamelen = max([len(key) for flen in flens for key,_ in flen.items()])


for i in range(len(flens)):
    for name, length in flens[i].items():
        print(name.ljust(maxnamelen+6),length)
    print("tot ",ext[i].ljust(maxnamelen+1),rows[i],"\n" if len(flens[i].items())>1 else "")
print("tot source".ljust(maxnamelen+6), sum(rows))