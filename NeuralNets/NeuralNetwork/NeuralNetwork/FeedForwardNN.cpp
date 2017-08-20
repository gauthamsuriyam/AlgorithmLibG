//
//  FeedForwardNN.cpp
//  NeuralNetwork
//
//  Created by Sharmili Sitharthan on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include "FeedForwardNN.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>

using namespace std;

FeedForwardNN::FeedForwardNN(){
    std::cout << "neural net initiated" << std::endl;
    TextProcessor();
}

void FeedForwardNN::InitNetwork(){
    
}

void FeedForwardNN::FeedFwd(){
    
}

void FeedForwardNN::BackProp(){
    
}

void directoryProbe(string folder){
    //cout << folder << endl;
    DIR *dirp = opendir(folder.c_str());
    struct dirent * rePt;
    //string fldtmp = folder;
    if(dirp){
        while((rePt = readdir(dirp)) != NULL){
            if( strcmp(rePt->d_name, ".") != 0 && strcmp(rePt->d_name, "..") != 0 && strcmp(rePt -> d_name, ".DS_Store") !=0){
                cout << rePt->d_name << "\n";
                directoryProbe(folder+"/"+rePt->d_name);
            }
        }
        closedir(dirp);
    }else{
        cout<< "no more folders found" << endl;
        string line;
        ifstream testfile(folder);
        if(testfile.is_open()){
            while(getline(testfile, line)){
                cout<< line << "\n" << endl;
            }
            testfile.close();
        }else{
            cout << "unable to open file" << endl;
        }
    }
}

void FeedForwardNN::TextProcessor(){
    string line = "/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/20news-bydate-train";
    const char * folder = line.c_str();
    directoryProbe(folder);
    
}
