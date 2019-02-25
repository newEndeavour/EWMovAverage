/*
  File:         EWMovAverage.cpp
  Version:      0.0.1
  Date:         19-Dec-2018
  Revision:     18-Feb-2019
  Author:       Jerome Drouin (jerome.p.drouin@gmail.com)

  Editions:	Please go to EWMovAverage.h for Edition Notes.
 
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

*/


#include "Arduino.h"
#include "EWMovAverage.h"


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
EWMovAverage::EWMovAverage(float _decay)
{

	//Auto Calibration of Baseline
	Autocal_Millis 		= INITIAL_AUTOCAL_FREQ;			

	//Set initial values	
	updateID		= 0;
	decay			= _decay;			// Time decay factor for moments

	raw_xi			= 0;
	raw_xi2			= 0;
	raw_mean		= raw_xi;
	raw_var			= raw_xi2 - (raw_xi * raw_xi);
	raw_stdev		= sqrt(raw_var);

	lastCal 		= millis();         		// set millis for start

	// Object parameter's error handling
	ResetErrors();
}


// Public Methods //////////////////////////////////////////////////////////////
// Returns the Version
String EWMovAverage::GetVersion()
{
	return VER_EWMovAverage;
}


// Returns the Release Date
String EWMovAverage::GetReleaseDate()
{
	return REL_EWMovAverage;
}


//Set the moment decay
void EWMovAverage::Setdecay(float _decay)
{

	//Set initial values	
	decay			= _decay;	// Moments decay factor

	// Object parameter's error handling
	ResetErrors();
}


//Get the raw_xi level
float EWMovAverage::Get_xi(void)
{
	return raw_xi;	
}


//Get the raw_xi2 level
float EWMovAverage::Get_xi2(void)
{
	return raw_xi2;	
}


//Get the raw_mean level
float EWMovAverage::Get_Mean(void)
{
	return raw_mean;	
}


//Get the raw_var level
float EWMovAverage::Get_Variance(void)
{
	return raw_var;	
}


//Get the raw_stdev level
float EWMovAverage::Get_StdDeviation(void)
{
	return raw_stdev;	
}


//Get the decay factor
float EWMovAverage::Getdecay(void)
{
	return decay;	
}


//Get the GetupdateID variable
long EWMovAverage::GetupdateID(void)
{
	return updateID;	
}


//Disable Auto Calibration
void EWMovAverage::Disable_AutoCal(void)
{
	Autocal_Millis = 0x0FFFFFFFL;
}

//Reset Auto Calibration
void EWMovAverage::Reset_AutoCal(void)
{
	Autocal_Millis = INITIAL_AUTOCAL_FREQ;
}


//Set the Autocal frequency parameter
void EWMovAverage::Set_Autocal_Millis(unsigned long _autocal_Millis)
{
	Autocal_Millis = _autocal_Millis;

	// Object parameter's error handling
	ResetErrors();
}


//Get the Autocal frequency parameter
unsigned long EWMovAverage::Get_Autocal_Millis(void) 
{
	return Autocal_Millis;
}


// Update levels 
int EWMovAverage::Update(float rawSignal)
{

	if (error < 0) return error;            // bad constructor variables

	//Increment updateID
	updateID++;

	//calculate
	return Update_Moments(rawSignal);
	
}


// Copy variables 
void EWMovAverage::Copy(EWMovAverage &source)
{

	//Copy values from source	
	updateID		= source.GetupdateID();
	decay			= source.Getdecay();		

	raw_xi			= source.Get_xi();
	raw_xi2			= source.Get_xi2();
	raw_mean		= source.Get_Mean();
	raw_var			= source.Get_Variance();
	raw_stdev		= source.Get_StdDeviation();
	
}


// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

// Update All Moments
int EWMovAverage::Update_Moments(float rawSignal)
{
	//raw_mean = MA(1) with decay factor

	raw_xi		= decay * raw_xi + (1 - decay) * rawSignal;
	raw_xi2		= decay * raw_xi2 + (1 - decay) * (rawSignal*rawSignal);
	
	raw_mean	= raw_xi;			// mean
	raw_var		= raw_xi2 - (raw_xi * raw_xi);	// variance
	raw_stdev	= sqrt(raw_var);		// std deviation
	
	return 1;

}



//Reset error flag following importance hierarchy
//by increasing error importance:
//	- decay		: -1
//	
void EWMovAverage::ResetErrors(void) 
{

	// Object parameter's error handling
	error = 1;

	if (decay<=MINdecayPARAM) 	error =-1;	// incorrect decay variable
	if (decay>=MAXdecayPARAM) 	error =-1;	// incorrect decay variable
	

}



// /////////////////////////////////////////////////////////////////////////////

