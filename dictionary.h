/*********************************
*   Buntspecht translation tool  *
*   										*
*	 Author: Josef Schulz			*
*	 File	 :	dictionary.h			*
**********************************/

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#pragma once

class dictionary
{
public:
	std::map<std::string, int> m_mapWords;
	std::vector<sentence> 		m_vSentence;
	int*								m_piSentenceLengths;
	int								m_iFirstDim;
	int**								m_ppiSentences;
	int*								m_piMapWords;
	std::string*		   		m_pStrMapWords;

	// Konstruktor und Destruktor
	dictionary();
	~dictionary() {Exit();}

	void Init();
	void Exit();

	bool 	 addWord(const std::string &str_word);
	int  	 getWord(const std::string &str_word);
	string getWord(int i);

	void	createWordMapping(); 

	std::map<std::string, int> &getWords() {return m_mapWords;}
	std::vector<sentence> 		&getSentences() {return m_vSentence;}
	
	int* getSentenceSize(int* iSentence);
	int* getWordOfSentence(int* iSentence, int* iWord);
	int getNumWords();
	
};

#endif
