#!/bin/bash
echo ====PLAYER====
/usr/local/bin/player /usr/local/share/player/config/pioneer.cfg &
sleep 2
echo ====WATCHDOG====
/home/cclub/Desktop/robot/watchdog &
sleep 2
echo ====FIREFOX====
firefox -a firefox -chrome chrome://robot/content/arbtest.xul &
sleep 2
echo ====KAMERA====
./trunk/camera/camserver &
