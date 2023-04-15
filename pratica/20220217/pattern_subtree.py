from os import listdir
from os.path import isfile, isdir, join, getmtime
import sys, re

def setfiles(path):
    loc = []
    for f in listdir(path):
        if isdir(join(path, f)):
            loc.extend(setfiles(join(path, f)))
        elif isfile(join(path, f)):
            loc.append(f)
    return loc


if len(sys.argv) < 2:
    exit()
files = setfiles(".")
match = []
for file in files:
    with open(file) as f:
        try:
            if re.search(sys.argv[1], f.read()):
                match.append(file)
        except UnicodeDecodeError:
            continue
match.sort(key=lambda x:-getmtime(x))
print(match)