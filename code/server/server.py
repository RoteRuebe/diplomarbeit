
import RPi.GPIO as gpio
from pyrf24 import *
import sys, time, threading
from flask import Flask

p_ce = 25
p_csn = 8

radio = RF24(25, 0)
address = b"00002"


radio.begin()
radio.openReadingPipe(0, address)
radio.setPALevel(RF24_PA_MAX)

radio.listen = True

radio.print_details()
x = 0
def write_log(filename, x):
    with open(f"./logs/{filename}.txt", "at") as f:
        f.write( x.split("\x00")[0] + "\n") 

def process(x):
    x = x.split(",")

    if x[0] == "log":
        write_log("log", x[1])

    elif x[0] == "vibration":
        write_log("vibration",x[1])

    elif x[0] == "acc":
        write_log("acc_x", x[1])
        write_log("acc_y", x[2])
        write_log("acc_z", x[3])
  
    elif x[0] == "gyro":
        write_log("gyro_x", x[1])
        write_log("gyro_y", x[2])
        write_log("gyro_z", x[3])

    elif x[0] == "temp":
        write_log("temp", x[1])

def serviceRadio():
    while True:
        if (radio.available() ):
            rec = radio.read()
            rec = rec.decode("utf-8")
            #print(rec)
            try:
                process(rec)
            except Exception as e: continue

threading.Thread(target=serviceRadio, args=() ).start()

app = Flask(__name__)

@app.route("/")
def index():
    return "jo"
