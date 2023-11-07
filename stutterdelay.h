#ifndef stutterdelay_h_
#define stutterdelay_h_
#include "Arduino.h"
#include "AudioStream.h"

#define startone  0
#define endone    9999
#define starttwo  10000
#define endtwo    19999

class AudioEffectStutterDelay : public AudioStream {
 public:
  AudioEffectStutterDelay(void) : AudioStream(1, inputQueueArray) {}
  
  // initialise the delay line
  void begin(int16_t *delay_line,  uint16_t max_delay_length);

  void onoroff(bool onoff);

  void setcycles(byte cyc);

  void delayorstutter(bool dos);

  void setrate(int ratey);

  void setlag(int laggy);

  bool ledstate();

  // set delay time
  void setdelay(byte channel, uint16_t delay_time);

  
void record(bool reccy);


  // main update routine
  virtual void update(void);

  void inspect(void) { dump_samples = true; };

  
  

 private:

  byte cycles = 4;
  byte cyclecount = 0; 
  byte cyclecount2 = 0;
  byte delorstut = 1;
  uint16_t stutterlength = 12000;
  uint16_t stuttercount = 0;
  int lag;
  bool rec = 1;
  bool LED = 0;

 
  audio_block_t *inputQueueArray[1];
  uint16_t max_delay_length_samples, half_delay_length_samples;   // length of the delay line in samples
  uint16_t write_index = startone;                                       // write head position
  uint16_t write_index2 = starttwo;                                      // write head position
  uint16_t read_index1 = startone;                                           // read_index for channel 0
  uint16_t read_index2 = starttwo;                                           // read_index for channel 1
  uint16_t read_index3 = startone;                                           // read_index for channel 2
  uint16_t read_index4 = startone;                                           // read_index for channel 3

  int16_t *sample_delay_line;                                     // pointer to delay line
  //int16_t *sample_delay_line2;                                    // pointer to delay line
  
  bool onOff = 1;
  bool use_sample = 1;                                            // bit decides if we read this iteration
  
  boolean buffer_filled = false;
  boolean dump_samples = false;
};
#endif
