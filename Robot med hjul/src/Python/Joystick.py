import keyboard


class Controls():
    def __init__(self, root):
        self.root = root 
        self.chooseMovement()

    def chooseMovement(self):
        while True:    
            if keyboard.is_pressed('w') and keyboard.is_pressed('d'):
                movement = (1,1)
                print(movement)
                break

            if keyboard.is_pressed('w'):
                movement = (0,1)
                print(movement)
                break
                
            if keyboard.is_pressed("s"):
                movement = (1,0)
                print(movement)
                break

            if keyboard.is_pressed("d"):
                movement = (0,-1)
                print(movement)
                break

            if keyboard.is_pressed("a"):
                movement = (-1,0)
                print(movement)
                break



    def commandHandler(self, bNo):
        if bNo == "Frem":
            print("Der er trykket på frem knappen")
        
        if bNo == "Tilbage":
            print("Der er trykket på tilbage knappen")
        
        if bNo == "Venstre":
            print("Der er trykket på venstre knappen")

        if bNo == "Højre":
            print("Der er trykket på højre knappen")

