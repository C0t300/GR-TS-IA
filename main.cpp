#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <limits>
#include <vector>
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

// avgCost / deltaPosibilities
// mejorqueseamenos / mejorqueseamas
// minimizar

vector<vector<double>> minCostPosibilities(vector<vector<vector<double>>> airplanes){
    double min = std::numeric_limits<double>::infinity(); 
    for (auto airplane : airplanes){
        vector<double> data = airplane.front(); // Ei, Ti, Li, gi, hi

    }
}

double greedyTS(vector<vector<vector<double>>> airplanes){
    
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
    printLargevector(instance);
    return 0;
}