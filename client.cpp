#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>  //for gethostbyname
#include <pthread.h>
#include <string>
#include <iomanip>
#include <ctime>
#include <map>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

int gcd(int a, int b);
int sundaram(int n);


class RSA{
private:
	string message;
	string message2;
 	string code;
	string codex;
	int ps;
	int qs;
	int p;
	int q;
	int d;
	unsigned int e;
	int n;
	int m;
	map <int, char>	 mess = {
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
	map<int, char>::iterator it = mess.begin();
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
	//	cout << "Ваш открытый ключ: " << e << ", " << n << endl;
	//	cout << "Ваш закрытый ключ: " << d << ", "  << n << endl;

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

	void Crypt(string Message){
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

					code+=to_string(c)+'-';
					}
				}

			}
			codex=code;
			code.clear();


	}
	void Decrtypt(string Code){
		codex = Code;
		string temporary;
		vector <unsigned long long int> Invert;
		unsigned long long crypt;

		//преобразование строки в числа и заполнение вектора
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
// преобразование строки в Чар!!

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

	string GetCode()
	{
		return codex;
	}
	string GetMessage()
	{
		return message;
	}
};


//прототип функции приема данных
void* recvv(void* sockk);


RSA rsa; //создание класса Шифрования
RSA rsa_public;

bool Check = false; //проверка на авторизацию

int main()
{

	char message[1024];
	char Name[1024];
	string mess;
	string login;


    int sock;
    struct sockaddr_in addr;


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

//для доменного адреса - тест
	struct hostent *host;
	//struct sockaddr_int addr2;
	host = gethostbyname("example.com"); // enter your ip adress

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345); // или любой другой порт...
    //addr.sin_addr.s_addr = inet_addr("192.168.0.100");
    addr.sin_addr.s_addr = *(long *) (host->h_addr_list[0]);

	int statcon;
	pthread_t threadrecv;
	 if(statcon = connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }



//Здесь нужно создать поток на прием сообщений
	int statusth = pthread_create(&threadrecv, NULL, &recvv, &sock);

	if (statusth != 0)
	cout << "Жаль, функция pthread_create вернула код ошибки: " << statusth << endl;

	cout << "Подключение успешно!" << endl;
	cout << "Подождите, генерируются ключи" << endl;


int Size;
	//Обмен ключами

	string KeyP = 'k'+to_string(rsa.GetPublicKey());
	strncpy(message, KeyP.c_str(), KeyP.length());
	message[KeyP.length()] = 0;
	send(sock, message, 1024, 0);
	message[0] = 0;

	string KeyN = 'n'+to_string(rsa.GetN());
	strncpy(message,KeyN.c_str(), KeyN.length());
	message[KeyN.length()] = 0;
	send(sock, message, 1024, 0);
	message[0] = 0;

	//авторизация
	string Login;
	string Names;
	string Password;
	string Massiv;
	string finalMess;

	do{
	cout << "Please, enter your login: ";
	cin >> Names;
	cout << "Please, enter your password: ";
	cin >> Password;

	Login = Names + ' ' + Password + ' ';
	rsa_public.Crypt(Login);
	Massiv = 'a'+rsa_public.GetCode();
	Size = Massiv.length();
	strncpy(message, Massiv.c_str(), Size);
	message[Size] = 0;
	send(sock, message, sizeof(message), 0);
	message[0] = 0;

	Massiv.clear();
	Login.clear();
	Names.clear();
	Password.clear();
	//ответ об авторизации

	cout << "Waiting..." << endl;
 	sleep(3);
}while(!Check);
//Отправка сообщений
cout << "Enter your messages:" << endl;

	cin.ignore();
	while(1){
	getline(cin,mess);

	rsa_public.Crypt(mess);
	Size = rsa_public.GetCode().length();
	strncpy(message, rsa_public.GetCode().c_str(), Size);
	message[Size] = 0;


	send(sock, message, sizeof(message), 0);
	message[0] = 0;

	}
    close(sock);

    return 0;
}


	void *recvv(void * sockk)
	{
	int bytes_read;
	char buf[1024];
	int* sock = (int*)sockk;

		while(1)
		{

			string temp;
			string temp2;
			unsigned int messs;
			string finalMess;

		bytes_read = recv(*sock,buf,sizeof(buf),0);
//обработка данных
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
	cout << "Auth fall!" << endl;
	finalMess = buf;
	finalMess.erase(finalMess.begin());
	rsa.Decrtypt(finalMess);
	cout << rsa.GetMessage() << endl;
		buf[0] = '\0';

		finalMess.clear();
}		else if (buf[0] == '@') {
			cout << "Auth success!" << endl;
			finalMess = buf;
			finalMess.erase(finalMess.begin());
			rsa.Decrtypt(finalMess);
			cout << rsa.GetMessage() << endl;
			buf[0] = '\0';
				finalMess.clear();
		Check = true;
}else{
		finalMess = buf;
		rsa.Decrtypt(finalMess);
		cout << rsa.GetMessage() << endl;
		buf[0] = '\0';

		if (bytes_read <= 0)
		{
		close(*sock);

		cout << "Соединение разорвано!" << endl;

		pthread_exit(NULL);

		}

		}
	}

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
