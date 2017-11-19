
```
sudo raspi-config
```

Go to "5 Interfacing Options  Configure connections to peripherals" and enable both:

* "P4 SPI         Enable/Disable automatic loading of SPI kernel module"
* "P5 I2C         Enable/Disable automatic loading of I2C kernel module"


```
sudo apt-get install python-smbus libi2c-dev python-dev python-pip gcc
```

Debian Stretch no longer uses evdev, apparently uses libinput instead...

The current Raspbian kernals suffer from a I2C problem that breaks the Lidar Lite:

http://www.robotshop.com/forum/lidar-lite-v3-return-always-zero-with-raspberry-pi-3-t15710

$ sudo apt-get install rpi-update
https://github.com/Hexxeh/rpi-firmware/commit/af9cb14d5053f89857225bd18d1df59a089c171e
$ sudo rpi-update  af9cb14d5053f89857225bd18d1df59a089c171e

...gets you kernel 4.4.38 - which works with the Lidar Lite


bluetoothctl
http://lifehacker.com/everything-you-need-to-set-up-bluetooth-on-the-raspberr-1768482065
