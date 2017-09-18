//
//  FeedForwardNN.cpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include "FeedForwardNN.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <queue>
#include <dirent.h>
#include <Eigen/Dense>      // Needs to be installed separately. Can use other matrix tools.

using namespace std;
using Eigen::MatrixXd;

unordered_multimap<string, unordered_set<string>> InputSet;// class, features
set<string> OutputSet; // class order

int udTrigger = 0;

double sigmoid(double z){
    return 1/(1+exp(-z));
}

double sigmoidPr(double z){
    return sigmoid(z)*(1-sigmoid(z));
}

FeedForwardNN::FeedForwardNN(int hlayer[], int sz, int learningRate, int batchsize, string dsPath){
    cout << "neural net initiated | layers: "<< hlayer[0] << endl;
    layerSize = 2+sz;
    lRate = learningRate;
    batchS = batchsize;
    udTrigger = batchsize;
    trainPhase = true;
    TextProcessor(dsPath);
    //cout << "sample eigen program" << endl;
    /*MatrixXd m(3,2);
    m(0,0) = 1;
    m(1,0) = 2;
    m(0,1) = 3;
    m(1,1) = 4;
    cout << m << endl;
    cout << m.transpose() << endl;
     auto it = InputSet.begin();
    cout << it->first << endl;
    for(auto itp = (it->second).begin(); itp != (it->second).end() ; itp++){
        cout << *itp << endl;
    }
    
    for(auto ito = OutputSet.begin(); ito!= OutputSet.end(); ito++){
        cout << *ito << endl;
    }*/
    //Eigen::Vector3d v(1,2,3);
    //v = v.unaryExpr(ptr_fun(sigmoid));
    //cout << v << endl;
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
    //Eigen::VectorXd sample = (lnet.find(layerSize-1)->second).bias;
    //cout << "lnet change" << endl;
    //cout << (lnet.find(0)->second).inpx[0] << endl;
    //inpVal[0] = 1;
    //cout << (lnet.find(0)->second).inpx[0] << endl;
    for(auto opIt = OutputSet.begin(); opIt!=OutputSet.end(); opIt++){
        cout << *opIt << endl;
    }
    for(int epoch =0; epoch <10; epoch++){
        cout << "epoch " << epoch+1 << endl;
        Classification("",trainPhase);
    }
}

void FeedForwardNN::Classification(string dsPath,bool train){
    
    if(!train){
        cout << "testing phase: " <<dsPath << endl;
        trainPhase = train;
        InputSet.clear();
        //OutputSet.clear();
        TextProcessor(dsPath);
        for(auto opIt = OutputSet.begin(); opIt!=OutputSet.end(); opIt++){
            cout << *opIt << endl;
        }
    }else{
        cout << "training phase: " <<dsPath <<endl;
    }
    for(auto it = InputSet.begin(); it != InputSet.end(); it++){
        cout << it->first << endl;
        if(trainPhase){
            (lnet.find(layerSize-1)->second).opTarget = Eigen::VectorXd::Zero(OutputSet.size());
            //cout << "inputlayer size" <<(lnet.find(0)->second).inpx.size() << endl;
            auto opbeg = OutputSet.begin();
            auto opos = OutputSet.find(it->first);
        
            if(opos!=OutputSet.end()){
                (lnet.find(layerSize-1)->second).opTarget[distance(opbeg, opos)] = 1;
            
            }else{
                cout << "class name not found" << endl;
            }
        }
        
        (lnet.find(0)->second).inpx = Eigen::VectorXd::Zero(Vocab.size());
        auto vbeg = Vocab.begin();
        for(auto itj = (it->second).begin(); itj != (it->second).end() ; itj++){
            auto vpos = Vocab.find(*itj);
            if(vpos!= Vocab.end()){
                //cout << *vpos << endl;
                (lnet.find(0)->second).inpx[distance(vbeg, vpos)] = 1;
            }
        }
        //cout << (lnet.find(layerSize-1)->second).opTarget.transpose() << endl;
        FeedFwd();
        // Begin FeedFwd
    }
}

void FeedForwardNN::FeedFwd(){
    //find z= s(wx) +b
    // inx//a// = sig(z)
    //cout << "layer size: " << layerSize << endl;
    for(int i =1; i < layerSize ; i++){
        layers * currL = &lnet.find(i)->second;
        layers * prevL = &lnet.find(i-1)->second;
        /*cout << "curr w size: " << currL->weights.size() << endl;
        cout << "prev i size: " << prevL->inpx.size() << endl;
        cout << "curr b size: " << currL->bias.size() << endl;*/
        currL->z = (currL->weights * prevL->inpx) + currL->bias;
        //cout << currL->z.transpose() << "|--------->"<<endl << endl;
        //cout << "feed fwd: "<< i << " | " << currL->z.size() << endl;
        currL->inpx = currL->z.unaryExpr(ptr_fun(sigmoid));
        //cout << currL->inpx.transpose() << endl;
    }
    if(trainPhase){
        BackProp();
    }else{
        Eigen::VectorXd::Index maxI;
        layers * l = &lnet.find(layerSize-1)->second;
        l->inpx.maxCoeff();
        double maxClass = l->inpx.maxCoeff(&maxI);
        cout << "MaxVal: " << maxClass << " MaxPos: " << maxI << "|----->"<<endl;
    }
}

void FeedForwardNN::BackProp(){
    //cout << "backprop" <<endl;
    // find initial cost rate
    //layers * opL = &lnet.find(layerSize-1)->second;
    //layers * npL = &lnet.find(layerSize-2)->second;
    Eigen::VectorXd deltaL;// = (opL->inpx - opL->opTarget).cwiseProduct(opL->z.unaryExpr(ptr_fun(sigmoidPr)));
    //opL->diffb = deltaL;
    //cout << "prev a: "<< npL->inpx.size() << "curr delta: " << deltaL.size() << endl;
    //opL->diffw = npL->inpx * deltaL.transpose();
    
    //cout << opL->diffb.size() << " | " << opL->diffw.size() << endl;
    //cout << "prev" << endl;
    for(int j = layerSize-1; j>0 ; j--){
        //cout << "inside loop" << endl;
        layers * currL = &lnet.find(j)->second;
        layers * prevL = &lnet.find(j-1)->second;
        layers * nextL = &lnet.find(j+1)->second;
        
        if(j==layerSize-1){
            //cout << "begin pass1" << endl;
            deltaL = (currL->inpx - currL->opTarget).cwiseProduct(currL->z.unaryExpr(ptr_fun(sigmoidPr)));
            //cout << "end pass1" << endl;
        }else{
            //cout << "begin pass2" << endl;
            deltaL = ((nextL->weights.transpose())*deltaL).cwiseProduct(currL->z.unaryExpr(ptr_fun(sigmoidPr)));
            //cout << "end pass2" << endl;
        }
        currL->diffb += deltaL;
        //cout << currL->weights << " |----|----->| " << endl;
        currL->diffw += deltaL * prevL->inpx.transpose() ;
        //cout << currL->diffw << " |-----|----->| " << endl;
        //cout << currL->diffb.size() << " | " << currL->diffw.size() << endl;
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
    cout << "batch Update" << endl;
    for(int k = 1; k< layerSize ; k++){
        layers * currL = &lnet.find(k)->second;
        //cout << "probe1" << endl;
        //cout << currL->bias << " | " << currL->diffb << endl;
        Eigen::MatrixXd w = currL->weights - ((lRate/batchS)*currL->diffw);
        //cout << "probe2" << endl;
        currL->bias = currL->bias - currL->diffb;
        //cout << "probe3" << endl;
    }
}
void directoryProbe(string folder, set<string> * Vocab){
    //cout << folder << endl;
    DIR *dirp = opendir(folder.c_str());
    struct dirent * rePt;
    //string fldtmp = folder;
    if(dirp){
        while((rePt = readdir(dirp)) != NULL){
            if( strcmp(rePt->d_name, ".") != 0 && strcmp(rePt->d_name, "..") != 0 && strcmp(rePt -> d_name, ".DS_Store") !=0){// for mac systems :/
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
        //s.erase(remove(s.begin(), s.end(), filter[i]),s.end());
        replace(s.begin(),s.end(),filter[i],' ');
    }
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    
    //cout << s << endl;
}
void FeedForwardNN::TextProcessor(string dsPath){
    
    cout << "trainphase "<< trainPhase << endl;
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
    
    for(auto it = folderset.begin(); it!= folderset.end(); it++){
        //cout<< "file: "<<*it<<endl;
        unordered_set<string> Ip;
        if(!tempCName.empty()){
            tempCName.pop();
        }
        ifstream testfile(*it);
        
        //const char * fname = it->c_str();
        words = *it;
        //cout << words << endl;
        //remove(words.begin(),words.end(),"/");
        words.erase(words.begin(), words.begin()+lsize);
        //cout << words << endl;
        stringstream iss(words);
        while(getline(iss, className, '/')){
            tempCName.push(className);
            //cout<< className << endl;
        }
        tempCName.pop();
        //cout<< "size of tempCname: " << tempCName.size() << endl;
        //words= "";
        if(testfile.is_open()){
            while(getline(testfile, line)){
                FilterWords(line);
                //words += line;
                /*stringstream iss(line);
                while(getline(iss,words,' ')){
                    Vocab.insert(words);
                }*/
                //cout<< line << endl;
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
            //pair<string,string> inputpair(tempCName.front(),words);
            //cout << tempCName.front() << endl;
            //cout << words << endl;
            InputSet.insert({tempCName.front(),Ip});
            if(trainPhase){
                OutputSet.insert(tempCName.front());
            }
            //cout << tempCName.front() << endl;
            tempCName.pop();
        }else{
            cout << "unable to open file" << endl;
        }
        testfile.close();
    }
    //auto it = Vocab.find(" ");
    
   // const char * p = it->c_str();
    
    //cout << p << endl;
    //cout<< "vocab list: "<<endl;
   /* if(it!=Vocab.end()){
        cout << "found: "<<*it << endl;
    }else{
        cout<< "end of list reached" << endl;
    }*/
    if(trainPhase){
        auto ite1 = Vocab.find("a");
        auto ite2 = Vocab.find("");    // bunch of empty value i have no idea that came from the test set
        Vocab.erase(Vocab.begin(),ite1);
        Vocab.erase(ite2,Vocab.end());
    }
    
    /*for(auto itv = Vocab.begin(); itv!= Vocab.end(); itv++){
        //const char * p = itv->c_str();
        cout<<"vocab: "<<*itv<<endl;
        //if(strpbrk(p, "") != NULL){
          //  cout << "found empty "<< endl;
        //}
    }*/
}
