#import RPi.GPIO as gpio
#from pyrf24 import *
from collections import deque
import threading, flask

### Shared Memory ###
gyroX = deque([1,2,3,4,5,6,7,8,9,10], maxlen=10)
gyroY = deque([10,9,8,7,6,5,4,3,2,1], maxlen=10)
gyroZ = deque([5,5,5,5,5,5,5,5,5,5], maxlen=10)

accX = deque(maxlen=10)
accY = deque(maxlen=10)
accZ = deque(maxlen=10)

temp = deque(maxlen=10)
log = deque(maxlen=10)


### Rf24 Thread ###
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
    radio = RF24(25, 0)
    address = b"00002"

    radio.begin()
    radio.openReadingPipe(0, address)
    radio.setPALevel(RF24_PA_MAX)

    radio.listen = True

    radio.print_details()

    while True:
        if (radio.available() ):
            rec = radio.read()
            rec = rec.decode("utf-8")
            #print(rec)
            try:
                process(rec)
            except Exception as e: continue

#threading.Thread(target=serviceRadio, args=() ).start()
            
### Webserver ###

app = flask.Flask(__name__)

@app.route("/")
def index():
    return flask.render_template("index.html", statusRobot="Hello", statusController="World", statusCamera="!")

@app.route("/data/<name>")
def data(name):
    if name == "log": return ["Testing environment", "Json Data jeah"]
    elif name == "temp": return [20]
    elif name == "gyro": return [list(gyroX), list(gyroY), list(gyroZ)]
    elif name == "acc": return [list(accX), list(accY), list(accZ)]

    else:
        return "Error 404 oder so"

if __name__ == "__main__":
    app.run()

    flask.url_for("static", filename="style.css")
    flask.url_for("static", filename="source.js")
