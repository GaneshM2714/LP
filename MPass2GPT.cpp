#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class MD {
public:
    string mnemonic;
    string op1;
    string op2;
    unordered_map<string, int> pnt;
};

class KDP {
public:
    int no;
    string name;
    string value;
};

class MN {
public:
    int no;
    string name;
    int pp, kp, mdtp, kdptp = -1;
    unordered_map<string, int> pnt;
};

class AP {
public:
    int no;
    string val;
};

vector<MN> mnt;
vector<MD> mdt;
vector<KDP> kdpt;
vector<vector<AP>> APT;

int main() {
    fstream obj("MNT.txt", ios::in);
    string line;
    while (getline(obj, line)) {
        if (line.empty()) continue;
        MN m;
        stringstream ss(line);
        ss >> m.no >> m.name >> m.pp >> m.kp >> m.mdtp >> m.kdptp;
        mnt.push_back(m);
    }
    obj.close();

    fstream obj1("MDT.txt", ios::in);
    while (getline(obj1, line)) {
        if (line.empty()) continue;
        MD m;
        stringstream ss(line);
        if (line.size() == 1) ss >> m.mnemonic;
        else ss >> m.mnemonic >> m.op1 >> m.op2;
        mdt.push_back(m);
    }
    obj1.close();

    fstream obj2("KDPT.txt", ios::in);
    while (getline(obj2, line)) {
        if (line.empty()) continue;
        KDP m;
        stringstream ss(line);
        ss >> m.no >> m.name >> m.value;
        kdpt.push_back(m);
    }
    obj2.close();

    fstream obj3("PNT.txt", ios::in);
    string curr_macro = "";
    while (getline(obj3, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        if ('A' <= line[0] && line[0] <= 'Z') {
            ss >> curr_macro;
        } else {
            for (auto &m : mnt) {
                if (m.name == curr_macro) {
                    string val;
                    int no;
                    ss >> no >> val;
                    m.pnt[val] = no;
                }
            }
        }
    }
    obj3.close();

    cout << "MNT Table:\n";
    for (auto &d : mnt) {
        cout << d.no << "\t" << d.name << "\t" << d.pp << "\t" << d.kp << "\t" << d.mdtp << "\t" << d.kdptp << endl;
    }
    cout << "KDPT Table:\n";
    for (auto &d : kdpt) {
        cout << d.no << "\t" << d.name << "\t" << d.value << endl;
    }
    cout << "PNT Table:\n";
    for (auto &d : mnt) {
        for (auto &pr : d.pnt) {
            cout << pr.second << "\t" << pr.first << endl;
        }
    }
    cout << "MDT:\n";
    int idx = 1;
    for (auto &m : mdt) {
        cout << idx << "\t" << m.mnemonic << " " << m.op1 << " " << m.op2 << endl;
        idx++;
    }

    // Process macro calls and generate APTAB and expanded code
    fstream calls("call1.txt", ios::in);
    fstream aptab_file("APTAB.txt", ios::out);
    fstream expanded_file("EXPANDED_CODE.txt", ios::out);

    while (getline(calls, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        vector<string> instructs;
        string word;
        while (ss >> word) {
            instructs.push_back(word);
        }

        // Find macro in MNT
        MN macro;
        bool found = false;
        for (const auto &m : mnt) {
            if (m.name == instructs[0]) {
                macro = m;
                found = true;
                break;
            }
        }
        if (!found) continue;

        // Create APTAB and initialize with positional and keyword arguments
        vector<AP> aptab(macro.pp + macro.kp + 1);
        int pp_count = 1;  // Track positional parameters

        // Process positional parameters
        for (int i = 1; i < instructs.size(); i++) {
            if (instructs[i].find('=') == string::npos) {  // Positional argument
                aptab[pp_count].no = pp_count;
                aptab[pp_count].val = instructs[i];
                pp_count++;
            } else {  // Keyword argument
                string param = instructs[i].substr(1, instructs[i].find('=') - 1);
                string value = instructs[i].substr(instructs[i].find('=') + 1);
                int pnt_index = macro.pnt[param];
                aptab[pnt_index].no = pnt_index;
                aptab[pnt_index].val = value;
            }
        }

        // Fill default values for keyword parameters not passed in call
        for (int i = macro.pp + 1; i <= macro.pp + macro.kp; i++) {
            if (aptab[i].val.empty()) {
                aptab[i].val = kdpt[i - macro.pp - 1].value;
            }
        }

        // Save APTAB to file
        for (int i = 1; i <= macro.pp + macro.kp; i++) {
            aptab_file << i << "\t" << aptab[i].val << endl;
        }

        // Generate expanded code using MDT and APTAB
        for (int i = macro.mdtp; i < mdt.size(); i++) {
            if (mdt[i].mnemonic == "MEND") break;

            string expanded_line = mdt[i].mnemonic;
            if (!mdt[i].op1.empty()) {
                if (mdt[i].op1[0] == '#') {  // Positional parameter
                    int pos = stoi(mdt[i].op1.substr(1));
                    expanded_line += " " + aptab[pos].val;
                } else {
                    expanded_line += " " + mdt[i].op1;
                }
            }
            if (!mdt[i].op2.empty()) {
                if (mdt[i].op2[0] == '#') {  // Positional parameter
                    int pos = stoi(mdt[i].op2.substr(1));
                    expanded_line += ", " + aptab[pos].val;
                } else {
                    expanded_line += ", " + mdt[i].op2;
                }
            }
            expanded_file << expanded_line << endl;
        }
    }

    calls.close();
    aptab_file.close();
    expanded_file.close();

    return 0;
}
