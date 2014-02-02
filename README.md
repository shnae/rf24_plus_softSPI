rf24_plus_softSPI
=================

This is a modified version of maniacbug's rf24 library.  It allows you to use rf24 over softSPI while using another SPI peripheral.  The SoftSPI library is by William Greiman.  The only modifications I've made here are to create a soft spi addressable nrf24l01 instance.  I used instructions provided in this arduino thread: http://forum.arduino.cc/index.php?topic=200122.15 by user nicoverduin.

To use, copy each folder to your Arduino Libraries folder.  Then select 'Import Library' in Arduino like you would for any other library.

The default settings are MISO- pin 16, MOSI- pin 15, SCK- pin 14, but you can change this by going into your rf24.h file in the RF24Soft folder and modifying the lines:

const uint8_t SOFT_SPI_MISO_PIN = 16;
const uint8_t SOFT_SPI_MOSI_PIN = 15;
const uint8_t SOFT_SPI_SCK_PIN  = 14;

Note: I have found that I sometimes need to remove the modified libraries from my Arduino libraries folder in order to get the regular SPI version of rf24 to work again.  I have not yet investigated the cause of this.

These libraries have allowed me to use a wiznet ethernet shield with an nrf24l01 simultaneously.  If anyone has a better way to do this, I would like to see it, but this has worked well for me so far.

-shnae
