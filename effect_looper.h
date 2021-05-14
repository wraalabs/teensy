#ifndef effect_looper_h_
#define effect_looper_h_
#include "Arduino.h"
#include "AudioStream.h"

class AudioEffectLooper : public AudioStream {
 public:
  AudioEffectLooper(void) : AudioStream(1, inputQueueArray) {}
  
  void changeState(bool rec, bool playy);

  bool isBufferFull();

  void begin(byte *delay_line, uint32_t max_delay_length); 

  // main update routine
  virtual void update(void);

  // change looper state 
  void state (bool rec, bool playy);

  // set octave (playback speed)
  void setoctave (byte oct);
  

  void inspect(void) { dump_samples = true; };

  

 private:
  audio_block_t *inputQueueArray[1];
  uint32_t max_delay_length_samples, half_delay_length_samples;   // length of the delay line in samples
  uint32_t write_index = 0;                                       // write head position
  uint32_t read_index = 0;                                            // read_index for channel 0
  uint32_t loop_end = 0;
  byte *sample_delay_line;                                     // pointer to delay line
  int16_t samp;                                                   // original sample  
  
  bool use_sample = 1;                                            // bit decides if we write this iteration
  byte sub_sample = 1;
  byte octave;
  byte sample;

  bool buffer_full = 0;
  bool recing;
  bool playing;

  boolean buffer_filled = false;                                 // looks like we have this twice?
  boolean dump_samples = false;
};
#endif
