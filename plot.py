import glob
import matplotlib.pyplot as plt
import numpy as np
import sys

SIMULATIONS_DIR:str = "Simulations/"
PARAMETERS_DIR:str = "Parameters/"
DATA_DIR:str = "Data/"

plt.rcParams["font.family"] = "serif"

# parameters file format: parameters_i.txt
# simulation file format: Sim_dataFileName_i_j.txt

class Results:
    def __init__(self, dataFileName:str, parametersFileName:str):
        self.dataFileName:str = dataFileName
        self.dataName:str = dataFileName.removesuffix(".txt")
        self.dataFilePath:str  = DATA_DIR + dataFileName
        
        self.parametersFileName:str  = parametersFileName
        self.parametersFilePath:str  = PARAMETERS_DIR + parametersFileName
        self.parametersNum:str = parametersFileName.removesuffix(".txt").split("_")[-1]
        
        self.parameters:dict = {"nSamples": list(), "threshold": list(), "lr": list(), "countFunction": list()}
        self.data:dict = {"x": list(), "y": list()}
        self.numSim:int
        
        self.simulationsFilepaths:list
        self.simulationsData:dict
        
        # Read data file (x, y)
        with open(self.dataFilePath, "r") as file:
            lines = file.readlines()
            for line in lines:
                x, y = line.strip("\n").split(" ")
                self.data["x"].append(float(x))
                self.data["y"].append(float(y))
        
        # Read parameters ($3 nSamples threshold lr countFunction)
        with open(self.parametersFilePath, "r") as file:
            lines = file.readlines()     
            for line in lines:
                if line[0] == "$":
                    self.numSim = int(line[1]) 
                    continue
                splitedLines = line.strip("\n").split(" ")
                self.parameters["nSamples"].append(int(splitedLines[1]))
                self.parameters["threshold"].append(float(splitedLines[2]))
                self.parameters["lr"].append(float(splitedLines[3]))
                self.parameters["countFunction"].append(int(splitedLines[4]))
               
        # Store simulation paths and sort them by parameter num (Sim_<dataFilename>_<paramsNum>_<paramsOrder>)        
        simFiles = glob.glob(f"{SIMULATIONS_DIR}/Sim__{self.dataName}__*.txt")
        self.simulationsFilepaths = [file for file in simFiles if file.split("__")[-1].split("_")[0] == self.parametersNum]
        self.simulationsFilepaths = sorted(self.simulationsFilepaths, key = lambda x: x.removesuffix(".txt").split("__")[-1].split("_")[1])
                
        # Read simulations results($threshold | $nInliers m b | $x y)
        self.simulationsData = {"threshold": list(),
                                "nInliers": list(),
                                "m": list(),
                                "b": list(),
                                "x": [[] for _ in range(self.numSim)],
                                "y": [[] for _ in range(self.numSim)]}

        for i, simulationFile in enumerate(self.simulationsFilepaths):
            with open(simulationFile, "r") as file:
                lines = file.readlines()
                for line in lines:
                    if line[0] == "$":
                        currentVars = line.strip("\n").strip("$").split(" ")
                    else:
                        varValues = line.strip("\n").split(" ")
                        for j, var in enumerate(currentVars):
                            if currentVars == ["x", "y"]:
                                self.simulationsData[var][i].append(float(varValues[j]))
                            else:
                                self.simulationsData[var].append(float(varValues[j]))
            

def plotSingleSimulation(results:Results, simNum:int, title:str):
    # Plot data
    plt.scatter(results.data["x"], results.data["y"], color = "gray", edgecolors = None, alpha = 0.2)
    
    # Plot Inliers
    plt.scatter(results.simulationsData["x"][simNum], results.simulationsData["y"][simNum], color = "black")
    
    # Title
    plt.title(title, fontsize = 17, pad = 10)
    
    # Model linear regression
    m = results.simulationsData["m"][simNum]
    b = results.simulationsData["b"][simNum]
    # plt.axline((0, b), slope = m, label = f"RANSAC | m: {m:.3f}, b: {b:.3f}") 
    plt.axline((0, b), slope = m, label = "RANSAC | m: {:.2e}, b: {:.2e}".format(m, b)) 
    
    # Compare with Linear Regression with the howl dataset
    m, b = np.polyfit(results.data["x"], results.data["y"], deg = 1)
    # plt.axline((0, b), slope = m, label = f"Linear Regression | m: {m:.3f}, b: {b:.3f}", color = "orange") 
    plt.axline((0, b), slope = m, label = "Linear Regression | m: {:.2e}, b: {:.2e}".format(m, b), color = "orange")
    
    # Labels
    plt.xlabel("X coordinate", labelpad = 10)
    plt.ylabel("Y coordinate")
    
    # plt.gca().ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
    
    plt.legend()
    plt.show()
        

def main():
    
    numArguments = 4
    numOptions = 1
    
    if (len(sys.argv) == 2):
        if sys.argv[1] == "-h":
            print("Usage: <dataFilename> <parametersFilename> <simNumber> [OPTIONS]\n")
            print("Options: \n")
            print("\t -t     the program ask for a title name, otherwise the title is ommited.")
            return
        else:
            raise Exception("try using '-h' for help.")
    
    title = ""
    if len(sys.argv) < numArguments: 
        raise Exception("not enough arguments, try using '-h' for help.")
    elif len(sys.argv) > numArguments + numOptions:
        raise Exception("invalid number of options, try using '-h' for help.")
    elif len(sys.argv) != numArguments:
        if sys.argv[4] != "-t":
            raise Exception("invalid argument, try using '-h' for help.")
        else:
            title = input("Plot title: ")
        
    results = Results(sys.argv[1], sys.argv[2])

    plotSingleSimulation(results, int(sys.argv[3]), title)
   

if __name__ == "__main__":
    main()
