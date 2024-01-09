from tkinter import *
from Joystick import Controls

class Interface(Frame):
    def __init__(self, master):
        Frame.__init__(self, master, background= "#ffffff")
        self.master = master

        self.createWidgets(master)
    
    def createWidgets(self, root):
        moving = Controls(root)

root = Tk()
root.title = ("Styring af Bevægende Wilson")
app = Interface(root)
app.mainloop()
