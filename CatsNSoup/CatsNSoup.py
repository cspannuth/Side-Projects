# Inspiration taken from Cats N' Soup game on the appstore
# Programmed by Connor Spannuth 3-15-25

import time as t
import keyboard

class game:

    def __init__(self):
        self.totalMoney = 0
        self.catCount = 0
        self.soupCat = soupCat()
        self.brocCat = brocCat()
        self.milkCat = milkCat()
        self.meatCat = meatCat()
        self.diceCat = diceCat()
        self.catList = [self.soupCat, self.brocCat, self.milkCat, self.meatCat, self.diceCat]
    
    def addMoney(self, timeDif):
        self.totalMoney += 1
        for cat in self.catList:
            if(cat.bought == True):
                self.totalMoney += (cat.moneyRate * timeDif)
                
    def upgradeCat(self, cat):
           if(not self.catIsValid(cat)):
              print("Invalid Cat!")
              t.sleep(2)
              return
           else:
               cat = int(cat)
           if(self.catList[cat-1].bought == False):
              print("Cannot Upgrade!")
              t.sleep(2)
              return
           elif(self.totalMoney < self.catList[cat-1].upgradeCost):
              print("Not enough money!")
              t.sleep(2)
              return
           else:
               if(self.totalMoney >= (self.catList[cat-1].upgradeCost * 2.48832)):
                   self.totalMoney -= (self.catList[cat-1].upgradeCost * 2.48832)
                   self.catList[cat-1].level += 5
                   self.catList[cat-1].moneyRate *=2.48832
                   self.catList[cat-1].upgradeCost *=2.48832
               self.totalMoney -= self.catList[cat-1].upgradeCost
               self.catList[cat-1].level += 1
               self.catList[cat-1].moneyRate *=1.2
               self.catList[cat-1].upgradeCost *=1.2    
    
    def catIsValid(self, cat):
        if(cat.isdigit()):
            cat = int(cat)
            if(cat > 0 and cat <= len(self.catList)):
                return True
        return False
    
    def buyCat(self, cat):
        if(not self.catIsValid(cat)):
           print("Invalid Cat!")
           t.sleep(2)
           return
        else:
            cat = int(cat)
        if(self.catList[cat-1].bought):
           print("Cannot Buy!")
           t.sleep(2)
           return
        elif(self.totalMoney < self.catList[cat-1].upgradeCost):
           print("Not enough money!")
           print(f"This cat costs {self.catList[cat-1].upgradeCost}")
           t.sleep(2)
           return
        else:
            self.totalMoney -= self.catList[cat-1].upgradeCost
            self.catList[cat-1].moneyRate *=1.2
            self.catList[cat-1].upgradeCost *=1.2
            self.catList[cat-1].bought = True
            self.catList[cat-1].level += 1
            self.catCount += 1
            match cat:
                case 1:
                    self.catList[0].name = "SoupCat"
                case 2:
                    self.catList[1].name = "BrocCat"
                case 3:
                    self.catList[2].name = "MilkCat"
                case 4:
                    self.catList[3].name = "MeatCat"
                case 5:
                    self.catList[4].name = "DiceCat"
                
    def downgradeCat(self, cat):
        if(self.catIsValid(cat) == False):
            print("Invalid Cat!")
            t.sleep(2)
            return
        else:
            cat = int(cat)
        if(self.catList[cat-1].bought == False):
            print("You haven't bought this cat!")
            t.sleep(2)
            return
        elif(self.catList[cat-1].level == 1):
            print("Cannot downgrade this cat")
            t.sleep(2)
            return
        else:
            self.catList[cat-1].level -= 1
            self.catList[cat-1].upgradeCost /= 1.2
            self.catList[cat-1].moneyRate /= 1.2
            self.totalMoney += (self.catList[cat-1].upgradeCost * 0.85)
            
    def renameCat(self, cat, name):
        if(self.catIsValid(cat)):
            if(len(name) > 0 and len(name) <= 7):
                cat = int(cat)
                if(self.catList[cat-1].bought):
                    for i in range(7-len(name)):
                        name += ' '
                    self.catList[cat-1].name = name
                else:
                    print("You must unlock this cat first!")
                    t.sleep(2)
            else:
                print("Invalid Length")
                t.sleep(2)
        else:
            print("Invalid Cat!")
            t.sleep(2)
    def getCost(self, cat):
        if(self.catIsValid(cat)):
            cat = int(cat)
            return self.catList[cat-1].upgradeCost
    
    def isBought(self, cat):
        if(self.catIsValid(cat)):
            cat = int(cat)
            if(self.catList[cat-1].bought):
                return True
        return False
        
                
    def printGame(self):
        print("\n\n\n\n")
        print("\nCats N' Soup! Terminal edition >_<")
        print("Total Money:", round(self.totalMoney, 2))
        print("    1             2           3            4             5")
        print(" ________     ________     ________     ________     ________")
        print(" |      |     |      |     |      |     |      |     |      |")
        print(" |      |     |      |     |      |     |      |     |      |")
        print(" |      |     |      |     |      |     |      |     |      |")
        print(f"  {self.soupCat.name}      {self.brocCat.name}      {self.milkCat.name}      {self.meatCat.name}      {self.diceCat.name}")
        print(f" |Lvl:{self.soupCat.level} |     |Lvl:{self.brocCat.level} |     |Lvl:{self.milkCat.level} |     |Lvl:{self.meatCat.level} |     |Lvl:{self.diceCat.level} |")
        print(" ________     ________     ________     ________     ________")
        print("1. Buy Cat, 2. Upgrade Cat, 3. Downgrade Cat, 4. Rename Cat, 5. Cat Upgrade Cost")
        
    def main(self):
        print("Welcome to Cats N' Soup! /Terminal/Edition/1.0")
        print("At any point to select an option, press space!")
        userInput = input("Do you want to start the game? (y/n): ")
        if(userInput != "y"): 
            return
        curTime = int(t.time())
        while(userInput != "q"):
            if(keyboard.is_pressed(' ')):
                self.printGame()
                userInput = input("$")
                match userInput:
                    case "1":
                        self.printGame()
                        userInput = input("Which cat do you want to buy?: ")
                        self.buyCat(userInput)
                    case "2":
                        self.printGame()
                        userInput = input("Which cat do you want to upgrade?: ")
                        self.upgradeCat(userInput)
                    case "3":
                        self.printGame()
                        userInput = input ("Which cat would you like to downgrade?: ")
                        self.downgradeCat(userInput)
                    case "4":
                        self.printGame()
                        catInput = input("Which cat would you like to rename?: ")
                        userInput = input("Enter a name (7 letters max): ")
                        self.renameCat(catInput, userInput)
                    case "5":
                        self.printGame()
                        cat = input("Which cat do you want to check the cost of?: ")
                        if(self.isBought(cat)):
                            print(f"This upgrade is ${self.getCost(cat)}")
                        elif(self.catIsValid(cat)):
                            print(f"This costs ${self.getCost(cat)} to buy")
                        t.sleep(2)
                    case "q":
                        return
            if(curTime != int(t.time())):
                timeDif = int(t.time()) - curTime
                curTime = int(t.time())
                self.addMoney(timeDif)
                
                self.printGame()

            
class soupCat:
    
    def __init__(self):
        self.name = "Locked!"
        self.level = 0
        self.moneyRate = 1
        self.upgradeCost = 10
        self.bought = False
        
class brocCat:
    
    def __init__(self):
        self.name = "Locked!"
        self.level = 0
        self.moneyRate = 5
        self.upgradeCost = 100
        self.bought = False
        
class milkCat:
    
    def __init__(self):
        self.name = "Locked!"
        self.level = 0
        self.moneyRate = 10
        self.upgradeCost = 500
        self.bought = False
        
class meatCat:
    
    def __init__(self):
        self.name = "Locked!"
        self.level = 0
        self.moneyRate = 20
        self.upgradeCost = 1000
        self.bought = False
        
class diceCat:
    
    def __init__(self):
        self.name = "Locked!"
        self.level = 0
        self.moneyRate = 50
        self.upgradeCost = 2500
        self.bought = False

Game = game()
Game.main()