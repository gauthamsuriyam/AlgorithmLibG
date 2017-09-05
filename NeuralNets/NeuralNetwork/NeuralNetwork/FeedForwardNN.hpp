//
//  FeedForwardNN.hpp
//  NeuralNetwork
//
//  Created by Sharmili Sitharthan on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#ifndef FeedForwardNN_hpp
#define FeedForwardNN_hpp

#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <set>

using namespace std;

class FeedForwardNN{
    set<string> Vocab;
    //unordered_multimap<string,string> InputSet;
public:
    FeedForwardNN(int layer[]);
    void InitNetwork(); // Data processing and neural net formation
    void FeedFwd(); // Data propagation through the network
    void BackProp();// Back propogation to find the weights and bias of the network
    void TextProcessor();
};

#endif /* FeedForwardNN_hpp */
