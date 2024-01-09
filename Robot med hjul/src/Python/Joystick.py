import keyboard
import time

class Controls():
    def __init__(self, root):
        self.root = root 
        self.chooseMovement()

    def chooseMovement(self):
        while True:  
            x = 0
            y = 0
            Controllist = ["w","a","s","d"]
            if keyboard.is_pressed(Controllist[0]) or keyboard.is_pressed("up_arrow"):
                y = y + 1
        
            if keyboard.is_pressed(Controllist[1]) or keyboard.is_pressed("left_arrow"):                   
                x = x - 1
            
            if keyboard.is_pressed(Controllist[2]) or keyboard.is_pressed("down_arrow"):
                y = y - 1
            
            if keyboard.is_pressed(Controllist[3]) or keyboard.is_pressed("right_arrow"):
                x = x + 1

            print(x,y)

    def commandHandler(self, bNo):
        if bNo == "Frem":
            print("Der er trykket på frem knappen")
        
        if bNo == "Tilbage":
            print("Der er trykket på tilbage knappen")
        
        if bNo == "Venstre":
            print("Der er trykket på venstre knappen")

        if bNo == "Højre":
            print("Der er trykket på højre knappen")

