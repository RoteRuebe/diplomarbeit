import RPi.GPIO as gpio
from pyrf24 import *
from collections import deque
import threading, flask, time, struct

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
    if ( x[0] == 0x00 ): #Data
        data = struct.unpack("< B 7f 2B B", x)

        print(i)
        print(data)
        gyroX[i].append(data[1])
        gyroY[i].append(data[2])
        gyroZ[i].append(data[3])

        accX[i].append(data[4])
        accY[i].append(data[5])
        accZ[i].append(data[6])

        temp[i].append(data[7])
        controllerConnected[i] = bool( data[8] )
        vibration[i] = data[9]

    else:
        data = struct.unpack("<32s", x)
        log[i].append( data[0].decode("ASCII").split( u"\x00" )[0] )


def serviceRadio():
    radio = RF24(25, 0)
    while (not radio.begin()):
        pass

    # Match Null-Terminated C "Strings"
    radio.openReadingPipe(0, bytearray( b"1-r-s\x00" ))
    radio.openReadingPipe(1, bytearray( b"2-r-s\x00" ))
    radio.openReadingPipe(2, bytearray( b"3-r-s\x00" ))

    radio.listen = True
    radio.print_details()

    timeNow = 0
    timeLastPacket = [0, 0, 0]
    index = 0

    print("Listening")
    while True:
        timeNow = time.time()

        hasPayload, index = radio.available_pipe()

        if ( hasPayload ):
            timeLastPacket[index] = timeNow
            robotConnected[index] = True

            rec = radio.read()

            try:
                process(index, rec)
            except Exception as e: print(e)


        for i in range(3):
            if ( timeNow - timeLastPacket[i] >= 0.250):
                robotConnected[i] = False
                controllerConnected[i] = False


threading.Thread(target=serviceRadio, args=() ).start()

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
    elif name == "status": return { "robotConnected": str( robotConnected[id] ), "controllerConnected": str( controllerConnected[id] ) }

    else: return "Ressource not found", 404

if __name__ == "__main__":
    app.run(host="0.0.0.0")

    flask.url_for("static", filename="style.css")
    flask.url_for("static", filename="source.js")
