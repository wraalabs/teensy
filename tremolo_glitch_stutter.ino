#define LEDD 3
#include <Bounce2.h>

#include "analyze_notefreq2.h"
#include "stutterdelay.h"


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=208.88333129882812,745.8833312988281
AudioMixer4              mixer8; //xy=285.8833312988281,1121.4833984375
AudioMixer4              mixer7; //xy=359.88336181640625,990.4833374023438
AudioSynthWaveform       waveform1; //xy=466.88330078125,712.9833984375
AudioFilterBiquad        biquad1; //xy=481.88330078125,509.9833984375
AudioEffectStutterDelay         delay1;         //xy=485,902
AudioEffectStutterDelay         delay2; //xy=489.75,1154.25
AudioSynthWaveform       waveform2; //xy=548.75,412.75006103515625
AudioFilterBiquad        biquad3; //xy=563.75,209.75006103515625
AudioSynthWaveformDc     dc1; //xy=567.8833312988281,634.9833679199219
AudioFilterBiquad        biquad2; //xy=631,706.2000122070312
AudioSynthWaveformDc     dc2; //xy=649.7500305175781,334.7500305175781
AudioEffectMultiply      multiply1; //xy=657.88330078125,561.9833984375
AudioAnalyzeNoteFrequency2 notefreq1; //xy=661.88330078125,505.9833984375
AudioEffectFade          fade1;          //xy=669,816
AudioEffectFade          fade3; //xy=674,914.2000122070312
AudioEffectFade          fade5;  //xy=673.75,1068.25
AudioEffectFade          fade2; //xy=676,867.2000122070312
AudioEffectFade          fade4; //xy=678,969.2000122070312
AudioEffectFade          fade7; //xy=678.75,1166.4500122070312
AudioEffectFade          fade6; //xy=680.75,1119.4500122070312
AudioEffectFade          fade8; //xy=682.75,1221.4500122070312
AudioFilterBiquad        biquad4; //xy=712.86669921875,405.9666748046875
AudioEffectMultiply      multiply2; //xy=739.75,261.75006103515625
AudioAnalyzeNoteFrequency2 notefreq2; //xy=743.75,205.75006103515625
AudioMixer4              mixer1; //xy=786.88330078125,662.9833374023438
AudioMixer4              mixer4; //xy=868.75,362.75
AudioMixer4              mixer3; //xy=867,869.2000122070312
AudioMixer4              mixer6; //xy=871.75,1121.4500122070312
AudioMixer4              mixer2; //xy=1179,807.2000122070312
AudioMixer4              mixer5; //xy=1183.75,908.75
AudioOutputI2S           i2s2;           //xy=1347.88330078125,853.88330078125
AudioConnection          patchCord1(i2s1, 0, biquad1, 0);
AudioConnection          patchCord2(i2s1, 0, multiply1, 0);
AudioConnection          patchCord3(i2s1, 0, mixer7, 0);
AudioConnection          patchCord4(i2s1, 1, multiply2, 0);
AudioConnection          patchCord5(i2s1, 1, biquad3, 0);
AudioConnection          patchCord6(i2s1, 1, mixer8, 0);
AudioConnection          patchCord7(mixer8, delay2);
AudioConnection          patchCord8(mixer7, delay1);
AudioConnection          patchCord9(waveform1, biquad2);
AudioConnection          patchCord10(biquad1, notefreq1);
AudioConnection          patchCord11(delay1, 0, fade1, 0);
AudioConnection          patchCord12(delay1, 1, fade2, 0);
AudioConnection          patchCord13(delay1, 2, fade3, 0);
AudioConnection          patchCord14(delay1, 3, fade4, 0);
AudioConnection          patchCord15(delay2, 0, fade5, 0);
AudioConnection          patchCord16(delay2, 1, fade6, 0);
AudioConnection          patchCord17(delay2, 2, fade7, 0);
AudioConnection          patchCord18(delay2, 3, fade8, 0);
AudioConnection          patchCord19(waveform2, biquad4);
AudioConnection          patchCord20(biquad3, notefreq2);
AudioConnection          patchCord21(dc1, 0, mixer1, 0);
AudioConnection          patchCord22(biquad2, 0, mixer1, 3);
AudioConnection          patchCord23(dc2, 0, mixer4, 0);
AudioConnection          patchCord24(multiply1, 0, mixer2, 0);
AudioConnection          patchCord25(fade1, 0, mixer3, 0);
AudioConnection          patchCord26(fade3, 0, mixer3, 2);
AudioConnection          patchCord27(fade5, 0, mixer6, 0);
AudioConnection          patchCord28(fade2, 0, mixer3, 1);
AudioConnection          patchCord29(fade4, 0, mixer3, 3);
AudioConnection          patchCord30(fade7, 0, mixer6, 2);
AudioConnection          patchCord31(fade6, 0, mixer6, 1);
AudioConnection          patchCord32(fade8, 0, mixer6, 3);
AudioConnection          patchCord33(biquad4, 0, mixer4, 3);
AudioConnection          patchCord34(multiply2, 0, mixer5, 0);
AudioConnection          patchCord35(mixer1, 0, multiply1, 1);
AudioConnection          patchCord36(mixer4, 0, multiply2, 1);
AudioConnection          patchCord37(mixer3, 0, mixer2, 1);
AudioConnection          patchCord38(mixer3, 0, mixer7, 1);
AudioConnection          patchCord39(mixer6, 0, mixer5, 1);
AudioConnection          patchCord40(mixer6, 0, mixer8, 1);
AudioConnection          patchCord41(mixer2, 0, i2s2, 0);
AudioConnection          patchCord42(mixer5, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1696.88330078125,124.88330078125
// GUItool: end automatically generated code



// AudioAnalyzeNoteFrequency2

// AudioEffectStutterDelay


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

//////////////////// TREMOLO
float note;
int division;
float depth;
byte waveform = 1;
byte lastwaveform = 100;

///////////////////  GLITCH DELAY
float fadefloat;
void fadeout();
void fadein();
unsigned int glitchtime = 300;
unsigned int fadetime = 50;
unsigned int density = 50;
unsigned int delaytime = 1000;
unsigned long currentMillis = millis();
byte whichfade = 0;   // 0 is fade in fade1, fade out 2, fade out 3. etc...
byte lastfade = 1;
bool latchstate = 0;  // the state of the footswitch in latching mode. 0 = off, 1 = on


//////////////////////////////////////////////////////////////////////////////////////////////// STUTTER

uint16_t delay_max = 20000; // number of samples at 44100 samples a second
int16_t delay_line[21000] = {};
int16_t delay_line2[21000] = {};

byte cycles;
uint16_t rate; 
uint16_t lag;
float floatlag;
int intlag;
float mixy;
int intmixy;
bool freeze = 0;     // 0 is record so it's called freeze



extern "C" uint32_t set_arm_clock(uint32_t frequency);

void setup() {
  
  // set_arm_clock(146000000);
 
  AudioMemory(1300); // the "40" represents how much internal memory (in the Teensy, not the external RAM chip) is allotted for audio recording. It is measured in sample blocks, each providing 2.9ms of audio.
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
 // sgtl5000_1.autoVolumeEnable();
  analogReadAveraging(128);  




///////////////////////////////////////////// TREMOLO 
  // sine wave / note detection
  waveform1.begin(1, 5, WAVEFORM_SINE);
  notefreq1.begin(.60);
  waveform2.begin(1, 5, WAVEFORM_SINE);
  notefreq2.begin(.60);
  // pre-note-freq filter & waveform filter
  biquad1.setLowpass(0, 1000, 0.7);
  biquad2.setLowpass(0, 500, 0.7);
  biquad3.setLowpass(0, 1000, 0.7);
  biquad4.setLowpass(0, 500, 0.7);
  //depth
  dc1.amplitude(1);
  waveform1.offset(1);
  dc2.amplitude(1);
  waveform2.offset(1);
  
 // notefreq1.on_off(1);                  // note freq on 
 // notefreq2.on_off(1);                  // note freq on 


/////////////////////////////////////////// GLITCH DELAY

  // fades mixer
  mixer3.gain(0,1);
  mixer3.gain(1,1);
  mixer3.gain(2,1);
  mixer3.gain(3,1);
  mixer6.gain(0,1);
  mixer6.gain(1,1);
  mixer6.gain(2,1);
  mixer6.gain(3,1);
  fade1.fadeOut(1); // fade out to begin
  fade2.fadeOut(1);
  fade3.fadeOut(1);
  fade4.fadeOut(1);
  fade5.fadeOut(1); // fade out to begin
  fade6.fadeOut(1);
  fade7.fadeOut(1);
  fade8.fadeOut(1);




  /////////////////////////////////////////////// STUTTER DELAY
  
  delay1.begin(delay_line, delay_max);
  delay2.begin(delay_line2, delay_max);
 // delay1.onoroff(1);
 // delay2.onoroff(1);


  
}

void loop() {

 checkToggle();
////////////////////////////// TREMOLO //////////////////////////////
if(left){

  // turn off other effects
  notefreq1.on_off(1);                  // note freq off  
  notefreq2.on_off(1);                  // note freq off  
  mixer2.gain(0,1);
  mixer5.gain(0,1);
  mixer2.gain(1,0);
  mixer5.gain(1,0);

while(left)
{
   checkToggle();
  
  // Depth
  footswitch.update();                                                                                   
  if(!digitalRead(0)) {depth = 0;}
  else depth = (float) analogRead(A3) / 4095;

  
  mixer1.gain(0,1-depth);
  mixer1.gain(3,depth);
  mixer4.gain(0,1-depth);
  mixer4.gain(3,depth);

  // read fundamental frequency
  if (notefreq1.available()) 
  {
    note = notefreq1.read(); 
    //division
    division = 7 - ((analogRead(A2) >> 9));// + 2);   // pot controls division, 5 to 2
    division = 2 << division;                         // the 4 divisions are 64 to 8
   // if
    // set waveform frequency
    note = note / division;
    waveform1.frequency(note);
  }
   
  if (notefreq2.available()) 
  {
    note = notefreq2.read(); 
    //division
    division = 7 - ((analogRead(A2) >> 9));// + 2);  // pot controls division, 5 to 2
    division = 2 << division;             // the 4 divisions are 64 to 8
    // set waveform frequency
    note = note / division;
    waveform2.frequency(note);
  }

 waveform = analogRead(A1) >> 9;  

 if(lastwaveform != waveform)
 {
   if(waveform == 0) { waveform1.begin(WAVEFORM_SINE); waveform2.begin(WAVEFORM_SINE);}
   if(waveform == 1) { waveform1.begin(WAVEFORM_SAWTOOTH); waveform2.begin(WAVEFORM_SAWTOOTH); }
   if(waveform == 2) { waveform1.begin(WAVEFORM_SAWTOOTH_REVERSE); waveform2.begin(WAVEFORM_SAWTOOTH_REVERSE); }
   if(waveform == 3) { waveform1.begin(WAVEFORM_SQUARE); waveform2.begin(WAVEFORM_SQUARE); }
   if(waveform == 4) { waveform1.begin(WAVEFORM_TRIANGLE); waveform2.begin(WAVEFORM_TRIANGLE); }
   if(waveform == 5) { waveform1.begin(WAVEFORM_PULSE); waveform2.begin(WAVEFORM_PULSE); }
   if(waveform == 6) { waveform1.begin(WAVEFORM_SAMPLE_HOLD); waveform2.begin(WAVEFORM_SAMPLE_HOLD); }
   if(waveform == 7) { waveform1.begin(WAVEFORM_SINE); waveform2.begin(WAVEFORM_SINE); }
   
   lastwaveform = waveform;
 }

}
}

//////////////////////////////////////////////////////////////////////////////////  GLITCH DELAY
if(middle)
{
    notefreq1.on_off(0);                  // note freq off  
    notefreq2.on_off(0);                  // note freq off              
    delay1.onoroff(1);                    // delay on   
    delay2.onoroff(1);                    // delay on 
    delay1.delayorstutter(0);             // stutter mode
    delay2.delayorstutter(0);  
    mixer2.gain(0,0);
    mixer5.gain(0,0);
    mixer2.gain(1,1);
    mixer5.gain(1,1);

    mixer7.gain(0,1);                     // feedback mixers
    mixer7.gain(1,0);                     
    mixer8.gain(0,1);                     
    mixer8.gain(1,0);                     


    mixer3.gain(0,1); mixer3.gain(1,1); mixer3.gain(2,1); mixer3.gain(3,1);   // fade mixers
    mixer6.gain(0,1); mixer6.gain(1,1); mixer6.gain(2,1); mixer6.gain(3,1);
    fade1.fadeOut(1); 
    fade2.fadeOut(1);
    fade3.fadeOut(1);
    fade4.fadeOut(1);
    fade5.fadeOut(1); 
    fade6.fadeOut(1);
    fade7.fadeOut(1);
    fade8.fadeOut(1);

    fadeout();
    fadein();

  while(middle)
  {
     checkToggle();
 
   if(millis() - currentMillis >= glitchtime)              //(count >= glitchtime);  // time to glitch
    {
      // rate of glitching
      glitchtime = (analogRead(A0) >> 3) + 80;  // 50 to 561

      fadetime = glitchtime / (64 - (analogRead(A1) >>  6)) ; // 64 to  1       // MAKE FLOAT !
      
      density = analogRead(A2);
      delaytime = (analogRead(A3) / 11) + 100;
      if(density >= random(4095))
      {
        fadeout();
  
        lastfade = whichfade;
        while(whichfade == lastfade)
        {whichfade = random(4);}

       // whichfade = 1;
        
        fadein();
      }
      currentMillis = millis();    
    }
  

  
    footswitch.update();
                                                                                        
      if(!digitalRead(0)) {mixer7.gain(0,0); mixer7.gain(1,1); mixer8.gain(0,0); mixer8.gain(1,1); digitalWrite(LEDD,HIGH);}                                    // turn on feedback
      else{mixer7.gain(0,1); mixer7.gain(1,0); mixer8.gain(0,1); mixer8.gain(1,0); digitalWrite(LEDD,LOW);}    
}        
}


  ///////////////////////////////////////////////////////// STUTTERDELAY

  if(right)
  {
    fade1.fadeIn(1);
    fade5.fadeIn(1);
    fade2.fadeIn(1);
    fade6.fadeIn(1);
    notefreq1.on_off(0);                  // note freq off  
    notefreq2.on_off(0);                  //              
    delay1.onoroff(1);                    // delay on 
    delay2.onoroff(1);                    // 
    delay1.delayorstutter(1);             // stutter mode
    delay2.delayorstutter(1);
   
    mixer2.gain(0,0);                     // output mixers
    mixer5.gain(0,0);
    mixer2.gain(1,1);
    mixer5.gain(1,1);
   
    mixer7.gain(0,1);                     // feedback mixers
    mixer7.gain(1,0);                     
    mixer8.gain(0,1);                     
    mixer8.gain(1,0);    

    mixer3.gain(0,1); mixer3.gain(1,1); mixer3.gain(2,0); mixer3.gain(3,0);   // fade mixers
    mixer6.gain(0,1); mixer6.gain(1,1); mixer6.gain(2,0); mixer6.gain(3,0);


    while(right)
    {
        checkToggle();
        rate = (8190 - (analogRead(A0) * 2)) + 1800 ;
        delay1.setrate(rate);
        delay2.setrate(rate);
        
        cycles = (analogRead(A1) >> 9) + 1;

        cycles *= 2;

        //cycles = 16;
        
        delay1.setcycles(cycles);
        delay2.setcycles(cycles);




        intlag = (float) (analogRead(A2) ) ;     // lag pot 
        floatlag = (float) intlag / 4095;        
        floatlag = (float) 1 - floatlag;
        if(floatlag > 0.97) floatlag = 1;    

        if(cycles == 2) lag = 1;
        else    
          lag = ((cycles / 2) - 1) * floatlag  ;

        

        //lag = 7;
    //     Serial.print(lag);
    //     Serial.print('\n');
    //    delay(500);
        
        delay1.setlag(lag);
        delay2.setlag(lag);
      
        
        intmixy = analogRead(A3) ;
        mixy = (float) intmixy / 4095;
        if((mixy <= 0.53) && (mixy >= 0.47))
        {
          mixer3.gain(1, 1);
          mixer3.gain(0, 1); 
          mixer6.gain(1, 1);
          mixer6.gain(0, 1);
        }
        else if (mixy < 0.47) // CCW, turn buffer 2 down 
        {
          mixer3.gain(1, (mixy + 0.5) );
          mixer3.gain(0,  1);
          mixer6.gain(1, (mixy * 2)); //+ 0.5) );
          mixer6.gain(0,  1);
        }
        else
        {
          mixy = 1 - mixy;// 0.54;
          //mixy = 0.47 - mixy;
          mixer3.gain(0, mixy + 0.5);
          mixer3.gain(1, 1);
          mixer6.gain(0,  mixy * 2); //+ 0.5);
          mixer6.gain(1, 1);
        }

        footswitch.update();                                                                                        
        if(!digitalRead(0)) freeze = 1;
        else freeze = 0;

        
        delay1.record(freeze);
        delay2.record(freeze);


     //   Serial.print(freeze);
    //    Serial.print('\n');
    //    delay(500);


        if(delay1.ledstate()) digitalWrite(LEDD,HIGH);
        else digitalWrite(LEDD,LOW);
    }
  
  }




  
}









// Glitch fades
void fadeout ()
{
  if(whichfade == 0) {fade1.fadeOut(fadetime); delay1.setdelay(2, delaytime * .29);   fade5.fadeOut(fadetime); delay2.setdelay(2, delaytime * .29); }
  if(whichfade == 1) {fade2.fadeOut(fadetime); delay1.setdelay(3, delaytime * .53);   fade6.fadeOut(fadetime); delay2.setdelay(3, delaytime * .53); }   
  if(whichfade == 2) {fade3.fadeOut(fadetime); delay1.setdelay(0, delaytime * .74);   fade7.fadeOut(fadetime); delay2.setdelay(0, delaytime * .74); }
  if(whichfade == 3) {fade4.fadeOut(fadetime); delay1.setdelay(1, delaytime * 1);     fade8.fadeOut(fadetime); delay2.setdelay(1, delaytime * 1);   }
}
void fadein ()
{
  if(whichfade == 0) {fade1.fadeIn(fadetime); fade5.fadeIn(fadetime); }
  if(whichfade == 1) {fade2.fadeIn(fadetime); fade6.fadeIn(fadetime); }
  if(whichfade == 2) {fade3.fadeIn(fadetime); fade7.fadeIn(fadetime); }
  if(whichfade == 3) {fade4.fadeIn(fadetime); fade8.fadeIn(fadetime); }
}
