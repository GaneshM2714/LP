#include<bits/stdc++.h>
using namespace std;

class process{
    public:
    int at;
    int bt;
    int ct;
    int wt;
    int tat;
    int ori_bt;
    int pri;
    bool completed = false;
};

class FCFS{
    int n;
    vector<process>processes;
    public:
    
    void takeinput(){
        cout<<"Enter Number of Processes: ";
        cin>>n;
        processes.resize(n);
        for(auto &p:processes){
            cout<<"Enter Arrival Time: ";
            cin>>p.at;
            cout<<"Enter Burst Time: ";
            cin>>p.bt;
        }
        computeCT();
    }

    static bool comp(process& p,process& q){
        return p.at<q.at;
    };

    void computeCT(){
        //Sort all processes according to Arrival Times:
        sort(begin(processes),end(processes),comp);

        //Initialize a curr times to 0;
        // If any processes within th curr time's arrival time, add them to ready queue;

        int curr_time = 0;
        int completed = 0;

        while (completed!=n)
        {   
            bool got = false;
            for(auto &p:processes){
                if(!p.completed && curr_time>=p.at){
                    curr_time += p.bt;
                    p.ct = curr_time;
                    p.completed = true;
                    completed++;
                    got = true;
                }
            }
            if(!got){
                curr_time++;
            }
        }

        computeTAT();
    }

    void computeTAT(){
        for(auto &p:processes){
            p.tat = p.ct - p.at;
        }

        computeWT();
    }

    void computeWT(){
        for(auto &p:processes){
            p.wt = p.tat - p.bt;
        }
        display();
    }   

    //Write Display function!
    void display() {
    cout << "FCFS Scheduling:\n";
    cout << "AT\tBT\tCT\tTAT\tWT\n";
    for (const auto &p : processes) {
        cout << p.at << '\t' << p.ori_bt << '\t' << p.ct << '\t' << p.tat << '\t' << p.wt << '\n';
    }
}

};


class SJF{
    int n;
    vector<process>processes;
    public:
    
    void takeinput(){
        cout<<"Enter Number of Processes: ";
        cin>>n;
        processes.resize(n);
        for(auto &p:processes){
            cout<<"Enter Arrival Time: ";
            cin>>p.at;
            cout<<"Enter Burst Time: ";
            cin>>p.bt;
            p.ori_bt = p.bt;
        }
        computeCT();
    }

    static bool comp(process p,process q){
        return p.at<q.at;
    };

    void computeCT(){
        //Sort all processes according to Arrival Times:
        sort(begin(processes),end(processes),comp);

        //Initialize a curr times to 0;
        // If any processes within th curr time's arrival time, add them to ready queue;

        int curr_time = 0;
        int completed = 0;

        while (completed!=n)
        {   
            int idx = -1;
            int curr_bt = INT_MAX;
            bool got = false;

            for(int i=0;i<n;i++){
                if(processes[i].at<=curr_time && !processes[i].completed && processes[i].bt<curr_bt){
                    curr_bt = processes[i].bt;
                    idx = i;
                }
            }
            
            if(idx==-1){
                curr_time++;
            }
            else{
                processes[idx].bt--;
                curr_time++;

                if(processes[idx].bt == 0){
                    processes[idx].completed = true;
                    processes[idx].ct = curr_time;
                    completed++;
                }
            }
        }

        computeTAT();
    }

    void computeTAT(){
        for(auto& p:processes){
            p.tat = p.ct - p.at;
        }

        computeWT();
    }

    void computeWT(){
        for(auto& p:processes){
            p.wt = p.tat - p.ori_bt;
        }
        display();
    }   

    //Write Display function!
    void display() {
    cout << "Shortest Job First Scheduling:\n";
    cout << "AT\tBT\tCT\tTAT\tWT\n";
    for (const auto &p : processes) {
        cout << p.at << '\t' << p.ori_bt << '\t' << p.ct << '\t' << p.tat << '\t' << p.wt << '\n';
    }
}


};

class RR{
    int n;
    int tq;
    vector<process>processes;
    public:
    
    void takeinput(){
        cout<<"Enter Number of Processes: ";
        cin>>n;
        cout<<"Enter Time Quantum: ";
        cin>>tq;
        processes.resize(n);
        for(auto &p:processes){
            cout<<"Enter Arrival Time: ";
            cin>>p.at;
            cout<<"Enter Burst Time: ";
            cin>>p.bt;
            p.ori_bt = p.bt;
        }
        computeCT();
    }

    static bool comp(process p,process q){
        return p.at<q.at;
    };

    void computeCT(){
        //Sort all processes according to Arrival Times:
        sort(begin(processes),end(processes),comp);


        //Initialize a curr times to 0;
        // If any processes within th curr time's arrival time, add them to ready queue;

        int curr_time = 0;
        int completed = 0;

        queue<int>q;
        while (completed!=n)
        {   

            for(int i=0;i<n;i++){
                if(!processes[i].completed && processes[i].at<=curr_time && processes[i].bt!=0){
                    q.push(i);
                }
            }
            
            if(q.empty()){
                curr_time++;
            }
            else{
                int idx = q.front();
                q.pop();

                if(processes[idx].completed) continue;
                if(processes[idx].bt<tq){
                    curr_time += processes[idx].bt;
                    processes[idx].bt = 0;
                }
                else{
                    curr_time+= tq;
                    processes[idx].bt-= tq;
                }

                if(processes[idx].bt == 0){
                    processes[idx].completed = true;
                    processes[idx].ct = curr_time;
                    completed++;
                }
            }
        }

        computeTAT();
    }

    void computeTAT(){
        for(auto& p:processes){
            p.tat = p.ct - p.at;
        }

        computeWT();
    }

    void computeWT(){
        for(auto& p:processes){
            p.wt = p.tat - p.ori_bt;
        }
        display();
    }   

    //Write Display function!
    void display() {
    cout << "Round Robin Scheduling:\n";
    cout << "AT\tBT\tCT\tTAT\tWT\n";
    for (const auto &p : processes) {
        cout << p.at << '\t' << p.ori_bt << '\t' << p.ct << '\t' << p.tat << '\t' << p.wt << '\n';
    }
}


};


class Priority{
    int n;
    int tq;
    vector<process>processes;
    public:
    
    void takeinput(){
        cout<<"Enter Number of Processes: ";
        cin>>n;
       
        processes.resize(n);
        for(auto &p:processes){
            cout<<"Enter Arrival Time: ";
            cin>>p.at;
            cout<<"Enter Burst Time: ";
            cin>>p.bt;
            cout<<"Enter Prioriy: ";
            cin>>p.pri;
            p.ori_bt = p.bt;
        }
        computeCT();
    }

    static bool comp(process p,process q){
        if(p.pri == q.pri) return p.at<q.at;
        return p.pri<q.pri;
    };

    void computeCT(){
        //Sort all processes according to Arrival Times:
        sort(begin(processes),end(processes),comp);


        //Initialize a curr times to 0;
        // If any processes within th curr time's arrival time, add them to ready queue;

        int curr_time = 0;
        int completed = 0;

        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>>pq;
        while (completed!=n)
        {   

            for(int i=0;i<n;i++){
                if(!processes[i].completed && processes[i].at<=curr_time && processes[i].bt!=0){
                    pq.push({processes[i].pri,i});
                }
            }
            
            if(pq.empty()){
                curr_time++;
            }
            else{
                auto curr = pq.top();
                pq.pop();
                int idx = curr.second;

                if(processes[idx].completed) continue;
                curr_time += processes[idx].bt;
                processes[idx].bt = 0;
                processes[idx].completed = true;
                processes[idx].ct = curr_time;

                if(processes[idx].bt == 0){
                    processes[idx].completed = true;
                    processes[idx].ct = curr_time;
                    completed++;
                }
            }
        }

        computeTAT();
    }

    void computeTAT(){
        for(auto& p:processes){
            p.tat = p.ct - p.at;
        }

        computeWT();
    }

    void computeWT(){
        for(auto& p:processes){
            p.wt = p.tat - p.ori_bt;
        }
        display();
    }   

    //Write Display function!
    void display() {
    cout << "Priority Scheduling:\n";
    cout << "AT\tBT\tPRI\tCT\tTAT\tWT\n";
    for (const auto &p : processes) {
        cout << p.at << '\t' << p.ori_bt << '\t' << p.pri << '\t' << p.ct << '\t' << p.tat << '\t' << p.wt << '\n';
    }
}

};






int main(){
    while (true)
    {
        cout<<"Please Select an Algorithm: \n";
        cout<<"1.FCFS\n 2.Round Robin\n 3.SJF\n 4.Priority\n5.Exit\n";
        int ch;
        cin>>ch;
        FCFS f;
        RR r;
        SJF s;
        Priority p;
         
        switch(ch){
            case 1:
                f.takeinput();
                break;
            case 2:
                r.takeinput();
            case 3:
                s.takeinput();
            case 4:
                p.takeinput();
            case 5:
                exit(0);
                break;
            default:
                cout<<"Enter a Correct Choice.\n";
                break;
        }
    }
    
}