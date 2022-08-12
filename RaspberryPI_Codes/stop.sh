#!/bin/bash
echo "CANSET_Sensor STOP"
`sudo systemctl stop CANSET_Sensor`

echo "CANSET_TX STOP"
`sudo systemctl stop CANSET_TX`

echo "CANSET_RX STOP"
`sudo systemctl stop CANSET_RX`

echo `sudo systemctl status CANSET_Sensor`
echo `sudo systemctl status CANSET_TX`
echo `sudo systemctl status CANSET_RX`
