#include <iostream>
#include <algorithm>
#include <cmath>
#include "utils.h"

double *linear(double *x, int size, double m, double b)
{
    double *y = new double[size];
    for (int i = 0; i < size; i++)
    {
        y[i] = m * x[i] + b;
    }
    return y;
}
double linear(double x, double m, double b)

{
    return m * x + b;
}
bool *getRandomIndexs(int nIndexs, int size)
{
    bool *randomIndexsArray = new bool[size];
    std::fill(randomIndexsArray, randomIndexsArray + size, false);

    int i = 0;
    int index;
    while (i < nIndexs)
    {
        index = std::rand() % size;
        if (!randomIndexsArray[index])
        {
            randomIndexsArray[index] = true;
            i++;
        }
    }
    return randomIndexsArray;
}
double median(double array[], int size)
{
    double *copyArray = new double[size];
    for (int i = 0; i < size; i++)
    {
        copyArray[i] = array[i];
    }

    std::sort(copyArray, copyArray + size);
    if (size % 2 == 0)
    {
        return (copyArray[size / 2] + copyArray[size / 2 - 1]) / 2;
    }
    else
    {
        return copyArray[(size - 1) / 2];
    }
}
double calculateMAD(double arrayOriginal[], int size)
{
    // MAD(data) = median(abs(y - median(y)))
    double dataMedian = median(arrayOriginal, size);
    double *arrayTemporal = new double[size];

    for (int i = 0; i < size; i++)
    {
        arrayTemporal[i] = abs(arrayOriginal[i] - dataMedian);
    }
    return median(arrayTemporal, size);
}

Data2D::Data2D(int size)
{
    this->size = size;
    x = new double[size];
    y = new double[size];
}

void Data2D::replace(Data2D replaceData)
{
    for (int i = 0; i < replaceData.size; i++)
    {
        x[i] = replaceData.x[i];
        y[i] = replaceData.y[i];
    }
    size = replaceData.size;
}

void fillRandom(Data2D dataFill, Data2D dataFrom)
{

    bool *randomIndexsArray;
    randomIndexsArray = getRandomIndexs(dataFill.size, dataFrom.size);

    for (int i = 0, k = 0; i < dataFrom.size; i++)
    {
        if (randomIndexsArray[i] == true)
        {
            dataFill.x[k] = dataFrom.x[i];
            dataFill.y[k] = dataFrom.y[i];
            k++;
        }
    }
}

double LinearRegressionModel::predict(double x)
{
    return linear(x, m, b);
}
double *LinearRegressionModel::predict(double *x, int size)
{
    return linear(x, size, m, b);
}
double LinearRegressionModel::calculateDm(Data2D trainData)
{
    double result = 0.;
    for (int i = 0; i < trainData.size; i++)
    {
        result = result + trainData.x[i] * (trainData.x[i] * m + b - trainData.y[i]);
    }
    result = (2 / (double)trainData.size) * result;
    return result;
}
double LinearRegressionModel::calculateDb(Data2D trainData)
{
    double result = 0.;
    for (int i = 0; i < trainData.size; i++)
    {
        result = result + trainData.x[i] * m + b - trainData.y[i];
    }
    result = (2 / (double)trainData.size) * result;
    return result;
}
void LinearRegressionModel::updateParameters(double m, double b)
{
    this->m = m;
    this->b = b;
}
void LinearRegressionModel::train(Data2D trainData, double lr, int maxIter)
{
    double dm;
    double db;
    m = (std::rand() % 100) / 100.;
    b = (std::rand() % 100) / 100.;

    for (int i = 0; i < maxIter; i++)
    {
        dm = calculateDm(trainData);
        db = calculateDb(trainData);

        m = linear(dm, -lr, m);
        b = linear(db, -lr, b);
        updateParameters(m, b);
    }
};
