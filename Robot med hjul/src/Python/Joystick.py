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

        self.MousePressed = False

        self.grid(sticky=N+S+E+W) # put frame in toplevel window
        master.bind('<Motion>', self.mouselocation)
        master.bind("<ButtonPress-1>", self.mouseDown)
        master.bind("<ButtonRelease-1>", self.mouseUp)

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

    def mouseDown(self, event):
        self.MousePressed = True

    def mouseUp(self, event):
        self.MousePressed = False

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
            x = self.x
            y = self.y

            if not (self.MousePressed):
                x = 0
                y = 0

            time.sleep(0.2)
            message = (f"x:{x}, y:{y}]")
            print(message)

            if self.X != x:
                self.X = x
                self.ws.send(f'rotate:{round(x*1000)}')

            if self.Y != y:
                self.Y = y
                self.ws.send(f'forward:{round(y*1000)}')



def updateTask(root, canvas):
    SIZE = root.winfo_width() ,root.winfo_height()
    root.after(RATE, updateTask, root, canvas)

root = Tk()
app = Controls(root)
app.master.title("Joystick")
app.mainloop()
