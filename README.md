PumpkinCandle
=============

This is the setup (hardware and software) to get a pair of LEDs to emulate a flickering flame. I used one yellow and one red LED. I have it set to vary the intensity of each independently at a random interval to a random value. The effect works well. Some smoothing to the values may be an improvment, but I've not fiddled with it in some while. The limits of the intensity and duration of the interval are adjustable only in the code. Pots might be a handy addition, but they would increase the size of the board. I had the PCBs produced and found the assembly after pleasant after the first iterations on breadboard.

/Fritzing - Holds the drawings and PCB layout made with the Fritzing tool. These files are an older version of fritzing than what is in production now, but they can be opened read-only and will appear as they did when I created them.

/Source - Holds the source code to be uploaded to the ATtINY45. It is not yet optimized and could likely use some tweaking. When introduced to the local electronics hobbyist suggestions were made to re-enable ICSP and other mods. This is my first attempt at creating a circuit from scratch so there are plenty of errors to uncover. Have fun!

The idea for the photoresistor came from EvilMadScientiest here: http://www.evilmadscientist.com/2008/a-dark-detecting-circuit-for-your-jack-o-lantern/. I wanted to get less of a simply on off light and more of a candle flickering effect. I patched the photoresistor from this article onto my ATtiny and started working the code.
