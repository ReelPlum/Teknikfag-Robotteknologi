from tkinter import *
import websocket
import keyboard
from threading import Thread
import time
from math import *

class Interface():
    
    def onMessage(self, app, msg):
        print(msg)
        
        m = msg.split(":")
    
    def __init__(self):
        #Frame.__init__(self, master, background= "#ffffff")
        #self.master = master
        #self.ip_popup()
        print("Startup")
        #websocket.enableTrace(True)
        self.ws = websocket.WebSocketApp("ws://192.168.1.100:1337", on_message=self.onMessage)
        
        Thread(target = self.ws.run_forever).start()
        self.chooseMovement()

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



#root = Tk()
#root.title = ("Styring af Bevægende Wilson")
app = Interface()
#app.mainloop()
