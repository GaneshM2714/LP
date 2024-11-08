#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define port 3000

using namespace std;

vector<string> split(char *buffer)
{
    string s = buffer;
    stringstream ss(s);

    vector<string> ans;
    string word;
    while (ss >> word)
    {
        ans.push_back(word);
    }
    return ans;
}

int sum(int a, int b)
{
    return a + b;
}
int sub(int a, int b)
{
    return a - b;
}
int mult(int a, int b)
{
    return a * b;
}
int divi(int a, int b)
{
    if (b == 0)
    {
        return -1;
    }
    return a / b;
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        cout << "Error setting SO_REUSEADDR\n";
        close(sockfd);
        return 1;
    }

    sockaddr_in serveradd;
    serveradd.sin_family = AF_INET;
    serveradd.sin_port = htons(port);
    serveradd.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr *)&serveradd, sizeof(serveradd)) < 0)
    {
        cout << "Error Encountered while binding!\n";
        return 0;
    }

    if (listen(sockfd, 5) < 0)
    {
        cout << "Error Encountered while binding!\n";
        return 0;
    }
    int clientfd = accept(sockfd, nullptr, nullptr);
    if (clientfd < 0)
    {
        cout << "Error Encountered while accepting connection!\n";
        return 0;
    }

    char buffer[1024] = {0};
    recv(clientfd, buffer, 1024, 0);

    cout << "Received Procedure with parameters: " << buffer << endl;
    vector<string> proc = split(buffer);

    for (auto it : proc)
    {
        cout << it << endl;
    }
    if (proc.size() != 3)
    {
        const char *error = "Invalid number of parameters\n";
        send(clientfd, error, sizeof(error), 0);
        return 0;
    }

    if (proc[0] == "Sum" || proc[0] == "sum")
    {
        cout << "INSIDE SUM: \n";
        int ans = sum(stoi(proc[1]), stoi(proc[2]));
        string ans1 = "The result of sum is: " + to_string(ans);
        send(clientfd, ans1.c_str(), ans1.size(), 0);
    }
    else if (proc[0] == "Sub" || proc[0] == "sub")
    {
        cout << "INSIDE SUBTRACTION: \n";
        int ans = sub(stoi(proc[1]), stoi(proc[2]));
        string ans1 = "The result of subtraction is: " + to_string(ans);

        send(clientfd, ans1.c_str(), ans1.size(), 0);
    }
    else if (proc[0] == "Mult" || proc[0] == "mult")
    {
        cout << "INSIDE MULTIPLICATION: \n";
        int ans = mult(stoi(proc[1]), stoi(proc[2]));
        string ans1 = "The result of multiplication is: " + to_string(ans);

        send(clientfd, ans1.c_str(), ans1.size(), 0);
    }
    else if (proc[0] == "Div" || proc[0] == "div")
    {
        cout << "INSIDE DIVISION: \n";
        int ans = divi(stoi(proc[1]), stoi(proc[2]));
        string ans1 = "The result of division is: " + to_string(ans);

        send(clientfd, ans1.c_str(), ans1.size(), 0);
    }
    else
    {
        const char *error = "Please enter Correct Procedure\n";
        send(sockfd, error, sizeof(error), 0);
    }

    close(sockfd);
    close(clientfd);

    return 0;
}