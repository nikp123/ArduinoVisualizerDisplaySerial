/**
 * This is a potential Spectrum Audio visualizer that works by reading serial data.
 * This code just displays it. Like an Arduino slave :P 
 * The idea is that the FFT calculated line heights are transfered through serial (USB or even better BLUETOOTH!! :D)
 * The processing is done on the host machine. And I thought if software like cava fxp. can be modified to work with this.
 * 
 * Enjoy your geek party :)
 */

#include <TVout.h>
#include <fontALL.h>

// Define how wide the lines in your visualizer you want to have
#define linewidth 1

// TV output variable
TVout TV;
// X and Y pointer coords
unsigned char x = 0, y = 0;
// Current line buffer
unsigned char line[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
// Last line buffer
unsigned char lineLast[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup()  {
  // Open serial for updating bar heights
  Serial.begin(9600);
  
  // Launch TVout in NTSC mode
  TV.begin(NTSC);
}

void loop() {
  // Updates the bar heights
  checkUpdates();

  // Loops through bars
  for(char i = 0; i < 32; i++)
  {
    /**
     *  Optimization tricks for Arduino ;)
     *  
     *  You have to do this because the Arduino is just an 7 MHz Microcontroller
     *  And we all know 7 MHz is NOT a lot of processing power. So be careful
     */
    if(line[i] > lineLast[i])
    {
      // If the updated line is larger than the last line
      for(x=(i*4);x<((i*4)+linewidth);x++){
        for(y=8+(88-line);y<8+(88-lineLast);y++){
          TV.set_pixel(x,y,1);
        }
      }
    }
    else if(line[i] < lineLast[i])
    {
      // If the updated line is smaller than the last line
      for(x=(i*4);x<((i*4)+linewidth);x++){
        for(y=8+(88-lineLast[i]);y<8+(88-lineLast[i]);y++){
          TV.set_pixel(x,y,0);
        }
      }
    }
  }
  
  // After the lines are drawn update the old buffer
  for(x = 0; x < 32; x++)
    lineLast[i] = line[i];
  
  // Give it some time to refresh
  TVout.delay(10);
}

void checkUpdates()
{
  // Checks if anything new is in the serial
  if(Serial.available())
  {
    // If there is reply and send message so that the machine knows what it's going to do now
    Serial.write(45);
    for(x=0;x<32;x++){
      // Wait for data
      while(!Serial.available());
      
      // Write it in
      line[x] = Serial.read();
    }
  }
}

