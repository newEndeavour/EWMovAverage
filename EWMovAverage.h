/*
  File:         EWMovAverage.h
  Version:      0.0.1
  Date:         19-Dec-2018
  Revision:     18-Feb-2019
  Author:       Jerome Drouin (jerome.p.drouin@gmail.com)

  EWMovAverage.h - Library for 'duino
  https://github.com/newEndeavour/EWMovAverage

  The EWMovAverage object implements an Exponentially Weighted Moving Average.


  Copyright (c) 2019 Jerome Drouin  All rights reserved.

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

  Editions:
  - 0.0.1	: First version

*/


// ensure this library description is only included once
#ifndef EWMovAverage_h
#define EWMovAverage_h

#include "Arduino.h"

// DEFINES /////////////////////////////////////////////////////////////
#define VER_EWMovAverage		"0.0.1"		//
#define REL_EWMovAverage		"18Feb2019"	//


#define INITIAL_AUTOCAL_FREQ	  	1000	// Autocalibration Frequency 5 secs
#define MINdecayPARAM 			0	// 
#define MAXdecayPARAM 	      		1	// 


// library interface description
class EWMovAverage
{
  // user-accessible "public" interface
  public:
  // methods
	EWMovAverage(float _decay);

	int 	Update(float rawSignal);			//Update and return the object status
	void 	Copy(EWMovAverage &source);			//Copy from Source 

	void 	Setdecay(float _decay);				//Set decay level

	float 	Getdecay(void);					//Get the decay factor
	long 	GetupdateID(void);				//Get the decay factor

	float 	Get_xi(void);					//Get the xi level
	float 	Get_xi2(void);					//Get the xi2 level
	float 	Get_Mean(void);					//Get the mean level
	float 	Get_Variance(void);				//Get the var level
	float 	Get_StdDeviation(void);				//Get the stdev level

	void 	Reset_AutoCal();				// Reset Autocal_Millis
	void 	Disable_AutoCal();				// Stops Baseline update. To Return to baseline update use 
								// Set_Autocal_Millis(x) where x>0

	void 	Set_Autocal_Millis(unsigned long autoCal_millis);
	unsigned long Get_Autocal_Millis(void);

	String 	GetVersion();
	String 	GetReleaseDate();


  // library-accessible "private" interface
  private:
  // variables

	int 		error;					//
	long 		updateID;				// update ID tag

	unsigned long  	Autocal_Millis;
	unsigned long  	lastCal;

	float		decay;					// 
	
	float		raw_xi; 
	float		raw_xi2; 
	float		raw_mean; 
	float		raw_var; 
	float		raw_stdev; 

  // methods
	int 	Update_Moments(float rawSignal);
	void 	ResetErrors(void);	// Error flag handling

};

#endif
