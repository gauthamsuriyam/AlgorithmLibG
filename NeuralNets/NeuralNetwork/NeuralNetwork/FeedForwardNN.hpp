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

class FeedForwardNN{
public:
    FeedForwardNN();
    void InitNetwork(); // Data processing and neural net formation
    void FeedFwd(); // Data propagation through the network
    void BackProp();// Back propogation to find the weights and bias of the network
    void TextProcessor();
};

#endif /* FeedForwardNN_hpp */
