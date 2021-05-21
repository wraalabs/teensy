    /* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Jonathan Payne (jon@jonnypayne.com) / Slightly edited by Tom @ Wraa (2021)
 * Based on Effect_Fade by Paul Stoffregen
 * Also samplerate reduction based on Pete Brown's bitcrusher here: 
 * http://10rem.net/blog/2013/01/13/a-simple-bitcrusher-and-sample-rate-reducer-in-cplusplus-for-a-windows-store-app
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */



#include <Audio.h>
#include <Arduino.h>
#include "effect_bitcrusher2.h"

void AudioEffectBitcrusher2::update(void)
{
  audio_block_t *block;
  uint32_t i;
  int32_t  sampleSquidge, sampleSqueeze; //squidge is bitdepth, squeeze is for samplerate           // tom's edit - squidge and squeeze are now signed ints

                                                                      
      
  if (crushBits2 == 16 && sampleStep2 <= 1) {
    // nothing to do. Output is sent through clean, then exit the function
    block = receiveReadOnly();
    if (!block) return;
    transmit(block);
    release(block);
    return;
  }
  // start of processing functions. Could be more elegant based on external
  // functions but left like this to enable code optimisation later.
  block = receiveWritable();
  if (!block) return;

  if (sampleStep2 <= 1) { //no sample rate mods, just crush the bitdepth.                        <------ just crush 
    for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
      // shift bits right to cut off fine detail sampleSquidge is a
      // uint32 so sign extension will not occur, fills with zeroes.



      
      sampleSquidge = block->data[i] >> (16-crushBits2);                                         
      // shift bits left again to regain the volume level.
      // fills with zeroes.
      block->data[i] = sampleSquidge << (16-crushBits2);
    }
  } else if (crushBits2 == 16) { //bitcrusher not being used, samplerate mods only.              <------ just downsample
    i=0;
    while (i < AUDIO_BLOCK_SAMPLES) {
      // save the root sample. this will pick up a root
      // sample every _sampleStep_ samples.
      sampleSqueeze = block->data[i];
      for (int j = 0; j < sampleStep2 && i < AUDIO_BLOCK_SAMPLES; j++) {
        // for each repeated sample, paste in the current
        // root sample, then move onto the next step.
        block->data[i] = sampleSqueeze;
        i++;
      }
    }
  } else {           //both being used. crush those bits and mash those samples.                <------ both
    i=0;
    while (i < AUDIO_BLOCK_SAMPLES) {
      // save the root sample. this will pick up a root sample
      // every _sampleStep_ samples.
      sampleSqueeze = block->data[i];
      for (int j = 0; j < sampleStep2 && i < AUDIO_BLOCK_SAMPLES; j++) {
        // shift bits right to cut off fine detail sampleSquidge
        // is a uint32 so sign extension will not occur, fills
        // with zeroes. 

   
      if(crushBits2 >= 8)                                                                       // tom's edit - flip bits if bit depth is 8 or above...
      {
        if(sampleSqueeze < 0)
        {
            if(crushBits2 == 15)      sampleSquidge = sampleSqueeze | 0b0000000000000001;
            else if(crushBits2 == 14) sampleSquidge = sampleSqueeze | 0b0000000000000011;
            else if(crushBits2 == 13) sampleSquidge = sampleSqueeze | 0b0000000000000111;
            else if(crushBits2 == 12) sampleSquidge = sampleSqueeze | 0b0000000000001111;
            else if(crushBits2 == 11) sampleSquidge = sampleSqueeze | 0b0000000000011111;
            else if(crushBits2 == 10) sampleSquidge = sampleSqueeze | 0b0000000000111111;
            else if(crushBits2 == 9)  sampleSquidge = sampleSqueeze | 0b0000000001111111;
            else if(crushBits2 == 8)  sampleSquidge = sampleSqueeze | 0b0000000011111111;
        }
    
        else 
        {
            if(crushBits2 == 15)      sampleSquidge = sampleSqueeze & 0b1111111111111110;
            else if(crushBits2 == 14) sampleSquidge = sampleSqueeze & 0b1111111111111100;
            else if(crushBits2 == 13) sampleSquidge = sampleSqueeze & 0b1111111111111000;
            else if(crushBits2 == 12) sampleSquidge = sampleSqueeze & 0b1111111111110000;
            else if(crushBits2 == 11) sampleSquidge = sampleSqueeze & 0b1111111111100000;
            else if(crushBits2 == 10) sampleSquidge = sampleSqueeze & 0b1111111111000000;
            else if(crushBits2 == 9)  sampleSquidge = sampleSqueeze & 0b1111111110000000;
            else if(crushBits2 == 8)  sampleSquidge = sampleSqueeze & 0b1111111100000000;
        }
         block->data[i] = sampleSquidge;
      }
  
      else                                                                                        // ... or zero out certain samples if 9 bit or under
      {
        if(sampleSqueeze < 256 && sampleSqueeze > - 256) sampleSqueeze = 0;

        else 
        {
          sampleSquidge = sampleSqueeze >> (16-crushBits2);                                       // normal bit shifting operation
          block->data[i] = sampleSquidge << (16-crushBits2);
        }
      }          
     
  
      i++;

                
      }
    }
  }
  transmit(block);
  release(block);
}

    
