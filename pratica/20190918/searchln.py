import os, sys, stat

path = "." if len(sys.argv)<2 else sys.argv[1]
inodeDict = {}
for cur, _, files in os.walk(path):
    for file in files:
        fullname = os.path.join(cur, file)
        st = os.lstat(fullname)
        if not stat.S_ISLNK(st.st_mode):
            if st.st_ino in inodeDict:
                inodeDict[st.st_ino].append(fullname)
            else:
                inodeDict[st.st_ino] = [fullname]
for _, vals in inodeDict.items():
    if len(vals)>1:
        print(" ".join(vals))