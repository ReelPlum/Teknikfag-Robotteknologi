from threading import Thread

def ThreadTing():
    while True:
        print("Hello")
        
        
thread = Thread(target = ThreadTing, args = ())
thread.start()