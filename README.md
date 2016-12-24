# Periodic-table
Arduino code for my home made periodic table, see project here: http://imgur.com/a/pc6aN

The program has three important parts that control the logic of the program 

- check buttons --> void checkButtons()
- update state --> void updateState()
- run animations --> executed in main loop, void loop()

The general idea is that the main loop execute each animation mode once, then returns to the top. Each time the main loop runs the buttons are checked with checkbuttons() and the choice of mode is changed if buttons are pressed. 

But because it takes a long time to go through each animation mode, the buttons are also checked and the state is also updated during the run of the animation mode. This is done by using a pause() function that replace delay() in this program. Pause() checks buttons and update the state continiously. If the mode is changed when pause() is running all pauses are ended so that the animation finnish quickly. Pause also returns true if mode is changed. This is used in some animations to break the loop so the animation can finish even quicker. 

The code is divided into parts that are clearly marked, and can be found in the following order

- Controll code
- Pixels code
- Game of life
- Party mode!!


Check list for adding new animations:

- Add name for mode to be displayed on LCD in char* modes[]
- Increase the value of int modesChoiceMax
- Add mode in void loop()
- Use pause() instead of delay() as pause() checks buttons while pausing.

Party mode:

When the party switch is set to HIGH, the party mode is initiated. In this mode all button input is ignored and predefined animations are played in random order. Remember to update nPartyModes when adding new animations. 

For an in depth toutorial on how to use and program the adressable LEDs, see adafruits guide: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library
