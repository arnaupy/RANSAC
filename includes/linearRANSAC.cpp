#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "utils.h"

int linearRANSAC(Data2D data,
                 std::string resultsFilepath,
                 int nSamples,
                 double threshold,
                 double lr,
                 CostFunctions costFunction)
{

    // $%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%
    //                                       Parameters Definition
    // $%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%$%

    std::ofstream resultsFile(resultsFilepath);

    int nData = data.size;

    // Number of times the samples are generated and evaluated
    const int maxIter = 1000;

    // Number of times each sample is trained
    const int maxIterLR = 10000;

    // Get the threshold estimation by the Median Absolute Deviation
    if (threshold == 0)
    {
        threshold = calculateMAD(data.y, nData);
    }
    resultsFile << "$threshold\n"
                << threshold << "\n";

    // Initialize variabbles
    Data2D sampleData(nSamples);
    Data2D currentInliers(nData);
    Data2D bestInliers(nData);
    bestInliers.size = 0;
    LinearRegressionModel currentModel;
    LinearRegressionModel bestModel;
    double *yPred = new double[nData];
    double error;

    for (int i = 0; i < maxIter; i++)
    {
        // Get random data
        fillRandom(sampleData, data);

        // Perform linear regression with `sampleData` points
        currentModel.train(sampleData, lr, maxIterLR);

        // Count inliers over the entire data set
        yPred = currentModel.predict(data.x, nData);
        currentInliers.size = 0;
        for (int j = 0; j < nData; j++)
        {
            if (costFunction == absoluteValue)
            {
                error = abs(yPred[j] - data.y[j]);
            }
            else
            {
                error = pow(yPred[j] - data.y[j], 2);
            }
            if (error <= threshold)
            {
                currentInliers.size++;
                currentInliers.x[currentInliers.size - 1] = data.x[j];
                currentInliers.y[currentInliers.size - 1] = data.y[j];
            }
        }

        // Decide the best model by the number of inliers and write results
        if (currentInliers.size > bestInliers.size)
        {
            bestInliers.replace(currentInliers);
            bestModel.updateParameters(currentModel.m, currentModel.b);
        }
    }

    // Perform linear regression with the best model
    bestModel.train(bestInliers, lr, maxIterLR);

    resultsFile << "$nInliers "
                << "m "
                << "b\n"
                << bestInliers.size << " " << bestModel.m << " " << bestModel.b << "\n";

    // Write best inliers
    resultsFile << "$x "
                << "y\n";
    for (int i = 0; i < bestInliers.size; i++)
    {
        resultsFile << bestInliers.x[i] << " " << bestInliers.y[i] << "\n";
    }

    resultsFile.close();
    return 0;
}
