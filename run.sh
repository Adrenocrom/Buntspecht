rm Buntspecht
g++ -o Buntspecht main.cpp buntspecht.cpp fileReader.cpp dictionary.cpp translater.cpp EMdictionary.cpp emThreads.cpp -lpthread -fopenmp
./Buntspecht corpura/europarl-v7.de-en.tokenizer.lowercase.clean.nubbed.4096.train.en.txt corpura/europarl-v7.de-en.tokenizer.lowercase.clean.nubbed.4096.train.de.txt BuntEMspecht/ train 10
