import RPi.GPIO as gpio
from pyrf24 import *
from collections import deque
import threading, flask, time

### Shared Memory ###
gyroX = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
gyroY = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
gyroZ = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]

accX = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
accY = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
accZ = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]

vibration = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]
temp = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]
log = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]

robotConnected = [False, False, False]
controllerConnected = [False, False, False]

### Rf24 Thread ###
def process(i, x):
    if x[0] == "log":
        log[i].append( x[1].split(u"\x00")[0] )

    elif x[0] == "vibration":
        vibration[i].append( x[1].split(u"\x00")[0] )

    elif x[0] == "acc":
        accX[i].append( float(x[1]) )
        accY[i].append( float(x[2]) )
        accZ[i].append( float(x[3].split(u"\x00")[0]) )
  
    elif x[0] == "gyro":
        gyroX[i].append( float(x[1]) )
        gyroY[i].append( float(x[2]) )
        gyroZ[i].append( float(x[3].split(u"\x00")[0]) )

    elif x[0] == "temp":
        temp[i].append(x[1])

    elif x[0] == "controllerConnected":
        if ( x[1].split(u"\x00")[0] == "1" ):
            controllerConnected[i] = True
        else:
            controllerConnected[i] = False

def serviceRadio():
    radio = RF24(25, 0)
    while (not radio.begin()):
        pass

    # Match Null-Terminated C Strings
    radio.openReadingPipe(1, b"1-r-s\x00")
    radio.openReadingPipe(2, b"2-r-s\x00")
    radio.openReadingPipe(3, b"3-r-s\x00")

    radio.listen = True
    radio.print_details()
    radio.failureDetected = 0

    timeNow = 0
    timeLastPacket = [0, 0, 0]
    timeLastSwitch = 0
    channelIndex = 0
    channels= [0, 50, 97]

    while True:
        timeNow = time.time()

        if ( radio.available() ):
            timeLastPacket[channelIndex] = timeNow
            robotConnected[channelIndex] = True

            rec = radio.read()
            rec = rec.decode("utf-8").split(",")

            try:
                process(channelIndex, rec)
            except: pass


        elif ( timeNow - timeLastPacket[channelIndex] >= 0.250):
            robotConnected[channelIndex] = False
            controllerConnected[channelIndex] = False

        if (timeNow - timeLastSwitch > 0.1):
            channelIndex += 1
            if (channelIndex == 3): channelIndex = 0

            radio.setChannel( channels[channelIndex] )

            # 2ms delay
            timeForDelay = timeNow
            while (timeForDelay - timeNow < 2/1000):
                timeForDelay = time.time()

            timeLastSwitch = timeNow

try:
    threading.Thread(target=serviceRadio, args=() ).start()
except:
    sys.exit()

#gyroX = [ [i for i in range(100)],  [50 for i in range(100)],  [100-i for i in range(100)] ]

### Webserver ###
app = flask.Flask(__name__)

@app.route("/")
def index():
    return flask.render_template("index.html")

@app.route("/about")
def about():
    return flask.render_template("about.html")

@app.route("/data/<name>/<id>")
def data(name,id):
    id = int(id)
    if name == "log": return list(log[id])
    elif name == "temp": return list(temp[id])
    elif name == "gyro": return { "x": list(gyroX[id]), "y": list(gyroY[id]), "z": list(gyroZ[id])}
    elif name == "acc": return { "x": list(accX[id]), "y": list(accY[id]), "z": list(accZ[id])}
    elif name == "status": return { "robotConnected": str(robotConnected[id]), "controllerConnected": str(controllerConnected[id]) }

    else: return "Ressource not found", 404

if __name__ == "__main__":
    app.run(host="0.0.0.0")

    flask.url_for("static", filename="style.css")
    flask.url_for("static", filename="source.js")
