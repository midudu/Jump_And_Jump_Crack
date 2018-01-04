import pythoncom
import pyHook
import win32api
import win32con
import win32gui
from ctypes import *
import time
import math

a_x = a_y = 0
b_x = b_y = 0
count = 0

#simulate a click by the left key
def mouse_click(x=None,y=None, distance = None):
    if not x is None and not y is None:
        mouse_move(x,y)
        time.sleep(0.05)

    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0)
    t = distance/352.16                #A constant figured out by experiments
    #print t
    time.sleep(t)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP, 0, 0, 0, 0)

def mouse_move(x,y):
    windll.user32.SetCursorPos(x, y)


#Keyboard Event
def onKeyboardEvent(event):
    global a_x, a_y, b_x, b_y
    global count 

    if event.Key == 'A':
        if count == 0:
            count = (count + 1)%2
            a_x, a_y = win32api.GetCursorPos()
            # print a_x, a_y
        else:
            count = (count + 1)%2
            b_x, b_y = win32api.GetCursorPos()
            # print b_x, b_y
            distance = math.sqrt( (a_x - b_x) * (a_x - b_x) + (a_y - b_y) * (a_y - b_y) )
            #print distance
            mouse_click(959, 539, distance)
 
    return True


def main():

    #start an hook-manager object
    hm = pyHook.HookManager()
 
    #watch all the keyboard events
    hm.KeyDown = onKeyboardEvent
    
    #set keyboard hook
    hm.HookKeyboard()
 
    #start watching
    pythoncom.PumpMessages()
    



if __name__ == "__main__":
    main()
