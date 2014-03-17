/*********************************
*   Buntspecht translation tool  *
*   										*
*	 Author: Josef Schulz			*
*	 File	 :	emThreads.h				*
**********************************/

#ifndef __EMTHREADS_H_
#define __EMTHREADS_H_

#pragma once

extern dictionary* g_pDictA;
extern dictionary* g_pDictB;
extern float*		 g_pfSum;
extern float		 g_fSum;
extern int			 g_iDimA;
extern int			 g_iDimB;
extern int			 g_iLoop;
extern int			 g_iLoob;
extern int			 g_iNumSe;
extern pthread_t*  g_pThreadEM;
extern pthread_mutex_t gMutex;
extern float*		 g_pfModel;
extern float*		 g_pfCount;

void initThreads(float* pfModel, float* pfCount);
void runThreads();
void exitThreads();

void* calcSpEThread(void* ptr);

#endif
