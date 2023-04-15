import os
inDir = {}
files = [f for f in os.listdir("./") if os.path.isfile("./"+f)]
for file in files:
    ext = "None" if file.find(".") == -1 else file.split(".")[-1]
    if ext in inDir.keys():
        inDir[ext].append(file)
    else:
        inDir[ext] = [file]
for ext, files in inDir.items():
    print(ext+": "+" ".join(files))