
import RPi.GPIO as gpio
from pyrf24 import *
import sys, time, datetime

p_ce = 25
p_csn = 8

radio = RF24(25, 0)
address = b"00002"


radio.begin()
radio.openReadingPipe(0, address)
radio.setChannel(200);
radio.setPALevel(RF24_PA_MAX)

radio.listen = True

print("Starting session on: " + str( datetime.datetime.today() ) )
radio.print_details()

x = 0

def process(x):
    tag, payload = x.split(",")

    if tag == "log":
        with open("./logs/robot_log.txt", "at") as f:
            for char in payload:
                if (char != "\x00"):
                    f.write(char)
                else:
                    f.write("\n")
                    break;

    elif tag == "data":
        with open("./logs/data.txt", "at") as f:
            for char in payload:
                if (char != "\x00"):
                    f.write(char)
                else:
                    f.write("\n")
                    break;

print("radio started")
while True:
    if (radio.available() ):
        // receive payload
        rec = radio.read()
        print("received:")
        print(rec)
        rec = rec.decode("utf-8")
        print("processed:")
        print(rec)
        process(rec)

        //send payload
        with open("./logs/msg_to_tobot.txt", "rt") as f:
            if f
            radio.listen = False
            radio.write(f.read)
