#!/usr/bin/env python
# Assignment 1 Question 2
# assign.py : assign students into teams to minimize the total amount of work for the course staff
# Tongxin Wang

##
# My program currently perform 10 runs and 1000 steps of descent per run, which will let the program to terminate
# within about 1 minute for 100 students and 30 seconds for 50 students on BURROW.
# Increase number of runs and steps could improve the result, but would require more execution time.
##

####################################################
# N - number of students in the class
# We use integer 0 to N-1 to represent each student
# We use a list "Team" of size N to represnet a state, which is a possible team assignment
# The i-th element in "Team" Team[i] is a set, which consists all the students in the same team with student i
# Team[i] consists at least 1 student and at most 3 students
# This means if Team[a] = {a, b c}, then Team[b] = Team[c] = {a, b, c}
# We define c(Team) as the cost function of Team, which is equal to the total amount of work for the staff
# given the tea massigment Team.
#
#### Problem Formulation ####
#
# state space: The set of all distinct Teams with total number of N students as defined above
#
# successor function SUCC(Team) = {all distinct Teams of moving student i to the team with student j, where
#                                  0 <= i <= N-1 and 0 <= j <= N-1 and |Team(j)| < 3 when j != i}
# We define moving student i to the team with student i as remove i from his/her current team and instead
# making him/her the only student in his/her team.
#
# edge weights: The edge weights of the search tree are all 0.
# This is because we do not care the how many times we apply movements on students as long as we get a small
# total time for the staff.
#
# goal state: The assigment of teams that yield the minimum total amount of work for the staff
# with these N students.
#
# heuristic functions: h(S) = c(S) - c_min, where c_min is the cost of the goal state, which is
# the minimum total amount of work for the staff
# As we can see, h(S) is always admissible since h(S) = h*(S), where h*(S) represents the minimun cost from
# state S to goal state. However, the problem here is that we do not know the value of c_min.
# This won't be a problem in Monte Carlo Descent since what we need to calculate is
# h(S') - h(S) = c(S') - c_min - (c(S) - c_min) = c(S') - c(S)
# And in Steepest Descent, we don't need a heuristic function.
#############################
#
##### Search Algorithms #####
# We implemented steepest descent and Monte Carlo descent.
# We did the S descent steps , where S is as large times as possible given that our algortihm could terminate in
# a reasonable time. We make the team assignment that yield the minimum cost in these S steps as the result of
# the algorithm.
#
# Since the result of steepest descent and Monte Carlo descent depends on the initial states and these algorithms
# are not guaranteed to find the global minimum (sometimes even the local minimum), we run the algorithms R
# times and take the result with minimum cost in these R times as the result to alleviate this problem.
#
# In the examples we tried, Monte Carlo descent tend to perform better than steepest descent since it is more
# unlikely to stuck at a local minimum. To exploit the advantage of Monte Carlo descent, we also implemented
# simulated annealing, which gradually reduce T as the step increases.
#
###########################
#
####### Discussions #######
# 1. Why local search?
#
# The problem seems to be a NP-hard problem at a glance. Even if we take a step back, the state space is
# extremely huge when N is larger.
#
# For example, consider assignment 100 students (which is similar to the size of CSCI-B551)
# into teams of equal size 2, there are
# 100!/(50! * 2^50) = 2.7254 * 10^78 possible team assignments.
#
# And since we allow team size of 1, 2, 3, the possible team assignments will be much larger than this number.
# So, we could first exclude exhaustive search.
#
# Moreover, since the value of k, m and n are arbitrary, there is not an easy way (as far as I could think of)
# to do pruning on the search tree and reduce the states with need to search dramatically.
# Instead, local search could give us a pretty good result with reasonable amount of searches. That's why we chose
# local search algorithms. However, since the value of k, m and n are arbitrary, the cost function is not
# always convex (actually, most of the time it is not convex), which means that we are not guaranteed to find
# the global minimum using local search.
#
# For steepest descent, in the case of 100 students, we need to search at most 100*100 = 10^4 states
# (may contain duplicate states) to make a step, which is still a lot, but is still much smaller than
# 10^78 even we do a lot of steps and multiple runs. However, steepest descent still seems pretty
# computational intensive when N is large.
#
# For Monte Carlo descent, we only need to choose a successor randomly during each step, which makes it a lot faster
# and make us able to do more steps using Monte Carlo descent, and Monte Carlo descent is less likely to stuck at
# a local minimum than steepest descent.
#
# Finally, implemented simulated annealing based on Monte Carlo descent, which will probably help us
# get closer to the global minimum intuitively.
#
#
# 2. How to choose T in Monte Carlo descent?
# Since the choice of T depends on the set up of the problem, especially depends of the value of k, m, n in our case.
# So in our case, choosing T as a constant seems unreasonable.
#
# We randomly chose 100 samples where the cost after the first step of Monte Carlo descent is larger than the initial
# state. We calculated the cost difference between the state after the first step and the initial state and
# chose the median as our T. This seems to work in our test examples and I hope it will work in the general case.
# For simulated annealing, we decrease T linearly in each step and reach the T of 0.1 in the last step (as 0 may
# cause numerial problems)
#
###########################

####################################################

import sys
import random
import math
from copy import deepcopy


# generate list of student names
def genStuNamesList(stuFile):
    idx = 0
    stuNames = []
    with open(stuFile) as myFile:
        for line in myFile:
            words = line.split()
            stuNames.append(words[0])

    return stuNames


# get students preference information
def getPreInfo(stuFile, stuNames):
    preSize = []  # list of integers
    preStu = []  # list of sets
    notPreStu = []  # list of sets
    with open(stuFile) as myFile:
        for line in myFile:
            words = line.split()  # stuName preSize preStu notPreStu
            preSize.append(int(words[1]))

            preStu_set = set()
            if words[2] != '_':
                preStu_str = words[2].split(',')
                for stu_str in preStu_str:
                    preStu_set.add(stuNames.index(stu_str))
            preStu.append(preStu_set)

            notPreStu_set = set()
            if words[3] != '_':
                notPreStu_str = words[3].split(',')
                for stu_str in notPreStu_str:
                    notPreStu_set.add(stuNames.index(stu_str))
            notPreStu.append(notPreStu_set)

    return (preSize, preStu, notPreStu)


# randomly put students into teams
def randTeam(numStu):
    init = [None] * numStu
    rand_order = range(0, numStu)
    random.shuffle(rand_order)
    idx = 0
    while idx < numStu:
        tmp_size = random.randint(1, maxPerTeam)
        tmp_stu = rand_order[idx:min(idx + tmp_size, numStu)]
        idx += tmp_size
        tmp_team = set(tmp_stu)
        for stu in tmp_stu:
            init[stu] = tmp_team

    return init


# calculate number of teams
def calNumTeams(teams):
    visited = [0] * len(teams)
    numTeams = 0
    for i in range(0, len(teams)):
        if visited[i] == 0:
            numTeams += 1
            for stu in teams[i]:
                visited[stu] = 1

    return numTeams


# calculate total time based on teams and preference information
def calTime(teams, preInfo):
    preSize = preInfo[0]  # list of integers
    preStu = preInfo[1]  # list of sets
    notPreStu = preInfo[2]  # list of sets
    totalTime = 0

    totalTime += k * calNumTeams(teams)  # time grading teams
    for i in range(0, len(teams)):
        if preSize[i] != 0 and preSize[i] != len(teams[i]):  # complain size
            totalTime += s
        for pre in preStu[i]:  # complain not being with liked ones
            if pre not in teams[i]:
                totalTime += n
        for stu in teams[i]:  # complain about being with hated ones
            if stu in notPreStu[i]:
                totalTime += m

    return totalTime


# remove fromStu from current teams
def removeStu(teams, fromStu):
    for stu in teams[fromStu]:
        tmp = teams[stu].copy()
        tmp.remove(fromStu)
        teams[stu] = tmp  # remove student fromStu from current team
    teams[fromStu] = set()

    return teams


# move fromStu to toStu team
def addStu(teams, fromStu, toStu):
    if fromStu == toStu:  # fromStu as a team with one person
        teams[fromStu].add(fromStu)
        return teams

    if len(teams[toStu]) >= maxPerTeam:  # move to team is full
        return -1
    else:
        for stu in teams[toStu]:
            tmp = teams[stu].copy()
            tmp.add(fromStu)
            teams[stu] = tmp  # add student fromStu to new team
        teams[fromStu] = teams[toStu]

    return teams


# doing one step of steepest descent
def steepestDescentStep(teams, preInfo):
    totalTime = calTime(teams, preInfo)
    succTeams = teams
    # successors: move one student into another team
    for fromStu in range(0, len(teams)):  # choose one student to move
        tmpFromStu = removeStu(deepcopy(teams), fromStu)  # remove fromStu from current teams
        for toStu in range(0, len(teams)):  # choose one team to move to
            if len(tmpFromStu[toStu]) < maxPerTeam:  # team that is not full and haven't been visited
                tmp = addStu(deepcopy(tmpFromStu), fromStu, toStu)
                if tmp != -1:
                    tmpTime = calTime(tmp, preInfo)
                    if tmpTime < totalTime:
                        totalTime = tmpTime
                        succTeams = tmp

    return (succTeams, totalTime)


# find optimal teams using steepest descent
def steepestDescent(preInfo, runs, steps):
    numStu = len(preInfo[0])
    totalTime = float('inf')
    optimalTeam = []
    for run in range(0, runs):
        oldTeam = randTeam(numStu)
        oldTime = calTime(oldTeam, preInfo)
        if oldTime < totalTime:
            totalTime = oldTime
            optimalTeam = oldTeam
        for step in range(0, steps):
            (newTeam, newTime) = steepestDescentStep(deepcopy(oldTeam), preInfo)
            if newTime >= oldTime:  # no descent
                break
            if newTime < totalTime:  # new optimal
                totalTime = newTime
                optimalTeam = newTeam
            oldTeam = newTeam
            oldTime = newTime

    return optimalTeam


# doing one step of Monte Carlo Descent
def MCDescentStep(teams, T, preInfo):
    totalTime = calTime(teams, preInfo)
    stuToMove = random.randint(0, len(teams)-1) # random choose a student to move
    validTeams = [] # valid teams for stuN to move in
    teamsTemp = removeStu(deepcopy(teams), stuToMove)

    for tN in range(0, len(teamsTemp)):
        if len(teamsTemp[tN]) < maxPerTeam:
            validTeams.append(tN)
    teamToMove = validTeams[random.randint(0, len(validTeams)-1)] # random choose a team for stuN to move to

    newTeams = addStu(deepcopy(teamsTemp), stuToMove, teamToMove)
    newTotalTime = calTime(newTeams, preInfo)
    if newTotalTime <= totalTime: # Doing MC descent
        return (newTeams, newTotalTime)
    else:
        prob = math.exp(-(newTotalTime - totalTime)/T)
        if random.uniform(0, 1) < prob:
            return (newTeams, newTotalTime)
        else:
            return (teams, totalTime)


# find optimal teams using Monte Carlo descent
def MCDescent(preInfo, runs, steps):
    numStu = len(preInfo[0])
    totalTime = float('inf')
    optimalTeam = []
    for run in range(0, runs):

        #How to choose T?
        T = findTemperature(preInfo)
        #T = 100

        oldTeam = randTeam(numStu)
        oldTime = calTime(oldTeam, preInfo)
        if oldTime < totalTime:
            totalTime = oldTime
            optimalTeam = oldTeam
        for step in range(0, steps):
            (newTeam, newTime) = MCDescentStep(deepcopy(oldTeam), T, preInfo)
            if newTime < totalTime:  # new optimal
                totalTime = newTime
                optimalTeam = newTeam
            oldTeam = newTeam
            oldTime = newTime

    return optimalTeam


# find optimal teams using Simulated Annealing
def simulatedAnnealing(preInfo, runs, steps):
    numStu = len(preInfo[0])
    totalTime = float('inf')
    optimalTeam = []
    for run in range(0, runs):
        T = findTemperature(preInfo)
        oldTeam = randTeam(numStu)
        oldTime = calTime(oldTeam, preInfo)
        if oldTime < totalTime:
            totalTime = oldTime
            optimalTeam = oldTeam
        TStep = (T-0.1)/(steps-1)
        for step in range(0, steps):
            (newTeam, newTime) = MCDescentStep(deepcopy(oldTeam), T, preInfo)
            if newTime < totalTime:  # new optimal
                totalTime = newTime
                optimalTeam = newTeam
            T = T - TStep # decrease temperature
            oldTeam = newTeam
            oldTime = newTime

    return optimalTeam

def findTemperature(preInfo):
    sampleNum = 100
    i = 0
    timeList = []
    numStu = len(preInfo[0])
    while i < sampleNum:
        oldTeam = randTeam(numStu)
        oldTime = calTime(oldTeam, preInfo)
        stuToMove = random.randint(0, len(oldTeam)-1) # random choose a student to move
        validTeams = [] # valid teams for stuN to move in
        teamsTemp = removeStu(deepcopy(oldTeam), stuToMove)

        for tN in range(0, len(teamsTemp)):
            if len(teamsTemp[tN]) < maxPerTeam:
                validTeams.append(tN)
        teamToMove = validTeams[random.randint(0, len(validTeams)-1)] # random choose a team for stuN to move to

        newTeams = addStu(deepcopy(teamsTemp), stuToMove, teamToMove)
        newTime = calTime(newTeams, preInfo)
        if (newTime > oldTime):
            timeList.append(newTime - oldTime)
            i = i+1

    return (median(timeList))

#calculate the median of a list
def median(listX):
    if len(listX)%2 != 0:
        return sorted(listX)[len(listX)/2]
    else:
        mid = (sorted(listX)[len(listX)/2] + sorted(listX)[len(listX)/2-1])/2.0
        return mid

def printResult(teams, stuNames, preInfo):
    visited = [0] * len(teams)  # prevent duplicate moves
    tmpStr = ""
    for stuTeam in range(0, len(teams)):
        if visited[stuTeam] == 0:
            for stu in teams[stuTeam]:
                tmpStr = tmpStr + stuNames[stu] + ' '
                visited[stu] = 1
            tmpStr = tmpStr + '\n'
    tmpStr = tmpStr + str(calTime(teams, preInfo)) + '\n'
    print tmpStr


input_file = str(sys.argv[1])
k = float(sys.argv[2]) # time to grade a team
m = float(sys.argv[3]) # time to complain being with hated ones
n = float(sys.argv[4]) # time to complain about not being with liked ones

#input_file = 'students.txt'
#k = 160  # time to grade a team
#n = 10  # time to complain about not being with liked ones
#m = 31  # time to complain about being with hated ones
s = 1  # time to complain size of the group
maxPerTeam = 3

stuNamesList = genStuNamesList(input_file)
preferInfo = getPreInfo(input_file, stuNamesList)

findTemperature(preferInfo)

runNum = 10
maxStep = 1000
#optimalTeams = steepestDescent(preferInfo, runNum, maxStep)
#optimalTeams = MCDescent(preferInfo, runNum, maxStep)
optimalTeams = simulatedAnnealing(preferInfo, runNum, maxStep)
printResult(optimalTeams, stuNamesList, preferInfo)
