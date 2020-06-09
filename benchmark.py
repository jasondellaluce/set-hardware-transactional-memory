#!/usr/bin/python
import subprocess

# Produces a CSV formatted table with ops/millisecs throughput values in each cell
limit = 100000
threadNums = [2, 3, 4, 5, 6, 7, 8]
benchmark = "./test.o"
for add, remove in [(18, 25), (3, 4)]:

    #CSV header
    print("")
    print("Workload: {:.2f}% contains operation".format((1 - (1/add) - (1/remove)) * 100))
    print(" ", end="");
    for threads in threadNums:
        print(", " + str(threads) + "T", end="")
    print("")
    
    for lock in ["coarse-lock", "lock-free", "itm-htm", "rtm-htm"]: 
        print(lock, end="")
        for threads in threadNums:
            cmd = benchmark + " " + str(lock) + " " + str(threads) + " " + str(limit) + " " + str(add) + " " + str(remove)
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
            output = p.communicate()[0]
            p.wait()
            result = float(output.decode().strip())
            print(", {:.6f}".format((limit * threads) / (result * 1000)), end="")
        print("")
