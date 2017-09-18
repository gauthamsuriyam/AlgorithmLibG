//
//  main.cpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include <iostream>
#include "FeedForwardNN.hpp"

int main() {
    int layer[] = {30};
    string trainPath = "/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/dlgdataset/train/";//"/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/20news-bydate-train/";
    FeedForwardNN foo(layer, sizeof(layer)/sizeof(layer[0]), 3,5,trainPath);
    string testPath = "/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/dlgdataset/test/";
    foo.Classification(testPath);
    return 0;
}
