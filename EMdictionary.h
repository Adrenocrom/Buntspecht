/*********************************
*   Buntspecht translation tool  *
*   							 *
*	 Author: Josef Schulz		 *
*	 File	 :	EMdictionary.h	 *
**********************************/

#ifndef _EMDICTIONARY_H_
#define _EMDICTIONARY_H_

#pragma once

struct SEMLenght
{
	int 	iW1;
	int 	iW2;
	float fChance;
	int	iCount;
};

class EMdictionary
{
private:
	dictionary* 	m_pDictA;
	dictionary* 	m_pDictB;

	float*			m_pfEMspModel;
	float*			m_pfEMspCount;
	float*			m_pfEMbiModel;
	float*			m_pfEMleModel;
	int 			m_iNumEMspModel;
	int				m_iNumEMbiModel;
	int 			m_iNumEMleModel;
	int				m_iMaxSentenceSize;
public:
	// Konstruktor und Destruktor
	EMdictionary();
	~EMdictionary() {Exit();}

	void Init(dictionary* pDictionaryA, dictionary* pDictionaryB);
	void Exit();

	void setDictionarys(dictionary* pDictionaryA, dictionary* pDictionaryB);
	void createEMspModel(int iSizea = 0, int iSizeb = 0);
	void createEMbiModel(int iSizea, int iSizeb);
	void createEMleModel(int iSizea, int iSizeb);

	void calcEMspModel(int iSteps, float fTime = -1.0f, bool bOld = false);
	void calcEMbiModel();
	void calcEMleModel();

	void clearEMleModel();
	void clearEMbiModel();
	void clearEMspModel();

	void 	addEMleModel(int l, int m);
	void	setEMleModel(int l, int m, float* PIn);
	float getEMleModel(int l, int m, float* pOut = NULL);
	int   getMaxSentenceSize() {return m_iMaxSentenceSize;}
	int   setMaxSentenceSize(int i) {return m_iMaxSentenceSize = i;}

	void 	addEMbiModel(int iW1, int iW2);
	void	setEMbiModel(int iW1, int iW2, float* PIn);
	float getEMbiModel(int iW1, int iW2, float* pOut = NULL);

	void  setEMspModel(int f, int e, float* pIn);
	float getEMspModel(int f, int e, float* pOut = NULL);

	std::list<std::pair<int, float> > getEMspBestnWords(int iWord, int n);
};

#endif
