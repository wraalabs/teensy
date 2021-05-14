#define LED 3
#include <Bounce.h>
#include "effect_looper.h"
#include <vibrato.h>


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=109,294
AudioFilterBiquad        biquad1;        //xy=253,292
AudioEffectLooper      lofilooper1;      //xy=399,293
AudioFilterBiquad        biquad2; //xy=553,290
AudioEffect_Vibrato        vibrato1;        //xy=702,289
AudioEffectBitcrusher    bitcrusher1;    //xy=863,290
AudioFilterBiquad        biquad3;        //xy=1019,288
AudioAmplifier           amp1;           //xy=1151,287
AudioOutputI2S           i2s2;           //xy=1287,283
AudioConnection          patchCord1(i2s1, 0, biquad1, 0);
AudioConnection          patchCord2(biquad1, lofilooper1);
AudioConnection          patchCord3(lofilooper1, biquad2);
AudioConnection          patchCord4(biquad2, vibrato1);
AudioConnection          patchCord5(vibrato1, bitcrusher1);
AudioConnection          patchCord6(bitcrusher1, biquad3);
AudioConnection          patchCord7(biquad3, amp1);
AudioConnection          patchCord8(amp1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=977,449
// GUItool: end automatically generated code


Bounce footswitch = Bounce(0, 50);  // debounce the footswitch
Bounce D1 = Bounce(1, 50);          // debounce the toggle switch
Bounce D2 = Bounce(2, 50);          // "  "  "  "  "  "  "  "  "

// this section includes the function to check the toggle position
bool right;
bool middle;
bool left;
void checkToggle () {               // this is our function to check toggle position...
D1.update();  D2.update();          // check digital inputs connected to toggle (can delete I think)
if(digitalRead(1) && !digitalRead(2))   {right = 1; middle = 0; left = 0;}    // toggle is right
if(digitalRead(1) && digitalRead(2))  {right = 0; middle = 1; left = 0;}      // toggle is in the middle
if(!digitalRead(1) && digitalRead(2))   {right = 0; middle = 0; left = 1;}    // toggle is left
}
#define SAMPLES_IN_BUFFER 240000

// sample buffer
uint32_t delay_max = SAMPLES_IN_BUFFER; // number of samples at 44100 samples a second
byte delay_line[SAMPLES_IN_BUFFER] = {};

bool recstate;
bool playstate;
bool bufferfull;
unsigned long currentMillis = millis();

bool buttonstate = 0;

// vibrato
float rate;
float depth;

//crush
int samplerate;

// filter
int lpf;

void setup() {
 
  AudioMemory(40); // the "40" represents how much internal memory (in the Teensy, not the external RAM chip) is allotted for audio recording. It is measured in sample blocks, each providing 2.9ms of audio.
  sgtl5000_1.enable();    // this turns on the SGTL5000, which is the audio codec on the audio board
  sgtl5000_1.volume(1);   // this sets the output volume (it can be between 0 and 1)
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // selects the audio input, we always use Line In
  analogReadResolution(12); // configure the pots to give 12 bit readings
  pinMode(0, INPUT_PULLUP); // internal pull-up resistor for footswitch
  pinMode(1, INPUT_PULLUP); // internal pull-up resistor for toggle
  pinMode(2, INPUT_PULLUP); // internal pull-up resistor for toggle
  pinMode(3, OUTPUT);       // pin 3 (the LED) is an output;
  Serial.begin(9600);       // initiate the serial monitor. USB is always 12 Mbit/sec

  sgtl5000_1.adcHighPassFilterDisable();
  sgtl5000_1.audioPostProcessorEnable();
  sgtl5000_1.audioPreProcessorEnable();
  sgtl5000_1.autoVolumeEnable();
  analogReadAveraging(32);  

  lofilooper1.begin(delay_line, delay_max);

  // pre-filter
  biquad1.setLowpass(0, 5000, 1.5);
  
  // post-filter
  biquad2.setLowpass(0, 4670, 0.4);
  biquad2.setLowpass(1, 4780, 0.6);
  biquad2.setLowpass(2, 5000, 0.8);
  biquad2.setLowpass(3, 5250, 1.5);
  
  bitcrusher1.bits(16);   // passthrough (even though it's already 8 bit!)

  
  
}

void loop() {
  
  footswitch.update();

  if(buttonstate && footswitch.risingEdge())
  buttonstate = 0;
  
  if(footswitch.fallingEdge() && !recstate && !playstate && !buttonstate) // start recording             
  {
    lofilooper1.changeState(1, 0);
    recstate = 1;   
    buttonstate = 1;
  }
  else if(footswitch.fallingEdge() && recstate && !buttonstate)           // stop recording/ start playing
  {
    lofilooper1.changeState(0, 1);   
    recstate = 0;
    playstate = 1;
    currentMillis = millis();
    buttonstate = 1;
  }
  else if(footswitch.fallingEdge() && playstate && !buttonstate)          // stop playing
  {
    lofilooper1.changeState(0, 0);   
    playstate = 0;
    buttonstate = 1;
  }

  // buffer full
  bufferfull = lofilooper1.isBufferFull();
  if(recstate && bufferfull)                                              // if buffer maxed while rec'ing
  {
    lofilooper1.changeState(0, 1);   
    recstate = 0;
    playstate = 1;
    bufferfull = 0;
  }

  // toggle (rec/playback speed) and make up gain for slower speeds
  checkToggle();
  if(left)  {lofilooper1.setoctave(0); amp1.gain(2);}
  if(middle){lofilooper1.setoctave(1); amp1.gain(1.5);}
  if(right) {lofilooper1.setoctave(2); amp1.gain(1);}
  

  // vibrato
  rate = (float) (analogRead(A0) / 1000) + 1;  
  depth = (float) analogRead(A1) / 400;
  vibrato1.modulation(rate, depth);

  //crush
  samplerate = (analogRead(A2) * 3) + 1000;
  bitcrusher1.sampleRate(samplerate);

  // variable filter
  lpf = analogRead(A3)  + 1000;
  biquad3.setLowpass(0, lpf, 1);


  // LEDS
  if(recstate) digitalWrite(LED, HIGH);                                         // LED on if rec'ing
  
   if(playstate && millis() - currentMillis < 50) digitalWrite(LED, HIGH);      // flash LED for play
   if(playstate && millis() - currentMillis >= 50) digitalWrite(LED, LOW);
   if (playstate && millis() - currentMillis >= 100) currentMillis = millis();
  
  else if(!playstate && !recstate) digitalWrite(LED, LOW);                      // turn LED off if idle
  

}
