import os, sys, shutil

srcs  = []
dests = []


if len(sys.argv) < 2:
    print("minimo 2 argomenti")
    exit()
else:
    srcs = sys.argv[1:3]
    dests = [sys.argv[3] if len(sys.argv) > 3 else "new"+srcs[0],
            sys.argv[4] if len(sys.argv) > 4 else "new"+srcs[1]]
if not os.path.isdir(srcs[0]) or not os.path.isdir(srcs[1]):
    print("Le sorgenti devono essere directory")
    exit()
os.makedirs(dests[0], exist_ok=True)
os.makedirs(dests[1], exist_ok=True)

common = []
for cur, _, files in os.walk(srcs[0]):
    for file in files:
        afile = os.path.join(cur, file)
        bfile = os.path.join(srcs[1], "/".join(afile.split("/")[1:]))
        if os.path.exists(bfile):
            apfile = os.path.join(dests[0], "/".join(afile.split("/")[1:]))
            bpfile = os.path.join(dests[1], "/".join(afile.split("/")[1:]))
            os.makedirs("/".join(apfile.split("/")[:-1]), exist_ok=True)
            os.makedirs("/".join(bpfile.split("/")[:-1]), exist_ok=True)
            shutil.copy(afile, apfile)
            shutil.copy(bfile, bpfile)