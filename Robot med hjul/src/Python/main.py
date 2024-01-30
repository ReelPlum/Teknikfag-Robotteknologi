from tkinter import *
import websocket
import keyboard
import _thread as thread
import time
from math import *

class Interface(Frame):
    def __init__(self, master):
        Frame.__init__(self, master, background= "#ffffff")
        self.master = master
        self.ip_popup()
        self.ws = websocket.WebSocketApp("ws://192.168.1.100:1337", on_message=self.onMessage)
        thread.start_new_thread(lambda *args: self.ws.run_forever(), ())

        self.chooseMovement()

    def onMessage(self, msg):
        print(msg)
        
        m = msg.split(":")
        if len(m) == 2:
            if m[0] == "xpos":
                pass

    def ip_popup(self):
        Pop_L = Label(root, text="Insert Ip for Server:" )
        Pop_L.grid(row = 0, column = 0)

        Pop_E = Entry(root)
        Pop_E.grid(row = 0, column = 1)
        
        Pop_B = Button(text="Proceed", command=lambda: print("hej"))
        Pop_B.grid(row = 0, column = 2)
        

    def chooseMovement(self):
        self.Y = 0
        self.X = 0

        while True:
            time.sleep(0.2)
            x = 0
            y = 0
            Controllist = ["w","a","s","d"]
            
            multiplier = 1

            if keyboard.is_pressed("shift"):
                multiplier = 0.5

            if keyboard.is_pressed("ctrl"):
                multiplier = 0.25

            if keyboard.is_pressed(Controllist[0]) or keyboard.is_pressed("up_arrow"):
                y = y + 1
        
            if keyboard.is_pressed(Controllist[1]) or keyboard.is_pressed("left_arrow"):
                x = x - 1
                
            if keyboard.is_pressed(Controllist[2]) or keyboard.is_pressed("down_arrow"):
                y = y - 1
            
            if keyboard.is_pressed(Controllist[3]) or keyboard.is_pressed("right_arrow"):
                x = x + 1
                    

            l = sqrt(x**2 + y**2)
            print(l)
            if x != 0:
                x = x/l

            if y != 0:
                y = y/l


            message = (f"x:{x}, y:{y}]")
            print(message)
            x = x * multiplier
            y = y * multiplier


            if self.X != x:
                self.X = x
                self.ws.send(f'rotate:{round(x*1000)}')

            if self.Y != y:
                self.Y = y
                self.ws.send(f'forward:{round(y*1000)}')



root = Tk()
root.title = ("Styring af Bevægende Wilson")
app = Interface(root)
app.mainloop()
