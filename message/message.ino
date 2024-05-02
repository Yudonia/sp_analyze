#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 105 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels
const int rc = 0.2;
/*
  //指数平滑関連
  #define EXPO_FIL_COEF 0.98
  double expoFilOut = 0.0;
*/
const int maxi = 220;
const int MSGEQ7_RESET = 14;
const int MSGEQ7_STROBE = 12;
const int MSGEQ7_OUT = A0;
const int FREQ_NUM = 7;
const int sw = 15;
int num = 1;
int valAmplitude[7] = {0, 0, 0, 0, 0, 0, 0};
int thsd=510;
void setup()
{
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  pinMode(MSGEQ7_RESET, OUTPUT);
  pinMode(MSGEQ7_STROBE, OUTPUT);
  Serial.begin(19200);
  digitalWrite(MSGEQ7_RESET, HIGH);
  digitalWrite(MSGEQ7_STROBE, HIGH);
  pinMode(PIN, OUTPUT);
  delayMicroseconds(1); // Reset Pulse Width(tr)
  //  for(int s=0;s<7;s++){
  //    valAmplitude[s]=0;
  //  }
  pixels.begin();       // INITIALIZE NeoPixel strip object (REQUIRED)
}
void loop()
{
  pixels.clear(); // Set all pixel colors to 'off'
  ReadMSGEQ7();
  int cdsum=0;
  if (digitalRead(sw) == 1) {
    num++;
    delay(500);
    if (num > 3) {
      num = 1;
    }
  }
  int maxlist=0;
  for(int i=0; i<6;i++){
    if (valAmplitude[maxlist]<valAmplitude[i+1]){
      maxlist=i+1;
      
    }else{
      maxlist=maxlist;
      
    }
      Serial.print("comp:");
      Serial.print(maxlist);
      Serial.print(",");
      Serial.print(valAmplitude[i]);
      Serial.print(",");
      Serial.println(valAmplitude[i+1]); 

  }

  switch (num) {
    case 1:
      // pixels.clear(); // Set all pixel colors to 'off'
      // ReadMSGEQ7();
      for(int i=0; i<7; i++){
        cdsum= cdsum +valAmplitude[i];
      }
      Serial.print("cdsum:");
      Serial.println(cdsum);
      if (cdsum<thsd){
        for (int i;i<7;i++){
          valAmplitude[i]=0;
        }
        thsd=550;
      }else{
        for (int i=0;i<7;i++){
          valAmplitude[i]=1.2*valAmplitude[i];
        }
        thsd=350;
      }
      
      for (int a = 0; a < 7; a++)
      {

        //指数平滑
        valAmplitude[0] += 30;
        int valu = (rc * valu + (1 - rc) * valAmplitude[a] / 10);
        int lnum = (min(15, valAmplitude[a]/10 - 6));
        for (int i = 0; i < lnum - 1; i++)
        {
          // For each pixel...

          // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
          // Here we're using a moderately bright green color:
          // pixels.setPixelColor(i + 15 * a, pixels.Color(30, 255 - 5 * a - 10 * i, 20 * a + 10 * i));
          if (a==maxlist){
            pixels.setPixelColor(i + 15 * a, pixels.Color(255, 0, 0));
          }else if(a>maxlist){
            pixels.setPixelColor(i + 15 * a, pixels.Color(30, 255 - 5 * a - 10 * i , 10 * a + 10 * i));
            pixels.setPixelColor(lnum - 1 + 15 * a, pixels.Color(255, 0, 0));
          }else{
            pixels.setPixelColor(lnum - 1 + 15 * a, pixels.Color(255, 0, 0));
            pixels.setPixelColor(i+15*a,pixels.Color(30, 255 - 5 * a - 10 * i , 10 * a + 10 * i));
          }
          
          
          //delay(DELAYVAL); // Pause before next pass through loop
        }
        pixels.setBrightness(16);
        pixels.show(); // Send the updated pixel colors to the hardware.
        /*
          for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

            // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
            // Here we're using a moderately bright green color:
            pixels.setPixelColor(i, pixels.Color(50+i,256-i, 0));

            pixels.show();   // Send the updated pixel colors to the hardware.

            //delay(DELAYVAL); // Pause before next pass through loop
          }
        */
      }
      break;
    case 2:
      pixels.clear(); // Set all pixel colors to 'off'
      ReadMSGEQ7();
      for (int a = 0; a < 7; a++)
      {

        //指数平滑
        valAmplitude[0] += 30;
       
        int valu = (rc * valu + (1 - rc) * valAmplitude[a] / 10);
        int lnum = (min(15, valu - 6));
        for (int i = 0; i < lnum - 1; i++)
        {
          // For each pixel...

          // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
          // Here we're using a moderately bright green color:
          pixels.setPixelColor(i + 15 * a, pixels.Color(30, 255, 20));
          pixels.setPixelColor(lnum - 1 + 15 * a, pixels.Color(0, 255, 0));
          //delay(DELAYVAL); // Pause before next pass through loop
        }
        pixels.show(); // Send the updated pixel colors to the hardware.
        /*
          for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

            // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
            // Here we're using a moderately bright green color:
            pixels.setPixelColor(i, pixels.Color(50+i,256-i, 0));

            pixels.show();   // Send the updated pixel colors to the hardware.

            //delay(DELAYVAL); // Pause before next pass through loop
          }
        */
      }
      break;
    case 3:
      pixels.clear(); // Set all pixel colors to 'off'
      ReadMSGEQ7();
      for (int a = 0; a < 7; a++)
      {

        //指数平滑
        valAmplitude[0] += 30;
        int valu = (rc * valu + (1 - rc) * valAmplitude[a] / 10);
        int lnum = (min(15, valu - 6));
        for (int i = 0; i < lnum - 1; i++)
        {
          // For each pixel...

          // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
          // Here we're using a moderately bright green color:
          pixels.setPixelColor(i + 15 * a, pixels.Color(20, 20, 200));
          pixels.setPixelColor(lnum - 1 + 15 * a, pixels.Color(0, 0, 255));
          //delay(DELAYVAL); // Pause before next pass through loop
        }
        pixels.show(); // Send the updated pixel colors to the hardware.
        /*
          for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

            // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
            // Here we're using a moderately bright green color:
            pixels.setPixelColor(i, pixels.Color(50+i,256-i, 0));

            pixels.show();   // Send the updated pixel colors to the hardware.

            //delay(DELAYVAL); // Pause before next pass through loop
          }
        */
      }break;
  }
}
void ReadMSGEQ7()
{
  digitalWrite(MSGEQ7_RESET, LOW);
  delayMicroseconds(100); // Reset to Strobe Delay(trs)

  // FREQ_NUMは7
  for (int freq = 0; freq < FREQ_NUM; freq++)
  {
    digitalWrite(MSGEQ7_STROBE, LOW);
    delayMicroseconds(50); // Output Setting Time(to)
    valAmplitude[freq] = min(analogRead(MSGEQ7_OUT) - 50, maxi);
    if (valAmplitude[freq] == maxi) {
      valAmplitude[freq] = 0;
      // valAmplitude[freq] = 0.2 * valAmplitude[freq];

    }
    delayMicroseconds(20); // 指定や名称無し。これとtoとtsの合計がStrobe to Strobe Delay(tss)
    digitalWrite(MSGEQ7_STROBE, HIGH);
    delayMicroseconds(30); // Strobe Pulse Width(ts)
    Serial.print(valAmplitude[freq]);
    Serial.print(",");
    
  }
  Serial.println("");
  digitalWrite(MSGEQ7_RESET, HIGH);
  delay(5); // Reset Pulse Width(tr)  --50ぐらいがちょうどいいですよ（Knd）
}
