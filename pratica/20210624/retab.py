import os, subprocess, sys

for path in ["."] if len(sys.argv) < 2 else sys.argv[1:]:
    for cur, _dirs, files in os.walk(path):
        for file in files:
            if file.split(".")[-1]=="c" or file.split(".")[-1]=="h":
                fp = os.path.join(cur, file)
                print("Adjusting",fp+"...")
                subprocess.run(("vim -n +norm!gg=G +wq "+fp).split(" "))