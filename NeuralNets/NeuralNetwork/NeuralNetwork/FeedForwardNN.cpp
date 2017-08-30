//
//  FeedForwardNN.cpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include "FeedForwardNN.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <dirent.h>

using namespace std;

FeedForwardNN::FeedForwardNN(){
    cout << "neural net initiated" << endl;
    TextProcessor();
}

void FeedForwardNN::InitNetwork(){
    
}

void FeedForwardNN::FeedFwd(){
    
}

void FeedForwardNN::BackProp(){
    
}

void directoryProbe(string folder, set<string> * Vocab){
    
    //cout << folder << endl;
    DIR *dirp = opendir(folder.c_str());
    struct dirent * rePt;
    //string fldtmp = folder;
    if(dirp){
        while((rePt = readdir(dirp)) != NULL){
            if( strcmp(rePt->d_name, ".") != 0 && strcmp(rePt->d_name, "..") != 0 && strcmp(rePt -> d_name, ".DS_Store") !=0){
                //cout << rePt->d_name << "\n";
                directoryProbe(folder+"/"+rePt->d_name , Vocab);
            }
        }
        closedir(dirp);
    }else{
        //cout<< "no more folders found" << endl;
        Vocab->insert(folder);
        /*string line;
        ifstream testfile(folder);
        if(testfile.is_open()){
            while(getline(testfile, line)){
                cout<< line << "\n" << endl;
            }
            testfile.close();
        }else{
            cout << "unable to open file" << endl;
        }*/
    }
    
}
void FilterWords(string &s){
    char filter[44];
    int count = 0;
    //filter[count++] = 0;
    filter[count++] = 9;
    filter[count++] = 10;
    
    for(int i= 33; i < 65; i++){
        filter[count++] = i;
    }
    for(int i = 91; i < 97; i++){
        filter[count++] = i;
    }
    for(int i = 123; i < 128; i++){
        filter[count++] = i;
    }
    filter[count] = '\0';
    
    //cout<< s << endl;
    
    for (unsigned int i = 0; i < sizeof(filter); ++i) {
        s.erase(remove(s.begin(), s.end(), filter[i]),s.end());
    }
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    
    //cout << s << endl;
}
void FeedForwardNN::TextProcessor(){
    char filter[3];
    int count = 0;
    //filter[count++] = 0;
    filter[count++] = 9;
    filter[count++] = 10;
    filter[count++] = 32;
    
    /*for(int i= 33; i < 65; i++){
        filter[count++] = i;
    }
    for(int i = 91; i < 97; i++){
        filter[count++] = i;
    }
    for(int i = 123; i < 128; i++){
        filter[count++] = i;
    }*/
    filter[count] = '\0';
    
    /*for(int j =0; j< 45; j++){
        cout << "filter"<<j<< " : "<<filter[j] << endl;
    }*/
        
    set<string> folderset;
    string line = "/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/20news-bydate-train";
    const char * folder = line.c_str();
    char * pch;
    directoryProbe(folder, &folderset);
    
    for(auto it = folderset.begin(); it!= folderset.end(); it++){
        //cout<< "from vocab begining: "<<*it<<endl;
        ifstream testfile(*it);
        if(testfile.is_open()){
            while(getline(testfile, line)){
                FilterWords(line);
                //cout<< line << endl;
                char * lch = new char[line.length()+1];
                strcpy(lch, line.c_str());
                pch = strtok(lch, filter);
                while(pch!=NULL){
                    cout << "word: "<< pch << endl;
                    Vocab.insert(pch);
                    pch = strtok (NULL, filter);
                }
                delete [] lch;
            }
            testfile.close();
        }else{
            cout << "unable to open file" << endl;
        }
    }
    auto it = Vocab.find("\nfido");
    
    cout<< "vocab list: "<<endl;
    if(it!=Vocab.end()){
        cout << "found: "<<*it << endl;
    }else{
        cout<< "end of list reached" << endl;
    }
    
    /*for(auto itv = Vocab.begin(); itv!= Vocab.end(); itv++){
        cout<<"vocab: "<<*itv<<endl;
        c = itv->c_str();
        char * ph;
        ph = strpbrk(c, "\0");mikael
        while(ph!=NULL){
            cout << "ph value found" << endl;
            printf("%c",*ph);
            ph = strpbrk(ph+1, filter);
        }
    }*/
    
}
