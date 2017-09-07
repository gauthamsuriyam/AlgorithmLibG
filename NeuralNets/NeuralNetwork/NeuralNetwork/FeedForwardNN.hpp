//
//  FeedForwardNN.hpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#ifndef FeedForwardNN_hpp
#define FeedForwardNN_hpp

#include <stdio.h>
#include <string.h>
#include <Eigen/Dense>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

struct layers{
    Eigen::MatrixXd weights;
    Eigen::VectorXd bias;
};


class FeedForwardNN{
    int layerSize;
    set<string> Vocab;
    map<int,layers> lnet;
    
    //unordered_multimap<string,string> InputSet;
public:
    FeedForwardNN(int hlayer[], int sz);
    void InitNetwork(int hlayer[]); // Data processing and neural net formation
    void FeedFwd(); // Data propagation through the network
    void BackProp();// Back propogation to find the weights and bias of the network
    void TextProcessor();
};

#endif /* FeedForwardNN_hpp */
