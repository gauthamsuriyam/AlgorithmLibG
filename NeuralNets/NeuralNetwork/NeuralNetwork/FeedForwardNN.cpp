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

void FeedForwardNN::TextProcessor(){
    string line;
    
    //ifstream testfile("/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/20news-bydate-train/talk.religion.misc/83476");
    DIR *dirp = opendir("/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/");
    struct dirent * rePt;
    if(dirp){
        while((rePt = readdir(dirp)) != NULL){
            if( strcmp(rePt->d_name, ".") != 0 && strcmp(rePt->d_name, "..") != 0 && strcmp(rePt -> d_name, ".DS_Store"))
                cout << rePt->d_name << "\n";
        }
        closedir(dirp);
    }
    
    /*if(testfile.is_open()){
        while(getline(testfile, line)){
            cout<< line << "\n" << endl;
        }
        testfile.close();
    }else{
        cout << "unable to open file" << endl;
    }*/
}
