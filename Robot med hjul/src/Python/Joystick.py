import time
from tkinter import *
from math import *
import _thread as thread
import websocket

RATE = 16
SIZE = 800, 500

class Controls(Frame):
    def __init__(self, master):
        Frame.__init__(self, master) 

        self.Master = master

        self.x = 0
        self.y = 0

        self.grid(sticky=N+S+E+W) # put frame in toplevel window
        master.bind('<Motion>', self.mouselocation)
        self.main()

        self.ws = websocket.WebSocketApp("ws://192.168.1.100:1337")
        thread.start_new_thread(lambda *args: self.ws.run_forever(), ())

        thread.start_new_thread(lambda *args: self.chooseMovement(), ())

    def main(self):
        top=self.winfo_toplevel()
        top.geometry("800x500")
        canvas = Canvas(width=SIZE[0], height = SIZE[1])
        #canvas.pack(expand=YES, fill=BOTH)
        updateTask(root, canvas)

    def mouselocation(self, event):
        SIZE = self.Master.winfo_width() ,self.Master.winfo_height()

        x,y= -(SIZE[0]/2 - event.x), SIZE[1]/2 - event.y
        x = x / (SIZE[0]/2)
        y = y / (SIZE[1]/2)

        l = sqrt(x**2 + y**2)
        
        if l > 1:
            x = x / l
            y = y / l

        self.x = x
        self.y = y
        
        #print('{}, {}'.format(x, y))

    def chooseMovement(self):
        self.Y = 0
        self.X = 0

        while True:
            time.sleep(0.2)
            message = (f"x:{self.x}, y:{self.y}]")
            print(message)

            if self.X != self.x:
                self.X = self.x
                self.ws.send(f'rotate:{round(self.x*1000)}')

            if self.Y != self.y:
                self.Y = self.y
                self.ws.send(f'forward:{round(self.y*1000)}')



def updateTask(root, canvas):
    SIZE = root.winfo_width() ,root.winfo_height()
    root.after(RATE, updateTask, root, canvas)

root = Tk()
app = Controls(root)
app.master.title("Joystick")
app.mainloop()
