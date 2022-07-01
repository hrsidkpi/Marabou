import os

files = os.listdir('.')
files.sort()

last_c = "c1.out"
last_c_num = 1
for f in files:
    if f[0] == 'c' and f[1:-4].isdigit() and f[-4:] == '.out':
        print("ffff")
        last_c_num = int(f[1:-4]) + 1
        last_c = "c" + str(last_c_num) + ".out"    
print(last_c_num)

done = False
while not done:
    done = True

    files = os.listdir('.')
    files.sort()
    for f in files:
        if f.startswith('slurm'):
            print(f, " -> ", last_c)
            os.rename(f, last_c)
            last_c_num += 1
            last_c = "c" + str(last_c_num) + ".out"
            done = False
            break
