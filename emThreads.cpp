/*********************************
*   Buntspecht translation tool  *
*   					 		 *
*	 Author: Josef Schulz		 *
*	 File	 :	emThreads.cpp	 *
**********************************/

#include "buntspecht.h"

dictionary* g_pDictA;
dictionary* g_pDictB;
float*		g_pfSum;
float		   g_fSum;
int			g_iDimA;
int			g_iDimB;
int			g_iLoop;
int			g_iLoob;
int			g_iNumSe;
pthread_t*  g_pThreadEM;
pthread_mutex_t gMutex;
float*		g_pfModel;
float*		g_pfCount;

void initThreads(float* pfModel, float* pfCount)
{
	g_iDimA = g_pDictA->getNumWords();
	g_iDimB = g_pDictB->getNumWords();

	g_iNumSe  = g_pDictA->getSentences().size();

	g_pfSum = new float[g_iDimB];

	g_pThreadEM = new pthread_t[g_iNumSe];

	g_pfModel = pfModel;
	g_pfCount = pfCount;

	pthread_mutex_init( &gMutex, NULL );
}

void runThreads()
{
 /*
	for(g_iLoop = 0; g_iLoop < g_iNumSe; g_iLoop++) {
		pthread_create( &g_pThreadEM[g_iLoop], NULL, &calcSpEThread, NULL);
	}

	for(g_iLoob = 0; g_iLoob < g_iNumSe; g_iLoob++) {
		pthread_join( g_pThreadEM[g_iLoop], NULL);
	}*/
}

void* calcSpEThread(void* ptr)
{/*
	int s = (int)ptr;
	float fSum;
	int i, iTemp, x, y;

	for(int j = 0; j < g_pDictA->m_piSentenceLengths[s]; j++) {
					fSum = 0.0f;

					for(i = 0; i < g_pDictB->m_piSentenceLengths[s]; i++) {
						x = g_pDictA->m_ppiSentences[s][j];
						y = g_pDictB->m_ppiSentences[s][i];

						fSum += g_pfModel[y*g_iDimA+x];
					}

					for(i = 0; i < g_pDictB->m_piSentenceLengths[s]; i++) {
						x = g_pDictA->m_ppiSentences[s][j];
						y = g_pDictB->m_ppiSentences[s][i];
						iTemp = y*g_iDimA+x;

						pthread_mutex_lock( &gMutex );
							g_pfCount[iTemp] += g_pfModel[iTemp]/fSum;
						pthread_mutex_unlock( &gMutex );

					}
	}*/
}



void exitThreads()
{
	delete[] g_pThreadEM;
	delete[] g_pfSum;
}
