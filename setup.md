
```
sudo raspi-config
```

Go to "9 Advanced Options" and enable both:

* "A5 SPI   automatic loading of SPI"
* "A6 I2C   automatic loading of I2C"


```
sudo apt-get install python-smbus libi2c-dev python-dev python-pip gcc
```

http://python-evdev.readthedocs.io/en/latest/install.html#from-source

```
sudo pip install evdev
```

bluetoothctl
http://lifehacker.com/everything-you-need-to-set-up-bluetooth-on-the-raspberr-1768482065
