//
//  FeedForwardNN.cpp
//  NeuralNetwork
//
//  Created by Gautham Suriya on 8/2/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include "FeedForwardNN.hpp"
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

unordered_map<string, string> InputSet;

FeedForwardNN::FeedForwardNN(int layer[]){
    cout << "neural net initiated | layers: "<< layer[0] << endl;
    TextProcessor();
    cout << "sample eigen program" << endl;
    MatrixXd m(2,2);
    m(0,0) = 1;
    m(1,0) = 2;
    m(0,1) = 3;
    m(1,1) = 4;
    cout << m << endl;
    cout << m.transpose() << endl;
    
}

void FeedForwardNN::InitNetwork(){
    // inputs-> Vocab
    // init weight -> size of Vocab
    // inner network weight -> specified // single hidden layer for build 1
    // Bias -> hidden layer size
    // OutputBias -> op size -> category.folder size
    // a = sum(wx) + b
    
}

void FeedForwardNN::FeedFwd(){
    
}

void FeedForwardNN::BackProp(){
    
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
void FeedForwardNN::TextProcessor(){
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
    string trainPath = "/Users/gauthamsuriya/Desktop/GauthamDev/AlgorithmLibG/20news-bydate/20news-bydate-train";
    size_t lsize = trainPath.size();
    const char * folder = trainPath.c_str();
    char * pch;
    directoryProbe(folder, &folderset);
    
    for(auto it = folderset.begin(); it!= folderset.end(); it++){
        //cout<< "from vocab begining: "<<*it<<endl;
        if(!tempCName.empty()){
            tempCName.pop();
        }
        ifstream testfile(*it);
        
        //const char * fname = it->c_str();
        words = *it;
        cout << words << endl;
        //remove(words.begin(),words.end(),"/");
        words.erase(words.begin(), words.begin()+lsize);
        cout << words << endl;
        stringstream iss(words);
        while(getline(iss, className, '/')){
            tempCName.push(className);
            cout<< className << endl;
        }
        tempCName.pop();
        cout<< "size of tempCname: " << tempCName.size() << endl;
        words= "";
        if(testfile.is_open()){
            while(getline(testfile, line)){
                FilterWords(line);
                words += line;
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
                    Vocab.insert(pch);
                    pch = strtok (NULL, " ");
                }
                delete [] lch;
            }
            //pair<string,string> inputpair(tempCName.front(),words);
            cout << tempCName.front() << endl;
            cout << words << endl;
            //InputSet.insert({"bla","bla"});
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
    auto ite1 = Vocab.find("a");
    auto ite2 = Vocab.find("");    // bunch of empty value i have no idea that came from the test set
    Vocab.erase(Vocab.begin(),ite1);
    Vocab.erase(ite2,Vocab.end());
    
    
    for(auto itv = Vocab.begin(); itv!= Vocab.end(); itv++){
        //const char * p = itv->c_str();
        cout<<"vocab: "<<*itv<<endl;
        //if(strpbrk(p, "") != NULL){
          //  cout << "found empty "<< endl;
        //}
    }
}
