import os
import glob

# with open('main.c', 'r') as file :
#   filedata = file.read()

filedata =""

kvec = []
N_i = []
for filename in glob.glob('*.txt'):
    temp = []
    with open(os.path.join(os.getcwd(), filename), 'r') as f:
        for readline in f:
            for str in readline.split():
                temp.append(str)
    kvec.append(temp)
            


for i in range(len(kvec)):
    N_i.append(len(kvec[i])//3)
for i in range(len(kvec)):
    filedata = "#include <cortos.h>\n" 
    filedata+=f"//CORTOS_N_i - Number of input centroids\n#define CORTOS_N_i {N_i[i]}\n"
    UIS ="{"
    for j in range(N_i[i]):
        UIS+="{"
        UIS+=kvec[i][3*j]+","
        UIS+=kvec[i][3*j+1]+","
        UIS+=kvec[i][3*j+2]+"},\n"

    UIS=UIS.rstrip(UIS[-1])
    UIS+="}"
    filedata+=f'long double UIS_{i}[CORTOS_N_i][3]={UIS};\n'
    UISname = f'UIS_{i}.h'
    with open(UISname, 'w') as file :
        file.write(filedata)
#try:
#    os.system('touch -m main.c')
