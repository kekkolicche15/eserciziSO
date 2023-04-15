import os, sys, math

if len(sys.argv) < 2:
    exit()
oldest, newest = ("", "")
oldestTime, newestTime = (math.inf, -math.inf)
for cur, _dirs, files in os.walk(sys.argv[1]):
    for file in files:
        ffile = os.path.join(cur, file)
        mtime = os.path.getmtime(ffile)
        if oldestTime>mtime:
            oldest = ffile
            oldestTime = mtime
        elif newestTime<mtime:
            newest = ffile
            newestTime = mtime
print("Il file piu' recente nel sottoalbero indicato e'",newest,"\nIl file meno recente nel sottoalbero indicato e'",oldest)