/*********************************
*   Buntspecht translation tool  *
*   										*
*	 Author: Josef Schulz			*
*	 File	 :	EMdictionary.cpp		*
**********************************/

#include "buntspecht.h"

EMdictionary::EMdictionary()
{
	m_pfEMspModel = NULL;
	m_pfEMspCount = NULL;
	m_pfEMbiModel = NULL;
	m_pfEMleModel = NULL;
}

void EMdictionary::Init(dictionary* pDictionaryA, dictionary* pDictionaryB)
{
	setDictionarys(pDictionaryA, pDictionaryB);

	m_iNumEMspModel = pDictionaryA->getNumWords()*pDictionaryB->getNumWords();
}

void EMdictionary::Exit()
{
	clearEMbiModel();
	clearEMspModel();
	clearEMleModel();
}

void EMdictionary::clearEMleModel()
{
	if(m_pfEMleModel) {
		delete m_pfEMleModel;
		m_pfEMleModel = 0;
	}
}

void EMdictionary::clearEMbiModel()
{
	if(m_pfEMbiModel) {
		delete m_pfEMbiModel;
		m_pfEMbiModel = 0;
	}
}
void EMdictionary::clearEMspModel()
{
	if(m_pfEMspModel) {
		delete m_pfEMspModel;
		m_pfEMspModel = 0;
	}

	if(m_pfEMspCount) {
		delete m_pfEMspCount;
		m_pfEMspCount = 0;
	}
}

void EMdictionary::createEMspModel(int iSizea, int iSizeb)
{
	// if memory exist create
	clearEMspModel();

	if((iSizea > 0) && (iSizeb > 0)) {
		iSizea *= iSizeb;

		m_pfEMspModel = new float[iSizea];
		m_pfEMspCount = new float[iSizea];
	}
	else {
		m_pfEMspModel = new float[m_iNumEMspModel];
		m_pfEMspCount = new float[m_iNumEMspModel];
	}
}

void EMdictionary::createEMbiModel(int iSizea, int iSizeb)
{
	// if memory exist create
	clearEMbiModel();

	iSizea *= iSizeb;
	m_pfEMbiModel = new float[iSizea];
}

void EMdictionary::createEMleModel(int iSizea, int iSizeb)
{
	// if memory exist create
	clearEMleModel();

	iSizea *= iSizeb;
	m_pfEMleModel = new float[iSizea];
}

void EMdictionary::setDictionarys(dictionary* pDictionaryA, dictionary* pDictionaryB)
{
	m_pDictA = pDictionaryA;
	m_pDictB = pDictionaryB;
}

void EMdictionary::setEMspModel(int f, int e, float* pIn)
{
	m_pfEMspModel[e*m_pDictA->getNumWords()+f] = *pIn;
}

float EMdictionary::getEMspModel(int f, int e, float* pOut)
{
	if(pOut) pOut = &m_pfEMspModel[e*m_pDictA->getNumWords()+f];
	return m_pfEMspModel[e*m_pDictA->getNumWords()+f];
}

void EMdictionary::calcEMspModel(int iSteps, float fTime, bool bOld)
{
	// if the memory exist destroy
	clearEMspModel();
	// Initialize
	m_pfEMspModel = new float[m_iNumEMspModel];
	m_pfEMspCount = new float[m_iNumEMspModel];
	clock_t 	start, ende, time1, time2;
	float		fSum = 0.0f;
	float    fInvSum;
	float		fInit = 1.0f/(float)m_iNumEMspModel;
	float 	fzSum[m_pDictB->getNumWords()];
	int		iTemp;
	int		x, y, n, s, j, i, u, v, f;
	int		iDictSentencesSizeA = m_pDictA->getSentences().size();
	int		iDictSentencesSizeB = m_pDictB->getSentences().size();
	int 		iDictNumWordsA = m_pDictA->getNumWords();
	int 		iDictNumWordsB = m_pDictB->getNumWords();

	//initThreads(m_pfEMspModel, m_pfEMspCount);

	for(int i = 0; i < m_iNumEMspModel; i++) {
		m_pfEMspModel[i] = fInit;
		m_pfEMspCount[i] = 0.0f;
	}

	if(fTime > 0.0f) iSteps = 3;

	std::cout<<std::endl;

	start = clock();
	// EMspModel steps


	for(n = 0; n < iSteps; n++) {
		time1  = clock();
		std::cout<<"Step ["<<n+1<<"] ... ";
		for(i = 0; i < m_iNumEMspModel; ++i)
			m_pfEMspCount[i] = 0.0f;
		// E-Step

		//runThreads();
		for(s = 0; s < iDictSentencesSizeA; ++s) {

				for(j = 0; j < m_pDictA->m_piSentenceLengths[s]; ++j) {
					fSum = 0.0f;

					for(i = 0; i < m_pDictB->m_piSentenceLengths[s]; ++i) {
						fSum += m_pfEMspModel[m_pDictB->m_ppiSentences[s][i]*iDictNumWordsA+m_pDictA->m_ppiSentences[s][j]];
					}

					//fInvSum = 1.0f/fSum;

					for(i = 0; i < m_pDictB->m_piSentenceLengths[s]; ++i) {
						iTemp = m_pDictB->m_ppiSentences[s][i]*iDictNumWordsA+m_pDictA->m_ppiSentences[s][j];

						m_pfEMspCount[iTemp] += m_pfEMspModel[iTemp]/fSum;
					}
				}

		}


		// M-Step
		for(u = 0; u < iDictNumWordsB; ++u) {
			fzSum[u] = 0.0f;

			for(f = 0; f < iDictNumWordsA; ++f)
				fzSum[u] += m_pfEMspCount[u*iDictNumWordsA+f];

		}

		for(v = 0; v < iDictNumWordsA; ++v) {
			for(u = 0; u < iDictNumWordsB; ++u) {
				iTemp = u*iDictNumWordsA+v;
				m_pfEMspModel[iTemp] = m_pfEMspCount[iTemp]/fzSum[u];
			}
		}


		time2  = clock();
		fInit = ((float)(time2 - time1)/CLOCKS_PER_SEC);
		std::cout <<"time used : "<< fInit <<"\n";

		if(fTime > 0.0f) {
				if(fTime < ((float)(time2 - start)/CLOCKS_PER_SEC)) {
					return;
				}

				iSteps++;
		}
	}

	//exitThreads();

	ende = clock();
	std::cout <<"time used : "<< ((float)(ende - start)/CLOCKS_PER_SEC) << endl;
}

void EMdictionary::addEMbiModel(int iW1, int iW2)
{
	m_pfEMbiModel[iW1*(m_pDictB->getNumWords()+1)+iW2] += 1.0f;
}

void EMdictionary::setEMbiModel(int iW1, int iW2, float* PIn)
{
	m_pfEMbiModel[iW1*(m_pDictB->getNumWords()+1)+iW2] = *PIn;
}

float EMdictionary::getEMbiModel(int iW1, int iW2, float* pOut)
{
	if(pOut) pOut = &m_pfEMbiModel[iW1*(m_pDictB->getNumWords()+1)+iW2];
	return m_pfEMbiModel[iW1*(m_pDictB->getNumWords()+1)+iW2];
}

void EMdictionary::calcEMbiModel()
{
	// if memory exist clear
	clearEMbiModel();

	int iRange  = (m_pDictB->getNumWords()+1)*(m_pDictB->getNumWords()+1);
	int iMarker = m_pDictB->getNumWords();
	m_pfEMbiModel = new float[iRange];
	clock_t 	start, ende;
	float		fNo[iMarker+1];
	int		iNull = 0;
	int		iTemp;

	m_iNumEMbiModel = iRange;

	start = clock();
	for(int i = 0; i < iRange; i++)
		m_pfEMbiModel[i] = 0.0f;

	for(int i = 0; i < m_pDictB->getSentences().size(); i++) {
		addEMbiModel(iMarker, *m_pDictB->getWordOfSentence(&i, &iNull));

		for(int n = 0; n < *m_pDictB->getSentenceSize(&i)-1; n++) {
			iTemp = n+1;
			addEMbiModel(*m_pDictB->getWordOfSentence(&i, &n),
							 *m_pDictB->getWordOfSentence(&i, &iTemp));
		}
			iTemp = *m_pDictB->getSentenceSize(&i)-1;
		addEMbiModel(*m_pDictB->getWordOfSentence(&i, &iTemp), iMarker);
	}

	// normalize
	for(int x = 0; x <= iMarker; x++) {
		fNo[x] = 0.0f;

		for(int y = 0; y <= iMarker; y++) {
			fNo[x] += getEMbiModel(x, y) + 1.0f;
		}
	}

	for(int x = 0; x <= iMarker; x++) {
		for(int y = 0; y <= iMarker; y++) {
			if(fNo[x] != 0.0f)
				m_pfEMbiModel[x*(m_pDictB->getNumWords()+1)+y] += 1.0f;
				m_pfEMbiModel[x*(m_pDictB->getNumWords()+1)+y] /= fNo[x];
		}
	}
	ende = clock();
	std::cout <<"time used : "<< ((float)(ende - start)/CLOCKS_PER_SEC) << endl;
}

void EMdictionary::addEMleModel(int l, int m)
{
	m_pfEMleModel[l*m_iMaxSentenceSize+m] += 1.0f;
}

void EMdictionary::setEMleModel(int l, int m, float* pIn)
{
	m_pfEMleModel[l*m_iMaxSentenceSize+m] = *pIn;
}

float EMdictionary::getEMleModel(int l, int m, float* pOut)
{
	if(pOut) pOut = &m_pfEMleModel[l*m_iMaxSentenceSize+m];
	return m_pfEMleModel[l*m_iMaxSentenceSize+m];
}

void EMdictionary::calcEMleModel()
{
	// if memory exist clear
	clearEMleModel();

	int iMax = 0;
	for(int i = 0; i < m_pDictA->getSentences().size(); i++) {
		if(iMax < *m_pDictA->getSentenceSize(&i)) {iMax = *m_pDictA->getSentenceSize(&i);}
		if(iMax < *m_pDictB->getSentenceSize(&i)) {iMax = *m_pDictB->getSentenceSize(&i);}
	}

	m_iMaxSentenceSize = iMax;
	m_iNumEMleModel 	 = iMax*iMax;
	m_pfEMleModel   	 = new float[m_iNumEMleModel];
	clock_t	start, ende;
	float		  fNo[iMax];

	start = clock();
	for(int i = 0; i < m_iNumEMleModel; i++)
		m_pfEMleModel[i] = 0.0f;

	for(int i = 0; i < m_pDictA->getSentences().size(); i++) {
			addEMleModel(*m_pDictB->getSentenceSize(&i)-1, *m_pDictA->getSentenceSize(&i)-1);
	}

	// normalize
	for(int l = 0; l < iMax; l++) {
		fNo[l] = 0.0f;

		for(int m = 0; m < iMax; m++) {
			fNo[l] += m_pfEMleModel[l*iMax+m] + 1.0f;
		}
	}

	for(int l = 0; l < iMax; l++) {
		for(int m = 0; m < iMax; m++) {
			if(fNo[l] != 0.0f)
				m_pfEMleModel[l*iMax+m] += 1.0f;
				m_pfEMleModel[l*iMax+m] /= fNo[l];
		}
	}

	ende = clock();
	std::cout <<"time used : "<< ((float)(ende - start)/CLOCKS_PER_SEC) << endl;
}

std::list<std::pair<int, float> > EMdictionary::getEMspBestnWords(int iWord, int n)
{
	std::list<std::pair<int, float> > liste;
	std::pair<int, float> 			   word;

	for(int i = 0; i < m_pDictA->getNumWords(); i++) {
		word = make_pair (i, getEMspModel(i, iWord));
		liste.push_back(word);
	}

	if(n >= m_pDictA->getNumWords()) {
		n = m_pDictA->getNumWords()-1;
	}

	liste.sort(compareFirst());
	for(int i = 0; i < (m_pDictA->getNumWords()-n); i++)
		liste.pop_back();

	return liste;
}

