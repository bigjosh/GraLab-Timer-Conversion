# 60Hz-PWM-sine

Hardware and software to make a clasic GraLab darkroom timer run off of 8xAA batteries rather than AC mains power so you can put it on a shelf.

Costs about $30 in parts and does not require any soldering. 

# Theory of Operation

We use an Arduino to produce a 60Hz PWM sine wave. Then we use a commidity motor conntroller board to convert the digital outputs of the Arduino
into a high current, AC waveform (but still low voltage). We then run that AC waveform into a transformer to boost it up to ~120VAC, suitable for driving
both the buzzer and the synchonous AC motor inside the darkroom timer. Then it is just a matter of doing the surgery to get everything connected up. 

Note that we are using a 120VAC to 6-0-6VAC transformer, but we are using it backwards and also using only one half of the low voltage side.  

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
