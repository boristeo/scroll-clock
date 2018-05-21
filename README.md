# scroll-clock
Arduino "clock" that receives serial input from a zigbee module, and demo code to wirelessly transmit crypto prices

  The Atmel Studio project is all the code for the Arduino. It is actually quite simple - only a few functions that alter the display
which can be called by sending a command code (e.g. "\b" followed by a byte value to set display brightness) over the serial connection
labeled "radio".

  The Python file crypto-clock.py is a small demo that shows the functionality of scroll-clock. When run, it writes the current time
and date to the serial port specified, along with a brightness adjustment command depending on time of day, then gets, parses, and writes
the top 30 market cap cryptocurrencies symbols and prices to be shown on the scroll-clock. During the night, crypto price scrolling is
disabled and the display shows a simple clock.
