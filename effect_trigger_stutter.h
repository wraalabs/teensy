#ifndef effect_trigger_stutter_h_
#define effect_trigger_stutter_h_
#include "Arduino.h"
#include "AudioStream.h"

class AudioEffectTriggerStutter : public AudioStream {
 public:
  AudioEffectTriggerStutter(void) : AudioStream(1, inputQueueArray) {}
  
  // initialise the delay line
  //void record(byte *delay_line, uint32_t max_delay_length);

  void begin(uint16_t *delay_line, uint32_t max_delay_length); 

  void stutter(uint16_t stutter_length, byte cycles);           // length in miliseconds, times repeated

  void updatestutter(uint16_t stutter_length, byte cycles);

  void setreverse(bool rev);
  
  // main update routine
  virtual void update(void);



  void inspect(void) { dump_samples = true; };

  

 private:
  audio_block_t *inputQueueArray[1];
  uint32_t max_delay_length_samples, half_delay_length_samples;   // length of the delay line in samples
  uint32_t write_index = 0;                                       // write head position
  uint32_t read_index = 0;                                         // read_index for channel 0
  uint32_t read_index2 = 0;                                        // read_index for channel 1
  uint16_t *sample_delay_line;                                     // pointer to delay line
  uint32_t loopend;

  bool reverse = 0;
  bool use_sample = 1; 

  bool recing;
  bool playing;

  boolean buffer_filled = false;                                 // looks like we have this twice?
  boolean dump_samples = false;
};
#endif
