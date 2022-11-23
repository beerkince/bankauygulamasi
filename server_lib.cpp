#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
#include <algorithm>
#include "server_lib.hpp"

using namespace std;
struct Bank{
    
    string name;
    int iban;
    int balance;

};
struct User{
    string userName;
    string name;
    string surName;
    string password;
    Bank banka;

};
static void totxt();
User *data()
{       
        static User member1;
        static User member2;
        static User member3;
        static User member4;
        

        static User arrayUser[4] = {member1,member2,member3 ,member4};
    return arrayUser;
}  
string *returnArray(User User){
    static string array[5];

    
    array[0] = User.name;
    array[1] = User.surName;
    array[2] = User.banka.name;
    array[3] = to_string(User.banka.iban);
    array[4] = to_string(User.banka.balance);
    return array;
    }
static int checknames(string name)
{
    User * arrayUser;
    arrayUser = data();
    int tmp =0;

    string nick;
    for(int i=0;i<4;i++){
        nick = (arrayUser+i)->userName;
        if((name) == (nick)){
            return 1;
        }
    }
    cout << "Wrong Username" <<endl;   //d
    return 0;
}
static int passcheck(string pass,string name)
{
    User * arrayUser;
    arrayUser = data();
    int tmp =0;
    
    //cout << arrayUser->password << endl;
    //cout << (arrayUser+1)->nameSurname <<endl;
    string password;
    string nick;                             
    for(int i=0;i<4;i++){
        nick = (arrayUser+i)->userName;
        if((name) == (nick)){
            password = (arrayUser+i)->password;
            
            if((pass) == (password)){
                cout << "Correct Password" <<endl;    //d
                cout << "Login Process Succesfull."<< endl; //d
                return 1;
            }
        }
    }
    cout << "Wrong Password" <<endl;
    cout << "Login Failed."<< endl;
    return 0;
}
static string *displayprofile(string userName){
    User *srch;

    srch=data();

    string nick,*array;

    for(int i=0;i<6;i++){

        nick = (srch+i)->userName;

        if((userName) == (nick)){

            array = returnArray(*(srch+i));
            
            return array;
            
        }
    }




    return 0;
}
static int deposit(string username,int value){

    User *srch;


    srch=data();
    string nick;

    for(int i=0;i<6;i++){

        nick = (srch+i)->userName;

        if((username) == (nick)){

            (srch+i)->banka.balance += value;
            
            return (srch+i)->banka.balance;
            
        }
    }




    return 0;
}
static int takeout(string username,int value){
     User *srch;
    srch=data();
    string nick;

    for(int i=0;i<6;i++){

        nick = (srch+i)->userName;

        if((username) == (nick)){
            if((srch+i)->banka.balance>=value){

                (srch+i)->banka.balance -= value;
            
                return (srch+i)->banka.balance;
            }
            else{
                return -1;
            }
            
        }
    }
    return 0;
}
static int transfer(string username,int accountnum,string tname,string tsurname, int value){
    User *srch;
    srch=data();
    int locationUser=-1;
    string isim,targetname,targetsurname;
    int handling,val,bankaNo,postval = 0;                         
    int locationTar=-1;
    
    transform(tname.begin(), tname.end(),tname.begin(), ::toupper);                
    transform(tsurname.begin(), tsurname.end(),tsurname.begin(), ::toupper);
    
    for(int i=0;i<6;i++){ 
        isim = (srch+i)->userName;
        if((username) == (isim)){
            locationUser = i;
            val = (srch+i)->banka.balance;
            break;
        }
    }
    for(int i=0;i<6;i++){
        bankaNo = (srch+i)->banka.iban;
        
        targetname = (srch+i)->name;
        
        targetsurname = (srch+i)->surName;

        transform(targetname.begin(), targetname.end(),targetname.begin(), ::toupper);
        transform(targetsurname.begin(), targetsurname.end(),targetsurname.begin(), ::toupper);
        

        if((accountnum == bankaNo) && (targetname==tname) && (targetsurname==tsurname)){
            locationTar = i;
            break;
        }
    }
    if(locationTar==-1){
        return -1;
    }
    if((srch+locationUser)->banka.name!=(srch+locationTar)->banka.name)
        {
        handling = 3;
        } 
    
    if(val >= (value + handling))
    {
        if (handling>0)
        {
            (srch+locationUser)->banka.balance = (srch+locationUser)->banka.balance - (value+handling);
            (srch+locationTar)->banka.balance = (srch+locationTar)->banka.balance + value;
            return (srch+locationUser)->banka.balance;
        }

        (srch+locationUser)->banka.balance = (srch+locationUser)->banka.balance - value;
        (srch+locationTar)->banka.balance = (srch+locationTar)->banka.balance + value;
        return (srch+locationUser)->banka.balance;

    }else
    {

        return -2;
    }



}
int socket()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "soket olusturulamadi" << endl;
        return -1;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    bind(listening, (sockaddr*)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);
    cout << "server bekleniyor" << endl;
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    char host[NI_MAXHOST];      
    char service[NI_MAXSERV];   
    memset(host, 0, NI_MAXHOST); 
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " port baglantisi basarili " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " port baglantisi basarili " << ntohs(client.sin_port) << endl;
    }
    close(listening);
    char buf[4096];
    string userName,userPass;
    
    while (true)
    {
        memset(buf, 0, 4096);
                    
        int bytesReceivedName = recv(clientSocket, buf, 4096, 0);

        if (bytesReceivedName == -1)
        {
            totxt();
            cerr << "Rcv error" << endl;
            break;
        }
 
        if (bytesReceivedName == 0)
        {   
            totxt();
            cout << "Client disconnected " << endl;
            break;
        }
        string enteredname ;
        string error = "error";
        string correct = "correct";   
        cout << "giriş yapiliyor. " << string(buf, 0, bytesReceivedName) << endl;
        
        if(true)
        {   
            enteredname = (string(buf, 0, bytesReceivedName));

            if(checknames(enteredname)){
                cout << "kullanici adi dogru" << endl;
            }

            send(clientSocket, correct.c_str(), correct.size() + 1, 0);
            int bytesReceivedLog = recv(clientSocket, buf, 4096, 0);

            if(passcheck((string(buf, 0, bytesReceivedLog)),enteredname)){
                string choice;
                send(clientSocket, correct.c_str(), correct.size() + 1, 0);
                cout << "secme islemi bekleniyor"<<endl;
                cout << " " << endl;
                bytesReceivedLog = recv(clientSocket, buf, 4096, 0);

                if (bytesReceivedLog == -1)
                {
                    totxt();
                    cerr << "Recv error" << endl;
                    break;
                }
 
                if (bytesReceivedLog == 0)
                {   
                    totxt();
                    cout << "Client disconnected " << endl;
                    break;
                }

                string response = "1";
                choice = (string(buf, 0, bytesReceivedLog));
                cout << choice <<endl;

                int intchoice= stoi(choice);

                string *array;
                array = displayprofile(enteredname);
                switch (intchoice){
                    
                    case 1:{
                        //Takeout Process
                         send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedLog = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedLog == -1)
                        {
                            totxt();
                            cerr << "rcv error" << endl;
                            break;
                        }
 
                        if (bytesReceivedLog == 0)
                        {   
                            totxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }
                        
                        int value = stoi(string(buf, 0, bytesReceivedLog));
                        int totalValue = takeout(enteredname,value); 
                        string valueStr = to_string(totalValue);
                        
                        send(clientSocket, valueStr.c_str(), valueStr.size() + 1, 0);

                    }
                    break;

                    case 2:{

                        cout << "Deposit Processes"<<endl;
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedLog = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedLog == -1)
                        {
                            totxt();
                            cerr << "rcv error" << endl;
                            
                            break;
                        }
 
                        if (bytesReceivedLog == 0)
                        {   
                            totxt();
                            cout << "Client disconnected " << endl;
                            
                            break;
                        }

                        
                        int value = stoi(string(buf, 0, bytesReceivedLog));
                        int totalValue = deposit(enteredname,value);
                        string valueStr = to_string(totalValue);
                        
                        send(clientSocket, valueStr.c_str(), valueStr.size() + 1, 0);

                    }
                    break;  

                    case 3:
                    {
                        //Transfer Process
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedLog = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedLog == -1)
                        {
                            totxt();
                            cout << "rcv error" << endl;         
                            
                            break;
                        }
 
                        if (bytesReceivedLog == 0)
                        {   
                            totxt();
                            cout << "Client disconnected " << endl;
                            
                            break;
                        }
                        
                        int targetNo = stoi(string(buf, 0, bytesReceivedLog));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedLog = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedLog == -1)
                        {
                            totxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedLog == 0)
                        {   
                            totxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        string tname = (string(buf, 0, bytesReceivedLog));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedLog = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedLog == -1)
                        {
                            totxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedLog == 0)
                        {   
                            totxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        string tsurname = (string(buf, 0, bytesReceivedLog));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedLog = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedLog == -1)
                        {
                            totxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedLog == 0)
                        {   
                            totxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        int value = stoi(string(buf, 0, bytesReceivedLog));
                        int snc = transfer(enteredname,targetNo,tname,tsurname,value);
                        string sncStr = to_string(snc);

                        send(clientSocket, sncStr.c_str(), sncStr.size() + 1, 0);
                    }    
                    break;   
                    case 4:{
                        //Showing Account Info
                        cout << " " << endl;
                        for(int i=0;i<5;i++){       
                            send(clientSocket, (array[i]).c_str(), (array[i]).size() + 1, 0);  
                            bytesReceivedLog = recv(clientSocket, buf, 4096, 0);  
                        }
                    }
                    break;      
                }
            }
            else{
                send(clientSocket, error.c_str(), error.size() + 1, 0);
            }
        }
        else{
            send(clientSocket, error.c_str(), error.size() + 1, 0);
        }       
    }
 
    
    close(clientSocket);
 
    return 0;
}
void getfromtxt(){

    User * arrayUser;
    arrayUser = data();

    string userName,name,surname,password,bankaName;
    int iban,paraValue;
    
    fstream gettext("bank_clients.txt");
    string line = "";

    if ( gettext.is_open() ){
        
        for(int i=0;i<4;i++){ 

            gettext >> userName >> name>> surname >> password >> bankaName >> iban >> paraValue; 
            
            (arrayUser+i)->userName = userName;
            (arrayUser+i)->name = name;
            (arrayUser+i)->surName = surname;
            (arrayUser+i)->password = password;
            (arrayUser+i)->banka.name = bankaName;
            (arrayUser+i)->banka.iban = iban;
            (arrayUser+i)->banka.balance = paraValue;
        }
        gettext.close();
    }
    
}
static void totxt(){
    User * arrayUser;
    arrayUser = data();
    int paraValue;
    string userName,name,surname,password,bankaName;
    int iban;
    fstream gettext("bank_clients.txt",ios::out | ios::ate);
    if ( gettext.is_open() ){
        
        for(int i=0;i<4;i++){ //burda da
            userName = (arrayUser+i)->userName ;
            name = (arrayUser+i)->name;
            surname = (arrayUser+i)->surName;
            password = (arrayUser+i)->password;
            bankaName = (arrayUser+i)->banka.name;
            iban = (arrayUser+i)->banka.iban;
            paraValue = (arrayUser+i)->banka.balance;

            gettext << userName << " "<< name << " " << surname << " "  << password << " "  << bankaName << " "  << iban << " "  << paraValue << endl; 
        }
        gettext.close();
    }



}
