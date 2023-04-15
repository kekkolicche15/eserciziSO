from os import listdir
from os.path import join
import subprocess
import sys

cate={}
mypath = sys.argv[1] if len(sys.argv)>1 else "."
files = [join(mypath, f) for f in listdir(mypath)]
for file in files:
    name,type = str(subprocess.run(["file", file], stdout=subprocess.PIPE).stdout).split(":")
    type = type.split(",")[0].replace("\\n'", "")
    name = name.replace("b'", "").split("/")[-1]
    if type in cate:
        cate[type].append(name)
    else:
        cate[type] = [name]
for key, vals in cate.items():
    print(key+": "+" ".join(vals))