import subprocess

users = subprocess.run(["getent", "passwd"], stdout=subprocess.PIPE).stdout.decode()
homes = {}
for user in users.splitlines():
    args = user.split(":")
    if len(args)<2 or len(args[-2])==0:
        continue
    userhome = args[-2]
    if len(userhome.split("/"))>2:
        userhome = "/".join(userhome.split("/")[:-1])
    if not userhome in homes:
        homes[userhome] = 1
    else:
        homes[userhome] +=1

for home, count in homes.items():
    print(home+"---"+str(count))