    #include <bits/stdc++.h>
    using namespace std;

    class Symbol
    {
    public:
        int no;
        int LC = -1;
    };
    class Literal
    {
    public:
        int no;
        string literal;
        int LC = -1;
    };
    unordered_map<string, Symbol> ST;
    vector<Literal> LT;

    pair<string, string> split(string &s)
    {   
        if(s.empty()) return {};
        // s.erase(remove(s.begin(),s.end(),'('),s.end());
        // s.erase(remove(s.begin(),s.end(),')'),s.end());
        s = s.substr(1,s.size()-2);
        stringstream ss(s);
        pair<string, string> ans;
        getline(ss, ans.first, ',');
        getline(ss, ans.second, ',');
        return ans;
    }

    class Data
    {
    public:
        int a=0, b=0, c=0;
    };

    int getLC(int no, string tab)
    {
        if (tab == "S")
        {
            for (auto &s : ST)
            {
                if (s.second.no == no)
                    return s.second.LC;
            }
        }
        for (auto &l : LT)
        {
            if (l.no == no)
                return l.LC;
        }
    }

    int main()
    {
        fstream sym("Symbol.txt", ios::in);
        fstream lit("Literal.txt", ios::in);
        fstream ic("IC.txt", ios::in);

        string line;
        while (getline(sym, line))
        {

            if (line.empty())
                continue;
            stringstream ss(line);
            Symbol s;
            string name;
            ss >> s.no >> name >> s.LC;

            ST[name] = s;
        }

        while (getline(lit, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            Literal l;
            ss >> l.no >> l.literal >> l.LC;
            LT.push_back(l);
        }

        cout << "SYMBOL TABLE:\n";
        for (auto &s : ST)
        {
            cout << s.second.no << " " << s.first << " " << s.second.LC << "\n";
        }

        cout << "LITERAL TABLE:\n";
        for (auto &s : LT)
        {
            cout << s.no << " " << s.literal << " " << s.LC << "\n";
        }

        cout<<"\n----------------------------\n";
        vector<Data> mc;

        while (getline(ic, line))
        {
            string o, o1, o2;
            stringstream ss(line);
            ss >> o >> o1 >> o2;

            pair<string, string> op = split(o);
            pair<string, string> op1 = split(o1);
            pair<string, string> op2 = split(o2);

            cout<<op.first<<op.second<<"    "<< op1.first<<op1.second<<"   "<<op2.first<<op2.second<<endl;
            Data d;
            if (op.first == "AD")
                continue;

            else if (op.first == "DL" && op.second == "01")
            {
                d.a = 0;
                d.b = 0;
                d.c = stoi(op1.second);
            }
            else if (op.first == "DL" && op.second == "02")
                continue;
            else
            {
                d.a = stoi(op.second);

                if (!op1.first.empty() || !op1.second.empty())
                {
                    if (op1.first != "S" && op1.first != "L")
                        d.b = stoi(op1.second);
                    else if (op1.first == "S")
                    {   
                        int no = stoi(op1.second);
                        int LC = getLC(no, "S");

                        d.b = LC;
                    }
                    else
                    {
                        int no = stoi(op1.second);
                        int LC = getLC(no, "L");

                        d.b = LC;
                    }
                }

                if (!op2.first.empty() || !op2.second.empty())
                {
                    if (op2.first != "S" && op2.first != "L")
                        d.c = stoi(op2.second);
                    else if (op2.first == "S")
                    {
                        int no = stoi(op2.second);
                        int LC = getLC(no, "S");

                        d.c = LC;
                    }
                    else
                    {
                        int no = stoi(op2.second);
                        int LC = getLC(no, "L");

                        d.c = LC;
                    }
                }
            }
            mc.push_back(d);
        }

        fstream MC("MC.txt",ios::out);
        for (auto &d : mc)
        {
            cout << d.a << "  " << d.b << "  " << d.c << endl;
            MC << d.a << "  " << d.b << "  " << d.c << endl;
        }
        return 0;
    }