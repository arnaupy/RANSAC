#include <iostream>

std::string getSimFilename(std::string dataFilename, std::string parametersFilename, int simNumber)
{
    std::string currentString = "Sim__";

    std::string dataFormat = ".txt";
    currentString += dataFilename.substr(0, dataFilename.length() - dataFormat.length()) + "__";

    std::string parametersAlias = "parameters_";
    parametersFilename.erase(0, parametersAlias.length());
    currentString += parametersFilename.substr(0, parametersFilename.length() - dataFormat.length()) + "_";

    currentString += std::to_string(simNumber) + ".txt";
    return currentString;
}