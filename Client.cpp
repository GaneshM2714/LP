#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define port 3000

using namespace std;

int main(){
    int clientfd = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in serveradd;
    serveradd.sin_family = AF_INET;
    serveradd.sin_port = htons(port);
    serveradd.sin_addr.s_addr = INADDR_ANY;

    if(connect(clientfd,(sockaddr*) &serveradd,sizeof(serveradd))<0){
        cout<<"Error while establishing connection\n";
        return 0;
    }

    cout<<"Select procedure: \n1.Sum\n2.Sub\n3.Multiply\n4.Divide\n";
    int ch;
    cin>>ch;

    cout<<"Enter Parameters: ";
    int a,b;
    cin>>a>>b;

    string buffer = "";
    if(ch==1){
        buffer = "sum " + to_string(a) + " " +to_string(b);
    }
    else if(ch==2){
        buffer = "sub "+ to_string(a) + " " +to_string(b);
    }
    else if(ch==3){
        buffer = "mult " + to_string(a) + " " +to_string(b);
    }
    else if(ch==4){
        buffer = "div " + to_string(a) + " " +to_string(b);
    }
    else{
        cout<<"Enter a valid choice\n";
    }
    if(send(clientfd,buffer.c_str(),buffer.size(),0)<0){
        cout<<"Error while sending the message\n";
        return 0;
    }

    char res[1024] = {0};
    if(recv(clientfd,res,sizeof(res),0)<0){
        cout<<"Error receiving the result: "<<endl;
        return 0; 
    }
    cout<<res<<endl;

    close(clientfd);
    return 0;

}