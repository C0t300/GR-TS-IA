#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <string.h>
using namespace std;

void printLargeList(list<list<list<double>>> l){
    for (auto v : l){
        for (auto i : v){
            for (auto i2: i){
                cout << i2 << " " << endl;
            }
        }
    }
}

list<string> split(string s, string delim){
    list<string> retorno;
    list<string>::iterator it = retorno.end();
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        if (! token.empty()){
            retorno.insert(it, token);
        }
        s.erase(0, pos + delim.length());
    }
    if (! s.empty()){
        retorno.insert(it, s);
    }
    return retorno;
}

list<list<list<double>>> openInstance(string nameFile){
    
    list<list<list<double>>> retorno;
    list<list<list<double>>>::iterator itR = retorno.end();

    // open a file in read mode.
    ifstream infile; 
    infile.open(nameFile); 
    string line;
    getline(infile, line);
    int cantAviones = stoi(line);
    int i;
    
    for (i = 0; i < cantAviones; i++){
        list<list<double>> bufPlane;
        list<list<double>>::iterator itP = bufPlane.end();
        list<double> buffer;
        list<double>::iterator it = buffer.end();
        
        if (getline(infile,line)) {
            list<string> splitted = split(line, " ");
            
            for (auto num : splitted){
                buffer.insert(it, stod(num));
            }
        }
        bufPlane.insert(itP, buffer);

        list<double> buffer2;
        list<double>::iterator it2 = buffer2.end();
        if (getline(infile,line)) {
            list<string> splitted = split(line, " ");

            for (auto num : splitted){
                buffer2.insert(it2, stod(num));
            }
        }
        bufPlane.insert(itP, buffer2);
        retorno.insert(itR, bufPlane);
    }

    // close the opened file.
    infile.close();
    return retorno;
}


// main() is where program execution begins.
int main() {

    list<list<list<double>>> instance = openInstance("instancias/airland1.txt"); //[[[1,2],[3,4]], [[5,6],[7,8]]]
    // printLargeList(instance);
    return 0;
}