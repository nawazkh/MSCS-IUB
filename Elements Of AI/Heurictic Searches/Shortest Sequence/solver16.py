# put your 15 puzzle solver here!
# ------------------------------------------------------------------------------
# description of how you formulated the search problem,
# Search problem:
#       For any input 15-puzzle, decide whether a goal state is possible.
#       If yes, provide a least number fo moves to reach the goal state.
#
#       Goal state:
#            [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 0]]
#
#       Initial State:
#           Input is any 15-puzzle satisfying the following conditions.
#           1) permutation Inversion of input state and the position of '0' from
#              bottom should not be even(or odd) at the same time.
#
#       Successor Function:
#           Generate 6 states by moving either 1 tile, 2 titles and 3 titles
#               to 0, to left or right, up or down.
#               6 successors are generated.
#           All the generation of the successors take only one unit cost
#
#       Heuristic Function : h(s) :
#           - (Sum of Manhattan distance of all the tiles) / 3
#           This h(s) is admissible. The cost to reach the goal node is always
#           less than actual cost needed to reach the goal node
#          - We choose (Sum of Manhattan distance of all the tiles) / 3
#            because the sliding of three tiles is considered as one move.
#
#       Edge Weights:
#           h(s) + number of moves to reach the state
# ------------------------------------------------------------------------------
# a brief description of how your search algorithm works;
#           First: Solve function is invoked.
#           then "initial input board is checked whether it is solvable or not"
#           if initial node is the goal node, initial state is returned
#           if not:
#               successors as defined in above as are generated
#               and stored in a heap
#               each successor is popped (cost of that state be: h(s) + travel cost)
#               popped state is stored in a data structure called closed
#                                            CLosed is implemented by has table.
#                   if the popped state is already a visited node(i.e belongs to closed)
#                   then discard popped state
#                   else:
#                   add it to fringe.
# ------------------------------------------------------------------------------
# -------------- discussion of any problems you faced -------------------------:
#   problem is with selection of right heuristic function.
#   In my implementation,
#   Linear conflicts had a time lag,
#   Weighted Manhattan distance was not optimal
#
# any assumptions: a tile of lower value is considered as higher priority
#                   hence more weightage is assigned to Manhattan distance
#                   of lower valued tiles
# simplications: 6 successors are generated and added to a heap in
#                the successor functoin itself.
# and/or design decisions you made:
#   changed the algorithm 3 a little bit.
#   successors not being checked if present in fringe or not.
# references :
# https://stackoverflow.com/questions/40781817/heuristic-function-for-solving-weighted-15-puzzle
#
# ------------------------------------------------------------------------------
# solved by Nawaz Hussain K, 0003561850
#
# ------------------------------------------------------------------------------
# accepts the input-board-filename as an argument.
# to run:
# python solver16.py <filename>
# ------------------------------------------------------------------------------
from __future__ import division
import sys
import heapq
from collections import deque
import copy

#traversal_cost = 0
# this function will return the board when input 2D array is passed to it
def print_board(puzzle):
    return "\n".join([" ".join([str(col) if col else " " for col in row]) for row in puzzle])

#This fuction will calculate the heuristic value of the current node.
def calculateHeuristic(currentState):
    calculateValue = 0.0
    i = 0
    quotient = 0
    reminder = 0
    actualRow = 0
    actualColumn = 0
    j = 0
    for row in currentState:
        j = 0
        #print row
        for element in row:
            #calculateValue = calculateValue + ((element/4) + element % 4)
            #print element
            quotient = int(int(element)/4)
            #print "quotient",quotient
            reminder = int(element) % 4
            #print "reminder",reminder
            if(reminder == 0 and int(element) != 0 ):
                actualRow = quotient - 1
                actualColumn = 3
            else:
                if(reminder == 0 and int(element) == 0):
                    actualRow = 3
                    actualColumn = 3
                else:
                    actualRow = quotient
                    actualColumn = reminder - 1
            #print "element is:",element,"actual row is",actualRow,"actual column is",actualColumn
            #print "element is:",element,"current row is",i,"current column is",j


            #calculateValue = calculateValue + (abs(actualRow - i) + abs(actualColumn - j))
            #implementing misplaced tiles.

            #if(element == 1 or element == 6 or element == 11 or element == 0 or element == 4 or element == 7 or element == 10 or element == 13):
            #    calculateValue = calculateValue + (abs(actualRow - i) + abs(actualColumn - j))

            '''if(element != 0):
                if(actualRow != i or actualColumn !=j):
                    calculateValue = calculateValue + (abs(actualRow - i) + abs(actualColumn - j))'''
            #temp_ans = linear_conflicts_row(currentState)
            if(element != 0):
                #manhattan distance
                calculateValue = calculateValue + (abs(actualRow - i) + abs(actualColumn - j))
                #weighted manhattan distance
                #calculateValue += float((abs(actualRow - i) + abs(actualColumn - j))*(16 - element)/15)


            #if(actualRow != i or actualColumn != j):
                #calculateValue = calculateValue + 1
                #calculateValue = calculateValue + (abs(actualRow - i) + abs(actualColumn - j))
            j += 1
        i += 1

    #temp_ans2= linear_conflicts_column(zip(*currentState))
    #return int(temp_ans + temp_ans)#round(int(calculateValue)*0.45,2)
    #return round((float(calculateValue) + temp_ans + temp_ans2)/3,2)
    return float((calculateValue)/3)

def goalRow(element):
    actualColumn = 0
    actualRow = 0
    quotient = int(int(element)/4)
    #print "quotient",quotient
    reminder = int(element) % 4
    if(reminder == 0 and int(element) != 0 ):
        actualRow = quotient - 1
        actualColumn = 3
    else:
        if(reminder == 0 and int(element) == 0):
            actualRow = 3
            actualColumn = 3
        else:
            actualRow = quotient
            actualColumn = reminder - 1
    return actualRow
def goalColumn(element):
    actualColumn = 0
    actualRow = 0
    quotient = int(int(element)/4)
    #print "quotient",quotient
    reminder = int(element) % 4
    if(reminder == 0 and int(element) != 0 ):
        actualRow = quotient - 1
        actualColumn = 3
    else:
        if(reminder == 0 and int(element) == 0):
            actualRow = 3
            actualColumn = 3
        else:
            actualRow = quotient
            actualColumn = reminder - 1
    return actualColumn
def linear_conflicts_row(puzzle):
    row_conflict = 0;
    for row in puzzle:
        for i in range(len(row)):
            for j in range(i+1,len(row)):
                if(goalRow(row[i]) == goalRow(row[j])):
                    if(goalColumn(row[i])>goalColumn(row[j])):
                        if(j>i):
                            row_conflict += 1
                j += 1
            i += 1
    return row_conflict*2
def linear_conflicts_column(puzzle):
    row_conflict = 0;
    for row in puzzle:
        for i in range(len(row)):
            for j in range(i+1,len(row)):
                if(goalRow(row[i]) == goalRow(row[j])):
                    if(goalColumn(row[i])>goalColumn(row[j])):
                        if(j>i):
                            row_conflict += 1
                j += 1
            i += 1
    #print row_conflict*2
    return row_conflict*2

def calculateHeuristic2(currentState):
    #print "here",currentState
    calculateValue = 0
    i = 0
    quotient = 0
    reminder = 0
    actualRow = 0
    actualColumn = 0
    j = 0
    for row in currentState:
        j = 0
        for element in row:
            quotient = int(int(element)/4)
            reminder = int(int(element) % 4)
            if(reminder == 0 and element != 0 ):
                actualRow = quotient - 1
                actualColumn = 3
            else:
                if(reminder == 0 and element == 0):
                    actualRow = 3
                    actualColumn = 3
                else:
                    actualRow = quotient
                    actualColumn = reminder - 1
            if(actualRow!=i or actualColumn!=j):
                calculateValue = calculateValue + (abs(actualRow - i) + abs(actualColumn - j))
            j += 1
        i += 1
    return calculateValue


#this will identify location of 0
def location_of_zero(current_puzzle):
    row_of_zero,column_of_zero = 0,0
    i = 0
    for row in current_puzzle:
        if(0 in row):
            row_of_zero,column_of_zero = i,row.index(0)#return the row and column of 0
        i += 1
    return row_of_zero,column_of_zero
# this function will add more states to down
def add_states_down(current_puzzle,row_of_zero,column_of_zero,row_limit):
    # to generate right movements of zero keeping row constant
    j=row_of_zero
    for i in range(row_of_zero+1,row_limit+1):
        current_puzzle[i][column_of_zero],current_puzzle[j][column_of_zero] = current_puzzle[j][column_of_zero],current_puzzle[i][column_of_zero]
        j += 1
    return current_puzzle # returns the right generated node
# this function will add more states to right
def add_states_right(current_puzzle,row_of_zero,column_of_zero,column_limit):
    # to generate right movements of zero keeping row constant
    j=column_of_zero
    for i in range(column_of_zero+1,column_limit+1):
        current_puzzle[row_of_zero][j],current_puzzle[row_of_zero][i] = current_puzzle[row_of_zero][i],current_puzzle[row_of_zero][j]
        j += 1
    return current_puzzle # returns the right generated node

#this function will add more states to left
def add_states_left(current_puzzle,row_of_zero,column_of_zero,column_limit):
    j=column_of_zero
    for i in range(column_of_zero-1,column_limit-1,-1):
        current_puzzle[row_of_zero][j],current_puzzle[row_of_zero][i] = current_puzzle[row_of_zero][i],current_puzzle[row_of_zero][j]
        j -= 1
    return current_puzzle
# this function will add more states to the top
def add_states_up(current_puzzle,row_of_zero,column_of_zero,row_limit):
    j=row_of_zero
    for i in range(row_of_zero-1,row_limit-1,-1):
        current_puzzle[j][column_of_zero],current_puzzle[i][column_of_zero] = current_puzzle[i][column_of_zero],current_puzzle[j][column_of_zero]
        j -= 1
    return current_puzzle

# This funciton checks if this is the goal state
def is_goal(current_puzzle):
    value = calculateHeuristic2(current_puzzle)
    if(value == 0):
        return True
    else:
        return False

# this function will generate successors of the current input node
def successors(current_puzzle,travel_cost_transferred,path_transferred):
    # add sucessors
    # from the current node, take one or two or three steps both up and down
    # and left and right to generate the successors of current node.
    temp_puzzle = copy.deepcopy(current_puzzle)
    states = []
    #global traversal_cost
    traversal_cost = int(travel_cost_transferred) + 1
    travel_path = copy.deepcopy(path_transferred)
    row_of_zero,column_of_zero = location_of_zero(temp_puzzle)
    ## Adding right states
    counter = 0
    for column_limit in range(column_of_zero+1,len(temp_puzzle[row_of_zero])):#for iterating over right side
        travel_path_temp = copy.deepcopy(travel_path)
        row_of_zeroo,column_of_zeroo = location_of_zero(temp_puzzle)
        temp_puzzle_alter = add_states_right(temp_puzzle,row_of_zeroo,column_of_zeroo,column_limit)
        #calculateHeuristic
        #print "traversal_cost",traversal_cost,"calculateHeuristic(temp_puzzle_alter)",calculateHeuristic(temp_puzzle_alter),"temp_puzzle_alter",temp_puzzle_alter
        #cost = traversal_cost + (calculateHeuristic(temp_puzzle_alter))
        cost = (traversal_cost + round((calculateHeuristic(temp_puzzle_alter))/1, 2))
        #cost = (traversal_cost + (calculateHeuristic(temp_puzzle_alter)))
        #print "altered puzzle is",temp_puzzle_alter
        #path of L
        counter = abs(column_of_zero - column_limit)
        travel_path_temp = copy.deepcopy(""+str(travel_path_temp)+"L"+str(counter)+str(row_of_zero+1)+" ")
        #print "path of L",travel_path_temp
        #heapq.heappush(states,(cost,[temp_puzzle_alter,traversal_cost,travel_path_temp]))
        heapq.heappush(states,(cost,temp_puzzle_alter,traversal_cost,travel_path_temp))
        temp_puzzle = copy.deepcopy(temp_puzzle_alter)
        #print "1 States: ",states
        #print "altered puzzle is",temp_puzzle
    #adding left states
    temp_puzzle = copy.deepcopy(current_puzzle)
    row_of_zero,column_of_zero = location_of_zero(temp_puzzle)
    counter = 0
    for column_limit in range(column_of_zero-1,-1,-1):#for iterating over left side
        travel_path_temp = copy.deepcopy(travel_path)
        row_of_zeroo,column_of_zeroo = location_of_zero(temp_puzzle)
        temp_puzzle_alter = add_states_left(temp_puzzle,row_of_zeroo,column_of_zeroo,column_limit)
        #cost = traversal_cost + (calculateHeuristic(temp_puzzle_alter))
        cost = (traversal_cost + round((calculateHeuristic(temp_puzzle_alter))/1, 2))
        #cost = (traversal_cost + (calculateHeuristic(temp_puzzle_alter)))
        #path of R
        counter = abs(column_of_zero - column_limit)
        travel_path_temp = copy.deepcopy(""+str(travel_path_temp)+"R"+str(counter)+str(row_of_zero+1)+" ")
        #print "path of R",travel_path_temp
        #heapq.heappush(states,(cost,[temp_puzzle_alter,traversal_cost,travel_path_temp]))
        heapq.heappush(states,(cost,temp_puzzle_alter,traversal_cost,travel_path_temp))
        #print "altered puzzle is",temp_puzzle_alter
        temp_puzzle = copy.deepcopy(current_puzzle)
        #print "2 States: ",states
    # call next node generation function
    #temp_list.append(add_states(current_puzzle))
    # append this to temp_list
    # adding down states
    temp_puzzle = copy.deepcopy(current_puzzle)
    row_of_zero,column_of_zero = location_of_zero(temp_puzzle)
    counter = 0
    for row_limit in range(row_of_zero+1,len(temp_puzzle[row_of_zero])):
        travel_path_temp = copy.deepcopy(travel_path)
        row_of_zeroo,column_of_zeroo = location_of_zero(temp_puzzle)
        temp_puzzle_alter = add_states_down(temp_puzzle,row_of_zeroo,column_of_zeroo,row_limit)
        #cost = traversal_cost + (calculateHeuristic(temp_puzzle_alter))
        cost = (traversal_cost + round((calculateHeuristic(temp_puzzle_alter))/1, 2))
        #cost = (traversal_cost + (calculateHeuristic(temp_puzzle_alter)))
        #states of D
        counter = abs(row_of_zero - row_limit)
        travel_path_temp = copy.deepcopy(""+str(travel_path_temp)+"U"+str(counter)+str(column_of_zero+1)+" ")
        #print "path of U",travel_path_temp
        #heapq.heappush(states,(cost,[temp_puzzle_alter,traversal_cost,travel_path_temp]))
        heapq.heappush(states,(cost,temp_puzzle_alter,traversal_cost,travel_path_temp))
        temp_puzzle = copy.deepcopy(temp_puzzle_alter)
        #print "3 States: ",states
        #print "altered puzzle is",temp_puzzle_alter
    # adding up states
    temp_puzzle = copy.deepcopy(current_puzzle)
    row_of_zero,column_of_zero = location_of_zero(temp_puzzle)
    counter = 0
    for row_limit in range(row_of_zero-1,-1,-1):
        travel_path_temp = copy.deepcopy(travel_path)
        row_of_zeroo,column_of_zeroo = location_of_zero(temp_puzzle)
        temp_puzzle_alter = add_states_up(temp_puzzle,row_of_zeroo,column_of_zeroo,row_limit)
        #cost = traversal_cost + (calculateHeuristic(temp_puzzle_alter))
        cost = (traversal_cost + round((calculateHeuristic(temp_puzzle_alter))/1, 2))
        #cost = (traversal_cost + (calculateHeuristic(temp_puzzle_alter)))
        #states of U
        counter = abs(row_of_zero - row_limit)
        travel_path_temp = copy.deepcopy(""+str(travel_path_temp)+"D"+str(counter)+str(column_of_zero+1)+" ")
        #print "path of D",travel_path_temp
        #heapq.heappush(states,(cost,[temp_puzzle_alter,traversal_cost,travel_path_temp]))
        heapq.heappush(states,(cost,temp_puzzle_alter,traversal_cost,travel_path_temp))
        #print "altered puzzle is",temp_puzzle_alter
        temp_puzzle = copy.deepcopy(current_puzzle)
    #print "Final States:\n","\n".join(str([state]) if state else 0 for state in states)
    return states

def is_solvable(puzzle):
    row_of_zero,column_of_zero = location_of_zero(puzzle)
    temp = copy.deepcopy(puzzle)
    total = 0
    element_count = 0
    row_count = 0
    column_count = 0

    for row in puzzle:
        column_count = 0
        for element in row:
            if(element != 0):
                #print "row is:",row_count,"column_count:",column_count
                #print "element is:",element,"\n---------"
                check_row_count = 0
                check_column_count = 0
                for rowrow in temp:
                    check_column_count = 0
                    if(check_row_count >= row_count):
                        for elementelement in rowrow:
                            if(check_row_count == row_count):
                                if(check_column_count >= column_count):
                                    if((elementelement < element) and (elementelement != 0)):
                                    #    print "-----check_row_count is:",check_row_count,"check_column_count:",check_column_count
                                    #    print "-----elementelement is:",elementelement,"\n---------"
                                        total = total + 1
                                    #add_value
                            if(check_row_count > row_count):
                                if((elementelement < element) and (elementelement != 0)):
                                #    print "-----check_row_count is:",check_row_count,"check_column_count:",check_column_count
                                #    print "-----elementelement is:",elementelement,"\n---------"
                                    total = total + 1
                            check_column_count += 1
                    check_row_count += 1
            column_count += 1
        row_count += 1
#    print "row_of_zero",int(len(puzzle[0])- (row_of_zero)),"total Parity",total
    parity_row_zero = (int(len(puzzle[0])- (row_of_zero)) % 2)
    parity_of_board = (int(total) % 2)
    if(parity_row_zero == 0 and parity_of_board != 0):
        answer = True
    elif(parity_row_zero != 0 and parity_of_board == 0):
        answer = True
    else:
        answer = False
    return answer
#this function will solve the given board
#def solve(puzzle)
def solve(puzzle):
    nodes_ignored_in_fringe = 0
    nodes_ignored_in_closed = 0
    hash_closed ={}
    fringe = []#implement priority queue for fringe
    if(is_solvable(puzzle)):#check if the inut is even solvable.
        if is_goal(puzzle):
            print "Goal is:",puzzle
            print "nodes_ignored_in_fringe",nodes_ignored_in_fringe
            print "nodes_ignored_in_closed",nodes_ignored_in_closed
            '''print "Path taken is",temp1[3]'''
            print "total moves are : 0"
            print " "
            return puzzle
        #print "initial Heuristic",calculateHeuristic(puzzle)
        # change the insertion here
        heapq.heappush(fringe,(round(calculateHeuristic(puzzle)/3,2),puzzle,0,""))# 0 is the travel cost of the first node
        #print len(fringe)
        while len(fringe) > 0:
            #print heapq.heappop(fringe)[1]
            temp1 = heapq.heappop(fringe)
            #print temp1
            #print "len(fringe)",len(fringe)
            #adding closed
            #print temp1[1]
            hash_closed[str(hash(str(temp1[1])))] = temp1
            #print temp1[3]
            if is_goal(temp1[1]):
                print "Goal is:",temp1[1]
                print "nodes_ignored_in_fringe",nodes_ignored_in_fringe
                print "nodes_ignored_in_closed",nodes_ignored_in_closed
                '''print "Path taken is",temp1[3]'''
                print "total moves are :",len(temp1[3].split())
                print temp1[3]
                return temp1[1]
            temp = successors(temp1[1],temp1[2],temp1[3])
            #print "len(temp)",len(temp)
            for i in range(len(temp)):
                s = heapq.heappop(temp)
                #print str(s[1])
                #print "------------"
                try:
                    #if s is in CLOSED; discard s; continue discards that node
                    board_exists = hash_closed[str(hash(str(s[1])))]
                    #print "yay",s
                    nodes_ignored_in_closed += 1
                    #print "ignored",board_exists[1]
                    continue
                except KeyError:
                    pass
                    heapq.heappush(fringe,(s[0],s[1],s[2],s[3]))

                    #print round(s[0],2),s[1],s[2],s[3]

            #heapq.heapify(fringe)
        return False
    else:
        return False
                            #heapq.heappush(fringe,(s[0],s[1],s[2],s[3]))
                            #print "s[0]",s[0],"s[3]",s[3]
input1 = sys.argv[1]
#print input1
puzzle = []
with open(input1) as inputFile:#opened the file
    for line in inputFile:#read rest of the lines
        puzzle.append([int(x) for x in line.split()])#split the lines into individual elements

print ("Given board is:\n" + print_board(puzzle) + "\nLooking for solution.....")
solution = solve(puzzle)
#solve(puzzle)
