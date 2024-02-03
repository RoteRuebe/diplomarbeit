killall python3
killall php

rm log.txt
echo "" > log.txt
rm data.txt
echo "" > log.txt
rm radio_log.txt
rm php_log.txt

python3 radio.py > radio_log.txt &
echo "Radio module started"
php -t ./ -q -S 172.24.5.105:8080 > php_log.txt &
echo "Server started"
