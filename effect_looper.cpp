/*
* Lofi looper effect that stretches the Teensy's internal RAM by recording at 8-bit & 22050 hz
* You can get about 10 seconds recorded audio this way, run the output through LPFs for less noise
* "setoctave" plays back at different speeds, pass 0 to this function for half speed, 1 for normal speed and 2 for double speed
* Pass 1s and 0s to "changeState" whenever you want to start/stop recording and playing
* Initialise the effect by passing an array of bytes to "begin"
* I use "isbufferfull" to jump from rec'ing to playing if the record buffer is full. this is done in the sketch
*
* Typical open source license, do what ye want with it, idc :D
* Tom @ Wraa, 2021
*/

#include <Arduino.h>
#include "effect_looper.h"

void AudioEffectLooper::begin(byte *delay_line, uint32_t max_delay_length) 
{
  sample_delay_line = delay_line;
  
  max_delay_length_samples = max_delay_length - 1;
}


void AudioEffectLooper::changeState(bool rec, bool playy)       // function to change playing / recording state, must only be called once upon change of state

{
  if(rec)             
  {
    recing = 1;
    playing = 0;
    write_index = 0;
  }
  else if(playy)
  {
    loop_end = write_index; 
    playing = 1;
    recing = 0;  
    read_index = 0;
  }
  else
  {
    recing = 0;
    playing = 0;
  }
}



bool AudioEffectLooper::isBufferFull()
{
  return buffer_full;
}

void AudioEffectLooper::setoctave (byte oct)
{
  octave = oct;
  use_sample = 1;                                                             // should move this to update
}



void AudioEffectLooper::update(void) {
  
  audio_block_t *blockw, *block1;                                             // create audio memory blocks and pointers...

  int16_t *blockw_pointer;                                                    // can we use only 1 of these? 
  int16_t *block_pointer1;
  
// check if have a delay line
  if (sample_delay_line == NULL) return;

  blockw = receiveWritable();                                                 // recieve writable block of samples 
  block1 = allocate();                                                        // allocate block for playing samples
  

//                                                        RECORDING BLOCK
  if (blockw)
  {
    blockw_pointer = blockw->data;

    if(recing)
    {
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)                           // iterate through audio block, combining samples and adding to buffer
      {
        
        if(use_sample)
        {
          samp = *blockw_pointer;
          sample = (samp + 32768) >> 8;
          sample_delay_line[write_index++] = sample;                          // write sample to buffer
    
          if (write_index >= max_delay_length_samples) buffer_full = 1;       // flip bit if buffer maxed out
          else buffer_full = 0;
        }
        use_sample = 1 - use_sample;
        
        blockw_pointer++;
      }
    }
    release(blockw);
  }


// channel 1                                              PLAYBACK BLOCK
  if(block1)
  {
    block_pointer1 = block1->data;
    
    if(playing)
    {
      for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
      {
        //  octave up             neutral                       octave down
        if(octave == 2 || (octave == 1 && use_sample) || (!octave && sub_sample == 1))
        
          {
            sample = sample_delay_line[read_index++];
            *block_pointer1 = (sample << 8) - 32768;                          // read sample out (no writing on channel 1)
               
            if (read_index >= loop_end) read_index = 0;                       // restart loop
          }
        
        else  *block_pointer1 = sample_delay_line[read_index];                // repeat last read sample (actually the next here)
  
        use_sample = 1 - use_sample;                                          // flip bool bit... (octave up mode without this line)

        sub_sample++;
        if(sub_sample == 5) sub_sample = 1;
        
        block_pointer1++; 
      }
    
    transmit(block1, 0);                                                      // transmit the block out on channel 0
    }
    release(block1);
  }
}
