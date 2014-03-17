/*********************************
*   Buntspecht translation tool  *
*   										*
*	 Author: Josef Schulz			*
*	 File	 :	fileReader.h			*
**********************************/

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#pragma once

class fileReader
{
private:
	EMdictionary* m_pEMdictionary;
	
public:
	// Konstruktor und Destrukor
	fileReader();
	~fileReader() {Exit();}

	void Init(EMdictionary* pEMdictionary);
	void Exit();

	static void loadFromFile(const std::string &str_filename, dictionary* pDictionary);
	void loadEMspModelFromFile(const char* pcFilename);
	void loadEMbiModelFromFile(const char* pcFilename);
	void loadEMleModelFromFile(const char* pcFilename);
	void saveEMspModelToFile(const char* pcFilename, dictionary* pDictA, dictionary* pDictB);
	void saveEMbiModelToFile(const char* pcFilename, dictionary* pDictA, dictionary* pDictB);
	void saveEMleModelToFile(const char* pcFilename, dictionary* pDictA, dictionary* pDictB);
	void saveDictionaryToFile(const char* pcFilename, dictionary* pDict);
	static void progressSentence(std::stringstream &line, dictionary* pDictionary);
};

#endif
