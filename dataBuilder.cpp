#include <iostream>
#include <fstream>
#include <stdexcept>
#include <random>

double *range(double min, double max, int nSamples);
double *linear(double *x, int size, double m, double b);
double max(double *array, int size);
double min(double *array, int size);

int main(int argc, char *argv[])
{

    // ---------------------------------| Read terminal arguments |---------------------------------

    if (argc == 1)
    {
        throw std::invalid_argument("Error: use -h for help.");
    }
    if (std::string(argv[1]) == "-h")
    {
        std::cout << "Usage: <dataFilename>(without '.txt') <nPoints>(int) <nInliers>(int)"
                  << "<noise>(double) <m>(double) <b>(double)\n";
        return 1;
    }

    if (argc != 6 + 1)
    {
        throw std::invalid_argument("Error: use -h for help.");
    }

    const std::string dataFilepath = "Data";
    const std::string dataFilename = dataFilepath + "/" + argv[1] + ".txt";
    int nInliers;
    int nOutliers;
    double noise;
    double m;
    double b;
    const double xMin = -10.;
    const double xMax = 10.;

    sscanf(argv[2], "%i", &nInliers);
    sscanf(argv[3], "%i", &nOutliers);
    sscanf(argv[4], "%lf", &noise);
    sscanf(argv[5], "%lf", &m);
    sscanf(argv[6], "%lf", &b);

    // ----------------------------------------| Store Data |---------------------------------------

    // Set random number generator according to a normal distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(0.0, 1.0);

    // Calculate x and y points according to m and b
    double *xInliers = range(xMin, xMax, nInliers);
    double *yInliers = linear(xInliers, nInliers, m, b);

    // Apply noise to inliers
    for (int i = 0; i < nInliers; i++)
    {
        yInliers[i] = yInliers[i] + noise * distribution(gen);
    }

    // Store inliers
    std::ofstream dataFile(dataFilename);
    for (int i = 0; i < nInliers; i++)
    {
        dataFile << xInliers[i] << " " << yInliers[i] << "\n";
    }

    // Store outlier
    double xOutlier;
    double yOutlier;
    double yGap = max(yInliers, nInliers) - min(yInliers, nInliers);
    double xGap = xMax - xMin;
    double yMin = min(yInliers, nInliers);

    const int steps = 100000;
    for (int i = 0; i < nOutliers; i++)
    {
        xOutlier = (((rand() % steps) / (double)steps) * xGap + xMin);
        yOutlier = ((rand() % 100) / 100.) * yGap + yMin;
        dataFile << xOutlier << " " << yOutlier << "\n";
    }
    dataFile.close();
    return 0;
}

double *range(double min, double max, int nSamples)
{
    double *array = new double[nSamples];
    for (int i = 0; i < nSamples; i++)
    {
        array[i] = min + ((max - 1 - min) / (nSamples - 1)) * i; // Assuming that fin > ini & n > 1
    }
    return array;
}
double *linear(double *x, int size, double m, double b)
{
    double *y = new double[size];
    for (int i = 0; i < size; i++)
    {
        y[i] = m * x[i] + b;
    }
    return y;
}
double max(double *array, int size)
{
    double max = array[0];
    for (int i = 1; i < size; i++)
        if (array[i] > max)
            max = array[i];
    return max;
}
double min(double *array, int size)
{
    double min = array[0];
    for (int i = 1; i < size; i++)
        if (array[i] < min)
            min = array[i];
    return min;
}