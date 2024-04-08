
### Shared Memory ###
gyroX=[deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
gyroY=[deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
...

accX = [deque(maxlen=100),deque(maxlen=100),deque(maxlen=100)]
...

vibration=[deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]
temp = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]
log = [deque(maxlen=10),deque(maxlen=10),deque(maxlen=10)]

radio.openReadingPipe(0, bytearray( b"1-r-s\x00" ))
radio.openReadingPipe(1, bytearray( b"2-r-s\x00" ))
radio.openReadingPipe(2, bytearray( b"3-r-s\x00" ))

hasPayload, index = radio.available_pipe()
if ( hasPayload ):
    timeLastPacket[index] = timeNow
    robotConnected[index] = True

    rec = radio.read()

    process(index, rec)

def process(i, x):
    if ( x[0] == 0x00 ): #Data
        data = struct.unpack("< B 7f 2B B", x)
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
        log[i].append( data[0].decode("ASCII”) 

def serviceRadio():
	initRadio()
     while true: 
         if ( received_packet ):
             process( packet )

threading.Thread(target=serviceRadio, args=() ).start()
