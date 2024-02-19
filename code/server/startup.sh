killall python3
killall php

rm ./logs/*.txt

python3 radio.py &
echo "Radio module started"
php -t ./ -q -S 172.24.5.105:8080 &
echo "Server started"
