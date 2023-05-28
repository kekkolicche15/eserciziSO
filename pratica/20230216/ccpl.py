import os, sys

path = sys.argv[1] if len(sys.argv) > 1 else "."

lens = {}

for cur, subdir, files in os.walk(path):
    for file in files:
        with open(os.path.join(cur, file), 'r') as f:
            try:
                for idx, line in enumerate(f.read().splitlines()):
                    if idx+1 in lens.keys() and len(line)!=0:
                        lens[idx+1] += len(line)
                    else:
                        lens[idx+1] = len(line)
            except UnicodeDecodeError:
                continue
for key,val in lens.items():
    print(key, val)