/*********************************
*   Buntspecht translation tool  *
*   							 *
*	 Author: Josef Schulz		 *
*	 File	 :	buntspecht.h	 *
**********************************/

#ifndef _BUNTSPECHT_H_
#define _BUNTSPECHT_H_

// Standart include
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <list>
#include <map>
#include <pthread.h>
#include <math.h>

using namespace std;

class EMdictionary;

// defines
#define NUM_THREADS 1

// Prototyps
void InitBuntspecht();
void ExitBuntspecht();

struct sentence
{
	std::vector<int> vWords;
};

struct compareFirst
{
   bool operator() (const std::pair<int, float>& l, const std::pair<int, float>& r) const
	{
   	return l.second > r.second;
   }
};

struct compareSecond
{
   bool operator() (const std::pair<vector<int>, float>& l, const std::pair<vector<int>, float>& r) const
	{
   	return l.second > r.second;
   }
};

struct compareThird
{
   bool operator() (const std::pair<vector<int>, float>& l, const std::pair<vector<int>, float>& r) const
	{
   	return l.second < r.second;
   }
};

// Program includes
#include "dictionary.h"
#include "fileReader.h"
#include "EMdictionary.h"
#include "emThreads.h"
#include "translater.h"

#endif
