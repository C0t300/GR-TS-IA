#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <limits>
#include <vector>
#include <math.h>
using namespace std;

void printLargevector(vector<vector<vector<double>>> l){
    cout << "printing large vector" << endl;
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

bool isViable(vector<vector<vector<double>>> airplanes, vector<vector<int>> timesUsed, bool debug = false){
    int i, i2;
    for (i = 0; i < (int)timesUsed.size(); i++){
        int pos = timesUsed[i][0];
        int time = timesUsed[i][1];
        vector<double> tiempos = airplanes[pos][1];
        vector<double> datos = airplanes[pos][0];// Ei, Ti, Li, gi, hi, originalPos
        if ((time < datos[0]) | (time > datos[2])){
            if (debug) {cout << "mal 1 " << pos << endl;}
            return false;
        }
        for(i2 = i+1; i2 < (int)timesUsed.size(); i2++){
            int posC = timesUsed[i2][0];
            int timeC = timesUsed[i2][1];
            // cout << timeC << " " << time << endl;
            int diff = abs(timeC - time);
            // cout << diff << endl;
            if (diff < tiempos[posC]){
                if (debug) {cout << "mal 2 " << pos << endl;}
                return false;
            }
        }
    }
    return true;
}

bool in(vector<int> v, int i){
    for (auto a : v){
        if (a == i){
            return true;
        } 
    }
    return false;
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
    int countDebug = 0;
    vector<int> notViable;
    // cout << posAirplaneSelected << endl;
    while(retry){
        retry = false;
        for (auto t : timesUsed){ // no se hace completo
            int pos = t[0];
            int timePosUsed = t[1];
            if (pos != posAirplaneSelected){
                int sep = timeOtherAirplanes[pos];
                int currentDiff = abs(currentProposal - timePosUsed);
                // cout << posAirplaneSelected << " " << pos << "  " << sep << " " << currentDiff << endl;
                if (currentDiff < sep){ // no es viable la opcion actual
                    notViable.push_back(currentProposal);
                    double bestCost = std::numeric_limits<double>::infinity();
                    int newTry = -1;
                    int triesBack = 0;
                    // cout << "asldkjas  " << earlyTime + triesBack << endl;
                    while ((abs(earlyTime + triesBack - timePosUsed) > sep) && (earlyTime + triesBack <= idealTime)){ // si es viable menos
                        if(!in(notViable, earlyTime + triesBack)){
                            if ((abs(earlyTime + triesBack - idealTime) * costEarly) < bestCost){
                                newTry = earlyTime + triesBack;
                                bestCost = abs(newTry - idealTime) * costEarly;
                            }
                        }
                        triesBack++;
                    }

                    int triesForward = 0;
                    while ((abs(lateTime - triesForward - timePosUsed) > sep) && (lateTime - triesForward >= idealTime)){ // si es viable mas
                        if(!in(notViable, lateTime - triesForward)){
                            if ((abs(lateTime - triesForward - idealTime) * costLate) < bestCost){
                                newTry = lateTime - triesForward;
                                bestCost = abs(newTry - idealTime) * costLate;
                            }
                        }
                        triesForward++;
                    }
                    if ((newTry != -1) & (countDebug < 500)){
                        currentProposal = newTry;
                        retry = true;
                        // cout << "retry N°" << ++countDebug << endl;
                        break;
                    }
                    else if (countDebug == 500){
                        cout << "probablemente llegamos a un loop infinito en el avion " << posAirplaneSelected << endl;
                    }
                }
            }
        }
    }
    return currentProposal;
}

// avgCost * deltaArrivals / 100
// mejorqueseamenos / mejorqueseamenostambien
// minimizar

void minCostPosibilities(vector<vector<vector<double>>> *airplanes, vector<vector<int>> *timesUsed, vector<vector<vector<double>>> *airplanesOG){
    double min = std::numeric_limits<double>::infinity(); 
    vector<vector<double>> retorno;
    int i = 0;
    int bestI;
    int orgBestI;
    for (auto airplane : *airplanes){
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
            orgBestI = data[5];
        }
        i++;
    }
    int timeChosen = timeViable(*airplanesOG, *timesUsed, orgBestI);
    vector<int> buf = {(int)round((*airplanes)[bestI][0][5]), timeChosen};
    (*timesUsed).push_back(buf);
    (*airplanes).erase((*airplanes).begin()+bestI); // creo que esto no pasa // ahora creo que si
    return;
}

void printDoubleVector(vector<vector<int>> v){
    cout << "Imprimiendo vector [avion, tiempo_escogido] " << endl;
    for (auto i : v){
        for (auto i2 : i){
            cout << i2 << " ";
        }
        cout << endl;
    }
}

double cost(vector<vector<vector<double>>> airplanes, vector<vector<int>> timesUsed){
    int i;
    double cost = 0;
    for(i = 0; i < (int)timesUsed.size(); i++){
        int pos = timesUsed[i][0];
        int time = timesUsed[i][1];
        vector<double> datos = airplanes[pos][0]; // Ei, Ti, Li, gi, hi, originalPos
        int delta = abs(datos[1] - time);
        if (delta != 0){
            if (time < datos[1]){
                cost = cost + delta * datos[3];
            }
            else{
                cost = cost + delta * datos[4];
            }
        }
    }
    return cost;
}

vector<vector<int>> taboo(vector<vector<vector<double>>> airplanes, vector<vector<int>> timesUsed, int tabooSize, int iterStop){
    double bestCost = std::numeric_limits<double>::infinity();
    vector<int> tries;
    int i, i2;
    vector<vector<int>> newTimesUsed = {{0},{0}};

    for (i = 0; i < iterStop; i++){ // es basicamente un while
        double bestCostHere = std::numeric_limits<double>::infinity();
        for (i2 = 0; i2 < (int)timesUsed.size(); i2++){
            // cout << i2 << endl;
            int pos = timesUsed[i2][0];
            int time = timesUsed[i2][1];
            double ideal = airplanes[pos][0][1];
            int newTime;
            if (time > ideal){
                newTime = time - 1;
            }
            else{
                newTime = time + 1;
            }
            timesUsed[i2][1] = newTime;
            if(isViable(airplanes, timesUsed)){ // aca hace what
                if (!in(tries, pos)){
                    bestCostHere = cost(airplanes, timesUsed);
                    tries.push_back(pos);
                    newTimesUsed.clear();
                    copy(timesUsed.begin(), timesUsed.end(), back_inserter(newTimesUsed));
                }
            }
            else{
                timesUsed[i2][1] = time;
            }
            if ((int)tries.size() > tabooSize){
                tries.erase(tries.begin());
            }
        }


        if (bestCostHere < bestCost){
            bestCost = bestCostHere;
        } 
    }
    return newTimesUsed;
}

bool checkDoubleVectorEquals00(vector<vector<int>> c){
    if (c[0][0] == 0 && c[1][0] == 0){
        return true;
    }
    return false;
}

double greedyTS(vector<vector<vector<double>>> airplanes, vector<vector<vector<double>>> airplanesOG){
    vector<vector<int>> timesUsed;
    // cout << airplanes.size() << endl;
    while (airplanes.size() > 0){
        minCostPosibilities(&airplanes, &timesUsed, &airplanesOG);
        // printDoubleVector(timesUsed);
    }
    if(isViable(airplanesOG, timesUsed)){
        cout << "La solucion es viable: " << endl;
    }
    printDoubleVector(timesUsed);
    cout << "El costo es: " << cost(airplanesOG, timesUsed) << endl;
    // printLargevector(airplanes);
    int size = round(((int)airplanesOG.size()) / 3);
    int iter = round(((int)airplanesOG.size()) * 2);
    // cout << size << " " << iter << endl;
    vector<vector<int>> resultadoTaboo = taboo(airplanesOG, timesUsed, size, iter); // int tabooSize, int iterStop
    if (!checkDoubleVectorEquals00(resultadoTaboo)){
        cout << "Taboo encontró un mejor resultado, con costo: " << cost(airplanesOG, resultadoTaboo) << endl;
        printDoubleVector(resultadoTaboo);
    }
    else{
        cout << "Taboo no logró encontrar un mejor resultado." << endl;
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
    const clock_t begin_time = clock();
    /*
    El vector es basicamente un vector gigante de vectores de datos de cada avion
    El subvector de datos del avion se separa en dos:
        El primero tiene Ei, Ti, Li, gi, hi. Es decir:
            early arrival
            ideal arrival
            late arrival
            penalización por aterrizar antes de Ti
            same pero despues de Ti
        El segundo tiene Sij, donde Sij es el tiempo de separacion entre el avion i y el j
    */

    string nombreArchivoInstancias = "instancias/airland1.txt";

    vector<vector<vector<double>>> instance = openInstance(nombreArchivoInstancias); //[[[1,2],[3,4]], [[5,6],[7,8]]]
    vector<vector<vector<double>>> instanceOG = openInstance(nombreArchivoInstancias);
    // printLargevector(instance);
    greedyTS(instance, instanceOG);
    cout << "Tiempo de procesamiento: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " segundos" << endl;
    return 0;
}