/*
  File:         EWMovAverageSketch.ino
  Version:      0.0.1
  Date:         19-Dec-2018
  Revision:     18-Feb-2019
  Author:       Jerome Drouin
  
  https://github.com/newEndeavour/EWMovAverage

  Copyright (c) 2018-2019 Jerome Drouin  All rights reserved.  

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <EWMovAverage.h>
#include <math.h>

#define DISP_MONITOR                 1
//#define DISP_PLOTTER_MEAN            1
//#define DISP_PLOTTER_SDEV           1

static long double const   pi  =  3.14159265358979323846264338L;

//---- General Variables ----------------------------------------------------
float Amplitude;
float MaxAmpl = 10;
float CentralPoint = 100;
float Angle;
float lambda = 0.90;
float RawSignal;

int Switch = 1;
int LoopCount = 0;

//---- EWMovAverage objects ----------------------------------------------------
#define EWMA1_MA1_DECAY_SPEED          0.900     // Fast Average
EWMovAverage EWMA1(EWMA1_MA1_DECAY_SPEED);
float Mean_1;
float Stdev_1;

#define EWMA2_MA1_DECAY_SPEED          0.985     // Slow Average
EWMovAverage EWMA2(EWMA2_MA1_DECAY_SPEED);
float Mean_2;
float Stdev_2;

//---- Setup ----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.print("\n---- Serial Started ----\n");

}

//---- Loop ----------------------------------------------------------------
void loop() {

  LoopCount++;

  //Generate a variable mimicking high Resonance
  Angle     = (float)(LoopCount/360.0)*pi;
  Switch    *=-1;
  Amplitude = abs(sin(Angle)*random(0,10));    
  RawSignal = CentralPoint*(1 + 0.05*cos((float)(LoopCount/120.0)*pi)) + Amplitude*Switch;
  
  //Call EWMovAverage
  EWMA1.Update(RawSignal);
  EWMA2.Update(RawSignal);

  Mean_1  = EWMA1.Get_Mean();
  Mean_2  = EWMA2.Get_Mean();
  Stdev_1 = EWMA1.Get_StdDeviation();
  Stdev_2 = EWMA2.Get_StdDeviation();

  #if defined(DISP_MONITOR)

    //We Copy EWMA2 into EWMA1 at 200 cycles
    if (LoopCount == 100) {
      Serial.print("\nBEFORE COPY");
      Serial.print("\nEWMA1:");
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_xi(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_xi2(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_Mean(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_Variance(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_StdDeviation(),4);
      Serial.print("\nEWMA2:");
      Serial.print("\nxi    :");
      Serial.print(EWMA2.Get_xi(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA2.Get_xi2(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA2.Get_Mean(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA2.Get_Variance(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA2.Get_StdDeviation(),4);
      
      EWMA1.Copy(EWMA2);
      
      Serial.print("\nAFTER COPY");
      Serial.print("\nEWMA1:");
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_xi(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_xi2(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_Mean(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_Variance(),4);
      Serial.print("\nxi    :");
      Serial.print(EWMA1.Get_StdDeviation(),4);
      delay(3000);
    }

    //-> Serial
    Serial.print("\n");
    Serial.print(LoopCount);
    Serial.print(")");
        
    Serial.print("\tRaw:");
    Serial.print(RawSignal,4);
    
    Serial.print("\tMean_1:");
    Serial.print(Mean_1,4);
        
    Serial.print("\tStdev_1:");
    Serial.print(Stdev_1,4);

    Serial.print("\tMean_2:");
    Serial.print(Mean_2,4);

    Serial.print("\tStdev_2:");
    Serial.print(Stdev_2,4);

    delay(100);
  #endif

  #if defined(DISP_PLOTTER_MEAN)
    if (LoopCount == 500) 
      EWMA1.Copy(EWMA2);

    //RAW
    Serial.print(RawSignal,4);
    Serial.print(" ");

    //EWMA1
    Serial.print(Mean_1,4);
    Serial.print(" ");
    
    //EWMA2
    Serial.print(Mean_2,4);
    Serial.print(" ");

    //Plotter Boundaries
    Serial.print(CentralPoint*1.2,4);
    Serial.print(" ");
    Serial.print(CentralPoint/1.2,4);
    Serial.print("\n");
    delay(10);
  #endif

  #if defined(DISP_PLOTTER_SDEV)
    //EWMA1
    Serial.print(Stdev_1,4);
    Serial.print(" ");
    
    //EWMA2
    Serial.print(Stdev_2,4);
    Serial.print(" ");

    Serial.print("\n");
    delay(10);
  #endif

}
