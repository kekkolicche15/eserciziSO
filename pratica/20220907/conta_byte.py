import os, sys, subprocess, re
print(sum([os.path.getsize(pa+'/'+f) if re.search("ELF\s.*exec", str(subprocess.run(["file", pa+"/"+f], stdout=subprocess.PIPE).stdout)) else 0 for pa in (sys.argv[1:] if len(sys.argv)>1 else ["."]) for f in os.listdir(pa)]),"bytes") if __name__=="__main__" else None