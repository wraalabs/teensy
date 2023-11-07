#include <Arduino.h>

#include "stutterdelay.h"


void AudioEffectStutterDelay::begin(int16_t *delay_line,  uint16_t max_delay_length) 
{
  sample_delay_line = delay_line;
  
  max_delay_length_samples = max_delay_length - 1;
}


void AudioEffectStutterDelay::onoroff(bool onoff)
{
  onOff = onoff;
}

void AudioEffectStutterDelay::setcycles(byte cyc)
{
  cycles = cyc;
}

void AudioEffectStutterDelay::delayorstutter(bool dos)
{
  delorstut = dos;
}

void AudioEffectStutterDelay::setrate(int ratey)
{
  stutterlength = ratey;
}

void AudioEffectStutterDelay::setlag(int laggy)
{
  lag = laggy;
}

void AudioEffectStutterDelay::record(bool reccy)
{
  rec = reccy;
}

bool AudioEffectStutterDelay::ledstate()
{
  return LED;
}


void AudioEffectStutterDelay::setdelay(byte channel, uint16_t delay_time)                 // pass delaytime in miliseconds
{
    
  delay_time = delay_time * AUDIO_SAMPLE_RATE_EXACT / 1000;                             // turn samples to miliseconds

  
 if(!delorstut) { 
  if(!channel)                                                                          // sets delay time for channel 0
  {    
      if(write_index - delay_time >= 0) read_index1 = write_index - delay_time;         
      else read_index1 = endtwo - (delay_time - write_index);    
  }

  else if (channel == 1)
  {   
      if(write_index - delay_time >= 0) read_index2 = write_index - delay_time;           // sets delay time for channel 1
      else read_index2 = endtwo - (delay_time - write_index);   
  }

  else if (channel == 2)
  {   
      if(write_index - delay_time >= 0) read_index3 = write_index - delay_time;           // sets delay time for channel 2
      else read_index3 = endtwo - (delay_time - write_index);   
  }

  else if (channel == 3)
  {
      if(write_index - delay_time >= 0) read_index4 = write_index - delay_time;           // sets delay time for channel 3
      else read_index4 = endtwo - (delay_time - write_index);
  }

 }
  
}







void AudioEffectStutterDelay::update(void) {

if(onOff)
{

  
  audio_block_t *blockw,  *block1, *block2, *block3, *block4;        // create audio memory blocks and pointers...

  int16_t *blockw_pointer;                                                   // can we use only 1 of these? 
  int16_t *block_pointer1;
  int16_t *block_pointer2;
  int16_t *block_pointer3;
  int16_t *block_pointer4;


// check if have a delay line
  if (sample_delay_line == NULL) return;

  blockw = receiveWritable();                                                // recieve writable block of samples 
  block1 = allocate(); 
  block2 = allocate();                                                      // recieve spare blocks for extra delay outputs
  block3 = allocate(); 
  block4 = allocate(); 
                                                  
if(!delorstut)            ///////////////////////////// GLITCH DELAY ///////////////////////////////
{

// channel 0  WRITING BLOCK                                                 where the delay is taken and stored from 
  if (blockw) {
    blockw_pointer = blockw->data;

    use_sample = 1;
  
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)                           // iterate through audio block
    {
      if(use_sample) 
      {
        sample_delay_line[write_index++] = *blockw_pointer;                  // write sample to buffer
  
        if (write_index >= endtwo) write_index = startone;       // wrap round indexes
      }
      

      use_sample = 1 - use_sample;                                                         // flip bool bit... 
      
      blockw_pointer++;                                                      // ...and move the block_pointer on each time
    }
    
    release(blockw);

  }


// channel 1                                                     
  if(block1) {
    block_pointer1 = block1->data;

    use_sample = 1;

    for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
    {
            
      if(use_sample) 
      {
        *block_pointer1 = sample_delay_line[read_index1];                 // read sample out (no writing on channel 1)
  
        if (read_index1 >= endtwo) read_index1 = startone;
      }
      else  *block_pointer1 = sample_delay_line[read_index1++];               // repeat last read sample (actually the next here)

      
      use_sample = 1 - use_sample;                                     // flip bool bit... (octave up mode without this line)
      block_pointer1++; 
      
    }
    transmit(block1, 0);                                                    // transmit the block out on channel 0
    release(block1);
  }

//#ifdef kjglllllllh


// channel 2                                                      
  if(block2) {
    block_pointer2 = block2->data;

    use_sample = 1;

    for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
    {
            
      if(use_sample) 
      {
        *block_pointer2 = sample_delay_line[read_index2];                 // read sample out (no writing on channel 1)
  
        if (read_index2 == endtwo) read_index2 = startone;
      }
      else  *block_pointer2 = sample_delay_line[read_index2++];               // repeat last read sample (actually the next here)

      
      use_sample = 1 - use_sample;                                     // flip bool bit... (octave up mode without this line)
      block_pointer2++; 
      
    }
    transmit(block2, 1);                                                    // transmit the block out on channel 0
    release(block2);
  }


// channel 3                                                     
  if(block3) {
    block_pointer3 = block3->data;

    use_sample = 1;

    for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
    {
      if(use_sample) 
      {
        *block_pointer3 = sample_delay_line[read_index3];                 // read sample out (no writing on channel 1)
  
        if (read_index3 >= endtwo) read_index3 = startone;
      }
      else  *block_pointer3 = sample_delay_line[read_index3++];               // repeat last read sample (actually the next here)

      use_sample = 1 - use_sample;
      block_pointer3++;
      
    }
    transmit(block3,2);                                                    // transmit the block out on channel 0
    release(block3);
  }


// channel 4                                                    
  if(block4) {
    block_pointer4 = block4->data;

    use_sample = 1;

    for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
    {
            
      if(use_sample == 1) 
      {
        *block_pointer4 = sample_delay_line[read_index4];                 // read sample out (no writing on channel 1)
  
        if (read_index4 == endtwo) read_index4 = startone;
      }
      else  *block_pointer4 = sample_delay_line[read_index4++];               // repeat last read sample (actually the next here)

      use_sample = 1 - use_sample;
      block_pointer4++;
      
    }
    transmit(block4, 3);                                                    // transmit the block out on channel 0
    release(block4);
  }


}



              /////////////////////////////////////////////////////  STUTTER ////////////////////////////////////////////////////////

  else if(delorstut)
  {
    if(blockw)                        // RECORD 1
    {
                                                   
        blockw_pointer = blockw->data;

        use_sample = 1;
      
        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)                             // iterate through audio block
        {           
          if(use_sample && (write_index <= endone) && !rec) 
          {
            sample_delay_line[write_index++] = *blockw_pointer;                   // write sample to buffer   
          } 

          if(use_sample && (write_index2 <= endtwo) && !rec)              // record buffer 2  <--------------------------------------------------
          {
            sample_delay_line[write_index2++] = *blockw_pointer;                      
          } 
       
          use_sample = 1 - use_sample;                                            // flip bool bit... 
          
           blockw_pointer++;  
                                                     
        }
                                                        // transmit the block out on channel 0
      release(blockw);
      
    }

    
                            /////////////  PLAY 1
    if(block1) 
    {
      block_pointer1 = block1->data;

      use_sample = 1;

      for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
      {
              
        if(use_sample) 
        {
          *block_pointer1 = sample_delay_line[read_index1];                 // read sample out (no writing on channel 1)
        }
        else  *block_pointer1 = sample_delay_line[read_index1++];               // repeat last read sample (actually the next here)
  
        
        use_sample = 1 - use_sample;                                          // flip bool bit... (octave up mode without this line)
        block_pointer1++; 
        
      }
  
  
      if( read_index1 >= stutterlength )                                           // CYCLE COMPLETE
      {
        LED = 1;
        cyclecount++;
        
        if(cyclecount ==  lag)                // lag control
        {
          write_index2 = starttwo;
        }
        
        if(cyclecount >= cycles ) 
        {
          cyclecount = 0;
          write_index = startone;
        }     
        

        read_index1 = startone;
        read_index2 = starttwo;
        
      }
      else if(read_index1 >= 800)
        LED = 0;
  
      
      transmit(block1, 0);                                                    // transmit the block out on channel 0
      release(block1);
    }
    



          ////////////////////////////////////// SECOND STUTTER BUFFER //////////////////////////////////////////


    if(block2) 
    {
      block_pointer2 = block2->data;

      use_sample = 1;

      for (int j = 0; j < AUDIO_BLOCK_SAMPLES; j++)                           // iterate through audio block
      {
              
        if(use_sample) 
        {
          *block_pointer2 = sample_delay_line[read_index2];                 // read sample out (no writing on channel 1)
        }
        else  *block_pointer2 = sample_delay_line[read_index2++];               // repeat last read sample (actually the next here)
  
        
        use_sample = 1 - use_sample;                                     // flip bool bit... (octave up mode without this line)
        block_pointer2++; 
        
      }





      
      transmit(block2, 1);                                                    // transmit the block out on channel 0
      release(block2);

    }



  if(block3) release(block3);
  if(block4) release(block4);
  
    
  }





}


  
}
