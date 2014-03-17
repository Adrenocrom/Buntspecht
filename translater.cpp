/*********************************
*   Buntspecht translation tool  *
*   							 *
*	 Author: Josef Schulz		 *
*	 File	 :	translater.cpp	 *
**********************************/

#include "buntspecht.h"

float g_fAlpha1;
float g_fAlpha2;
float g_fAlpha3;
float g_fAlpha4;
int   g_iStackSize;
int   g_iTranslateSteps;
bool  g_bShowStack;

translater::translater(dictionary* pDictA, dictionary*pDictB, EMdictionary* pEMDict)
{
    m_pDictA = pDictA;
    m_pDictB = pDictB;
    m_pEMDict = pEMDict;

    m_pvTranslations = NULL;
    m_pvTranslations = new vector< pair<float, vector<int> > >();

    g_fAlpha1 = 1.0f; // Bigram
    g_fAlpha2 = 1.0f; // Align
    g_fAlpha3 = 1.0f; // Length
    g_fAlpha4 = 1.5f; // Trans
    g_iStackSize = 1000;
    g_iTranslateSteps = 100;
    g_bShowStack = true;
}

translater::~translater()
{
    if(m_pvTranslations) {
        delete m_pvTranslations;
        m_pvTranslations = NULL;
    }
}

float translater::calcTranslation(vector<int> vE, vector<int> vF)
{
     float fResult = 0.0f, fTemp;
     float fSpeachmodel = 0.0f;
     float fAlignment   = 0.0f;
     float fLengthmodel = 0.0f;
     float fTransmodel  = 0.0f;
     int iMarker = m_pDictB->getNumWords();
     int iLengthE = vE.size();
     int iLengthF = vF.size();
     int W1, W2, i, e, f;

     // ---------------------------------------------------
     // Sprachmodel
     if(iLengthE <= 1) {
     }
     else {
         // Bigramm
         for( i = 0; i < iLengthE-1; i++) {
             W1 = i;
             W2 = i+1;

             fSpeachmodel += log(m_pEMDict->getEMbiModel(W1, W2));
         }
     }

     // ---------------------------------------------------
     // alignment
     fAlignment += log(1.0f/(pow((float)iLengthE, (float)iLengthF)));

     // ---------------------------------------------------
     // Laengenmodel
     if(vE[iLengthE-1] == iMarker) fLengthmodel += log(m_pEMDict->getEMleModel(iLengthE-2, iLengthF));
     else fLengthmodel += log(m_pEMDict->getEMleModel(iLengthE-1, iLengthF));

     // ---------------------------------------------------
     // translationmodel

     for(f = 0; f < iLengthF; f++) {
         fTemp = 0.0f;

        for(e = 1; e < iLengthE; e++) {
             if(vE[e] == iMarker) {}
             else fTemp += m_pEMDict->getEMspModel(vE[e], vF[f]);
        }

        fTransmodel += log(fTemp);
     }
     fResult = g_fAlpha1*fSpeachmodel +
               g_fAlpha2*fAlignment   +
               g_fAlpha3*fLengthmodel +
               g_fAlpha4*fTransmodel;

/*
    fResult = g_fAlpha1*fSpeachmodel +
              g_fAlpha2*fAlignment   +
              g_fAlpha3*fLengthmodel +
              g_fAlpha4*fTransmodel;*/

    if(g_bShowStack) {
        for(int a = 0; a < vE.size(); a++) {
            printf("%s ", m_pDictA->getWord(vE[a]).c_str());
        }
        printf("%.3f\n", fResult);
    }

    return fResult;
}

/***********************************************************

Die Übersetztung begint mit dem setzen der Startmarke
und endet auch mit ihr:       # W_1 W_2 W_3 ... W_N #


***********************************************************/
vector<int> translater::searchBestScentences(vector<int> vSentence)
{
    int iMarker = m_pDictB->getNumWords();
    int iLength = vSentence.size();
    int iLastWord = 0;
    int iTemp;
    int iCount = 0;
    float fW;
    list<pair<vector<int>, float> > hypotheses;
    list<pair<vector<int>, float> >::iterator Iterartor;
    vector<pair<int, float> >  vWordSet;
    pair<vector<int>, float> hypothese;
    vector<int> vTemp;



    // Die Wortmenge Initialisieren
    for(int i = 0; i < vSentence.size(); i++) {
        iTemp = vSentence[i];

        list<pair<int, float> >::iterator it;
        list<pair<int, float> > li = m_pEMDict->getEMspBestnWords(iTemp, 3);

        for(it = li.begin(); it != li.end(); it++) {
			pair<int, float> word = *it;
			word.second = log(word.second);
			vWordSet.push_back(word);
        }

        vWordSet.push_back(make_pair(iMarker, 1.0));
    }


    // Initiale beste Hypothese setzten
    vTemp.push_back(iMarker);
    hypothese = make_pair(vTemp, 1.0f);

    do {
       for(int i = 0; i < vWordSet.size(); i++) {
           // Die Menge der Hypothesen wird nun erweitert,
           // indem die Acktuelle Hypothese um jedes Wort erweitert wird
           vTemp = hypothese.first;
           iTemp = vWordSet[i].first;
           vTemp.push_back(iTemp);

           hypotheses.push_back(make_pair(vTemp, calcTranslation(vTemp, vSentence)));

           // Hypothesen der Wahrscheinlickeit nach Sortieren
           hypotheses.sort(compareSecond());

           while(hypotheses.size() >= g_iStackSize) hypotheses.pop_back();

       }



       Iterartor = hypotheses.begin();
       hypothese = *Iterartor;

       iCount++;
/*
       //hypotheses.compareThird();
       for(Iterartor = hypotheses.begin(); Iterartor!=hypotheses.end(); Iterartor++) {
            hypothese = *Iterartor;

            for(int a = 0; a < hypothese.first.size(); a++) {
                 printf("%s ", m_pDictA->getWord(hypothese.first[a]).c_str());
            }

            printf("%.3f\n", hypothese.second);
       }*/

       // Und das Letzte Element aushängen
       vTemp = hypothese.first;
       iLastWord = vTemp[vTemp.size()-1];
       hypotheses.pop_front();
    } while((iLastWord != iMarker) && (iCount < g_iTranslateSteps));

    return vTemp;
}
