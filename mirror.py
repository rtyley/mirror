#!/usr/bin/python

import math
import time
from array import array
from random import random
from dotstar import Adafruit_DotStar
from lidar_lite import Lidar_Lite
import os
import select
# from evdev import InputDevice, ecodes

lidar = Lidar_Lite()
connected = lidar.connect(1)

#if connected < -1:
#  print "Not Connected"

# print lidar.getDistance()

numpixels = 204 # Number of LEDs in strip

pixelLength = numpixels / 2

es = array('f', [0.0] * pixelLength)

strip     = Adafruit_DotStar(numpixels)

strip.begin()           # Initialize pins for output

color = 0xFFFFFF        # 'On' color

def ledForBrightness(norm):
  total = int(math.floor(767 * norm))
  return ~( (~(total % 256)) << (8 * (total / 256)) )

def candleBright(norm):
  return ledForBrightness(min(1.0,norm)) # 2 * min(random() * norm,0.5))

def addEnergy(centrePixel):
  for n in range(pixelLength):
    es[n] = min(50.0,es[n] + 2/(1+math.pow((n-centrePixel)*2,4)))

def degrade():
  for n in range(pixelLength):
    es[n] *= 0.96

def showCandle():
  for n in range(pixelLength):
    cb = candleBright(es[n])
    strip.setPixelColor(n, cb)
    strip.setPixelColor(numpixels - 1 - n, cb)
  strip.show()

# p = select.poll()
# dev = InputDevice('/dev/input/event0')

# p.register(dev, select.POLLIN)

lit = False

def sweepTo(c):
  for n in range(numpixels):
    strip.setPixelColor(n, c)
    strip.show()

def moo():
	global lit
	d = lidar.getDistance()
	print d
	num = d / 1.6

	if num > 1 and num < (pixelLength):
		addEnergy(num)
	degrade()
	
#	events = p.poll(0)
#	if events:
#        	data = list(dev.read())
#		if filter(lambda e: e.type == ecodes.EV_KEY and e.value == 1, data):
#			lit = not lit
#			print lit
#			sweepTo(color if lit else 0)

	if not lit:
		showCandle()
			

sweepTo(color)

while True:                              # Loop forever
	moo()
