from tkinter import *
import websocket
import keyboard
import _thread as thread
import time

class Interface(Frame):
    def __init__(self, master):
        Frame.__init__(self, master, background= "#ffffff")
        self.master = master
        self.ip_popup()
        self.ws = websocket.WebSocketApp("ws://192.168.1.100:1337")
        thread.start_new_thread(lambda *args: self.ws.run_forever(), ())

        self.chooseMovement()


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
            time.sleep(0.5)
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
                    
            message = (f"x:{x}, y:{y}]")
            print(message)

            if self.X != x:
                self.X = x
                self.ws.send(f'x:{x}')

            if self.Y != y:
                self.Y = y
                self.ws.send(f'y:{y}')



root = Tk()
root.title = ("Styring af Bevægende Wilson")
app = Interface(root)
app.mainloop()
