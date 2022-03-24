import os

# with open('main.c', 'r') as file :
#   filedata = file.read()

filedata =""
kvec = []
with open('gsc.txt', 'r') as f:
    for readline in f:
        for str in readline.split():
            kvec.append(str)

filedata += "#include <cortos.h>\n" 
N_GC = input("N_GC? ")
filedata+="//CORTOS_GC - Number of stars in Star Catalogue\n#define CORTOS_GC "+N_GC+"\n"

#foc = input("foc? ")
#filedata+="#define CORTOS_foc "+foc+"\n"

UIS ="{"
for i in range(int(N_GC)):
    UIS+="{"
    UIS+=kvec[4*i]+","
    UIS+=kvec[4*i+1]+","
    UIS+=kvec[4*i+2]+","
    UIS+=kvec[4*i+3]+"},"

UIS=UIS.rstrip(UIS[-1])
UIS+="}"

filedata+="double sm_GC[CORTOS_GC][4]="+UIS+";\n"


# filedata = filedata.replace('return 0;', 'cortos_exit(0);')
# filedata = filedata.replace('printf(', 'CORTOS_INFO(')

with open('sm_GC.h', 'w') as file :
    file.write(filedata)
#try:
#    os.system('touch -m main.c')
