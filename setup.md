
```
sudo raspi-config
```

Go to "5 Interfacing Options  Configure connections to peripherals" and enable both:

* "P4 SPI         Enable/Disable automatic loading of SPI kernel module"
* "P5 I2C         Enable/Disable automatic loading of I2C kernel module"


```
sudo apt-get install python-smbus libi2c-dev python-dev python-pip gcc
```

http://python-evdev.readthedocs.io/en/latest/install.html#from-source

```
sudo pip install evdev
```

bluetoothctl
http://lifehacker.com/everything-you-need-to-set-up-bluetooth-on-the-raspberr-1768482065
