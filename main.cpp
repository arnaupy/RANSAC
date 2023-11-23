#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "includes/logFunctionalities.h"
#include "includes/fileFunctionalities.h"
#include "includes/linearRANSAC.h"
#include "includes/utils.h"

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc == 1)
    {
        throw std::invalid_argument("Error: use -h for help.");
    }

    if (std::string(argv[1]) == "-h")
    {
        std::cout << "Usage: <dataFilename> <parametersFilename> [OPTIONS]\n";
        std::cout << "\nOptions\n\n";
        std::cout
            << "\t-d     detachMode | Logs will not be printed in screen.\n";
        std::cout
            << "\t-l     logsMode   | Logs will be stored in 'logs.txt' file.\n";
        return 1;
    }

    if (argc < 3)
    {
        throw std::invalid_argument("Error: use -h for help.");
    }

    enum Modes
    {
        detachMode_ = 'd',
        logsMode_ = 'l'
    };
    bool detachMode = false;
    bool logsMode = false;

    for (int i = 3; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case detachMode_:
                detachMode = true;
                break;
            case logsMode_:
                logsMode = true;
                break;
            default:
                throw std::invalid_argument("Error: option do not exists");
            }
        }
        else
        {
            throw std::invalid_argument("Error: invalid terminal option format.");
        }
    }

    MessagePrinter system("logs.txt", detachMode, logsMode);

    system.print("\n-----------------------| Linear RANSAC algorithm |-----------------------\n", true);

    std::string dataDirectory = "Data";
    system.print("Data directory: ");
    system.print(dataDirectory, true);

    std::string parametersDirectory = "Parameters";
    system.print("Parameters directory: ");
    system.print(parametersDirectory, true);

    std::string simDirectory = "Simulations";
    system.print("Simulations directory: ");
    system.print(simDirectory, true);

    system.print("\nParameters:\n", true);
    system.print("\t*nSamples        ");
    system.print("size of random samples.", true);
    system.print("\t*threshold       ");
    system.print("inliers count condition. 0. (value estimated by the algorithm)", true);
    system.print("\t*learning rate   ");
    system.print("steps rate to perform the gradient descent.", true);
    system.print("\t*costFunction    ");
    system.print("function used for counting inliers. 0 (absolute error), 1 (mean square error)", true);

    system.print("done", true);

    std::string dataFilename = argv[1];
    system.print("\nData filename: ");
    system.print(dataFilename, true);

    system.print("searching for data file...");
    std::ifstream dataFile(dataDirectory + "/" + dataFilename);
    if (!dataFile.is_open())
    {
        throw std::invalid_argument("Error: Data file not found");
    }
    system.print("done", true);

    system.print("reading data...");
    int nData = 0;
    double auxVar;
    while (dataFile)
    {
        dataFile >> auxVar >> auxVar;
        nData++;
    }
    nData--;
    dataFile.close();

    Data2D data(nData);
    dataFile.open(dataDirectory + "/" + dataFilename);
    for (int i = 0; i < nData; i++)
    {
        dataFile >> data.x[i] >> data.y[i];
    }
    dataFile.close();
    system.print("done\n", true);

    std::string parametersFilename = argv[2];
    system.print("\nParameters filename: ");
    system.print(parametersFilename, true);

    system.print("searching for parameters file...");
    std::ifstream parametersFile(parametersDirectory + "/" + parametersFilename);

    if (!parametersFile.is_open())
    {
        throw std::invalid_argument("Error: Parameters file not found");
    }
    system.print("done", true);

    system.print("reading parameters...");
    std::string line;
    int numSim;
    while (parametersFile.is_open())
    {
        parametersFile >> line;
        if ((char)line[0] == '$')
        {
            numSim = line[1] - '0';
            parametersFile.close();
        }
    }

    int nSamples[numSim];
    double threshold[numSim];
    double lr[numSim];
    int auxCostFunction;
    CostFunctions costFunction[numSim];

    int i;
    parametersFile.open(parametersDirectory + "/" + parametersFilename);
    while (std::getline(parametersFile, line))
    {
        if ((char)line[0] == '$')
        {
            continue;
        }
        std::istringstream iss(line);
        iss >> i >> nSamples[i] >> threshold[i] >> lr[i] >> auxCostFunction;
        costFunction[i] = static_cast<CostFunctions>(auxCostFunction);
    }
    parametersFile.close();
    system.print("done\n", true);
    system.print(numSim);
    system.print(" simulations to make\n", true);

    std::string simFilename;
    for (int i = 0; i < numSim; i++)
    {
        system.print("Sim ");
        system.print(i);
        system.print("...");
        simFilename = getSimFilename(dataFilename, parametersFilename, i);
        linearRANSAC(data, simDirectory + "/" + simFilename, nSamples[i], threshold[i], lr[i], costFunction[i]);
        system.print("done", true);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    system.print("\nExecution time: ");
    system.print(elapsed_seconds.count());
    system.print("sec", true);

    system.print("\n-------------------------------------------------------------------------", true);
    system.close();
    return 0;
}