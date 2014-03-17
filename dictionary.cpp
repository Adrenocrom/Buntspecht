/*********************************
*   Buntspecht translation tool  *
*   					  		 *
*	 Author: Josef Schulz		 *
*	 File	 :	dictionary.cpp	 *
**********************************/

#include "buntspecht.h"



dictionary::dictionary()
{
	m_pStrMapWords = NULL;
	m_piSentenceLengths = NULL;
	m_ppiSentences = NULL;
}

void dictionary::Init()
{

}

void dictionary::Exit()
{
	if(m_pStrMapWords) {
		delete[] m_pStrMapWords;
		m_pStrMapWords = NULL;
	}

	if(m_piSentenceLengths) {
		delete[] m_piSentenceLengths;
		m_piSentenceLengths = NULL;
	}

	if(m_ppiSentences) {
		for(int i = 0; i < m_iFirstDim; i++) {
			delete[] m_ppiSentences[i];
		}

		delete[] m_ppiSentences;
		m_ppiSentences = NULL;
	}
}

bool dictionary::addWord(const std::string &str_word)
{
	pair<map<std::string, int>::iterator, bool> ret;
	int iIndex = m_mapWords.size();
 	ret = m_mapWords.insert ( pair<std::string, int>(str_word, iIndex) );
	return ret.second;
}

int dictionary::getWord(const std::string &str_word)
{
	std::map<std::string, int>::iterator it;

	it = m_mapWords.find(str_word);
	if(it != m_mapWords.end())
		return (*it).second;
	else return  -1;
}

void	dictionary::createWordMapping()
{
	Exit();
	//m_piMapWords 	= new int[getNumWords()];
	m_pStrMapWords = new std::string[getNumWords()];
	std::map<std::string, int>::iterator it;

	for(it = m_mapWords.begin(); it != m_mapWords.end(); it++) {
			m_pStrMapWords[(*it).second] = (*it).first;
	}

	// Create SentenceLength array
	m_piSentenceLengths = new int[getSentences().size()];

	for(int i = 0; i < getSentences().size(); i++)
		m_piSentenceLengths[i] = m_vSentence[i].vWords.size();

	// Vector to arrays
	m_iFirstDim  = getSentences().size();

	m_ppiSentences = new int*[m_iFirstDim];

	for(int i = 0; i < m_iFirstDim; i++) {
		m_ppiSentences[i] = new int[m_piSentenceLengths[i]];
	}

	for(int i = 0; i < m_iFirstDim; i++) {
		for(int j = 0; j  < m_piSentenceLengths[i]; j++) {
			m_ppiSentences[i][j] = m_vSentence[i].vWords[j];
		}
	}
}

int dictionary::getNumWords()
{
	return m_mapWords.size();
}

string dictionary::getWord(int i)
{
	if((i < m_mapWords.size()) && (i > -1))
		return m_pStrMapWords[i];
	else	return "#";//"_DONT_EXIST_";
}

int* dictionary::getSentenceSize(int* iSentence)
{
	return &m_piSentenceLengths[*iSentence];
}

int* dictionary::getWordOfSentence(int* iSentence, int* iWord) {
	return &m_ppiSentences[*iSentence][*iWord];
}

