import os, sys



path = ("." if len(sys.argv)<2 else sys.argv[1])

fline = {}

for cur, dir, files in os.walk(path):
    for file in files:
        try:
            with open(os.path.join(cur, file)) as f:
                try:
                    if f.read(2) == "#!":
                        f.seek(0)
                        line = f.readline().strip()
                        if line in fline.keys():
                            fline[line].append(file)
                        else:
                            fline[line] = [file]
                except UnicodeDecodeError:
                    continue
        except FileNotFoundError:
            continue
        except PermissionError:
            continue

for line, files in fline.items():
    print(line+": "+" ".join(files))