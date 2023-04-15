import threading
import subprocess
import sys

class mTh(threading.Thread):
    def __init__(self, arg):
        threading.Thread.__init__(self)
        self.arg = arg
    def run(self):
        subprocess.run(self.arg, shell=True)

if __name__=="__main__":
    args = list(map(lambda i:i.strip(), " ".join(sys.argv[1:]).split("//")))
    for arg in args:
        th = mTh(arg)
        th.start()