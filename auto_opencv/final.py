from ctypes import *
from PIL import ImageGrab
import pythoncom
import pyHook
import win32api
import win32con
import win32gui
import time
import math
import os



def mouse_move(x,y):
    windll.user32.SetCursorPos(x, y)

def mouse_click(x=None,y=None, distance = None):
    if not x is None and not y is None:
        mouse_move(x,y)
        time.sleep(0.05)
    
    t = distance/306.0    #A constant figured out by experiments
    print t
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0) 
    time.sleep(t)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP, 0, 0, 0, 0)

def get_chess_coordinate_from_file():
    file_object = open("coordinate.txt",'r')

    x_chess_coordinate = file_object.readline()
    y_chess_coordinate = file_object.readline()

    file_object.close()

    return int(x_chess_coordinate), int(y_chess_coordinate)

def get_next_block_coordinate_from_file():
    file_object = open("next_block.txt",'r')
    
    next_block_location = file_object.readline()
    
    file_object.close()
    
    return int(next_block_location)


def main():
    jump_dll = cdll.LoadLibrary("Jump_And_Jump.dll")

    while True:
        current_screenshot = ImageGrab.grab()
        current_screenshot.save("screenshot.png")

        jump_dll.FindChess()
        x_chess_coordinate, y_chess_coordinate = get_chess_coordinate_from_file()
        # mouse_move(x_chess_coordinate, y_chess_coordinate)
        # print x_chess_coordinate, y_chess_coordinate

        jump_dll.FindNextBlock()
        next_block_location = get_next_block_coordinate_from_file()
        # mouse_move(next_block_location, y_chess_coordinate)
        # print next_block_location
        
        x_distance = abs(x_chess_coordinate - next_block_location)
        mouse_click(960,540, x_distance)
        os.remove("screenshot.png")
        mouse_click(0, 0, 0)
        time.sleep(0.5)


if __name__ == '__main__':
    main()
