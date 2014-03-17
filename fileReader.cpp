/*********************************
*   Buntspecht translation tool  *
*   										*
*	 Author: Josef Schulz			*
*	 File	 :	fileReader.cpp			*
**********************************/

#include "buntspecht.h"

fileReader::fileReader()
{
	
}

void fileReader::Init(EMdictionary* pEMdictionary)
{
	m_pEMdictionary = pEMdictionary;
}

void fileReader::Exit()
{
}

void fileReader::loadFromFile(const std::string &str_filename, dictionary* pDictionary)
{
	ifstream file(str_filename.c_str());
	
	if (!file.is_open()) 
	{
		std::cerr << "Error: Could not find file "<<str_filename<<"."<<std::endl;
		return;
	}

	string str_line;

	while(getline(file, str_line, '\n'))
	{
		stringstream line(str_line);

		progressSentence(line, pDictionary);
	}

	pDictionary->createWordMapping();

	file.close();
}

void fileReader::progressSentence(std::stringstream &line, dictionary* pDictionary)
{
	string 				entry;
	sentence		    	vSentence;	// is one sentence

	while(getline(line, entry, ' ')) {

		if (entry.empty())
			continue;
		
		pDictionary->addWord(entry);
		vSentence.vWords.push_back(pDictionary->getWord(entry));
	}

	pDictionary->getSentences().push_back(vSentence);
}

void fileReader::loadEMspModelFromFile(const char* pcFilename)
{
	ifstream file(pcFilename);
	
	if (!file.is_open()) 
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;
	}

	char  	cBuffer[20];
	int  		a, b;
	float 	fBuffer;

	string str_line;
	getline(file, str_line, '\n');
	stringstream line(str_line);
		
	string SizeA;
	string Buffer;
	string SizeB;
	line>>SizeA;
	line>>Buffer;
	line>>SizeB;

	stringstream strSSizeA(SizeA); strSSizeA >> a;
	stringstream strSSizeB(SizeB); strSSizeB >> b;

	m_pEMdictionary->createEMspModel(a, b);

	for(int y = 0; y < a; y++) {
		for(int x = 0; x < b; x++) {
			fBuffer = 0.0f;
			m_pEMdictionary->setEMspModel(y, x, &fBuffer);
		}
	}

	while(getline(file, str_line, '\n'))
	{
		stringstream line(str_line);
		
		string str_a;
		string str_b;
		string chance;
		line>>str_a;
		line>>str_b;
		line>>chance;

		stringstream strSstr_a(str_a); strSstr_a >> a;
		stringstream strSstr_b(str_b); strSstr_b >> b;
		stringstream strSchance(chance); strSchance >> fBuffer;

		m_pEMdictionary->setEMspModel(a, b, &fBuffer);
	}

	file.close();
}

void fileReader::loadEMbiModelFromFile(const char* pcFilename)
{
	ifstream file(pcFilename);
	
	if (!file.is_open()) 
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;
	}

	char  	cBuffer[20];
	int  		a, b;
	float 	fBuffer;

	string str_line;
	getline(file, str_line, '\n');
	stringstream line(str_line);
		
	string SizeA;
	string Buffer;
	string SizeB;
	line>>SizeA;
	line>>Buffer;
	line>>SizeB;

	stringstream strSSizeA(SizeA); strSSizeA >> a;
	stringstream strSSizeB(SizeB); strSSizeB >> b;

	m_pEMdictionary->createEMbiModel(a, b);

	for(int y = 0; y < a; y++) {
		for(int x = 0; x < b; x++) {
			fBuffer = 0.0f;
			m_pEMdictionary->setEMbiModel(y, x, &fBuffer);
		}
	}

	while(getline(file, str_line, '\n'))
	{
		stringstream line(str_line);
		
		string str_a;
		string str_b;
		string chance;
		line>>str_a;
		line>>str_b;
		line>>chance;

		stringstream strSstr_a(str_a); strSstr_a >> a;
		stringstream strSstr_b(str_b); strSstr_b >> b;
		stringstream strSchance(chance); strSchance >> fBuffer;

		m_pEMdictionary->setEMbiModel(a, b, &fBuffer);
	}

	file.close();
}

void fileReader::loadEMleModelFromFile(const char* pcFilename)
{
	ifstream file(pcFilename);
	
	if (!file.is_open()) 
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;
	}

	char  	cBuffer[20];
	int  		a, b;
	float 	fBuffer;

	string str_line;
	getline(file, str_line, '\n');
	stringstream line(str_line);
		
	string SizeA;
	string Buffer;
	string SizeB;
	line>>SizeA;
	line>>Buffer;
	line>>SizeB;

	stringstream strSSizeA(SizeA); strSSizeA >> a;
	stringstream strSSizeB(SizeB); strSSizeB >> b;

	m_pEMdictionary->createEMleModel(a, b);

	m_pEMdictionary->setMaxSentenceSize(a);
 
	for(int y = 0; y < a; y++) {
		for(int x = 0; x < b; x++) {
			fBuffer = 0.0f;
			m_pEMdictionary->setEMleModel(y, x, &fBuffer);
		}
	}

	while(getline(file, str_line, '\n'))
	{
		stringstream line(str_line);
		
		string str_a;
		string str_b;
		string chance;
		line>>str_a;
		line>>str_b;
		line>>chance;

		stringstream strSstr_a(str_a); strSstr_a >> a;
		stringstream strSstr_b(str_b); strSstr_b >> b;
		stringstream strSchance(chance); strSchance >> fBuffer;

		m_pEMdictionary->setEMleModel(a, b, &fBuffer);
	}

	file.close(); 
}

void fileReader::saveEMspModelToFile(const char* pcFilename, dictionary* pDictA, dictionary* pDictB)
{
	FILE* pFile;
	if((pFile = fopen(pcFilename, "w")) == NULL)
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;	
	}

	fprintf(pFile, "%d x %d\n", pDictA->getNumWords(), pDictB->getNumWords());

	for(int f = 0; f < pDictA->getNumWords(); f++) {
		for(int e = 0; e < pDictB->getNumWords(); e++) {
			if(m_pEMdictionary->getEMspModel(f, e) > 0.0f) {
				fprintf(pFile, "%d %d %.20f\n", f, e, m_pEMdictionary->getEMspModel(f, e));
			}
		}
	}

	fclose(pFile);
	pFile = NULL;
}

void fileReader::saveEMbiModelToFile(const char* pcFilename, dictionary* pDictA, dictionary* pDictB)
{
	FILE* 	pFile;
	if((pFile = fopen(pcFilename, "w")) == NULL)
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;	
	}

	fprintf(pFile, "%d x %d\n", (pDictB->getNumWords()+1), (pDictB->getNumWords()+1));

	for(int a = 0; a < (pDictB->getNumWords()+1); a++) {
		for(int b = 0; b < (pDictB->getNumWords()+1); b++) {
			if(m_pEMdictionary->getEMbiModel(a, b) > 0.0f) {
				fprintf(pFile, "%d %d %.20f\n", a, b, m_pEMdictionary->getEMbiModel(a, b));
			}
		}
	}

	fclose(pFile);
	pFile = NULL;
}

void fileReader::saveEMleModelToFile(const char* pcFilename, dictionary* pDictA, dictionary* pDictB)
{
	FILE* 	pFile;
	if((pFile = fopen(pcFilename, "w")) == NULL)
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;	
	}
	
	fprintf(pFile, "%d x %d\n", m_pEMdictionary->getMaxSentenceSize(), m_pEMdictionary->getMaxSentenceSize());

	for(int e = 0; e < m_pEMdictionary->getMaxSentenceSize(); e++) {
		for(int f = 0; f < m_pEMdictionary->getMaxSentenceSize(); f++) {
			if(m_pEMdictionary->getEMleModel(e, f) > 0.0f) {
				fprintf(pFile, "%d %d %.20f\n", e, f, m_pEMdictionary->getEMleModel(e, f));
			}
		}
	}

	fclose(pFile);
	pFile = NULL;
}

void fileReader::saveDictionaryToFile(const char* pcFilename, dictionary* pDict)
{
	FILE* 	pFile;
	if((pFile = fopen(pcFilename, "w")) == NULL)
	{
		std::cerr << "Error: Could not find file "<<pcFilename<<"."<<std::endl;
		return;	
	}

	for(int i = 0; i < pDict->getNumWords(); i++)
		fprintf(pFile, "%s\n", pDict->getWord(i).c_str());

	fclose(pFile);
	pFile = NULL;
}
