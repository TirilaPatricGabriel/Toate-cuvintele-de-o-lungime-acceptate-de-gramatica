#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

unordered_map<string, vector<string>> Map;
int n;

class Menu{
    static Menu* instance;
    vector<string> cuvinte;
    vector<string> lastSt;
    Menu();
public:
    static Menu* getInstance() {
        if (instance == nullptr) {
            instance = new Menu();
        }
        return instance;
    }
    void start();
    void citireTastatura();
    void citireFisier();
    void bt(string simbol, string cuv);
    bool findSt(string simbol);
    friend ostream& operator<<(ostream& out, const Menu&);
};
bool Menu::findSt(string simbol){
    for(int i=0; i<this->lastSt.size(); i++){
        if(this->lastSt[i] == simbol) return 1;
    }
    return 0;
}
Menu::Menu(){
    this->cuvinte = {};
    this->lastSt = {};
}
void Menu::start(){
    int inp;
    cout<<"Citire de la tastatura (1) sau din fisier (2)?"; cin>>inp;
    bool ok = 0;
    while(!ok){
        switch(inp){
            case 1: {
                this->citireTastatura();
                ok = 1;
                break;
            }
            case 2: {
                this->citireFisier();
                ok = 1;
                break;
            }
            default: {
                break;
            }
        }
    }
}
void Menu::citireFisier(){
    ifstream f("C:\\Users\\tiril\\CLionProjects\\L-NFA-TO-DFA\\gramatica.in");
    int m, numarP;
    f>>n; f>>m;
    while(n == 0){
        cout<<"Citeste un numar diferit de 0 pentru lungimea cuvintelor:"; cin>>n;
    }
    string simbolNeterminala, simbolTerminala, drum;


    for(int i=0; i<m; i++){
        f>>simbolNeterminala;
        f>>numarP;
        Map[simbolNeterminala] = {};
        for(int j=0; j<numarP; j++){
            f>>drum;
            Map[simbolNeterminala].push_back(drum);
        }
    }

    for(int i=0; i<Map["S"].size(); i++){
        bt("S", Map["S"][i]);
    }

    cout<<*this;
}
void Menu::citireTastatura(){
    int m, numarP;
    cout<<"lungime cuvant: "; cin>>n;
    while(n == 0){
        cout<<"Citeste un numar diferit de 0 pentru lungimea cuvintelor:"; cin>>n;
    }
    cout<<"numar simboluri neterminale: "; cin>>m;
    string simbolNeterminala, simbolTerminala, drum;


    for(int i=0; i<m; i++){
        cout<<"simbol neterminala: "; cin>>simbolNeterminala;
        cout<<"numar productii: "; cin>>numarP;
        Map[simbolNeterminala] = {};
        for(int j=0; j<numarP; j++){
            cout<<"drum: "; cin>>drum;
            Map[simbolNeterminala].push_back(drum);
        }
    }
    

    for(int i=0; i<Map["S"].size(); i++){
        bt("S", Map["S"][i]);
    }

    cout<<*this;
}
void Menu::bt(string simbol, string cuv){
    if(simbol == "0" && cuv.size() == n) {
        if(find(this->cuvinte.begin(), this->cuvinte.end(), cuv) == this->cuvinte.end()) {
            this->cuvinte.push_back(cuv);
        }
        return;
    } else if(cuv.size() == n+1){
        for(int i=0; i<Map[simbol].size(); i++){
            if(Map[simbol][i] == "lambda"){  // pot termina, e cuvant acceptat
                if(cuv[0] >= 'A' && cuv[0] <= 'Z') cuv.erase(0, 1);
                else cuv.erase(cuv.size()-1, 1);
                this->cuvinte.push_back(cuv);
            }
        }
        return;
    }

    int i;
    if(cuv[0] >= 'A' && cuv[0] <= 'Z') i = 0;
    else i = cuv.size()-1;
    if(cuv[i] >= 'A' && cuv[i] <= 'Z') {
        string begin = cuv.substr(0, i), end = cuv.substr(i + 1);
        string c(1, cuv[i]);
        for (int j = 0; j < Map[c].size(); j++) {
            if(Map[c][j] != "lambda"){
                if(Map[c][j].size() > 2){
                    if(Map[c][j][0] >= 'A' && Map[c][j][0] <= 'Z'){
                        string ns(1, Map[c][j][0]);
                        if(ns != simbol && !this->findSt(ns)){
                            this->lastSt.push_back(ns);
                            bt(ns, cuv);
                        }
                    } else {
                        string ns(1, Map[c][j][Map[c][j].size()-1]);
                        if(ns != simbol && !this->findSt(ns)){
                            this->lastSt.push_back(ns);
                            bt(ns, cuv);
                        }
                    }
                } else {
                    string add = Map[c][j];
                    char b = '0';

                    if (add[0] >= 'A' && add[0] <= 'Z') {
                        b = add[0];
                    }
                    else if(add[1] >= 'A' && add[1] <= 'Z'){
                        b = add[1];
                    }

                    string nb(1, b);
                    this->lastSt.clear();
                    bt(nb, begin + add + end);
                }
            }
        }
    }

}
ostream& operator<<(ostream& out, const Menu& m){
    for(int i=0; i<m.cuvinte.size(); i++){
        cout<<m.cuvinte[i]<<endl;
    }
    if(m.cuvinte.size() == 0) cout<<"Nu au fost cuvinte gasite!";
    return out;
}

Menu* Menu::instance = nullptr;

int main() {
    Menu* m = Menu::getInstance();
    m->start();

    return 0;
}
