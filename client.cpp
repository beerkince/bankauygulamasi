#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>


using namespace std;

int client()
{

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    int port = 54000;
    string ipAddress = "127.0.0.1";
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    char buf[4096];
    string pmember;
	int i=0;
    do {

		if(i==0){
			cout << "kullanici adi: ";
			cin >> pmember;
		}

        int sendRes = send(sock, pmember.c_str(), pmember.size() + 1, 0);
        
		if (sendRes == -1 )
        {
            cout << "send error";
            continue;
        }
             
		memset(buf, 0, 4096);
        int i=1;
        string gelen;  
        string choiceStr;                                      
        int bytesReceived = recv(sock, buf, 4096, 0);
        int choice;
                
        if (true){
            
			cout << "sifre: ";    
			cin >> pmember;
			send(sock, pmember.c_str(), pmember.size() + 1, 0);
            bytesReceived = recv(sock, buf, 4096, 0);
            
            if (string(buf,0, bytesReceived) == ("correct")){
                cout<< ""<< endl;
                cout << "Hos geldiniz" << endl;
                cout<< ""<< endl;
                do {

                cout << "1-Para Çekme" << endl;
                cout << "2-Para Yatırma." << endl;
                cout << "3-Başka Hesaba Yatırma" << endl;
                cout << "4-Bilgilerimi Göster" << endl;
                cout<< ""<< endl;
                cout << "Lütfen yapmak istediğiniz işlemi tuşlayiniz: ";
                cin>>choice;
                cout<< ""<< endl;
                if(choice<=0 || choice>=5) {
                    cout << "Tekrar Tuşlayınız"<<endl;
                    cout<< ""<< endl;
                }

                }while(choice<=0 || choice>=5);

                
                string response="1";
                switch(choice) {

                    case 1:{
                        int value;
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        string oldValue = (string(buf,0, bytesReceived));
                        int toplamBakiye = stoi(oldValue);
                        do {
                            if(value > toplamBakiye){
                            cout << "Yetersiz Bakiye" << endl;
                            }
                        cout << "Kalan Bakiye Tutarı: "<< oldValue << endl;
                        cout<< ""<< endl;
                        cout << "Çekmek istediğiniz miktarı giriniz: ";
                        cin >> value;
                        cout<< "" << endl;
                        }while(value > toplamBakiye);
                        string valueStr;
                        valueStr= to_string(value);

                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        
                        toplamBakiye = stoi((string(buf,0, bytesReceived)));
                        if(toplamBakiye != -1) {
                            cout << "Hesap Bakiyeniz: " << (string(buf,0, bytesReceived)) <<endl;
                        }
                        cout<< ""<< endl;
                    }
                    break;

                    case 2:{
                        int value;
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);

                        cout << "Hesap Bakiyeniz: "<< (string(buf,0, bytesReceived)) << endl;
                        cout<< ""<< endl;
                        
                        cout << "Yatırmak istediğiniz miktarı giriniz: ";
                        cin >> value;
                        cout<< ""<< endl;
                        string valueStr;
                        valueStr= to_string(value);
                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        cout << "Hesap Bakiyeniz: " << (string(buf,0, bytesReceived)) <<endl;
                        cout<< ""<< endl;

                    }
                    break;

                    case 3:
                    {
                        string tarName,tarSurname,noStr,valueStr;
                        int targetNo,value;
                        choiceStr=to_string(choice);
                        
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        string oldValue = (string(buf,0, bytesReceived));
                        cout << "-- Diğer Bankalara Ait Hesaplara Para Yatırma İşlemlerinde 3,00 TL işlem ücreti hesabınızdan tahsil edilmektedir--"<< endl;
                        cout<< "" << endl;
                        cout << "Hesap Bakiyeniz: $"<< oldValue << endl;
                        cout << "Para Gönderilecek Hesap Numarası: ";
                        cin >> targetNo;
                        noStr = to_string(targetNo);
                        
                        send(sock, noStr.c_str(), noStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Para Gönderilecek Kişinin İsmi: ";
                        cin >> tarName;
                        
                        send(sock, tarName.c_str(), tarName.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Para Gönderilecek Kişinin Soyismi: ";
                        cin >> tarSurname;
                        
                        send(sock, tarSurname.c_str(), tarSurname.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Göndermek istenen miktar: TL";
                        cin >> value;
                        valueStr = to_string(value);
                        
                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);

                        string bakiyeLast = (string(buf,0, bytesReceived));
                        int bakiyeInt = stoi(bakiyeLast);
                        if(bakiyeInt==-1){
                            cout << "Hesap Bilgileri Yanlış"<<endl;
                            }
                        else if(bakiyeInt==-2)
                        {
                            cout << "Yetersiz Bakiye"<<endl;
                        }
                        else{
                        
                             cout << "Hesap Bakiyeniz:"<< bakiyeLast << "TL" << endl;
                        }
                       

                    }
                    break;
                    case 4:{
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        for(int i=0;i<5;i++){
                            if(i==0){
                                cout << "İsim: ";
                            }
                            else if(i==1){
                                cout << "Soyisim: ";
                            }
                            else if(i==2){
                                cout << "Banka Adı: ";
                            }
                            else if(i==3){
                                cout << "Hesap Numarası: ";
                            }else
                            {
                                cout << "Hesap Bakiyesi: TL";
                                
                            }
                            bytesReceived = recv(sock, buf, 4096, 0);
                            cout << string(buf,0, bytesReceived) << endl;
                            send(sock, response.c_str(), response.size() + 1, 0);

                        }
                        cout<< " "<< endl;
                }
                    
                    
                }

            }else{

                cout << "Kullanıcı adı veya parola hatalı." << endl;
            
                continue;

            }

            
            
        }else
        {
            
            
            continue;
        }
        
    } while(true);

   
    close(sock);

    return 0;
}




int main(){
	
	client();

}
