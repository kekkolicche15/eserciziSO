import subprocess, os, sys

path = sys.argv[1] if len(sys.argv)>1 else "."
scripts = [f for f in os.listdir(path) if "script" in str(subprocess.run(["file", f], stdout=subprocess.PIPE).stdout)]
for script in scripts:
    ext = script.split(".")[-1]
    if "sh" in ext:
        subprocess.run(["./"+script])
    elif "py" in ext:
        subprocess.run(["python", script])
    else:
        print("Non so come eseguire "+script+", provo ad eseguirlo con /bin/bash")
        subprocess.run(["./"+script])