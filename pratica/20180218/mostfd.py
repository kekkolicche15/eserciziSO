import os

path = "/proc"

mostfdno = -1
mostfdproc = []

dirs = [f for f in os.listdir(path) if os.path.isdir(os.path.join(path, f)) and f.isdigit()]
for dir in dirs:
    try:
        fds = [fd for fd in os.listdir(os.path.join(path, dir, "fd"))]
        if mostfdno<len(fds):
            mostfdproc = [dir]
            mostfdno = len(fds)
        elif mostfdno==len(fds):
            mostfdproc.append(dir) 
    except PermissionError:
        continue

if len(mostfdproc)==1:
    print("with "+str(mostfdno)+"," , ",".join(mostfdproc), "is the process with most fds used")
else:
    print("with "+str(mostfdno)+"," , ",".join(mostfdproc), "are the processes with most fds used")
