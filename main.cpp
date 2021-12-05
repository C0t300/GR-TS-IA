#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <limits>
#include <vector>
#include <math.h>
using namespace std;

void printLargevector(vector<vector<vector<double>>> l){
    for (auto v : l){
        for (auto i : v){
            for (auto i2: i){
                cout << i2 << " " << endl;
            }
        }
    }
}

int abs(int n){
    if (n < 0){
        return n*-1;
    }
    return n;
}

int timeViable(vector<vector<vector<double>>> airplanes, vector<vector<int>> timesUsed, int posAirplaneSelected){ // timesUsed should be a vector of vectors which have [posAirplane, timeUsedInThatAirplane]
    
    vector<double> timeOtherAirplanes = airplanes[posAirplaneSelected][1];
    int earlyTime = round(airplanes[posAirplaneSelected][0][0]);
    int idealTime = round(airplanes[posAirplaneSelected][0][1]);
    int lateTime = round(airplanes[posAirplaneSelected][0][2]);
    double costEarly = airplanes[posAirplaneSelected][0][3];
    double costLate = airplanes[posAirplaneSelected][0][4];
    int currentProposal = idealTime;
    bool retry = true;
    while(retry){
        retry = false;
        for (auto t : timesUsed){
            int pos = t[0];
            int timePosUsed = t[1];
            int sep = timeOtherAirplanes[pos];
            int currentDiff = abs(currentProposal - timePosUsed);
            if (currentDiff < sep){ // no es viable la opcion actual
                double bestCost = std::numeric_limits<double>::infinity();
                int newTry = -1;
                int triesBack = 0;
                while (abs(earlyTime + triesBack - timePosUsed) < sep){ // si es viable menos
                    if ((abs(earlyTime + triesBack - idealTime) * costEarly) < bestCost){
                        newTry = earlyTime + triesBack;
                    }
                    triesBack += 1;
                }

                int triesForward = 0;
                while (abs(lateTime - triesForward - timePosUsed) < sep){ // si es viable mas
                    if ((abs(lateTime - triesForward - idealTime) * costLate) < bestCost){
                        newTry = lateTime - triesForward;
                    }
                    triesForward += 1;
                }
                if (newTry != -1){
                    currentProposal = newTry;
                    retry = true;
                }
            }
        }
    }
    return currentProposal;
}

// avgCost * deltaArrivals / 100
// mejorqueseamenos / mejorqueseamenostambien
// minimizar

vector<vector<int>> minCostPosibilities(vector<vector<vector<double>>> airplanes, vector<vector<int>> timesUsed){
    double min = std::numeric_limits<double>::infinity(); 
    vector<vector<double>> retorno;
    int i = 0;
    int bestI;
    for (auto airplane : airplanes){
        vector<double> data = airplane[0]; // Ei, Ti, Li, gi, hi, originalPos
        double avgCost = (data[3] + data[4]) / 2;
        // cout << "cost " << avgCost << endl;
        double deltaArrivals = data[2] - data[0];
        // cout << "delta " << deltaArrivals << endl;
        double ratio = avgCost * deltaArrivals / 100;
        // cout << "ratio " << ratio << endl;
        // cout << "- - -" << endl; 
        if (ratio < min){
            min = ratio;
            bestI = i;
        }
        i++;
    }
    int timeChosen = timeViable(airplanes, timesUsed, bestI);
    vector<int> buf = {(int)round(airplanes[bestI][0][5]), timeChosen};
    timesUsed.push_back(buf);
    airplanes.erase(airplanes.begin()+bestI); // creo que esto no pasa
    return timesUsed;
}

void printDoubleVector(vector<vector<int>> v){
    cout << "printing double vector " << endl;
    for (auto i : v){
        for (auto i2 : i){
            cout << i2 << " ";
        }
        cout << endl;
    }
}

double greedyTS(vector<vector<vector<double>>> airplanes){
    vector<vector<int>> timesUsed;
    while (airplanes.size() > 0){
        timesUsed = minCostPosibilities(airplanes, timesUsed);
        printDoubleVector(timesUsed);
    }
    
    return 1;
}

vector<string> split(string s, string delim){
    vector<string> retorno;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        if (! token.empty()){
            retorno.push_back(token);
        }
        s.erase(0, pos + delim.length());
    }
    if (! s.empty()){
        retorno.push_back(s);
    }
    return retorno;
}

vector<vector<vector<double>>> openInstance(string nameFile){
    
    vector<vector<vector<double>>> retorno;

    // open a file in read mode.
    ifstream infile; 
    infile.open(nameFile); 
    string line;
    getline(infile, line);
    int cantAviones = stoi(line);
    int i;
    
    for (i = 0; i < cantAviones; i++){
        vector<vector<double>> bufPlane;
        vector<double> buffer;
        
        if (getline(infile,line)) {
            vector<string> splitted = split(line, " ");
            
            for (auto num : splitted){
                buffer.push_back(stod(num));
            }
            buffer.push_back(i);
        }
        bufPlane.push_back(buffer);

        vector<double> buffer2;
        if (getline(infile,line)) {
            vector<string> splitted = split(line, " ");

            for (auto num : splitted){
                buffer2.push_back(stod(num));
            }
        }
        bufPlane.push_back(buffer2);
        retorno.push_back(bufPlane);
    }

    // close the opened file.
    infile.close();
    return retorno;
}


int main() {

    /*
    La vectora es basicamente una vectora gigante de vectoras de datos de cada avion
    La subvectora de datos del avion se separa en dos:
        La primera tiene Ei, Ti, Li, gi, hi. Es decir:
            early arrival
            ideal arrival
            late arrival
            penalización por aterrizar antes de Ti
            same pero despues de Ti
        La segunda tiene Sij, donde Sij es el tiempo de separacion entre el avion i y el j
    */
    vector<vector<vector<double>>> instance = openInstance("instancias/airland1.txt"); //[[[1,2],[3,4]], [[5,6],[7,8]]]
    // printLargevector(instance);
    greedyTS(instance);
    return 0;
}