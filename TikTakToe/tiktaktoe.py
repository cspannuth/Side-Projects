# Tik Tak Toe
# Programmed by: Connor Spannuth
# Date: 4/22/2024

import random
import time


class Board:
    def __init__(self):
        self.length = 3
        self.width = 3
        self.slots = [[' '] * self.width for row in range(self.length)]

    def __repr__(self):
        s = ''
        for row in range(self.length):
            s += '\n'
            s += str(row) + ' |'
            for col in range(self.width):
                s += self.slots[row][col] + '|'
        s += '\n'
        s += '  -------\n'
        s += '   0 1 2'
        return s

    def addMark(self, mark, order):
        """Adds a mark to the board"""
        if (self.slots[int(order[0])][int(order[1])] == ' '):
            self.slots[int(order[0])][int(order[1])] = mark

    def reset(self):
        """rests the board"""
        for row in range(self.length):
            for col in range(self.width):
                self.slots[row][col] = ' '

    def removeMark(self, order):
        """removes marks"""
        if (self.slots[int(order[0])][int(order[1])] != ' '):
            self.slots[int(order[0])][int(order[1])] = ' '

    def canAddTo(self, order):
        """Tests if the spot can be added to"""
        if (self.slots[int(order[0])][int(order[1])] == ' '):
            return True
        else:
            return False

    def isFull(self):
        """Checks if the board is full"""
        for row in range(self.width):
            for col in range(self.length):
                if (self.slots[row][col] == ' '):
                    return False
        return True

    def isWin(self, mark):
        """Determines if there is a win on the board"""
        return self.isWinAcross(mark) or self.isVerticalWin(mark) or self.isDiagonalWin(mark)

    def isWinAcross(self, mark):
        """Tests if there is a win across"""
        for row in range(self.length):
            if (self.slots[row][0] == mark and self.slots[row][1] == mark and self.slots[row][2] == mark):
                return True
        return False

    def isVerticalWin(self, mark):
        """Tests if there is a vertical win"""
        for col in range(self.width):
            if (self.slots[0][col] == mark and self.slots[1][col] == mark and self.slots[2][col] == mark):
                return True
        return False

    def isDiagonalWin(self, mark):
        """Tests if there is a diagonal up win"""
        if (self.slots[0][0] == mark and self.slots[1][1] == mark and self.slots[2][2] == mark):
            return True
        elif (self.slots[2][0] == mark and self.slots[1][1] == mark and self.slots[0][2] == mark):
            return True
        else:
            return False


class Player:
    def __init__(self, mark):
        self.mark = mark
        self.numMoves = 0
        self.isAi = False

    def __repr__(self):
        s = 'Player ' + self.mark
        return s

    def nextMove(self, b):
        order = input('Where would you like to go? \n')
        while (b.canAddTo(order) == False or len(order) != 2):
            print('Try again!')
            order = input('Where would you like to go? \n')
        self.numMoves += 1
        return order

    def opponentMark(self):
        if (self.mark == 'X'):
            return 'O'
        else:
            return 'X'

    def randomMove(self, b):
        """returns a random move on the board"""
        spotList = []
        for row in range(b.length):
            for col in range(b.width):
                if (b.canAddTo(str(row) + str(col))):
                    spotList += [str(row) + str(col)]
        return random.choice(spotList)

class randomPlayer(Player):
    """Subclass of Player, plays the game by choosing a random spot in the board"""

    def __init__(self, mark):
        super().__init__(mark)
        self.isAi = True

    def nextMove(self, b):
        self.numMoves += 1
        return self.randomMove(b)


class AiPlayer(Player):
    """Subclass of Player, plays the game with artificial intelligence"""

    def __init__(self, mark):
        super().__init__(mark)
        self.isAi = True

    def logic(self, b):
        """Runs the algorithm for the AI, and is sent to nextMove"""
        if(self.win1Turn(self.mark, b) != False):
            return self.win1Turn(self.mark, b)
        elif(self.win1Turn(self.opponentMark(), b) != False):
            return self.win1Turn(self.opponentMark(), b)
        elif(self.win2Turns(b) != False):
            return self.win2Turns(b)
        return self.randomMove(b)
    
    def win2Turns(self, b):
        """Checks if self could win in two turns. Returns the order if True, False if False"""
        for row in range(b.length):
            for col in range(b.width):
                subOrder = str(row) + str(col)
                if(b.canAddTo(subOrder)):
                    b.addMark(self.mark, subOrder)
                    for row1 in range(b.length):
                        for col1 in range(b.width):
                            subOrder1 = str(row) + str(col)
                            if(b.canAddTo(subOrder1)):
                                b.addMark(self.mark, subOrder1)
                                if(b.isWin(self.mark)):
                                    b.removeMark(subOrder1)
                                    return subOrder
                                b.removeMark(subOrder1)
                    b.removeMark(subOrder)
        return False
    
    def win1Turn(self, mark, b):
        """Checks if self could win in one turn. Returns the order if True, False if False"""
        for row in range(b.length):
            for col in range(b.width):
                subOrder = str(row) + str(col)
                if(b.canAddTo(subOrder)):
                    b.addMark(mark, subOrder)
                    if(b.isWin(mark)):
                        b.removeMark(subOrder)
                        return subOrder
                    b.removeMark(subOrder)
        return False

    def nextMove(self, b):
        self.numMoves += 1
        order = self.logic(b)
        return order

def intro():
    print("Welcome to TikTakToe!")
    answer = input("Would you like to play against another player? (y/n): ")
    if answer == "y":
        TikTakToe(Player("X"), Player("O"))
    else:
        answer = input("Easy or Hard difficulty? (e/h): ")
        if answer == "e":
            TikTakToe(Player("X"), randomPlayer("O"))
        else:
            TikTakToe(Player("X"), AiPlayer("O")) 

    answer = input("Would you like to play again? (y/n): ")
    if answer == "y":
        intro()
    else:
        print("Thank you for playing! -Programmed by Connor Spannuth")
        time.sleep(5)

def TikTakToe(p1, p2):
    """Initiates the game, and continus it until a win occurs"""
    if (p1.mark == p2.mark or p2.mark not in 'XO' or p1.mark not in 'XO'):
        print('Players need to have different checkers!')
        return None

    print('\nEnter your choice as two digits, first row, then column.\n You are player X!')
    print()
    b = Board()
    print(b)
    print()

    while True:
        if process_move(p1, b) == True:
            return b

        if process_move(p2, b) == True:
            return b



def process_move(p, b):
    """Processes a move by a player"""
    print(p, "'s Turn!\n")
    b.addMark(p.mark, p.nextMove(b))
    print(b)
    if (b.isWin(p.mark)):
        print(p, 'wins in', p.numMoves, 'moves!')
        return True
    elif (b.isFull()):
        print('The game is a tie!')
        return True
    else:
        return False

intro()