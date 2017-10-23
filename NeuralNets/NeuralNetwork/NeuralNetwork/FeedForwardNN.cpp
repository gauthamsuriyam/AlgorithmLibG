//
//  FeedForwardNN.cpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include "FeedForwardNN.hpp"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <dirent.h>
#include <Eigen/Dense>      // Needs to be installed separately. Can use other matrix tools.

using namespace std;
using Eigen::MatrixXd;


unordered_multimap<string, Eigen::VectorXd> InputSet;// class, inputParameters

set<string> OutputSet; // class order

int udTrigger = 0;
float accuCount = 0;

double sigmoid(double z){
    double a = 1/(1+exp(-z));
    //cout << a << endl;
    return a;
}

double sigmoidPr(double z){
    double aprime = sigmoid(z)*(1-sigmoid(z));
    //cout << aprime << endl;
    return aprime;
}

FeedForwardNN::FeedForwardNN(int hlayer[], int sz, int learningRate, int batchsize, string dsPath){
    cout << "neural net initiated | layers: "<< hlayer[0] << endl;
    layerSize = 2+sz;
    lRate = learningRate;
    batchS = batchsize;
    udTrigger = batchsize;
    trainPhase = true;
    TextProcessor(dsPath);
    InitNetwork(hlayer);
}

void FeedForwardNN::InitNetwork(int hlayer[]){
    
    // inputs-> Vocab
    // init weight -> size of Vocab
    // inner network weight -> specified // single hidden layer for build 1
    // Bias -> hidden layer size
    // OutputBias -> op size -> category.folder size
    // a = sum(wx) + b
    
    unsigned long int inl = Vocab.size();
    cout<< "input layer size: " << inl << endl;
    unsigned long int opl = OutputSet.size();
    cout << "output layer size: " <<opl << endl;
    Eigen::VectorXd inpVal = Eigen::VectorXd::Zero(inl);
    layers inputLayer;
    
    inputLayer.inpx = inpVal;
    lnet.insert({0,inputLayer});
    
    for(int i = 0; i < layerSize-2 ; i++){
        layers hiddenLayer;
        hiddenLayer.weights = MatrixXd::Random(hlayer[i],inl);
        hiddenLayer.bias = Eigen::VectorXd::Random(hlayer[i]);
        hiddenLayer.diffb = Eigen::VectorXd::Zero(hlayer[i]);
        hiddenLayer.diffw = MatrixXd::Zero(hlayer[i],inl);
        lnet.insert({i+1,hiddenLayer});
    }
    
    layers outputLayer;
    outputLayer.weights = MatrixXd::Random(opl,hlayer[layerSize-3]);
    outputLayer.diffw = MatrixXd::Zero(opl,hlayer[layerSize-3]);
    //cout << outputLayer.weights << endl;
    outputLayer.bias = Eigen::VectorXd::Random(opl);
    outputLayer.diffb = Eigen::VectorXd::Zero(opl);
    outputLayer.opTarget = Eigen::VectorXd::Zero(opl);
    lnet.insert({layerSize-1,outputLayer});
    
    cout << "neural net model has been created" << endl;
    for(auto opIt = OutputSet.begin(); opIt!=OutputSet.end(); opIt++){
        cout << *opIt << endl;
    }
    for(int epoch =0; epoch <100; epoch++){
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        cout << "epoch: " << epoch+1 << "Time: " << asctime(timeinfo) << endl;
        Classification("",trainPhase);
    }
}

void FeedForwardNN::Classification(string dsPath,bool train){ //put conversion and assignment of input set at processor to reduce time
    //cout<< "start class" << endl;
    if(!train){
        cout << "testing phase: " <<dsPath << endl;
        trainPhase = train;
        InputSet.clear();
        //OutputSet.clear();
        TextProcessor(dsPath);
        for(auto opIt = OutputSet.begin(); opIt!=OutputSet.end(); opIt++){
            cout << *opIt << endl;
        }
    }
    for(auto it = InputSet.begin(); it != InputSet.end(); it++){
        //cout << it->first << endl;
        if(trainPhase){
            (lnet.find(layerSize-1)->second).opTarget = Eigen::VectorXd::Zero(OutputSet.size());
            //cout << "inputlayer size" <<(lnet.find(0)->second).inpx.size() << endl;
            auto opbeg = OutputSet.begin();
            auto opos = OutputSet.find(it->first);
        
            if(opos!=OutputSet.end()){
                (lnet.find(layerSize-1)->second).opTarget[distance(opbeg, opos)] = 1;
                //cout << distance(opbeg, opos) << endl;
            }else{
                cout << "class name not found" << endl;
            }
        }
        (lnet.find(0)->second).inpx = it->second;
        FeedFwd(it->first);
    }
    if(!train){
        float classPerIn = (accuCount/InputSet.size())*100;
        cout << "accuCount= "<< accuCount << " Input size = " << InputSet.size()<< "correct classified % = " << classPerIn << endl;
    }
}

void FeedForwardNN::FeedFwd(string cName){
   // cout << "feed fwd" << endl;
    //find z= s(wx) +b
    // inx//a// = sig(z)
    //cout << "layer size: " << layerSize << endl;
    for(int i =1; i < layerSize ; i++){
        layers * currL = &lnet.find(i)->second;
        layers * prevL = &lnet.find(i-1)->second;
        currL->z = (currL->weights * prevL->inpx) + currL->bias;
        currL->inpx = currL->z.unaryExpr(ptr_fun(sigmoid));
    }
    if(trainPhase){
        BackProp();
    }else{
        Eigen::VectorXd::Index maxI;
        layers * l = &lnet.find(layerSize-1)->second;
        auto it = OutputSet.begin();
        double maxClass = l->inpx.maxCoeff(&maxI);
        advance(it,maxI);
        cout << cName << " MaxVal: " << maxClass << " MaxPos: " << maxI+1 << "|----->"<<*it<<endl;
        if(!cName.compare(it->c_str())){
            accuCount++;
        }
        
    }
}

void FeedForwardNN::BackProp(){
    Eigen::VectorXd deltaL;
    for(int j = layerSize-1; j>0 ; j--){
        layers * currL = &lnet.find(j)->second;
        layers * prevL = &lnet.find(j-1)->second;
        layers * nextL = &lnet.find(j+1)->second;
        
        if(j==layerSize-1){
            deltaL = (currL->inpx - currL->opTarget).cwiseProduct(currL->z.unaryExpr(ptr_fun(sigmoidPr)));
        }else{
            deltaL = ((nextL->weights.transpose())*deltaL).cwiseProduct(currL->z.unaryExpr(ptr_fun(sigmoidPr)));
        }
        currL->diffb += deltaL;
        currL->diffw += deltaL * prevL->inpx.transpose();
    }
    if(--udTrigger<1){
        BatchUpdate();
        udTrigger = batchS;
        //reset diffb and diffw for next batch
        for(int j = layerSize-1; j>0 ; j--){
            layers * currL = &lnet.find(j)->second;
            currL->diffb = Eigen::VectorXd::Zero(currL->diffb.size());
            currL->diffw = Eigen::MatrixXd::Zero(currL->diffw.rows(), currL->diffw.cols());
        }
    }
}

void FeedForwardNN::BatchUpdate(){ // add learning rate and batch size
    //cout << "batch Update" << endl;
    for(int k = 1; k< layerSize ; k++){
        layers * currL = &lnet.find(k)->second;
        currL->weights = currL->weights - ((lRate/batchS)*currL->diffw);
        currL->bias = currL->bias - currL->diffb;
    }
}
void directoryProbe(string folder, set<string> * Vocab){
    //cout << folder << endl;
    DIR *dirp = opendir(folder.c_str());
    struct dirent * rePt;
    if(dirp){
        while((rePt = readdir(dirp)) != NULL){
            if( strcmp(rePt->d_name, ".") != 0 && strcmp(rePt->d_name, "..") != 0 && strcmp(rePt -> d_name, ".DS_Store") !=0){                directoryProbe(folder+"/"+rePt->d_name , Vocab);
            }
        }
        closedir(dirp);
    }else{
        Vocab->insert(folder);
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
        //s.erase(remove(s.begin(), s.end(), filter[i]),s.end());
        replace(s.begin(),s.end(),filter[i],' ');
    }
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    
    //cout << s << endl;
}
void FeedForwardNN::TextProcessor(string dsPath){
    unordered_multimap<string, unordered_set<string>> InputWordSet;// class, features
    //cout << "trainphase "<< trainPhase << endl;
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
    string words;
    string className;
    string line;
    queue<string> tempCName;
    string trainPath = dsPath;//"/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/20news-bydate-train";
    size_t lsize = trainPath.size();
    const char * folder = trainPath.c_str();
    char * pch;
    directoryProbe(folder, &folderset);
    
    for(auto it = folderset.begin(); it!= folderset.end(); it++){   //Vocab and Input layer data are created
        //cout<< "file: "<<*it<<endl;
        unordered_set<string> Ip;
        if(!tempCName.empty()){
            tempCName.pop();
        }
        ifstream testfile(*it);
        words = *it;
        words.erase(words.begin(), words.begin()+lsize);
        stringstream iss(words);
        while(getline(iss, className, '/')){
            tempCName.push(className);
        }
        tempCName.pop();
        if(testfile.is_open()){
            while(getline(testfile, line)){
                FilterWords(line);
                char * lch = new char[line.length()+1];
                strcpy(lch, line.c_str());
                pch = strtok(lch, " ");
                while(pch!=NULL && (strpbrk(pch, "") == NULL)){
                    //cout << "word: "<< pch << endl;
                    if(trainPhase){
                        Vocab.insert(pch);
                    }
                    Ip.insert(pch);
                    pch = strtok (NULL, " ");
                }
                delete [] lch;
            }
            InputWordSet.insert({tempCName.front(),Ip});
            if(trainPhase){
                OutputSet.insert(tempCName.front());
            }
            tempCName.pop();
        }else{
            cout << "unable to open file" << endl;
        }
        testfile.close();
    }
    if(trainPhase){
        auto ite1 = Vocab.find("a");
        auto ite2 = Vocab.find("");    // bunch of empty value i have no idea that came from the test set
        Vocab.erase(Vocab.begin(),ite1);
        Vocab.erase(ite2,Vocab.end());
    }
    cout<< "input size" << InputWordSet.size()<<endl;
    for(auto it = InputWordSet.begin(); it != InputWordSet.end(); it++){
        auto vbeg = Vocab.begin();
        cout <<" setting input set: " <<it->first <<" : "<<(it->second).size()<< endl;
        Eigen::VectorXd inpTrig = Eigen::VectorXd::Zero(Vocab.size());
        for(auto itj = (it->second).begin(); itj != (it->second).end() ; itj++){
            auto vpos = Vocab.find(*itj);
            if(vpos!= Vocab.end()){
                //cout << *vpos << endl;
                inpTrig[distance(vbeg, vpos)] = 1;
            }
        }
        InputSet.insert({it->first,inpTrig});
    }
    
}
