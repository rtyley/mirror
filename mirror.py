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

if connected < -1:
  print "Not Connected"

print lidar.getDistance()

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
  return ledForBrightness(max(0.0,min(1.0,norm))) # 2 * min(random() * norm,0.5))

def addEnergy(centrePixel):
  for n in range(pixelLength):
    es[n] = min(50.0,es[n] + 10/(1+math.pow((n-centrePixel)*2,4)))

def degrade():
  for n in range(pixelLength):
    es[n] *= 0.99

def showCandle():
  for n in range(pixelLength):
    setPixelBothSides(n,candleBright(es[n]))
  strip.show()

def showThrobs(num, timeSinceInteractionStart, timeSinceInteractionStop):
  interactionAge = timeSinceInteractionStart if timeSinceInteractionStop is None else (timeSinceInteractionStart - timeSinceInteractionStop)
  print interactionAge

  intensity = max(0,math.log1p((interactionAge/10.0)))
#  print intensity
  for n in range(pixelLength):
    distIntensity = 1/(1+math.pow((n-num)/(1+4*timeSinceInteractionStart),2))
    throbImportance = 1/(timeSinceInteractionStart/10.0+1)
    throb = math.sin((15*timeSinceInteractionStart)-(math.fabs(n-num)*math.pi/16))
    foo = intensity * distIntensity * (2+(throbImportance*throb))
    setPixelBothSides(n,candleBright(foo))
  strip.show()



def setPixelBothSides(n, cb):
  strip.setPixelColor(n, cb)
  strip.setPixelColor(numpixels - 1 - n, cb)



# p = select.poll()
# dev = InputDevice('/dev/input/event0')

# p.register(dev, select.POLLIN)


def sweepTo(c):
  for n in range(numpixels):
    strip.setPixelColor(n, c)
    strip.show()

def moo():
	global lit


#		addEnergy(num)
#	degrade()
	
#	events = p.poll(0)
#	if events:
#        	data = list(dev.read())
#		if filter(lambda e: e.type == ecodes.EV_KEY and e.value == 1, data):
#			lit = not lit
#			print lit
#			sweepTo(color if lit else 0)


sweepTo(color)
lit = True
interactionStartTime = None
interactionStopTime = None
previousInteractionNum = None
interactionAge = None
timeSinceInteractionStart = None
timeSinceInteractionStop = None

while True:                              # Loop forever
	d = lidar.getDistance()
	num = d / 1.6

	t = time.time()

	if num > 1 and num < (pixelLength):
		if interactionStopTime is not None and t > interactionStopTime + 0.1:
			interactionStartTime = t
			interactionStopTime = None
#		print interactionAge
		previousInteractionNum = num
	else:
		if interactionStopTime is None:
			interactionStopTime = t

	if interactionStartTime is not None:
		timeSinceInteractionStart = t - interactionStartTime
		if interactionStopTime is None:
			timeSinceInteractionStop = None
		else:
			timeSinceInteractionStop = t - interactionStopTime

		showThrobs(previousInteractionNum, timeSinceInteractionStart, timeSinceInteractionStop)

