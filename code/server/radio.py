
import RPi.GPIO as gpio
from pyrf24 import *
import sys, time

p_ce = 25
p_csn = 8

radio = RF24(25, 0)
address = b"00002"


radio.begin()
radio.openReadingPipe(0, address)
radio.setChannel(200);
radio.setPALevel(RF24_PA_MAX)

radio.listen = True

radio.print_details()

x = 0

def write_log(filename, x):
     with open(f"./logs/{filename}.txt", "at") as f:
        for char in payload:
            if (char != "\x00"):
                f.write(char)
            else:
                f.write("\n")
                return;

def process(x):
    tag, payload = x.split(",")

    if tag == "log":
        write_log("log", payload)

    elif tag == "vibration":
        write_log("vibration",payload)

    elif tag == "gyro":
        write_log("acc_x", payload[0])
        write_log("acc_y", payload[1])
        write_log("acc_z", payload[2])

        write_log("gyro_x", payload[3])
        write_log("gyro_y", payload[4])
        write_log("gyro_z", payload[5])

        write_log("temp", payload[6])

print("radio started")
while True:
    if (radio.available() ):
        rec = radio.read()
        print("received:")
        print(rec)
        rec = rec.decode("utf-8")
        print("processed:")
        print(rec)

        process(rec)
