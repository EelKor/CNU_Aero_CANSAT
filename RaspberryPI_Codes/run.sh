#!/bin/bash
echo "Start CANSET_Sensor.service" 
`sudo systemctl start CANSET_Sensor`

echo "Start CANSET_TX.service" 
`sudo systemctl start CANSET_TX`

echo "Start CANSET_RX.service"
`sudo systemctl start CANSET_RX`

echo "Start Apache2.service"
`sudo systemctl restart apache2`

echo "Start Bind9 - Nameserver"
`sudo service bind9 restart`

echo "Start Node.js WebServer"
`sudo systemctl start CANSET_nodejs`

echo `sudo systemctl status CANSET_Sensor`
echo `sudo systemctl status CANSET_TX`
echo `sudo systemctl status CANSET_RX`
echo `sudo systemctl status CANSET_nodejs`


