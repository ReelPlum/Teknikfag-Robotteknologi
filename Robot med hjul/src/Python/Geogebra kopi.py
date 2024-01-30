# -------------------------------------------------------------------------------
# Name:        tkinter demo app
# Purpose:
#
# Author:      josa
#
# Created:     15-01-2020
# -------------------------------------------------------------------------------
from tkinter import *
from Grapher import Grapher


class Application(Frame):  # Application is a Frame (inheritance from Frame)
    def __init__(self, master):
        Frame.__init__(self, master, background='#ffffff')
        self.grid(sticky=N+S+E+W)  # put frame in toplevel window
        self.createWidgets(master)

    def commandHandler(self, bNo):
        print("Cmd handler called: " + str(bNo))

    def createWidgets(self, root):
        top = self.winfo_toplevel()
        # top.geometry("500x500")
        top.rowconfigure(0, weight=1)     # toplevel window rows scalable
        top.columnconfigure(0, weight=1)  # toplevel window colums scalable
        canvas = Grapher(self)
        canvas.grid(row=4, column=1, rowspan=3, columnspan=4, sticky=N+S+E+W)
        self.columnconfigure(1, weight=1)
        self.rowconfigure(4, weight=1)

        #Graph(canvas, "a*x^3+b*x^2+c*x+d")

        strVar = StringVar()

        formulaEntry = Entry(self, textvariable=strVar)
        formulaEntry.grid(row=1, column=1, sticky=N+S+E+W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(1, weight=1)


root = Tk()

app = Application(root)
app.master.title('Wilson lokation')
app.mainloop()


