import random
FILENAME = "good20.txt"
f = open(FILENAME,"w")
numVariables = 20
f.write(str(numVariables)+"\n")
for i in range(70):
    for a in range(2):
        var = random.randint(1,numVariables)
        k = random.randint(0,1)
        if k==0:
            var = -var
        f.write(str(var))
        f.write(" ")
    
    b = random.randint(0,15)
    if b != 0:
        var = random.randint(1,numVariables)
        k = random.randint(0,1)
        if k==0:
            var = -var
    f.write(str(var))
    f.write("\n")
f.close()