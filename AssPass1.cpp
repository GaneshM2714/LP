#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <bits\stdc++.h>
using namespace std;

unordered_map<string, pair<string, int>> MOT;
unordered_map<string, int> CT;
unordered_map<string, int> RT;

void initiateMOT()
{
    MOT["STOP"] = {"IS", 0};
    MOT["ADD"] = {"IS", 1};
    MOT["SUB"] = {"IS", 2};
    MOT["MULT"] = {"IS", 3};
    MOT["MOVER"] = {"IS", 4};
    MOT["MOVEM"] = {"IS", 5};
    MOT["COMP"] = {"IS", 6};
    MOT["BC"] = {"IS", 7};
    MOT["DIV"] = {"IS", 8};
    MOT["READ"] = {"IS", 9};
    MOT["PRINT"] = {"IS", 10};
    MOT["START"] = {"AD", 1};
    MOT["END"] = {"AD", 2};
    MOT["ORIGIN"] = {"AD", 3};
    MOT["EQU"] = {"AD", 4};
    MOT["LTORG"] = {"AD", 5};
    MOT["DC"] = {"DL", 1};
    MOT["DS"] = {"DL", 2};
}

void initiateCT()
{
    CT["LT"] = {1};
    CT["LE"] = {2};
    CT["EQ"] = {3};
    CT["GT"] = {4};
    CT["GE"] = {5};
    CT["ANY"] = {6};
}

void initiateRT()
{
    RT["AREG"] = {1};
    RT["BREG"] = {2};
    RT["CREG"] = {3};
    RT["DREG"] = {4};
}

vector<string> split(string &s)
{
    stringstream ss(s);

    vector<string> ans;
    string word;
    while (ss >> word)
    {
        ans.push_back(word);
    }
    return ans;
}

class Data
{
public:
    pair<string, string> opcode;
    pair<string, string> operand1;
    pair<string, string> operand2;
};

int LC = 0;

int litcnt = 1;
int symcnt = 1;
class Literal
{
public:
    int no;
    string literal;
    int LC = -1;
};

class Symbol
{
public:
    int no;
    int LC = -1;
};

vector<Literal> LT;
unordered_map<string, Symbol> ST;
vector<int>PT;

vector<Data> IC;
void createIC(string &s)
{
    vector<string> instructs = split(s);
    // for(auto& i:instructs) cout<<i<<" ";
    // cout<<"\n";
    Data d;
    bool topush = true;
    for (auto i : instructs)
    {
        if (MOT.find(i) != MOT.end())
        {   
            if (MOT[i].first == "AD")
            {
                if (i == "START")
                {
                    LC = stoi(instructs[1]);
                    d.opcode.first = MOT[i].first;
                    d.opcode.second = to_string(MOT[i].second);
                }
                else if (i == "EQU")
                {   
                    topush = false;
                    if(ST.find(instructs[0])!=ST.end()){
                        ST[instructs[0]].LC = ST[instructs[2]].LC; 
                    }
                    else{
                        Symbol s;
                        s.no = symcnt;
                        symcnt++;
                        s.LC = ST[instructs[2]].LC;
                        ST[instructs[0]] = s;
                    }
                }
                else if (i == "ORIGIN")
                {

                    LC = ST[instructs[1]].LC;

                    d.opcode.first = MOT[i].first;
                    d.opcode.second = to_string(MOT[i].second);

                }
                else if (i == "LTORG")
                {   
                    topush = false;
                    for(auto &l:LT){
                        if(l.LC!=-1) continue;

                        Data d1;
                        l.LC = LC;
                        LC++;

                        d1.opcode.first = "DL";
                        d1.opcode.second = "01";

                        d1.operand1.first = "C";
                        string lit = l.literal.substr(1,l.literal.size()-1);
                        d1.operand1.second = l.literal;
                        IC.push_back(d1);
                    }

                }
                else
                {
                    d.opcode.first = MOT[i].first;
                    d.opcode.second = to_string(MOT[i].second);
                    
                    IC.push_back(d);
                    topush = false;

                    
                    for(auto &l:LT){
                        if(l.LC!=-1) continue;

                        Data d1;
                        l.LC = LC;
                        LC++;

                        d1.opcode.first = "DL";
                        d1.opcode.second = "01";

                        d1.operand1.first = "C";
                        // string lit = l.literal.substr(1,l.literal.size()-1);
                        d1.operand1.second = l.literal;
                        IC.push_back(d1);
                    }
                }   
            }
            else if (MOT[i].first == "DL")
            {
                if (i == "DC")
                {   
                    d.opcode.first = MOT[i].first;
                    d.opcode.second =to_string(MOT[i].second);
                    LC++;
                    continue;
                }
                else
                {   
                    d.opcode.first = MOT[i].first;
                    d.opcode.second =to_string(MOT[i].second);
                    LC += stoi(instructs[2]);
                }
            }
            else
            {   
                d.opcode.first = MOT[i].first;
                d.opcode.second =to_string(MOT[i].second);
                LC++;
            }
        }
        else if (CT.find(i) != CT.end())
        {
            if (d.operand1.first.empty() && d.operand1.second.empty())
            {
                d.operand1.first = "";
                d.operand1.second = to_string(CT[i]);
            }
            else
            {
                d.operand2.first = "";
                d.operand2.second = to_string(CT[i]);
            }
        }
        else if (RT.find(i) != RT.end())
        {
            if (d.operand1.first.empty() && d.operand1.second.empty())
            {
                d.operand1.first = "RG";
                d.operand1.second = to_string(RT[i]);
            }
            else
            {
                d.operand2.first = "RG";
                d.operand2.second = to_string(RT[i]);
            }
        }
        else if (i[0] == '=')
        {
            int num = 0;
            for(int k=2;k<i.size();k++){
                if(0<=i[k]-'0' && i[k]-'0'<=9){
                    num =num*10+i[k]-'0';
                }

            }

            string val = to_string(num);

            Literal l;
            l.literal = val;
            l.no = litcnt;
            LT.push_back(l);

            d.operand2.first = "L";
            d.operand2.second = to_string(l.no);

            litcnt++;
        }
        else if ('A' <= i[0] && i[0] <= 'Z')
        {
            if (i == instructs[0])
            {
                if (ST.find(i) != ST.end())
                {
                    ST[i].LC = LC;
                }
                else
                {
                    Symbol s;
                    s.no = symcnt;
                    symcnt++;
                    s.LC = LC;
                    ST[i] = s;
                }
            }
            else
            {
                if (ST.find(i) == ST.end())
                {
                    Symbol s;
                    s.no = symcnt;
                    symcnt++;

                    ST[i] = s;
                }

                if (d.operand1.first.empty() && d.operand1.second.empty())
                {
                    d.operand1.first = "S";
                    d.operand1.second = to_string(ST[i].no);
                }
                else
                {
                    d.operand2.first = "S";
                    d.operand2.second = to_string(ST[i].no);
                }
            }
        }
        else
        {
            int num = 0;
            for (int k = 0; k < i.size(); k++)
            {
                if (0 <= i[k] - '0' && i[k] - '0' <= 9)
                {
                    num = num * 10 + i[k]-'0';
                }
            }

            d.operand2.first = "C";
            d.operand2.second = to_string(num);
        }
    }
    if(topush) IC.push_back(d);
}


int main()
{   
    initiateMOT();
    initiateCT();
    initiateRT();
    fstream obj;
    obj.open("asseg1.txt");

    while (!obj.eof())
    {
        string line;
        getline(obj, line);
        createIC(line);
    }

    fstream ic("IC.txt",ios::out);
    for(auto &d: IC){
        cout<<"("+d.opcode.first+","+d.opcode.second+")  ";
        if(!d.operand1.first.empty() || !d.operand1.second.empty())
        cout<<"("+d.operand1.first+","+d.operand1.second+")  ";
        if(!d.operand2.first.empty() || !d.operand2.second.empty())
        cout<<"("+d.operand2.first+","+d.operand2.second+")  \n";
        else cout<<"\n";

        ic<<"("+d.opcode.first+","+d.opcode.second+")  ";
        if(!d.operand1.first.empty() || !d.operand1.second.empty())
        ic<<"("+d.operand1.first+","+d.operand1.second+")  ";
        if(!d.operand2.first.empty() || !d.operand2.second.empty())
        ic<<"("+d.operand2.first+","+d.operand2.second+")  \n";
        else ic<<"\n";
    }

    
    fstream sym("Symbol.txt",ios::out);
    cout<<"SYMBOL TABLE:\n";
    for(auto& s:ST){
        cout<<s.second.no<<" "<<s.first<<" "<<s.second.LC<<"\n";
        sym<<s.second.no<<" "<<s.first<<" "<<s.second.LC<<"\n";
    }

    fstream lit("Literal.txt",ios::out);
    cout<<"LITERAL TABLE:\n";
    for(auto& s:LT){
        cout<<s.no<<" "<<s.literal<<" "<<s.LC<<"\n";
        lit<<s.no<<" "<<s.literal<<" "<<s.LC<<"\n";
    }
    return 0;
}