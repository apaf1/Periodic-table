# Periodic-table
Arduino code for my home made periodic table, see project here: http://imgur.com/a/pc6aN

The program has three important parts that are separated. 

check buttons --> void checkButtons()
update state --> void updateState()
run animations --> executed in main loop, void loop()
The code is divided into to parts that are clearly marked, and can be found in the following order

Controll code
LED animation code


Check list for adding new animations:

Add name for mode to be displayed on LCD in char* modes[]
Increase the value of int modesChoiceMax
Add mode in void loop()
Use pause() instead of delay() as pause() checks buttons while pausing.

Party mode:

When the party switch is set to HIGH, the party mode is initiated. In this mode all button input is ignored and predefined animations are played in random order. Remember to update nPartyModes when adding new animations. Use delay() instead of pause() when writing party mode animations as button input should be ignored.

Please check your code for mistakes before uploading to github, and atleast make sure that it is compilable :)

For an in depth toutorial on how to use and program the adressable LEDs, see adafruits guide: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library
