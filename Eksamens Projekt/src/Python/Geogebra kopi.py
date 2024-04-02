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

import websocket
import keyboard
from threading import Thread
import time
from math import *

class Application(Frame):  # Application is a Frame (inheritance from Frame)
    def __init__(self, master):
        Frame.__init__(self, master, background='#ffffff')
        self.grid(sticky=N+S+E+W)  # put frame in toplevel window
        self.createWidgets(master)
    

        self.ws = websocket.WebSocketApp("ws://192.168.1.100:1337", on_message=self.onMessage)
        
        self.ws.X = 0
        self.ws.Y =0
        self.ws.Angle = 0

        self.X = 0
        self.Y = 0
        self.Angle = 0
        
        Thread(target = self.ws.run_forever).start()
        Thread(target = self.chooseMovement).start()

    def onMessage(self, app, msg):
        m = msg.split(":")
        if len(m) == 2:
            if m[0] == "xpos":
                app.X = float(m[1])
                
                self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)
            if m[0] == "ypos":
                app.Y = float(m[1])
                
                self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)
            if m[0] == "angle":
                app.Angle = float(m[1])
                
                self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)
            if m[0] == "targetx":
                app.TargetX = float(m[1])
                
                self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)
            if m[0] == "targety":
                app.TargetY = float(m[1])
                
                self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)
            if m[0] == "locationtoggle":
                if int(m[1]) == 1:
                    app.LocationToggle = True
                else:
                    app.LocationToggle = False
            
        else:
            return
        
        #Update point on graph
        self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)

    def commandHandler(self, bNo):
        print("Cmd handler called: " + str(bNo))

    def createWidgets(self, root):
        top = self.winfo_toplevel()
        # top.geometry("500x500")
        top.rowconfigure(0, weight=1)     # toplevel window rows scalable
        top.columnconfigure(0, weight=1)  # toplevel window colums scalable
        self.Canvas = Grapher(self)
        self.Canvas.grid(row=4, column=1, rowspan=3, columnspan=4, sticky=N+S+E+W)
        self.columnconfigure(1, weight=1)
        self.rowconfigure(4, weight=1)

        #Graph(canvas, "a*x^3+b*x^2+c*x+d")

        strVar = StringVar()

        formulaEntry = Entry(self, textvariable=strVar)
        formulaEntry.grid(row=1, column=1, sticky=N+S+E+W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(1, weight=1)

    def chooseMovement(self):
        self.Y = 0
        self.X = 0

        while True:
            time.sleep(0.1)
            x = 0
            y = 0
            Controllist = ["w","a","s","d"]
            
            multiplier = 1

            if keyboard.is_pressed("shift"):
                multiplier = 0.5

            if keyboard.is_pressed("ctrl"):
                multiplier = 0.25

            if keyboard.is_pressed("space"):
                multiplier = 0    

            if keyboard.is_pressed(Controllist[0]) or keyboard.is_pressed("up_arrow"):
                y = y + 1
        
            if keyboard.is_pressed(Controllist[1]) or keyboard.is_pressed("left_arrow"):
                x = x - 1
                
            if keyboard.is_pressed(Controllist[2]) or keyboard.is_pressed("down_arrow"):
                y = y - 1
            
            if keyboard.is_pressed(Controllist[3]) or keyboard.is_pressed("right_arrow"):
                x = x + 1
                    

            l = sqrt(x**2 + y**2)
            #print(l)
            if x != 0:
                x = x/l

            if y != 0:
                y = y/l


            message = (f"x:{x}, y:{y}]")
            #print(message)
            x = x * multiplier
            y = y * multiplier


            if self.X != x:
                self.X = x
                self.ws.send(f'rotate:{round(x*1000)}')

            if self.Y != y:
                self.Y = y
                self.ws.send(f'forward:{round(y*1000)}')
                
            self.ws.send(f'locationx:{round(self.Canvas.TX*1000)}')
            self.ws.send(f'locationy:{round(self.Canvas.TY*1000)}')
            v = 0
            if self.Canvas.TargetEnabled:
                v = 1
            
            self.ws.send(f'toggle_location:{v}')

root = Tk()

app = Application(root)
app.master.title('Wilson lokation')
app.mainloop()


