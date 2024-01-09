from tkinter import *
from Joystick import Controls

class Interface(Frame):
    def __init__(self, master):
        Frame.__init__(self, master, background= "#ffffff")
        self.master = master

        self.createWidgets(master)
    
    def createWidgets(self, root):
        Button(root, text = "W", command=lambda: print("defafe")).grid(sticky=N+S+W+E, row=0, column=1)
        moving = Controls(root)

root = Tk()
root.title = ("Styring af Bevægende Wilson")
app = Interface(root)
app.mainloop()
