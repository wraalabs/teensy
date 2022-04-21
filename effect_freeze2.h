#ifndef effect_freeze2_h_
#define effect_freeze2_h_
#include "Arduino.h"
#include "AudioStream.h"

class AudioEffectFreeze2 : public AudioStream {
 public:
  AudioEffectFreeze2(void) : AudioStream(1, inputQueueArray) {}
  
 

  void begin(int16_t *delay_line, uint16_t max_delay_length); 

  void freeze(bool freezey);

  void changeLength(uint16_t samples);
 
  // main update routine
  virtual void update(void);

  void inspect(void) { dump_samples = true; };

  

 private:
  audio_block_t *inputQueueArray[1];
  uint16_t max_delay_length_samples, half_delay_length_samples;   // length of the delay line in samples

  
  uint16_t windex = 0;                                       // write head position
  uint16_t rindex1 = 0;                                            // read_index for channel 0
  uint16_t rindex2 = 0;                                            // read_index for channel 1


  //freeze 2
  bool forb = 0;                 // forwards or backwards , 0 = forwards


  

    
  int16_t *sample_delay_line;                                     // pointer to delay line
  int16_t samp;                                                   // original sample  


  bool writing = 0;
  bool reading = 0;

  boolean buffer_filled = false;                                 // looks like we have this twice?
  boolean dump_samples = false;
};
#endif
