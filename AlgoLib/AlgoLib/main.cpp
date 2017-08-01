//
//  main.cpp
//  AlgoLib
//
//  Created by Gautham Suriya on 7/28/17.
//  Copyright © 2017 GauthamSuriya. All rights reserved.
//

#include <iostream>

using namespace std;

void printArray(int *input, int size){

    for(int i= 0 ; i < size ; i++){
        cout << " " << input[i];
    }
    cout << "" << endl;
    cout.flush();
}

void merge(int *input, int ls, int rs, int re){
    int p[rs -ls];
    int q[re - rs+1];
    
    copy(input+ls, input+rs, p);
    //printArray(p, rs-ls);
    
    copy(input+rs, input+re+1, q);
    //printArray(q, re-rs+1);
    
    int countP = 0; int countQ= 0;
    
    for( int i = ls; i <= re; i++){
        
       //cout<< "coutp: " <<countP << " countQ: "<<countQ <<endl;
        
        if((countP < rs - ls) && (countQ < (re - rs + 1))){
            
            if(p[countP] < q[countQ]){
             
             input[i] = p[countP];
             
             countP++;
             
             }else{
             
             input[i] = q[countQ];
             
             countQ++;
             
             }
           // cout<< "replaced val: " << input[i] << endl;
        }else if (countP >= rs - ls && countQ < re - rs + 1){
            
            input[i] = q[countQ];
            //cout<< "replaced val Q: " << q[countQ] << endl;
            countQ++;
            
        }else if(countP < rs - ls && countQ >= re - rs + 1){
            
            input[i] = p[countP];
            //cout<< "replaced val P: " << p[countP] << endl;
            countP++;
            
        }else{
            cout<< "error in code" <<endl;
        }
        
        
    }
}


void sortMerge(int *input,int p,int endP){ // array, blockStart, end
    if(p < endP){
        int midpt = (p + endP)/2;
        //cout<< "midpt " << midpt <<endl;
        
    
        sortMerge(input , p, midpt);
        sortMerge(input, midpt+1 , endP);
        
        //cout<< "p: "<< p <<"midpt: "<< midpt<< "endP: "<< endP<<endl;
        
        merge(input, p, midpt+1, endP);
    }
    
}
void sortInsertion(int *input, int size){

    //cout << "input pointer" << *(++input) << endl;
    for(int i=1; i < size ; i++){
        int key = input[i];
        for (int j = 0; j < i ; j++){
            if(input[j] > key){
                int temp = input[j];
                input[j] = key;
                key = temp;
            }
        }
        input[i] = key;
    }
    
    cout << "sort insertion : sorted list ->" << endl;
}


int main() {
    
    int option = 0;

    do {
        cout<< "select the sorting method \n 0. End Program \n 1. Insertion sort \n 2. Merge sort" << endl;
        cin >>option;
        
        int input[] = {12,9,11,3,16,5,10,13,6,14,2,15,7,1,8,4};
        int size = sizeof(input)/ sizeof(*input);
        
        printArray(input, size);
        
        switch (option) {
            case 1:
                sortInsertion(input,size);
                break;
            case 2:
                 sortMerge(input,0, size-1);
                 break;
            default:
                break;
       
        }
        printArray(input, size);
        
     }while (option > 0);
    
    return 0;
}
