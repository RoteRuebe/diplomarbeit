
import RPi.GPIO as gpio
import pyrf24
from flask import Flask
import sys, time, threading

p_ce = 25
p_csn = 8

radio = pyrf24.RF24(25, 0)
address = b"00002"


radio.begin()
radio.openReadingPipe(0, address)
radio.setPALevel(pyrf24.RF24_PA_MAX)

radio.listen = True

print("Radio initialized")
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

t = threading.Thread( target=serviceRadio, args=() )
t.start()

app = Flask(__name__)
print()
print("Server initialized")


@app.route("/")
def index_page():
    return "jo"

