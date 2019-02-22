# LineFollower
Arduino 2560 Mega Line Follower

Implemented by Electrical Team and Dr. Steve Foster

Pin assignments, S0, S1, S2, and S3 correspond to the RGB module control signals.
S0 and S1 control the frequency scaling of the detected light, more light the higher
the frequency and lower the period. See https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf.
S2 and S3 control the filter over the sensor's window, 00 is red, 01 is blue, 10 clear and 11 is green.
 
The wiper of a potentiometer connected is to analog input #7,
The other ends are at GND and +5 V. The ADC will return a value 
between 0 and 1023 depending on the wiper position.
This value is reduced by a factor of eight to make it more in line
with the sensors levels when above the strip. The potentiometer can
be adjusted for different heights of the sensors and floor reflectivity.

See source code for more pin assignments and additional info.