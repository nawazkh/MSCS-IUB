#!/usr/bin/env python
# nrooks.py : Solve the N-Rooks problem!
# D. Crandall, 2016
# Updated by Zehua Zhang, 2017
# Updated and Solved by Nawaz Hussain K 0003561850
#
# The N-rooks problem is: Given an empty NxN chessboard, place N rooks on the board so that no rooks
# can take any other, i.e. such that no two rooks share the same row or column.

import sys
import time

# Count # of pieces in given row
def count_on_row(board, row):
#    print "count_on_row"
    strtr = sum( board[row] )
#    print strtr
    return strtr

# Count # of pieces in given column
def count_on_col(board, col):
#    print "count_on_col"
    tes = sum( [ row[col] for row in board ] )
    #tes = sum(zip(*board)[col])#zip transforms rows to columns. makes the summation easiler
    return tes

# Count total # of pieces on board
def count_pieces(board):
#    print "count_pieces"
    strtemp = sum([ sum(row) for row in board ] )
    #strtemp = sum([sum(x) for x in zip(*board)])
#    print strtemp
    return strtemp

def count_on_left_right_diagonal(board, row, col):
    r1 = row
    c1 = col
    r2 = row - 1
    c2 = col - 1
    strtr = 0
    strtc = 0
    while r1 < N and c1 < N:
        #print "row and col are:", r1,c1

        strtr = strtr + board[r1][c1]
        #print strtr
        r1 += 1
        c1 += 1
    while r2 >= 0 and c2 >= 0:
        #print "row and col are:", r2,c2
        strtc = strtc + board[r2][c2]
        #print strtc
        r2 -= 1
        c2 -= 1
#    print "total elements in this diagonal from left to right is:", strtc + strtr - 1
    return (strtc + strtr)

def count_on_right_left_diagonal(board, row, col):
    r1 = row
    c1 = col
    r2 = row + 1
    c2 = col - 1
    strtr = 0
    strtc = 0
    while r1 >= 0 and c1 < N:
        #print "row and col are:", r1,c1

        strtr = strtr + board[r1][c1]
        #print strtr
        r1 -= 1
        c1 += 1
    while r2 < N and c2 >= 0:
        #print "row and col are:", r2,c2
        strtc = strtc + board[r2][c2]
        #print strtc
        r2 += 1
        c2 -= 1
    #print "total elements in this diagonal from left to right is:", strtc + strtr - 1
    return (strtc + strtr)


# Return a string with the board rendered in a human-friendly format
def printable_board(board):
#    print "printable_board"
    return "\n".join([ " ".join([ "R" if col else "_" for col in row ]) for row in board])

def modified_printable_board2_nqueen(board):
    printRow = ""
    r = 0
    c = 0
    for row in board:
        c = 0
        for col in row:
            if(r == rowExclude):
                if(c == colExclude):
                    printRow += "X "
                else:
                    if(col == 0):
                        printRow += "_ "
                    else:
                        printRow += "Q "
            else:
                if(col == 0):
                    printRow += "_ "
                else:
                    printRow += "Q "

            c += 1
        r += 1
        printRow += "\n"
    return printRow


def modified_printable_board2_nrook(board):
    printRow = ""
    r = 0
    c = 0
    for row in board:
        c = 0
        for col in row:
            if(r == rowExclude):
                if(c == colExclude):
                    printRow += "X "
                else:
                    if(col == 0):
                        printRow += "_ "
                    else:
                        printRow += "R "
            else:
                if(col == 0):
                    printRow += "_ "
                else:
                    printRow += "R "

            c += 1
        r += 1
        printRow += "\n"
    return printRow

# adds piece
def add_piece(board, row, col):
    stringTemp = board[0:row] + [board[row][0:col] + [1,] + board[row][col+1:]] + board[row+1:]
    return stringTemp

#initial Fringe = [000],[000],[000]
def successors2_nqueen_modified(board):
    tempList = []
    itemsOnRow = 0
    itemsOnBoard = count_pieces(board)
    for r in range(N):
        itemsOnRow = count_on_row(board,r)
        for c in range(N):
            if(itemsOnBoard <= N-1):# to remove adding of more than N items
                if(itemsOnRow == 0):#count_pieces(board)#if the item is present a row, exclude that row and if the item not present in that row, then add item
                    if(count_on_col(board,c) == 0):#if item not present in that column, then add item
                        if(count_on_left_right_diagonal(board,r,c) == 0):#checking the number of items on l-r diagonal. added if 0
                            if(count_on_right_left_diagonal(board,r,c) == 0):#checking the number of items on r-l diagonal. added if 0
                                if(board[r][c] != 1):#implies if 1 is at [1,1] do not re add onto it
                                    if(rowExclude < 0 or colExclude < 0):
                                        #do not add the state spaces which were marked X
                                        #how?
                                        #check (row,col) we are adding is equal to exclude ones
                                        #if yes, then do not add it in the search space itself
                                        #else add the case
                                        tempList.append(add_piece(board, r, c))
                                    else:
                                        if(r == rowExclude and c == colExclude):
                                            pass
                                        else:
                                            tempList.append(add_piece(board, r, c))
                                else:
                                    pass
                            else:
                                pass
                        else:
                            pass
                    else:
                        pass
                else:
                    pass
            else:
                pass
    return tempList


#def successors2_nqueen(board):
#    print board
#    tempList=[]
#    print "hi"
#    print tempList
#    for r in range(0,N):
#        for c in range(0,N):
#            if(board[r][c] == 1):
#                pass
#            else:       #add it to the fringe
#                if(count_pieces(board) <= N-1):
#                    print "in new else" ,
#                    if(count_on_row(board,r) == 0):
#                        if(count_on_col(board,c) == 0):
#                            if(count_on_left_right_diagonal(board,r,c) == 0):
                                #print "Am I here?"
#                                if(count_on_right_left_diagonal(board,r,c) == 0):
#
#                                    if(rowExclude < 0 or colExclude < 0):
#                                        tempList.append(add_piece(board, r, c))
                                        #return [add_piece(board, r, c)]
                                        #return tempList
#                                    else:
#                                        if(r == rowExclude and c == colExclude):
#                                            pass
#                                        else:
#                                            tempList.append(add_piece(board, r, c))
                                            #return [add_piece(board, r, c)]
                                            #return tempList
                                    #print "count_on_row(board,r)",count_on_row(board,r),"count_on_col(board,c)",count_on_col(board,c)
                                    #print "count_on_left_right_diagonal",count_on_left_right_diagonal(board,r,c),"count_on_right_left_diagonal",count_on_right_left_diagonal(board,r,c)
#                                else:
#                                    pass
#                            else:
#                                pass
#                        else:# ignore expansion if the column already has an item
#                            pass
#                    else:#if an item already there at row r, ignore expansion
#                        pass
#                else:
                    #print "excluding expanding of state space if the number of items are more than N"
#                    pass
#    return tempList


# Get list of next states of given board state for rooks
def successors2_nrook(board):
    tempList=[]
    itemsOnBoard = count_pieces(board)
    itemsOnRow = 0
#    print "hi"
#    print tempList
    for r in range(0,N):
        itemsOnRow = count_on_row(board,r)
        for c in range(0,N):
            if(board[r][c] == 1):#implies if 1 is at [1,1] do not re add onto it
                pass
            else:       #add it to the fringe
                if(itemsOnBoard <= N-1 ):# to remove adding of more than 3 items
                    #print "in new else" , count_pieces(board)#if the item is present a row, exclude that row
                    if(itemsOnRow == 0):#if the item not present in that row, then add item
                        if(count_on_col(board,c) == 0):#if item not present in that column, then add item
                            #print "count_on_row(board,r)",count_on_row(board,r),"count_on_col(board,c)",count_on_col(board,c)
                            if(rowExclude < 0 or colExclude < 0):
                                tempList.append(add_piece(board, r, c))
                            else:
                                if(r == rowExclude and c == colExclude):
                                    pass
                                else:
                                    tempList.append(add_piece(board, r, c))
                            #tempList.append(add_piece(board, r, c))
                        else:# ignore expansion if the column already has an item
                            pass
                    else:#if an item already there at row r, ignore expansion
                        pass
                else:
                    #print "excluding expanding of state space if the number of items are more than N"
                    pass
    return tempList

def is_goal(board):#is goal ??
#    print "is_goal"
    return count_pieces(board) == N and \
        all( [ count_on_row(board, r) <= 1 for r in range(0, N) ] ) and \
        all( [ count_on_col(board, c) <= 1 for c in range(0, N) ] )

# Solve n-rooks!
def solve_nrook(initial_board):
#    print "solve"
    fringe = [initial_board]
#    print fringe
    while len(fringe) > 0:
        #print "hi"
        #print fringe
        for s in successors2_nrook( fringe.pop() ):
            #print "fringe popped"
            #print s
            #print "---"
            if is_goal(s):
                return(s)

            fringe.append(s)
#            print fringe
    return False

def solve_nqueen_modified(initial_board):
#    print "solve"
    fringe = [initial_board]
#    print fringe
    while len(fringe) > 0:
        #print "hi"
#        print "fringe is: ",fringe
        s = fringe.pop()

#        print "Considered state is:", s
        #print "popped fringe is: ",s
        if is_goal(s):
            #print "goal state found"
            return(s)
        #print "-----"
        for s1 in successors2_nqueen_modified( s ):
            #print "fringe popped"
            #print s
            #print "aye aye"
#            print "successor of the earlier state is",s1
            fringe.append(s1)
#            print fringe
    return False


# This is N, the size of the board. It is passed through command line arguments.
N = int(sys.argv[2])
choice = str(sys.argv[1])
rowExclude = int(sys.argv[3]) - 1
colExclude = int(sys.argv[4]) - 1
#just added a new comment to check git
#added a new line of comment.
#print "Initial params set"
# The board is stored as a list-of-lists. Each inner list is a row of the board.
# A zero in a given square indicates no piece, and a 1 indicates a piece.
initial_board = [[0]*N]*N
#print ("Starting from initial board:\n" + modified_printable_board2(initial_board) + "\n\nLooking for solution...\n")
tempTime = int(time.time()*1000000)
if(choice == 'nqueen'):
    solution = solve_nqueen_modified(initial_board)
    print (modified_printable_board2_nqueen(solution) if solution else "Sorry, no solution found. :(")
    tempTime2 = int(time.time()*1000000)
    #print "Time Elapsed in milliseconds for",N," rooks using stack",tempTime2 - tempTime
elif(choice == 'nrook'):
    solution = solve_nrook(initial_board)
    print (modified_printable_board2_nrook(solution) if solution else "Sorry, no solution found. :(")
    tempTime2 = int(time.time()*1000000)
    #print "Time Elapsed in milliseconds for",N," rooks using stack",tempTime2 - tempTime
else:
    print "Wrong choice"
