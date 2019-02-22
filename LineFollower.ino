/*
 * Pin assignments, S0, S1, S2, and S3 correspond to the RGB module control signals.
 * S0 and S1 control the frequency scaling of the detected light, more light the higher
 * the frequency and lower the period. See https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf.
 * S2 and S3 control the filter over the sensor's window, 00 is red, 01 is blue, 10 clear and 11 is green.
 */
int s0_pin =24;
int s1_pin =26;
int s2_pin =28;
int s3_pin =30;
int red1 =13;  //Right(?) Spike relay control for lighting red(?) LED on right(?) LED strip
int red2 =53;  //Left(?) Spike relay control for lighting red(?) LED on left(?) LED strip
int wht1 =12;  //Right(?) Spike relay control for lighting blue(?) LED on right(?) LED strip
int wht2 =51;  //Left(?) Spike relay control for lighting blue(?) LED on left(?) LED strip
int i;  // Trump's favorite word
/*
 * Pin assignments for the output of the eight RGB module, the modules are order left to right 
 * or right to left, don't know which way. These outputs indicate the light level by their frequency. 
 */
unsigned int led[8]; // Assign pins to the input light level
unsigned int out_pin[8] = {3, 10, 9, 11, 2, 8, 5, 4}; 
int LED_sensor=0;  // Contains the sensor level
int threshold=50; // period threshold that indicates sufficient light is present, the 50 is ignored.


void setup() {
// assign input and output to the Ardiuno I/O pin number
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(wht1, OUTPUT);
  pinMode(wht2, OUTPUT);
  pinMode(s0_pin, OUTPUT);
  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);
  pinMode(s3_pin, OUTPUT);
  for(i=0; i < 8; i++) {
    pinMode(out_pin[i], INPUT);
  }
  digitalWrite(s0_pin,HIGH); // Use the 20% frequency scaling factor
  digitalWrite(s1_pin,LOW);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(s2_pin,HIGH);  // Select the clear light filter
  digitalWrite(s3_pin,LOW);
  /*
   * The wiper of a potentiometer connected is to analog input #7,
   * The other ends are at GND and +5 V. The ADC will return a value 
   * between 0 and 1023 depending on the wiper position.
   * This value is reduced by a factor of eight to make it more in line
   * with the sensors levels when above the strip. The potentiometer can
   * be adjusted for different heights of the sensors and floor reflectivity.
   */
  threshold=analogRead(7)/8;
  Serial.print(threshold);
  Serial.print(" ");

  LED_sensor=0;
   // sequentially read all eight light sensors
  for(i=0; i < 8; i++) {
    led[i] = pulseIn(out_pin[i], LOW);
//    Serial.print(led[i]);
//    Serial.print(" ");
    if (led[i]>threshold) { // if intensity low (long period) then sensor is not seeing light
      led[i]=0;
    }
    else { // if intensity high (short period) then sensor is seeing light
      led[i]=1;
    }
    LED_sensor=(LED_sensor<<1)+led[i];  // Shift left the state of the LED into LED_sensor
  
    Serial.print(led[i]);
  }
  Serial.println("  ");
  delay(100);
  
//  if((LED_sensor==8) || (LED_sensor==16) || (LED_sensor==24)) {
  if(LED_sensor&0x18) {  // if middle sensors see light then a line is detected
    digitalWrite(wht1,LOW);  // turn on all LEDS, purple
    digitalWrite(wht2,LOW);
    digitalWrite(red1,LOW);
    digitalWrite(red2,LOW);
  }
  else if(LED_sensor>24) { // if right sensors see the line then turn on right blue(?) LEDs
    digitalWrite(red1,HIGH);
    digitalWrite(wht1,HIGH);
    digitalWrite(red2,HIGH);
    digitalWrite(wht2,LOW);
  }
  else if(LED_sensor>0){ // if left sensors see the line then turn on left red(?) LEDs
    
    digitalWrite(red1,LOW);
    digitalWrite(wht1,HIGH);
    digitalWrite(red2,HIGH);
    digitalWrite(wht2,HIGH);
  
  }
  else { // if no sensors see the line then turn off all LEDs
    digitalWrite(red1,HIGH);
    digitalWrite(wht1,HIGH);
    digitalWrite(red2,HIGH);
    digitalWrite(wht2,HIGH);
  }


}

