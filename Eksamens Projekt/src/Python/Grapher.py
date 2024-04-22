from tkinter import *
from math import *


class Grapher(Canvas):
    def __init__(self, master):
        Canvas.__init__(self, master, bg="white")
        self.grid(sticky=N + S + E + W)

        self.A = 0
        self.B = 0
        self.C = 0

        self.X = 0
        self.Y = 0
        self.Angle = 0

        self.Zoom = 1
        self.Offset = [0, 0]

        self.LastMousePosition = None

        x, y = self.calculate(self.X, self.Y)
        self.Oval = self.create_oval(x - 5, y - 5, x + 5, y + 5, fill="black")
        self.Line = self.create_line(0, 0, 0, 0)

        def motion(event):
            if not self.LastMousePosition:
                return

            self.Offset[0] = (
                self.Offset[0] - (event.x - self.LastMousePosition[0]) / self.Zoom
            )
            self.Offset[1] = (
                self.Offset[1] - (event.y - self.LastMousePosition[1]) / self.Zoom
            )

            self.LastMousePosition = (event.x, event.y)

            self.render()

        def mouseDown(event):
            self.LastMousePosition = (event.x, event.y)

        def mouseRelease(event):
            self.LastMousePosition = None

        def changed(event):
            self.render()

        self.bind("<B1-Motion>", motion)
        self.bind("<Button-1>", mouseDown)
        self.bind("<ButtonRelease-1>", mouseRelease)
        self.bind("<Configure>", changed)

        self.render()

    def updatePoint(
        self,
        x,
        y,
        angle,
    ):
        self.X = x
        self.Y = y
        self.Angle = angle

        x, y = self.calculate(self.X, self.Y)

        # self.move(self.Oval, x, y)
        self.delete(self.Oval)
        self.delete(self.Line)

        self.Oval = self.create_oval(x - 8, y - 8, x + 8, y + 8, fill="black")

        xEnd = x + sin(angle) * 25
        yEnd = y - cos(angle) * 25

        self.Line = self.create_polygon(
            [
                x,
                y,
                x + cos(angle) * 10,
                y + sin(angle) * 10,
                xEnd,
                yEnd,
                x - cos(angle) * 10,
                y - sin(angle) * 10,
            ]
        )

    def calculate(self, x, y):
        size = (self.winfo_width(), self.winfo_height())

        middleX = self.Offset[0]
        middleY = self.Offset[1]

        zoom = self.Zoom

        x0 = size[0] / 2 - middleX * zoom
        y0 = size[1] / 2 - middleY * zoom

        xPos = x0 + x * zoom
        yPos = y0 - y * zoom

        return xPos, yPos

    def setZoom(self, zoom):
        self.Zoom = zoom

    def render(self):

        self.delete(ALL)

        size = (self.winfo_width(), self.winfo_height())

        # Render the graph.
        middleX = self.Offset[0]
        middleY = self.Offset[1]

        zoom = self.Zoom

        zooms = zoom
        if zooms > 1:
            zooms = 1

        x0 = size[0] / 2 - middleX * zoom
        y0 = size[1] / 2 - middleY * zoom

        # Render squares
        num = round((50 / zoom) / 10) * 10
        if num <= 10:
            num = 10

        xTextPos = 30
        yTextPos = -30
        # Create bigger lines

        for i in range(1, int((size[1] / zooms - y0) / num) + 2, 1):

            y1 = y0 + num * zoom * i

            self.create_line(0, y1, size[0], y1, fill="gray")

            # Render text
            self.create_text(
                x0 + xTextPos, y0 + num * zoom * i, text=f"-{round(i * num)}"
            )

        for f in range(1, int((size[0] / zooms - x0) / num) + 2, 1):
            x1 = x0 + num * zoom * f

            self.create_line(x1, 0, x1, size[1], fill="gray")

            # Render text
            self.create_text(
                x0 + num * zoom * f, y0 + yTextPos, text=f"{round(f * num)}"
            )

        for i in range(0, int(y0 / zooms / num) + 2, 1):
            y1 = y0 - num * zoom * i

            self.create_line(0, y1, size[0], y1, fill="gray")

            # Render text
            self.create_text(x0 + xTextPos, y0 - num * zoom * i, text=f"{round(i*num)}")

        for f in range(0, int(x0 / zooms / num) + 2, 1):
            x1 = x0 - num * zoom * f

            self.create_line(x1, 0, x1, size[1], fill="gray")

            # Render text
            self.create_text(
                x0 - num * zoom * f, y0 + yTextPos, text=f"-{round(f * num)}"
            )

        # Render x-axis
        self.create_line(0, y0, size[0], y0, fill="black", width=0)

        # Render y-axis
        self.create_line(x0, 0, x0, size[1], fill="black", width=0)
