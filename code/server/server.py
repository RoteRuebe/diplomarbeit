#import RPi.GPIO as gpio
from pyrf24 import *
from collections import deque
import threading, flask

### Shared Memory ###
gyroX = deque(maxlen=100)
gyroY = deque(maxlen=100)
gyroZ = deque(maxlen=100)

accX = deque(maxlen=100)
accY = deque(maxlen=100)
accZ = deque(maxlen=100)

vibration = deque(maxlen=10)
temp = deque(maxlen=10)
log = deque(maxlen=10)

### Rf24 Thread ###
def process(x):
    x = x.split(",")

    if x[0] == "log":
        log.append( x[1].split(u"\x00")[0] )

    elif x[0] == "vibration":
        vibration.append( x[1].split(u"\x00")[0] )

    elif x[0] == "acc":
        accX.append( float(x[1]) )
        accY.append( float(x[2]) )
        accZ.append( float(x[3].split(u"\x00")[0]) )
  
    elif x[0] == "gyro":
        gyroX.append( float(x[1]) )
        gyroY.append( float(x[2]) )
        gyroZ.append( float(x[3].split(u"\x00")[0]) )

    elif x[0] == "temp":
        temp.append(x[1])

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

threading.Thread(target=serviceRadio, args=() ).start()
            
### Webserver ###
app = flask.Flask(__name__)

@app.route("/")
def index():
    return flask.render_template("index.html", statusRobot="Hello", statusController="World", statusCamera="!")

@app.route("/about")
def about():
    return flask.render_template("about.html")

@app.route("/data/<name>")
def data(name):
    if name == "log": return list(log)
    elif name == "temp": return list(temp)
    elif name == "gyro": return { "x": list(gyroX), "y": list(gyroY), "z": list(gyroZ)}
    elif name == "acc": return { "x": list(accX), "y": list(accY), "z": list(accZ)}

    else:
        return "Ressource not found", 404

if __name__ == "__main__":
    app.run()

    flask.url_for("static", filename="style.css")
    flask.url_for("static", filename="source.js")
