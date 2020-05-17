#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <conio.h>

#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <ctime>
#include <map>
#include <cmath>
#include <cstdlib>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

SOCKET clientSocket;//�������� ������



int gcd(int a, int b);
int sundaram(int n);


class RSA{
private:
	std::string message;
	std::string message2;
 	std::string code;
	std::string codex;
	int ps;
	int qs;
	int p;
	int q;
	int d;
	unsigned int e;
	int n;
	int m;
	std::map <int, char>	 mess = {
	{27,'a'},
	{2,'b'},
	{3,'c'},
	{4,'d'},
	{5,'e'},
	{6,'f'},
	{7,'g'},{8,'h'},{9,'i'},{10,'j'},{11,'k'},{12,'l'},{13,'m'},
	{14,'n'},{15,'o'},{16,'p'},{17,'q'},{18,'r'},{19,'s'},{20,'t'},{21,'u'},{22,'v'},{23,'w'},{24,'x'},{25,'y'},{26,'z'},
	{28,' '},{29,'A'},{30,'B'},{31,'C'},{32,'D'},{33,'E'},{34,'F'},{35,'G'},{36,'H'},{37,'I'},{38,'J'},{39,'K'},{40,'L'},{41,'M'},
	{42,'N'},{43,'O'},{44,'P'},{45,'Q'},{46,'R'},{47,'S'},{48,'T'},{49,'U'},{50,'V'},{51,'W'},{52,'X'},{53,'Y'},{54,'Z'},{55,','},
	{56,'.'}, {57, '@'}, {58, '!'}, {59, '?'}, {60, ')'},{61, '('},{62, '*'},{63, '-'},{64, '_'},{65, '%'},{66, ':'}
	};
	std::map<int, char>::iterator it = mess.begin();
	int ds =0;
	unsigned int es = 0;
	int temp;
	unsigned long long c;
	int num = 0;

	unsigned long long mi;

public:
	RSA(){
		srand ( (unsigned) time (NULL) );
		ps = rand()%1000;
		qs = rand()%1000;
		p = sundaram(ps);
		q = sundaram(qs);
		n = p*q;
		m = (p-1)*(q-1);
		{
		while(ds!=1)
		{
		d = rand()%1000;
		ds=gcd(d,m);
		}
		}

		e=0;

		while(es!=1 || e <= 1)
		{
		e+=1;
		es=(e*d)%(m);
		}
	//	cout << "��� �������� ����: " << e << ", " << n << endl;
	//	cout << "��� �������� ����: " << d << ", "  << n << endl;

	}

	unsigned int GetN()
	{
		return n;
	}

	unsigned int GetPublicKey()
	{
		return e;
	}

	unsigned int GetPrivateKey()
	{
		return d;
	}

	unsigned int SetN(unsigned int N)
	{
		n = N;

	}

	unsigned int SetP(unsigned int E)
	{
		e = E;

	}

	void Crypt(std::string Message){
			message = Message;
			temp = Message.length();


			for(int j =0;j<=temp;j++)
			{
				for(auto itt = it; itt != mess.end() ; itt++)
				{

					if(Message[j] == itt->second)
					{
					c=1;
					unsigned int i = 0;
					while(i<e)
					{
					c=c*itt->first;
					c=c%n;
					i++;
					}

					num++;

					code+=std::to_string(c)+'-';
					}
				}

			}
			codex=code;
			code.clear();


	}
	void Decrtypt(std::string Code){
		codex = Code;
		std::string temporary;
		std::vector <unsigned long long int> Invert;
		unsigned long long crypt;

		//�������������� ������ � ����� � ���������� �������
		int numer = Code.size();

		for (int j=0, i=0; j < numer; j++, i++)
		{
			if(Code[j] == '-')
			{
				crypt = stoi(temporary);
		//		cout << crypt;
				Invert.push_back(crypt);
				temporary.clear();
				i=0;
			}
				else {
					temporary += Code[j];
				}

		}

for (auto const &v : Invert)
{
		mi = 1;
		unsigned int i=0;
		while (i<d)
		{
// �������������� ������ � ���!!

		mi = mi*v;
		mi = mi%n;
		i++;
		}

		for (auto itt = it; itt != mess.end();itt++)
		{
		if(mi == itt->first)
		{
		message2+= itt->second;
		}

		}
		}
		message = message2;
		message2.clear();
	}

	std::string GetCode()
	{
		return codex;
	}
	std::string GetMessage()
	{
		return message;
	}
};

bool Check = false; //�������� �� �����������

RSA rsa; //�������� ������ ����������
RSA rsa_public;


void ClientHandler()
{

	int bytes_read;
	char buf[1024];

		while(1)
		{

			std::string temp;
			std::string temp2;
			unsigned int messs;
			std::string finalMess;

		bytes_read = recv(clientSocket,buf,sizeof(buf),0);
//��������� ������
		if(buf[0] == 'k'){
		temp = buf;
		temp.erase(temp.begin());
		messs = stoi(temp);
		rsa_public.SetP(messs);

		temp.clear();
		temp2.clear();
			buf[0] = '\0';
	} else if(buf[0] == 'n'){
		temp = buf;
		temp.erase(temp.begin());
		messs = stoi(temp);
		rsa_public.SetN(messs);

		temp.clear();
		temp2.clear();
	buf[0] = '\0';

} else if (buf[0] == '!') {
	std::cout << "Auth fall!" << std::endl;
	finalMess = buf;
	finalMess.erase(finalMess.begin());
	rsa.Decrtypt(finalMess);
	std::cout << rsa.GetMessage() << std::endl;
		buf[0] = '\0';

		finalMess.clear();
}		else if (buf[0] == '@') {
			std::cout << "Auth success!" << std::endl;
			finalMess = buf;
			finalMess.erase(finalMess.begin());
			rsa.Decrtypt(finalMess);
			std::cout << rsa.GetMessage() << std::endl;
			buf[0] = '\0';
				finalMess.clear();
		Check = true;
}else{
		finalMess = buf;
		rsa.Decrtypt(finalMess);
		std::cout << rsa.GetMessage() << std::endl;
		buf[0] = '\0';

		if (bytes_read <= 0)
		{
		//close(clientSocket);
closesocket(clientSocket);
		std::cout << "Connection finished!" << std::endl;

		//pthread_exit(NULL);
		}

		}
	}
}

	//-------------------------------





int main()
{
	setlocale(LC_ALL, "Russian");

	char message[1024];
	char Name[1024];
	std::string mess;
	std::string login;

	WSADATA wsaData;//������������� ������
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::cout << "Please enter ip adress of our server: ";
	std::string ipp;// = "192.168.0.100";������������� � �������
	std::cin >> ipp;
	const char* ip = ipp.c_str();
	#define port 12345

	unsigned long inet_addr(const char FAR *cp);


	//hostent hn *gethostbyname("worldlaw.ddns.net");

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(ip);
	//clientService.sin_addr.s_addr = *(DWORD* ) hn->h_addr_list[0];
	clientService.sin_port = htons(port);


	connect(
		clientSocket,
		reinterpret_cast<SOCKADDR*>(&clientService),
		sizeof(clientService)
	);

	std::cout << "Connection is success!" << std::endl;

	std::cout << "Waiting, keys are generating" << std::endl;

	int Size;
	//�������� ������ �������
	//char data[5];
	//#define bufsize 256


	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL,NULL, NULL);

	//����� �������

	std::string KeyP = 'k'+std::to_string(rsa.GetPublicKey());
	strncpy(message, KeyP.c_str(), KeyP.length());
	message[KeyP.length()] = 0;
	send(clientSocket, message, 1024, 0);
	message[0] = 0;

	std::string KeyN = 'n'+std::to_string(rsa.GetN());
	strncpy(message,KeyN.c_str(), KeyN.length());
	message[KeyN.length()] = 0;
	send(clientSocket, message, 1024, 0);
	message[0] = 0;

	//�����������
	std::string Login;
	std::string Names;
	std::string Password;
	std::string Massiv;
	std::string finalMess;

	do{
	std::cout << "Please, enter your login: ";
	std::cin >> Names;
	std::cout << "Please, enter your password: ";
	std::cin >> Password;

	Login = Names + ' ' + Password + ' ';
	rsa_public.Crypt(Login);
	Massiv = 'a'+rsa_public.GetCode();
	Size = Massiv.length();
	strncpy(message, Massiv.c_str(), Size);
	message[Size] = 0;
	send(clientSocket, message, sizeof(message), 0);
	message[0] = 0;

	Massiv.clear();
	Login.clear();
	Names.clear();
	Password.clear();
	//����� �� �����������

	std::cout << "Waiting..." << std::endl;
 	Sleep(3000);
	}while(!Check);
//�������� ���������
	std::cout << "Enter your messages:" << std::endl;

	std::cin.ignore();

	while(1){
	getline(std::cin,mess);

	rsa_public.Crypt(mess);
	Size = rsa_public.GetCode().length();
	strncpy(message, rsa_public.GetCode().c_str(), Size);
	message[Size] = 0;


	send(clientSocket, message, sizeof(message), 0);
	message[0] = 0;

	}

	closesocket(clientSocket);//������ ����������
	// ���� ������ � �������� ������ �� �������������� �������� WSACleanup()
	WSACleanup();

	_getch();

	return 0;
}




	int gcd(int a, int b)
	{
	int c;
	while (b)
		{
		c=a%b;
		a=b;
		b=c;
		}
	return abs(a);
	}

	int sundaram(int n)
	{
	int *a = new int [n],i,j,k;
	memset(a,0,sizeof(int) * n);
	for(i =1;3*i+1<n; i++)
	{
	for(j=1; (k=i+j+2*i*j) < n && j <= i; j++)
	a[k]=1;
	}
	for(i=n-1;i>=1;i--)
	{
	if(a[i] == 0)
	{
	return (2*i+1);
	break;
	}
	}
	delete []a;
	}
