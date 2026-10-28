import serial
import pyautogui
import pydirectinput
import time
from ahk import AHK

ahk = AHK()

pyautogui.PAUSE = 0
pydirectinput.PAUSE = 0
pyautogui.FAILSAFE = False

ser = serial.Serial('COM8', 115200)

first_reading = True

LastLeftClickTime = 0
LastRightClickTime = 0
RightClickInterval = 0.3
LeftClickInterval = 0.1
sens = 0.5

while True:
    line = ser.readline().decode('utf-8', errors='ignore').strip()



    parts = line.split(',')
    cursorX = float(parts[0])
    cursorY = float(parts[1])
    button1 = int(parts[2])
    button2 = int(parts[3])


    pyautogui.move(cursorX * sens, cursorY * sens * -1)

    now = time.time()

    if button1 == 0 and (now - LastLeftClickTime) >= LeftClickInterval:
        ahk.click(button='left')  # uses AHK's Click command under the hood
        LastLeftClickTime = now

    if button2 == 0 and (now - LastRightClickTime) >= RightClickInterval:
        ahk.click(button='right')
        LastRightClickTime = now
