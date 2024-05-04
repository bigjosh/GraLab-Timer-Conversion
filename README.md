# 60Hz-PWM-sine

Hardware and software to make a clasic GraLab darkroom timer run off of 8xAA batteries rather than AC mains power so you can put it on a shelf.

Costs about $30 in easy to get parts and does not require any soldering. 

# Theory of Operation

We use an Arduino to produce a 60Hz modified sine wave using a commidity motor conntroller board. We run that high current, low voltage AC waveform into a
transformer to boost it up to ~120VAC, suitable for driving both the buzzer and the synchonous AC motor inside the darkroom timer. Then it is just a matter
of doing the surgery to get everything connected up. 

Note that we are using a 120VAC to 6-0-6VAC transformer, but we are using it backwards and also using only one half of the low voltage side.  

# The tricky parts
I orginally used an Arduino clone and it rebooted sometimes when the timer hand hit zero. I think this might have been due to 
electrical noise of the switch inside the motor assembly. I swapped in a real Arduino UNO and the problem went away.

I orginally used a 1000 sample PWM to generate a smooth(er) sine wave, but this turned out to be unessisary since the motor
and buzzer are perfectly happy with a modified sine wave as long as as the high and low parts are equally as wide as the zero parts. 
The PWM used a little bit more power and the code was much more complicated. 

I added in a watchdog timer when debuging the rebooting issue. It turned out not to be nessisary, but I left it in just in case.

I wired mine so that if it is powered on, then either the motor or the buzzer is engaged. Normally you can leave it powered on with the 
timer at zero and the buzzer turned off, but this would eventually make the batteries go dead since the Arduino would still be using power.

You might think think that we only need to input 6V into the transformer to get 120VAC out, but remember that 120VAC is actually RMS power,
so peaks at about 180V. And the system is messy and lossy. 

I tried to make my conversion reversable, so I used Wago wire nuts everywhere and saved the orginal cord and stuck it inside the case before I sealed it back up.

# Photo

![PXL_20240504_013340778 MP](https://github.com/bigjosh/60Hz-PWM-sine/assets/5520281/61d20655-cf82-4bc6-8787-543b82a87e47)


# Parts List

120V-AC to 12V-AC Transformer Center Tapped CT 6V-0-6V
https://amzn.to/4b9mPJN

L298N Motor Drive Controller Board Module Dual H Bridge 
https://amzn.to/4aRL4Mk

Arduino UNO
https://amzn.to/43ZS4V1

12V AA Battery Holder Case Box with DC 5.5x2.1mm Male Connector
https://amzn.to/3vQTPHv

WAGO 221-412 Lever-Nuts - 2-Conductor Compact Splicing Connectors (optional)
https://amzn.to/3UgL9nn

And, or course, any GraLab darkroom timer
https://www.google.com/search?sca_esv=452cfffdbe2077f7&q=gralab+darkroom+timer&tbm=shop

# Support

Open an issue if you have any questions or problems making your own. 
