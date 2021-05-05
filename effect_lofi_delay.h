#ifndef effect_lofi_delay_h_
#define effect_lofi_delay_h_
#include "Arduino.h"
#include "AudioStream.h"

class AudioEffectLofiDelay : public AudioStream {
 public:
  AudioEffectLofiDelay(void) : AudioStream(1, inputQueueArray) {}
  
  // initialise the delay line
  void begin(int16_t *delay_line, uint16_t max_delay_length);

  // set delay time
  void setdelay(byte channel, uint16_t delay_time);

  void acidMode(bool acid);
  
  void setoctave(byte oct);

  // main update routine
  virtual void update(void);

  void inspect(void) { dump_samples = true; };

 private:
  audio_block_t *inputQueueArray[1];
  uint16_t max_delay_length_samples, half_delay_length_samples;   // length of the delay line in samples
  uint16_t write_index = 0;                                       // write head position
  uint16_t read_index1;                                            // read_index for channel 0
  uint16_t read_index2;                                           // read_index for channel 1
  uint16_t read_index3;                                           // read_index for channel 1
  uint16_t read_index4;                                           // read_index for channel 1
  uint16_t read_index5;                                           // read_index for channel 1
  uint16_t read_index6;                                           // read_index for channel 1
  int16_t *sample_delay_line;                                     // pointer to delay line
  
  bool use_sample = 1;                                            // bit decides if we write this iteration
  byte octave = 0;                                                // if 1, octave up mode (for now)
  byte sub_sample = 1;                                            // play sub sample on the 1st, reset after 4

  bool acid_mode = 0;                                             // set to 1 t reset read_indexes every time delay time is changed

  boolean buffer_filled = false;
  boolean dump_samples = false;
};
#endif
