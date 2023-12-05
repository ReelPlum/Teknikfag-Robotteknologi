from tkinter import *


class Joystick(Frame):
    def __init__(self, master):
        Frame.__init__(self, master, background= "#ffffff")
        self.createWidgets()

    
    def createWidgets(self):
        B_Frem = Button(root, text="Frem", command= lambda: self.commandHandler("Frem")).grid(row=0, column =1, sticky= N+S+W+E)
        B_Tilbage = Button(root, text="Tilbage", command= lambda: self.commandHandler("Tilbage")).grid(row=2, column =1, sticky= N+S+W+E)
        B_Venstre = Button(root, text="Venstre", command= lambda: self.commandHandler("Venstre")).grid(row=1, column =0, sticky= N+S+W+E)
        B_Højre = Button(root, text="Højre", command= lambda: self.commandHandler("Højre")).grid(row=1, column =2, sticky= N+S+W+E)
        
    def commandHandler(self, bNo):
        if bNo == "Frem":
            print("Der er trykket på frem knappen")
        
        if bNo == "Tilbage":
            print("Der er trykket på tilbage knappen")
        
        if bNo == "Venstre":
            print("Der er trykket på venstre knappen")

        if bNo == "Højre":
            print("Der er trykket på højre knappen")

    




root = Tk()
root.title = ("Styring af Bevægende Wilson")
app = Joystick(root)
app.mainloop()

