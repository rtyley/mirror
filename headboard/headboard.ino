#include "SoftPot.h"

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
// #include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         0
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
/*=========================================================================*/

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// The service and characteristic index information 
int32_t gattServiceId;
int32_t gattNotifiableCharId;

SoftPot left(A0);
SoftPot right(A1);

uint16_t position = 1;

void setup() {
  while (!Serial);  // required for Flora & Micro
  delay(500);
  boolean success;

  Serial.begin(115200);
  
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);
  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // this line is particularly required for Flora, but is a good idea
  // anyways for the super long lines ahead!
  // ble.setInterCharWriteDelay(5); // 5 ms

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Headboard Remote': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Headboard Remote")) ) {
    error(F("Could not set device name?"));
  }


  /* Add the Custom GATT Service definition */
  /* Service ID should be 1 */
  Serial.println(F("Adding the Custom GATT Service definition: "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID128=F1-78-09-BF-BF-CE-4C-82-9E-58-52-30-90-E4-CA-03"), &gattServiceId);
  if (! success) {
    error(F("Could not add Custom GATT service"));
  }
 
  /* Add the Readable/Notifiable characteristic */
  Serial.println(F("Adding the Notifiable characteristic: "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=FD-D8-D9-70-9D-96-4A-C7-A2-E1-84-D2-04-F6-52-3B,PROPERTIES=0x12,MIN_LEN=1, MAX_LEN=3, VALUE=0x00FF00"), &gattNotifiableCharId);
    if (! success) {
    error(F("Could not add Custom Notifiable characteristic"));
  }
  

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  Serial.println();
}

void loop() {
  position = left.readPos();
  Serial.println(left.readPos());Serial.println(right.readPos());
  // Serial.println(right.readPos());
  // delay a bit
    delay(250);

    /* Command is sent when \n (\r) or println is called */
    /* AT+GATTCHAR=CharacteristicID,value */
    ble.print( F("AT+GATTCHAR=") );
    ble.print( gattNotifiableCharId );
    ble.print( F(",") );
    ble.println(position, HEX);
  
    /* Check if command executed OK */
    if ( !ble.waitForOK() )
    {
      Serial.println(F("Failed to get response!"));
    }
}
