import hashlib, os, sys
def sha256sum(filename):
    h  = hashlib.sha256()
    b  = bytearray(128*1024)
    mv = memoryview(b)
    with open(filename, 'rb', buffering=0) as f:
        while n := f.readinto(mv):
            h.update(mv[:n])
    return h.hexdigest()

path = "." if len(sys.argv)<2 else sys.argv[1]
files = [os.path.join(path, f) for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))]
di = {}
for file in files:
    digest = sha256sum(file)
    if digest in di.keys():
        di[digest].append(file)
    else:
        di[digest] = [file]

for _, fs in di.items():
    for i in range(1, len(fs)):
        os.remove(fs[i])
        os.link(fs[0], fs[i])