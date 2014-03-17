/*********************************
*   Buntspecht translation tool  *
*   							 *
*	 Author: Josef Schulz		 *
*	 File	 :	translater.h	 *
**********************************/

#ifndef _TRANSLATER_H_
#define _TRANSLATER_H_

#pragma once

extern float g_fAlpha1;
extern float g_fAlpha2;
extern float g_fAlpha3;
extern float g_fAlpha4;
extern int   g_iStackSize;
extern int   g_iTranslateSteps;
extern bool  g_bShowStack;

class translater
{
public:
    dictionary*  m_pDictA;
	dictionary*  m_pDictB;
	EMdictionary* m_pEMDict;

    translater(dictionary* pDictA, dictionary*pDictB, EMdictionary* pEMDict);
    ~translater();

    vector< pair<float, vector<int> > >* m_pvTranslations;

    vector<int> searchBestScentences(vector<int> vSentence);
    float calcTranslation(vector<int> vE, vector<int> vF);
};

#endif
