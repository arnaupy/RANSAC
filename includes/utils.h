#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <algorithm>
#include <cmath>

double *linear(double *x, int size, double m, double b);
double linear(double x, double m, double b);
bool *getRandomIndexs(int nIndexs, int size);
double median(double array[], int size);
double calculateMAD(double arrayOriginal[], int size);
class Data2D
{
public:
    int size;
    double *x;
    double *y;
    Data2D(int size);
    void replace(Data2D replaceData);
};

void fillRandom(Data2D dataFill, Data2D dataFrom);

enum CostFunctions
{
    absoluteValue = 0,
    meanSquareError = 1
};

class LinearRegressionModel
{
public:
    double m;
    double b;
    double predict(double x);
    double *predict(double *x, int size);
    double calculateDm(Data2D trainData);
    double calculateDb(Data2D trainData);
    void updateParameters(double m, double b);
    void train(Data2D trainData, double lr, int maxIter);
};

#endif
