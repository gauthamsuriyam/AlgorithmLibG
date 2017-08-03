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
    ifstream testfile("/Users/Sharmili/Desktop/GauthamDev/20news-bydate/20news-bydate-train/talk.religion.misc/83476");
    if(testfile.is_open()){
        while(getline(testfile, line)){
            cout<< line << "\n" << endl;
        }
        testfile.close();
    }else{
        cout << "unable to open file" << endl;
    }
}
