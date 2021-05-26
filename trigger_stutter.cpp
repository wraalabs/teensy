// working draft of stutter effect library.

#include <Arduino.h>

#include "effect_trigger_stutter.h"

void AudioEffectTriggerStutter::begin(uint16_t *delay_line, uint32_t max_delay_length) 
{
  sample_delay_line = delay_line;
  
  max_delay_length_samples = max_delay_length - 1;
}


void AudioEffectTriggerStutter::stutter(uint16_t stutter_length, byte cycles) 
{
  loopend = stutter_length * AUDIO_SAMPLE_RATE_EXACT / 1000;                              // turn samples to miliseconds
  write_index = 0;
  read_index = 0;
  recing = 1;
}

void AudioEffectTriggerStutter::updatestutter(uint16_t stutter_length, byte cycles)       // same as above method, but don't reset indexes. Dduring the first cycle we reset the division, having read the new frequency
{
  loopend = stutter_length * AUDIO_SAMPLE_RATE_EXACT / 1000;                              // turn samples to miliseconds
  recing = 1;
}

void AudioEffectTriggerStutter::setreverse (bool rev)
{
  reverse = rev;
              
  if(write_index == 0) {read_index = max_delay_length_samples;}             // regardless of "reverse" state, read_index should be 1 behind write_index before glitching
  else read_index = write_index - 1;  
    
}



void AudioEffectTriggerStutter::update(void) {
  
  audio_block_t *blockw, *block1;                                           // create audio memory blocks and pointers...

  int16_t *blockw_pointer;                                                  // can we use only 1 of these? 
  int16_t *block_pointer1;
  
// check if have a delay line
  if (sample_delay_line == NULL) return;

  blockw = receiveWritable();                                               // recieve writable block of samples 
  block1 = allocate();                                                      // allocate block for playing samples
  

//                                                        WRITING BLOCK
  if (blockw)
  {
    blockw_pointer = blockw->data;

      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)                         // iterate through audio block, combining samples and adding to buffer
      {
   //     if(use_sample)
   //     {

      if(recing)
      {   
          sample_delay_line[write_index++] = *blockw_pointer;               // write sample to buffer
    
          if (write_index >= loopend) recing = 0;                            // stop recording at "loopend"
      }   
      
    //    }
    //    use_sample = 1 - use_sample;                                        // flip bit
        blockw_pointer++;                                                   // advance pointer
      
      }
      release(blockw);
  }


// channel 1                                              READING BLOCK
  if(block1)
  {
    block_pointer1 = block1->data;
    
  
    for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
    {
  //    if(use_sample)  // going 44100hz for now
  //    {
        
        *block_pointer1  = sample_delay_line[read_index++];               // read sample out
         
        if (read_index >= loopend) read_index = 0;       // wrap around
        
  //    }
  //    else  *block_pointer1 = sample_delay_line[read_index];                // repeat last read sample (actually the next here)
  //    use_sample = 1 - use_sample;                                          // flip bool bit... (octave up mode without this line)
      block_pointer1++; 
    }
    
    transmit(block1, 0);                                                    // transmit the block out on channel 0
    release(block1);
  }

  
}
