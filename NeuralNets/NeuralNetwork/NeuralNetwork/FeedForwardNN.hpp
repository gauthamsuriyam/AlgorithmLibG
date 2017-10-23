//
//  FeedForwardNN.hpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#ifndef FeedForwardNN_hpp
#define FeedForwardNN_hpp

//#include <stdio.h>
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
    Eigen::VectorXd inpx; // input
    Eigen::VectorXd opTarget; // target output
    Eigen::VectorXd z;
    Eigen::MatrixXd diffw;
    Eigen::VectorXd diffb;
};


class FeedForwardNN{
    int layerSize;
    int lRate;
    int batchS;
    bool trainPhase;
    set<string> Vocab;
    map<int,layers> lnet;
    
    //unordered_multimap<string,string> InputSet;
public:
    FeedForwardNN(int hlayer[], int sz, int learningRate, int batchsize, string dsPath);
    void InitNetwork(int hlayer[]); // Data processing and neural net formation
    void FeedFwd(string cName); // Data propagation through the network
    void BackProp();// Back propogation to find the weights and bias of the network
    void TextProcessor(string dsPath);
    void BatchUpdate();
    void Classification(string dsPath = "",bool train = false);
};

#endif /* FeedForwardNN_hpp */
