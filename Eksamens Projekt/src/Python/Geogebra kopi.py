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
        Frame.__init__(self, master, background="#ffffff")

        self.ws = websocket.WebSocketApp(
            "ws://192.168.1.104:1337", on_message=self.onMessage
        )

        self.ws.X = 0
        self.ws.Y = 0
        self.ws.Angle = 0

        self.X = 0
        self.Y = 0
        self.Angle = 0

        self.K = 0.015
        self.GyroSens = 20
        self.KI = 2.0
        self.KP = 305
        self.TargetAngle = 0

        self.grid(sticky=N + S + E + W)  # put frame in toplevel window
        self.createWidgets(master)

        Thread(target=self.ws.run_forever).start()
        Thread(target=self.chooseMovement).start()

    def onMessage(self, app, msg):
        try:

            m = msg.split(":")
            if len(m) == 2:
                if m[0] == "xpos":
                    app.X = float(m[1])

                    print(f"X: {float(m[1])}")
                if m[0] == "ypos":
                    app.Y = float(m[1])

                    print(f"Y: {float(m[1])}")
                if m[0] == "angle":
                    app.Angle = float(m[1])
                if m[0] == "error":
                    self.CurrentAngle.set(f"Error: {float(m[1]) / 1000}")

                if m[0] == "rightencoder":
                    # self.CurrentAngle.set(f"Error: {m[1]/1000}")
                    # print(f"Right encoder: {m[1]}")
                    pass

                if m[0] == "leftencoder":
                    # self.CurrentAngle.set(f"Error: {m[1]/1000}")
                    # print(f"Left encoder: {m[1]}")
                    pass

            else:
                return

            # Update point on graph
            self.Canvas.updatePoint(app.X, app.Y, app.Angle, app.TargetX, app.TargetY)

        except Exception as e:
            print(f"error: {e}")

    def commandHandler(self, bNo):
        print("Cmd handler called: " + str(bNo))

    def updateGyroSens(self, a, b, c):
        try:
            value = float(self.SensVar.get())

            self.GyroSens = value

            self.ws.send(f"set_kd:{round(self.GyroSens*1000)}")

        except Exception as e:
            print(f"Error on set kd: {e}")

    def updateK(self, a, b, c):
        try:
            value = float(self.KVar.get())
            if value > 1:
                value = 1

            if value < 0:
                value = 0

            self.K = value

            self.ws.send(f"setk:{round(self.K*1000)}")
        except Exception as e:
            print(f"Error on set k: {e}")

    def updateKI(self, a, b, c):
        try:
            value = float(self.KIVar.get())

            self.KI = value

            self.ws.send(f"set_ki:{round(self.KI*1000)}")
        except Exception as e:
            print(f"Error on set KI: {e}")

    def updateKP(self, a, b, c):
        try:
            value = float(self.KPVar.get())

            self.KP = value

            self.ws.send(f"set_kp:{round(self.KP*1000)}")
        except Exception as e:
            print(f"Error on set KP: {e}")

    def updateTargetAngle(
        self,
        a,
        b,
        c,
    ):
        try:
            value = float(self.TargetAngleVar.get())

            self.TargetAngle = value

            self.ws.send(f"set_target_angle:{round(self.TargetAngle*1000)}")
        except Exception as e:
            print(f"Error on set KP: {e}")

    def toggleBuzzer(self, a):
        # Toggle buzzer
        self.ws.send(f"togglebuzzer:1")

    def createWidgets(self, root):
        top = self.winfo_toplevel()
        # top.geometry("500x500")
        top.rowconfigure(0, weight=1)  # toplevel window rows scalable
        top.columnconfigure(0, weight=1)  # toplevel window colums scalable
        self.Canvas = Grapher(self)
        self.Canvas.grid(row=4, column=1, rowspan=3, columnspan=8, sticky=N + S + E + W)
        self.columnconfigure(1, weight=1)
        self.rowconfigure(4, weight=1)

        self.ws.Canvas = self.Canvas

        # Graph(canvas, "a*x^3+b*x^2+c*x+d")

        self.KVar = StringVar()
        self.KVar.trace_add(mode="write", callback=self.updateK)
        self.KVar.set(str(self.K))

        self.SensVar = StringVar()
        self.SensVar.trace_add(mode="write", callback=self.updateGyroSens)
        self.SensVar.set(str(self.GyroSens))

        self.KIVar = StringVar()
        self.KIVar.trace_add(mode="write", callback=self.updateKI)
        self.KIVar.set(str(self.KI))

        self.KPVar = StringVar()
        self.KPVar.trace_add(mode="write", callback=self.updateKP)
        self.KPVar.set(str(self.KP))

        self.TargetAngleVar = StringVar()
        self.TargetAngleVar.trace_add(mode="write", callback=self.updateTargetAngle)
        self.TargetAngleVar.set(str(0))

        self.CurrentAngle = StringVar()
        self.CurrentAngle.set(f"Error: {str(0)}")

        # formulaEntry = Entry(self, textvariable=strVar)
        # formulaEntry.grid(row=1, column=1, sticky=N+S+E+W)
        # self.rowconfigure(1, weight=0)
        # self.columnconfigure(1, weight=1)

        SensorFusionTxt = Label(self, text="K:")
        SensorFusionTxt.grid(row=1, column=1, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(1, weight=0)

        self.KEntry = Entry(self, textvariable=self.KVar)
        self.KEntry.grid(row=1, column=2, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(2, weight=1)

        KDTxt = Label(self, text="KD:")
        KDTxt.grid(row=1, column=3, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(3, weight=0)

        self.SensEntry = Entry(self, textvariable=self.SensVar)
        self.SensEntry.grid(row=1, column=4, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(4, weight=1)

        KITxt = Label(self, text="KI:")
        KITxt.grid(row=1, column=5, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(5, weight=0)

        self.KIEntry = Entry(self, textvariable=self.KIVar)
        self.KIEntry.grid(row=1, column=6, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(6, weight=1)

        KPTxt = Label(self, text="KP:")
        KPTxt.grid(row=1, column=7, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(7, weight=0)

        self.KPEntry = Entry(self, textvariable=self.KPVar)
        self.KPEntry.grid(row=1, column=8, sticky=N + S + E + W)
        self.rowconfigure(1, weight=0)
        self.columnconfigure(8, weight=1)

        TargetAngleEntry = Entry(self, textvariable=self.TargetAngleVar)
        TargetAngleEntry.grid(row=5, column=4, sticky=N + S + E + W)
        self.rowconfigure(5, weight=0)

        TargetAngleTxt = Label(self, text="Target Angle:")
        TargetAngleTxt.grid(row=5, column=3, sticky=N + S + E + W)

        TargetAngleTxt = Label(
            self, text="Target Angle:", textvariable=self.CurrentAngle
        )
        TargetAngleTxt.grid(row=6, column=4, sticky=N + S + E + W)

        ToggleBuzzerButton = Button(
            self, text="Toggle Buzzer", command=self.toggleBuzzer
        )
        ToggleBuzzerButton.grid(row=6, column=5, sticky=N + S + E + W)

    def chooseMovement(self):
        self.Y = 0
        self.X = 0

        locy = 0
        locx = 0
        loctog = False

        while True:
            time.sleep(0.1)
            x = 0
            y = 0
            Controllist = ["w", "a", "s", "d"]

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

            if keyboard.is_pressed(Controllist[3]) or keyboard.is_pressed(
                "right_arrow"
            ):
                x = x + 1

            l = sqrt(x**2 + y**2)
            # print(l)
            if x != 0:
                x = x / l

            if y != 0:
                y = y / l

            x = x * multiplier
            y = y * multiplier

            if self.X != x:
                self.X = x
                self.ws.send(f"rotate:{round(x*1000)}")

            if self.Y != y:
                self.Y = y
                self.ws.send(f"forward:{round(y*1000)}")


root = Tk()

app = Application(root)
app.master.title("Wilson lokation")
app.mainloop()
