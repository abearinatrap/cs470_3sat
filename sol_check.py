# Andrew Renninger
values = input()
FILENAME = input()
bvalues = values.split()
def check():
    f = open(FILENAME,"r")
    line = f.readline()
    num_variables = int(line)
    clause_num = 1
    while line!="":
        line = f.readline()
        if len(line)==0:
            break
        lit = list(map(int,line.split()))
        ctrue = False
        for i in range(3):
            var = abs(lit[i]) - 1
            truth_value = bvalues[var]
            if lit[i] < 0:
                if truth_value=='0':
                    ctrue = True
            else:
                if truth_value=='1':
                    ctrue = True
        if not ctrue:
            print("claus",clause_num, "fail")
            return
        clause_num += 1
    f.close()
    print("solution works")
check()