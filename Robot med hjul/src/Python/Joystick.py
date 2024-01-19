import time
from tkinter import *
from math import *


class Controls(Frame):
    def __init__(self, master):
        Frame.__init__(self, master) 
        self.grid(sticky=N+S+E+W) # put frame in toplevel window


root = Tk()
app = Controls(root)
app.master.title("Joystick")
app.mainloop()