#include <Arduino.h>

#include "effect_freeze2.h"

void AudioEffectFreeze2::begin(int16_t *delay_line, uint16_t max_delay_length) 
{
  sample_delay_line = delay_line;
  
  max_delay_length_samples = max_delay_length - 1;
}


void AudioEffectFreeze2::freeze(bool freezey)       // function to begin and end freezing
{
  writing = freezey;
  reading = freezey;

  if(freezey)                                       // when we start freezing...
  {
    windex = 0;                                     // write from the top
    rindex1 = 0;                                    // read from the top
    forb = 0;                                       // go forwards first
  }
}


void AudioEffectFreeze2::changeLength(uint16_t samples)
{
  max_delay_length_samples = samples;
}


void AudioEffectFreeze2::update(void) {
  
  audio_block_t *blockw, *block1;                                  // create audio memory blocks and pointers...

  int16_t *blockw_pointer;                                                 
  int16_t *block_pointer1;
  
// check if have a delay line
  if (sample_delay_line == NULL) return;

  blockw = receiveWritable();                                               // recieve writable block of samples 
  block1 = allocate();                                                      // allocate block for playing samples




//                                                        WRITING BLOCK
  if (blockw)
  {
    blockw_pointer = blockw->data;

    if(writing)
    {
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)                         // iterate through audio block, combining samples and adding to buffer
      {
        
        sample_delay_line[windex++] = *blockw_pointer;                      // write sample to buffer
  
        if (windex >= max_delay_length_samples)                             // end of short buffer, stop recording 
        {
          writing = 0;                                                      // stop writing
          windex = 0;                                                       // reset write index for next time
        }
        
        blockw_pointer++;                                                   // advance pointer
      
      }
      
    }
    release(blockw);            // DOES THIS GO AFTER NEXT BRACKET?
  }


//                                                       READING BLOCK 1
  if(block1)
  {
    block_pointer1 = block1->data;
                                             
    
    for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                             // iterate through audio block
    {
      


      if(reading)
      {

          *block_pointer1  = sample_delay_line[rindex1];                          // read sample out

          if(!forb)                               // if going forwards
          {    
            
            if (rindex1 >= max_delay_length_samples)                                // if played a full buffer     
            {
              forb = 1;                           //changed to backwards
                
              rindex1--;  
            }
            else rindex1++;
            
          }
    
          else if(forb)                         // if going backwards
          {
    
              if (rindex1 == 0)                                                     // if back at the start
              {
                forb = 0;                           //changed to forwards
                  
                rindex1++;  
              }
              else rindex1--;
    
          }
          
      }
      else *block_pointer1 = 0;       // if not freezing, write zeroes to output
      
      block_pointer1++;               // this goes forwards every time i believe
      
    }
    
    transmit(block1, 0);                                                    // transmit the block out on channel 0
    release(block1);
  }

  
}
