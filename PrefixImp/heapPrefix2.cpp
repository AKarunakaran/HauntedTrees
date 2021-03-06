//#include <Rcpp.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;
//using namespace Rcpp;

// [[Rcpp::export]]
void swapInt(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// [[Rcpp::export]]
void swapDouble(double& a, double& b) {
    double temp = a;
    a = b;
    b = temp;
}

//Fix up for min heap, starting from index k
// [[Rcpp::export]]
void fixUpMin(std::vector<int>& heapValue, std::vector<double>& heapWeight, int k) {
    while(k && heapValue[k/2] > heapValue[k]) {
        swapInt(heapValue[k/2], heapValue[k]);
        swapDouble(heapWeight[k/2], heapWeight[k]);
        k /= 2;
    }
}

//Fix up for max heap, starting from index k
// [[Rcpp::export]]
void fixUpMax(std::vector<int>& heapValue, std::vector<double>& heapWeight, int k) {
    while(k && heapValue[k/2] < heapValue[k]) {
        swapInt(heapValue[k/2], heapValue[k]);
        swapDouble(heapWeight[k/2], heapWeight[k]);
        k /= 2;
    }
}

//Fix down for min heap, starting from index k
// [[Rcpp::export]]
void fixDownMin(std::vector<int>& heapValue, std::vector<double>& heapWeight, int k) {
    int heapSize = heapValue.size();
    while(2*k+1 < heapSize && (heapValue[2*k+1] > heapValue[k] || (2*k+2 < heapSize && heapValue[2*k+2] > heapValue[k]))) {
        if(2*k+2 < heapSize) {
            if(heapValue[2*k+1] < heapValue[2*k+2]) {
                swap(heapValue[2*k+1], heapValue[k]);
                swap(heapWeight[2*k+1], heapWeight[k]);
                k = 2*k+1;
            } else {
                swap(heapValue[2*k+2], heapValue[k]);
                swap(heapWeight[2*k+2], heapWeight[k]);
                k = 2*k+2;
            }
        } else {
            swap(heapValue[2*k+1], heapValue[k]);
            swap(heapWeight[2*k+1], heapWeight[k]);
            k = 2*k+1;
        }
    }
}

//Fix down for the max heap, starting from index k
// [[Rcpp::export]]
void fixDownMax(std::vector<int>& heapValue, std::vector<double>& heapWeight, int k) {
    int heapSize = heapValue.size();
    while(2*k+1 < heapSize && (heapValue[2*k+1] < heapValue[k] || (2*k+2 < heapSize && heapValue[2*k+2] < heapValue[k]))) {
        if(2*k+2 < heapSize) {
            if(heapValue[2*k+1] > heapValue[2*k+2]) {
                swap(heapValue[2*k+1], heapValue[k]);
                swap(heapWeight[2*k+1], heapWeight[k]);
                k = 2*k+1;
            } else {
                swap(heapValue[2*k+2], heapValue[k]);
                swap(heapWeight[2*k+2], heapWeight[k]);
                k = 2*k+2;
            }
        } else {
            swap(heapValue[2*k+1], heapValue[k]);
            swap(heapWeight[2*k+1], heapWeight[k]);
            k = 2*k+1;
        }
    }
}

// [[Rcpp::export]]
void addMin(std::vector<int>& heapValue, std::vector<double>& heapWeight, int toAddVal, double toAddWeight) {
    heapValue.push_back(toAddVal);
    heapWeight.push_back(toAddWeight);
    fixUpMin(heapValue, heapWeight, heapValue.size()-1);
}

// [[Rcpp::export]]
void addMax(std::vector<int>& heapValue, std::vector<double>& heapWeight, int toAddVal, double toAddWeight) {
    heapValue.push_back(toAddVal);
    heapWeight.push_back(toAddWeight);
    fixUpMax(heapValue, heapWeight, heapValue.size()-1);
}

// [[Rcpp::export]]
void popMin(std::vector<int>& heapValue, std::vector<double>& heapWeight) {
    heapValue[0] = heapValue.back();
    heapWeight[0] = heapWeight.back();
    heapValue.pop_back();
    heapWeight.pop_back();
    fixDownMin(heapValue, heapWeight, 0);
}

// [[Rcpp::export]]
void popMax(std::vector<int>& heapValue, std::vector<double>& heapWeight) {
    heapValue[0] = heapValue.back();
    heapWeight[0] = heapWeight.back();
    heapValue.pop_back();
    heapWeight.pop_back();
    fixDownMax(heapValue, heapWeight, 0);
}

// [[Rcpp::export]]
void addToLevelSet(std::vector<int>& ltHeapValue, std::vector<double>& ltHeapWeight,
      std::vector<int>& gtHeapValue, std::vector<double>& gtHeapWeight, int& medVal,
      double& medWeight, double& ltWeight, double& gtWeight, int toAddVal, double toAddWeight) {
    if(toAddVal < medVal) {
        ltWeight += toAddWeight;
        addMax(ltHeapValue, ltHeapWeight, toAddVal, toAddWeight);
    } else {
        gtWeight += toAddWeight;
        addMin(gtHeapValue, gtHeapWeight, toAddVal, toAddWeight);
    }
    double totalWeight = medWeight + ltWeight + gtWeight;
    while(ltWeight > totalWeight/2 && gtWeight > totalWeight/2) {
        if(ltWeight > gtWeight) {
            addMin(gtHeapValue, gtHeapWeight, medVal, medWeight);
            gtWeight += medWeight;
            medVal = ltHeapValue[0];
            medWeight = ltHeapWeight[0];
            ltWeight -= ltHeapWeight[0];
            popMax(ltHeapValue, ltHeapWeight);
        } else {
            addMax(ltHeapValue, ltHeapWeight, medVal, medWeight);
            ltWeight += medWeight;
            medVal = gtHeapValue[0];
            medWeight = gtHeapWeight[0];
            gtWeight -= gtHeapWeight[0];
            popMin(gtHeapValue, gtHeapWeight);
        }
    }
}

// [[Rcpp::export]]
void merge(std::vector<int>& ltHeapValue_a, std::vector<double>& ltHeapWeight_a, std::vector<int>& gtHeapValue_a, std::vector<double>& gtHeapWeight_a,
      int& medVal_a, double& medWeight_a, double& ltWeight_a, double& gtWeight_a, std::vector<int>& ltHeapValue_b, std::vector<double>& ltHeapWeight_b,
      std::vector<int>& gtHeapValue_b, std::vector<double>& gtHeapWeight_b, int& medVal_b, double& medWeight_b) {
    for(int i = 0; i < ltHeapValue_b.size(); ++i) {
        addToLevelSet(ltHeapValue_a, ltHeapWeight_a, gtHeapValue_a, gtHeapWeight_a, medVal_a,
          medWeight_a, ltWeight_a, gtWeight_a, ltHeapValue_b[i], ltHeapWeight_b[i]);
    }
    for(int i = 0; i < gtHeapValue_b.size(); ++i) {
        addToLevelSet(ltHeapValue_a, ltHeapWeight_a, gtHeapValue_a, gtHeapWeight_a, medVal_a,
          medWeight_a, ltWeight_a, gtWeight_a, gtHeapValue_b[i], gtHeapWeight_b[i]);
    }
    addToLevelSet(ltHeapValue_a, ltHeapWeight_a, gtHeapValue_a, gtHeapWeight_a, medVal_a,
          medWeight_a, ltWeight_a, gtWeight_a, medVal_b, medWeight_b);
}

// [[Rcpp::export]]
std::vector<int> prefixIsoRegL1(const std::vector<int>& ycoords,
                                       const std::vector<double>& weights, std::vector<double>& error) {
    size_t data_size = ycoords.size();

    std::vector<std::vector<int>> ltHeapValues;
    std::vector<std::vector<double>> ltHeapWeights;

    std::vector<std::vector<int>> gtHeapValues;
    std::vector<std::vector<double>> gtHeapWeights;

    std::vector<int> medValues;
    std::vector<double> medWeights;

    std::vector<double> ltWeights;
    std::vector<double> gtWeights;

    std::vector<int> lefts;
    
    for (int i = 0; i < data_size; ++i) {
    	//initialize level sets
        ltHeapValues.push_back({});
        ltHeapWeights.push_back({});
        gtHeapValues.push_back({});
        gtHeapWeights.push_back({});
        medValues.push_back(ycoords[i]);
        medWeights.push_back(1);
        ltWeights.push_back(0);
        gtWeights.push_back(0);
        lefts.push_back(i);
        
        int back = medValues.size()-1;
        int stBack = medValues.size()-2; //second to back
        while (back > 0 &&
               medValues[back] <= medValues[stBack]) {

            //merge the level sets

            merge(ltHeapValues[stBack], ltHeapWeights[stBack], gtHeapValues[stBack],
                gtHeapWeights[stBack], medValues[stBack], medWeights[stBack],
                ltWeights[stBack], gtWeights[stBack], ltHeapValues[back],
                ltHeapWeights[back], gtHeapValues[back], gtHeapWeights[back],
                medValues[back], medWeights[back]);

            ltHeapValues.pop_back();
            ltHeapWeights.pop_back();
            gtHeapValues.pop_back();
            gtHeapWeights.pop_back();
            medValues.pop_back();
            medWeights.pop_back();
            ltWeights.pop_back();
            gtWeights.pop_back();
            lefts.pop_back();

            --back;
            --stBack;
        }
        
    }

    std::vector<int> reg_data(data_size);

    size_t curLSet = 0;
    reg_data[0] = medValues[0];
    error[0] = abs(ycoords[0]-reg_data[0]);
    for(size_t i = 1; i < data_size; ++i) {
        if(curLSet != medValues.size()-1 && lefts[curLSet+1] == i) {
            ++curLSet;
        }
        reg_data[i] = medValues[curLSet];
        error[i] = error[i-1] + abs(ycoords[i]-reg_data[i]);
    }
    
    return reg_data;
    
}

// [[Rcpp::export]]
std::vector<int> unimodalRegL1(std::vector<int>& ycoords, std::vector<double>& weights) {
    size_t size = ycoords.size();


    std::vector<double> incErrors(size, 0);
    std::vector<int> increasing = prefixIsoRegL1(ycoords, weights, incErrors);
    
    //reverse std::vectors, then call prefix L1 on it again
    reverse(ycoords.begin(), ycoords.end());
    reverse(weights.begin(), weights.end());

    std::vector<double> decErrors(size, 0);
    std::vector<int> decreasing = prefixIsoRegL1(ycoords, weights, decErrors);
    
    //now find the best point for the mode, and copy the data used in
    //the optimal unimodal regression into a new std::vector
    
    //initialized to be the min of fully increasing or fully decreasing
    double min_error = min(decErrors.back(), incErrors.back());
    //if all are decreasing, set last increasing to be size to denote none
    size_t last_increasing = min_error == incErrors.back() ? size - 1 : size;
    
    //the prefix regression did the decreasing regression in reverse order,
    //so instead of reversing the resulting std::vector we just go from the back
    for (size_t i = 0; i < size - 2; ++i) {
        double error_at_i = incErrors[i] + decErrors[size - 2 - i];
        if (min_error > error_at_i) {
            min_error = error_at_i;
            last_increasing = i;
        }
    }

    if (last_increasing == size) {
        return decreasing;
    }
    else if (last_increasing == size - 1) {
        return increasing;
    }
    
    std::vector<int> unimodal(size);

    for (size_t i = 0; i <= last_increasing; ++i) {
        unimodal[i] = increasing[i];
    }
    for (size_t i = last_increasing + 1; i < size; ++i) { //double check bounds here***
        unimodal[i] = decreasing[size - 1 - i];
    }

    return unimodal;

}

int main() {
    ifstream in;
    in.open("uniRegData1.txt");
    size_t size;
    in >> size;
    std::vector<int> ys(size);
    std::vector<double> ws(size);
    size_t i = 0;
    while (in >> ys[i] >> ws[i]) {
        ++i;
    }
    
    std::vector<int> unimodal_data = unimodalRegL1(ys, ws);
    
    cout <<"\tmean values:\n";
    for (size_t i = 0; i < size; ++i) {
        cout << unimodal_data[i] << ' ';
    }
    /*cout << endl << "\terror values:\n";
    for (size_t i = 0; i < size; ++i) {
        cout << errors[i] << ' ';
    }
    */cout << endl;
    
    return 0;
}


