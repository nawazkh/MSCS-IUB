#!/usr/bin/env python
# program to generate random preferences
import numpy as np
import random
import sys

def randStudents(numStu, maxPerTeam, fileName):
    students = range(0, numStu)
    f = open(fileName, 'w')
    for i in range(0, numStu):
        strTemp = str(i) + ' ' + str(random.randint(0, maxPerTeam)) + ' '
        like = random.randint(0, numStu)
        if like == 0:
            strTemp += '_ '
        else:
            random.shuffle(students)
            strTemp += ",".join(str(stu) for stu in students[:like])
            strTemp += ' '
        hate = random.randint(0, numStu)
        if hate == 0:
            strTemp += '_\n'
        else:
            random.shuffle(students)
            strTemp += ",".join(str(stu) for stu in students[:hate])
            strTemp += '\n'
        f.write(strTemp)
    f.close()

numStu = int(sys.argv[1]) # number of students
maxPerTeam = int(sys.argv[2]) # max number of studetns per team
fileName = str(sys.argv[3]) # output file name
randStudents(numStu, maxPerTeam, fileName)
