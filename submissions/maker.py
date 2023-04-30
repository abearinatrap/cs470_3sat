import random
import shutil
import subprocess
bestcorr = 1
corri = 0
numVariables = 20
numClauses = 70
numHard = 14
for i in range(1000000):
    FILENAME = "testubg/good20_.txt"
    f = open(FILENAME,"w")
    f.write(str(numVariables)+"\n")
    for j in range(numClauses):
        for a in range(2):
            var = random.randint(1,numVariables)
            k = random.randint(0,1)
            if k==0:
                var = -var
            f.write(str(var))
            f.write(" ")
        
        b = random.randint(0,numHard)
        if b != 0:
            var = random.randint(1,numVariables)
            k = random.randint(0,1)
            if k==0:
                var = -var
        f.write(str(var))
        f.write("\n")
    f.close()
    cmd = "./h.out "+FILENAME+" 1000"
    return_value = subprocess.check_output(cmd,shell=True)
    strval = return_value.decode()
    iter = 0
    fvalue = ""
    bvalue = ""
    while strval[iter]!="/":
        fvalue += strval[iter]
        iter += 1
    iter+=1
    while strval[iter]!=" ":
        bvalue += strval[iter]
        iter += 1
    fvalue = int(fvalue)
    bvalue = int(bvalue)
    corr = fvalue/bvalue
    if corr != 0 and corr < .8 :
        print(corr, i)
        if corr < bestcorr:
            bestcorr = corr
            print(bestcorr, "testubg/testingbest"+str(corri)+".txt")
            shutil.copyfile(FILENAME,"testubg/testingbest"+str(corri)+".txt")
            corri += 1