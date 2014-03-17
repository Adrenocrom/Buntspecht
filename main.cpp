#include "buntspecht.h"

// global variables
pthread_t	  g_threadRealLanguageA;
pthread_t	  g_threadRealLanguageB;
std::string   g_str_filenameA;
std::string   g_str_filenameB;

// prototyps
void startLoadLanguages(std::string str_filenameA,
								std::string str_filenameB,
								dictionary* pDictA,
								dictionary* pDictB);
void* pThreadLoadLanguageA(void* ptr);
void* pThreadLoadLanguageB(void* ptr);
void  joinLoadLanguages();
void  readme();

int main( int argc, const char* argv[] )
{
	dictionary* pLanguageA;			// language a = f
	dictionary* pLanguageB;			// language b = e
	fileReader* pFileReader;
	EMdictionary* pEMdictionary;
	translater* pTranslater;
	float		fTrainTime  = -1.0f;
	int			iIterations = 10;

	// Header
	std::string str_languageA;
	std::string	str_languageB;
	std::string str_buntDataFile;
	std::string str_mode;
	std::string str_EMspFile;
	std::string str_EMbiFile;
	std::string str_EMleFile;
	std::string str_dictA;
	std::string str_dictB;
	std::string strDa;
	std::string strDb;


	std::cout<<"------------------------------------ \033[1;31mBuntspecht\033[0m --------------------------------\n"<<std::endl;
	std::cout<<"\033[1;31m";
	std::cout<<"                                                            .-+*+*+:-.         "<<std::endl;
	std::cout<<"                                                          .-*:****==**:.       "<<std::endl;
	std::cout<<"                                                   .......++*+...-+*****-.     "<<std::endl;
	std::cout<<"                                             ..-::+****+++:*=:.....-*=*=+.     "<<std::endl;
	std::cout<<"                                         ..+=======***+:..:*=*.....-+:+*+-.    "<<std::endl;
	std::cout<<"                                     ..-*=#========*+++...:=**+::-..+*+:.-.    "<<std::endl;
	std::cout<<"                                ..-+==####==#####=====*+..+=*+..-:+++:---:.    "<<std::endl;
	std::cout<<"                             .:+*=####===##=============*-+**+-..:----+++::.   "<<std::endl;
	std::cout<<"                        ..:*==#####=***=*====*+-......:***:**+--.     .-+*++-. "<<std::endl;
	std::cout<<"                    ..-+*====#==#==+-.-......--..--.:+*+=*+-+::.        .-++:-."<<std::endl;
	std::cout<<"               ..-:**=#====#=======**::+****======***==**+:.::.            .++:"<<std::endl;
	std::cout<<"           ....:++**#====#=========#===============*****:-.--.                ."<<std::endl;
	std::cout<<"       .::++++***================*================****:----.                   "<<std::endl;
	std::cout<<" ...--::+++++:++++++**=******=*====*+++++::----------...:.                     "<<std::endl;
	std::cout<<".-:+++:++++++*****=*=****++*******+--:---------:::----:-.                      "<<std::endl;
	std::cout<<"   ..-::++++***+::-.       .-:::+++:-:------------.--.                         "<<std::endl;
	std::cout<<"                                    .::---..---.--.                            \033[0m\n"<<std::endl;

	str_languageA 	  = argv[1];

	if(str_languageA == "--help") {
		readme();
		return 0;
	}

	str_languageB 	  = argv[2];
	str_buntDataFile = argv[3];
	str_mode 		  = argv[4];

	str_EMspFile = str_buntDataFile;
	str_EMbiFile = str_buntDataFile;
	str_EMleFile = str_buntDataFile;
	str_dictA 	 = str_buntDataFile;
	str_dictB 	 = str_buntDataFile;

	str_EMspFile += "EMspModel.txt";
	str_EMbiFile += "EMbiModel.txt";
	str_EMleFile += "EMleModel.txt";
	str_dictA	 += "dictAionary.txt";
	str_dictB	 += "dictBionary.txt";

	// option
	std::string sOption;
	sOption = argv[5];

	if(sOption == "-t") {
		if(argc > 6) {
			sOption = argv[6];

			std::istringstream ( sOption ) >> fTrainTime;
		}
	}
	else {std::istringstream ( sOption ) >> iIterations;}

	std::cout<<"==========================";
	InitBuntspecht();

	std::cout<<"==========";
	pEMdictionary = new EMdictionary();

	std::cout<<"===========";
	pLanguageA = new dictionary();
	pLanguageA->Init();

	std::cout<<"===========";
	pLanguageB = new dictionary();
	pLanguageB->Init();

	std::cout<<"============";
	pFileReader = new fileReader();
	pFileReader->Init(pEMdictionary);

	std::cout<<"=========="<<std::endl;

	if(str_mode == "train") {
		strDa = str_languageA;
		strDb = str_languageB;
	}
	else {
		strDa = str_dictA;
		strDb = str_dictB;
	}

	std::cout<<"lode file language e: "<<str_languageB<<std::endl;
	std::cout<<"lode file language f: "<<str_languageA<<std::endl;
	startLoadLanguages(strDa,
							 strDb,
							 pLanguageA,
							 pLanguageB);

	joinLoadLanguages();

	std::cout<<"num words language e: "<<pLanguageB->getNumWords()<<std::endl;
	std::cout<<"num words language f: "<<pLanguageA->getNumWords()<<std::endl;

	std::cout<<"create EM Workgroup"<<std::endl;
	pEMdictionary->Init(pLanguageA, pLanguageB);

	std::cout<<"modus used: "<<str_mode<<std::endl;
	std::cout<<std::endl;


	std::cout<<"================================================================================"<<std::endl;

	if(str_mode == "train") {
		std::cout<<"------------------------------------- training ---------------------------------\n"<<std::endl;
		std::cout<<"calculate length model ...      ";
			pEMdictionary->calcEMleModel();
		std::cout<<"calculate bigram model ...      ";
			pEMdictionary->calcEMbiModel();
		std::cout<<"calculate tranlation model ...  ";
			pEMdictionary->calcEMspModel(iIterations, fTrainTime);
		std::cout<<"save length model ...                                                       ";
			pFileReader->saveEMleModelToFile(str_EMleFile.c_str(), pLanguageA, pLanguageB);
				std::cout<<"[OK]\n";
		std::cout<<"save bigram model ...                                                       ";
			pFileReader->saveEMbiModelToFile(str_EMbiFile.c_str(), pLanguageA, pLanguageB);
				std::cout<<"[OK]\n";
		std::cout<<"save tranlation model ...                                                   ";
			pFileReader->saveEMspModelToFile(str_EMspFile.c_str(), pLanguageA, pLanguageB);
				std::cout<<"[OK]\n";
		std::cout<<"save dictionary language a ...                                              ";
			pFileReader->saveDictionaryToFile(str_dictA.c_str(), pLanguageA);
				std::cout<<"[OK]\n";
		std::cout<<"save dictionary language b ...                                              ";
			pFileReader->saveDictionaryToFile(str_dictB.c_str(), pLanguageB);
				std::cout<<"[OK]\n\n";

		std::cout<<"================================================================================\n"<<std::endl;
	}
	else if(str_mode == "lookup") {
		std::cout<<"------------------------------------- lookup -----------------------------------\n"<<std::endl;
		std::cout<<"load data sets ..."<<std::endl;
		std::cout<<"load length model set ...                                                   ";
			pFileReader->loadEMleModelFromFile(str_EMleFile.c_str());
				std::cout<<"[OK]\n";

		std::cout<<"load bigram model set ...                                                   ";
			pFileReader->loadEMbiModelFromFile(str_EMbiFile.c_str());
				std::cout<<"[OK]\n";

		std::cout<<"load translation model set ...                                              ";
			pFileReader->loadEMspModelFromFile(str_EMspFile.c_str());
				std::cout<<"[OK]\n\n";

		std::cout<<"================================================================================\n"<<std::endl;
	}
	else if(str_mode == "decode") {
	    std::cout<<"------------------------------------- decode -----------------------------------\n"<<std::endl;
	}

	pTranslater = new translater(pLanguageA, pLanguageB, pEMdictionary);

	// Buntspecht konsole
	std::string str_input = "";
	std::string str_option1;
	std::string str_option2;
	std::string str_option3;
	std::string str_option4;

	while(str_input != "exit") {
		std::cout<<"Buntspecht:~$ ";
		std::getline(std::cin, str_input);

		stringstream line(str_input);
		line>>str_input;
		if(str_input == "--help")
			readme();

		if(str_input == "set") {
			line>>str_option1;
			line>>str_option2;
			line>>str_option3;
			line>>str_option4;

			if(str_option1 == "alpha1") g_fAlpha1 = (double)atof(str_option2.c_str());
			if(str_option1 == "alpha2") g_fAlpha2 = (double)atof(str_option2.c_str());
			if(str_option1 == "alpha3") g_fAlpha3 = (double)atof(str_option2.c_str());
			if(str_option1 == "alpha4") g_fAlpha4 = (double)atof(str_option2.c_str());

			if(str_option1 == "show") {
			    if(str_option2 == "stack") {
			        if(str_option3 == "true")  g_bShowStack = true;
			        if(str_option3 == "false") g_bShowStack = false;
			    }
			}

			int 	a;			 std::istringstream ( str_option2 ) >> a;
			int 	b;			 std::istringstream ( str_option3 ) >> b;
			float	chance;	std::istringstream ( str_option4 ) >> chance;

			if(str_option1 == "le") pEMdictionary->setEMleModel(a, b, &chance);
			if(str_option1 == "bi") pEMdictionary->setEMbiModel(a, b, &chance);
			if(str_option1 == "sp") pEMdictionary->setEMspModel(a, b, &chance);
			if(str_option1 == "stack") g_iStackSize = a;
			if(str_option1 == "steps") g_iTranslateSteps = a;
		}

		if(str_input == "get") {
			line>>str_option1;
			line>>str_option2;
			line>>str_option3;
			line>>str_option4;

			int 	a;			std::istringstream ( str_option2 ) >> a;
			int 	b;			std::istringstream ( str_option3 ) >> b;

			if(str_option1 == "le") std::cout<<"EMleModel("<<a<<","<<b<<"): "<<pEMdictionary->getEMleModel(a, b)<<std::endl;
			if(str_option1 == "bi") std::cout<<"EMbiModel("<<a<<","<<b<<"): "<<pEMdictionary->getEMbiModel(a, b)<<std::endl;
			if(str_option1 == "sp") std::cout<<"EMspModel("<<a<<","<<b<<"): "<<pEMdictionary->getEMspModel(a, b)<<std::endl;
			if(str_option1 == "alpha1") std::cout<<"alpha= "<<g_fAlpha1<<std::endl;
			if(str_option1 == "alpha2") std::cout<<"alpha= "<<g_fAlpha2<<std::endl;
			if(str_option1 == "alpha3") std::cout<<"alpha= "<<g_fAlpha3<<std::endl;
			if(str_option1 == "alpha4") std::cout<<"alpha= "<<g_fAlpha4<<std::endl;
			if(str_option1 == "stack") std::cout<<"Stack Size= "<<g_iStackSize<<std::endl;
			if(str_option1 == "steps") std::cout<<"Steps= "<<g_iTranslateSteps<<std::endl;
			if(str_option1 == "show") {
			    if(str_option2 == "stack") {
			        if(g_bShowStack) std::cout<<"true"<<std::endl;
			        else             std::cout<<"false"<<std::endl;
			    }
			}
			if(str_option1 == "word") {
					if(str_option3.empty()) b = 2;

					std::list<std::pair<int, float> >::iterator it;
					std::list<std::pair<int, float> >  li = pEMdictionary->getEMspBestnWords(pLanguageB->getWord(str_option2), b);

					for(it = li.begin(); it != li.end(); it++) {
						std::pair<int, float> word = *it;
						printf("%.20f - %s\n", word.second, pLanguageA->getWord(word.first).c_str());
					}
			}
		}

		if(str_input == "trans") {
		    vector<int> vTrans;
            string str_word;

            getline(line, str_word, ' ');
            while(getline(line, str_word, ' ')) {
                vTrans.push_back(pLanguageB->getWord(str_word));
            }

            vTrans = pTranslater->searchBestScentences(vTrans);

            printf("\n");

            for(int i = 0; i < vTrans.size(); i++) {
                printf("%s ", pLanguageA->getWord(vTrans[i]).c_str());
            }
            std::cout<<std::endl;
		}

	}

	pFileReader->Exit();

	if(pFileReader) {
		delete pFileReader;
		pFileReader = NULL;
	}

    if(pTranslater) {
        delete pTranslater;
        pTranslater = NULL;
    }

	pLanguageA->Exit();
	pLanguageB->Exit();
	pEMdictionary->Exit();

	if(pLanguageA) {
		delete pLanguageA;
		pLanguageA = NULL;
	}

	if(pLanguageB) {
		delete pLanguageB;
		pLanguageB = NULL;
	}

	if(pEMdictionary) {
		delete pEMdictionary;
		pEMdictionary = NULL;
	}

	ExitBuntspecht();


	return 0;
}

void readme()
{
	std::cout<<"================================================================================\n"<<std::endl;
	std::cout<<"----------------------------------- manual page --------------------------------\n"<<std::endl;

		 printf("                      Willkommen auf der page von Buntspecht\n\n"
				  "Diese Seite gibt dient der Orientierung im Übersetzungsprogramm Buntspecht.\n"
				  "Das Programm ist besteht aus drei wesentlichen bestandteilen um Übersetzung-\n"
				  "en mittels Buntspecht zu ermöglichen.\n\n"
				  "Zu erst müssen die entsprechenden Sprachen trainiert werden, dazu werden\n"
				  "Trainingsdaten benötigt.\n"
				  "Der zweite Modus wird zur Kalibrierung des Systems verwendet und zu letzt\n"
				  "wird im dritten Modus das trainierte angewendet und Übersetzt.\n\n"
				  "Um das Programm zu starten sollte folgende Syntax eingehalten werden:\n\n"
				  "    Buntspecht --help\n"
				  "    Buntspecht <english> <german> <base-dir> <modus> <<iter>|-t <time in secs>\n\n"
				  "Folgende Modi sind vorhanden: train\n"
				  "                              lookup\n"
				  "                              decode\n"
				  "Es besteht die Möglichkeit eine bestimmte Anzahl von Iterationen für den\n"
				  "Lernprozess anzugeben andernfalls kann auch eine Zeit übergeben werden die\n"
				  "den Abbruch des Lernvorgangs bestimmt.\n\n"
				  "Nach der Initialisierung gelangen Sie in die Buntspechtconsole in der Sie\n"
				  "die Funktionalität von Buntspecht mit den Folgenden Befehlen nutzen können:\n\n"
				  "   set < sp | le | bi > <int-word german> <int-word english>\n"
				  "   set <alpha1 | ... | alpha4> <float-weight>\n"
				  "   set <stack | steps> <int-count>\n"
				  "   get < sp | le | bi > <int-word german> <int-word english>\n"
				  "   get word <word englis> [int-num-words]\n"
				  "   trans <Sentence>\n"
				  "   exit\n"
				  "   --help\n\n"
				  "Die Befehle geben Ihnen Zugriff auf alle Variablen, aber Achtung die Werte\n"
				  "werden nicht auf Speicherüberlauf getestet.\n\n"
				  "                                    Viel Erfolg\n\n");

	std::cout<<"================================================================================\n"<<std::endl;
}

void startLoadLanguages(std::string str_filenameA,
								std::string str_filenameB,
								dictionary* pDictA,
								dictionary* pDictB)
{
	int iRcA;
	int iRcB;

	g_str_filenameA = str_filenameA;
	g_str_filenameB = str_filenameB;

	g_pDictA = pDictA;
	g_pDictB = pDictB;

	iRcA = pthread_create( &g_threadRealLanguageA, NULL, &pThreadLoadLanguageA, NULL);
	iRcB = pthread_create( &g_threadRealLanguageB, NULL, &pThreadLoadLanguageB, NULL);
}

void* pThreadLoadLanguageA(void* ptr)
{
	fileReader::loadFromFile(g_str_filenameA, g_pDictA);
}

void* pThreadLoadLanguageB(void* ptr)
{
	fileReader::loadFromFile(g_str_filenameB, g_pDictB);
}

void joinLoadLanguages()
{
	pthread_join( g_threadRealLanguageA, NULL);
	pthread_join( g_threadRealLanguageB, NULL);
}
