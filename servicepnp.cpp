#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <iostream>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <string>
#include <iomanip>
#include <ctime>
#include <map>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <natpmp.h>

using namespace std;

int gcd(int a, int b);
int sundaram(int n);

void redirect(uint16_t privateport, uint16_t publicport)
{
int r;
natpmp_t natpmp;
natpmpresp_t response;
initnatpmp(&natpmp,0,0);
sendnewportmappingrequest(&natpmp, NATPMP_PROTOCOL_TCP, privateport, publicport, 3600);//10 sec
do {
fd_set fds;
struct timeval timeout;
FD_ZERO(&fds);
FD_SET(natpmp.s, &fds);
getnatpmprequesttimeout(&natpmp, &timeout);
select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
r = readnatpmpresponseorretry(&natpmp, &response);
} while(r==NATPMP_TRYAGAIN);
printf("mapped public port %hu to localport %hu liftime %u\n",
response.pnu.newportmapping.mappedpublicport,
response.pnu.newportmapping.privateport,
response.pnu.newportmapping.lifetime);
closenatpmp(&natpmp);
}


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
//		cout << "Ваш открытый ключ: " << e << ", " << n << endl;
//		cout << "Ваш закрытый ключ: " << d << ", "  << n << endl;

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


//класс для авторизации
struct user{
  int id;
  string login;
  string pass;
  bool Authorisation =false ;
};

class Auth
{
private:
  int id;
  string login;
  string pass;
  int check;
  vector <user> Users;

public:
    user User;
  Auth()
  {
  }
  void PrintUsers()
  {
    cout << "\nСписок лиц: " << endl;
    for(auto i : Users){
    cout << i.login << " " << i.pass << endl;
    }
  }

  void Inisialize()
  {
    string pass2;
    ifstream fin("List.txt");
  	while(!fin.eof()){
  	pass2 = User.pass;
  	getline(fin,User.login,' ');
    getline(fin,User.pass, ' ');
    if (pass2 != User.pass)
  	Users.push_back(User);
  	}
  }

  void AddUser(user& Use)
	{
	ofstream fout;
	fout.open("List.txt",ios_base::app); //открываем файл для добавления инфы в конец файла
	fout << Use.login << " " << Use.pass << " ";
	fout.close();
  //Users.push_back(Use);
	}

  user GetUser() const
  {
    return User;
  }

  void ConvertToUser(string &Str)
  {
    int numer = Str.length()-1;
    string temporary;
    for (int j=0; j < numer; j++)
		{
			if(Str[j] == ' ')
			{
				User.login = temporary;
				temporary.clear();
			}
				else {
					temporary += Str[j];
				}
		}
    User.pass = temporary;
    if(CheckUser(User))
    User.Authorisation = true;
		else User.Authorisation = false;
  }

  bool CheckUser(user &Use)
  {
    for (auto i : Users)
    {
      if (i.login == Use.login && i.pass == Use.pass){
      return true;
      }
    }
  }
};




int main()
{
    int listener;
    struct sockaddr_in addr;
    char buf[1024];
    char keys[1024];
    int bytes_read;
		string temp;
		string temp2;

    RSA rsa;
		RSA rsa_public;

		Auth auth;
	  user Use;
		auth.Inisialize();//Инициализация вектора Юзеров

		redirect(12345,12345); //natpmp 3600 sec.

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

	fcntl(listener, F_SETFL, O_NONBLOCK);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 2);

	set<int> clients;
  unsigned int messs;
  map <int, unsigned long long> Keys;
  map <int , unsigned long long>  Nums;
	map <int , bool>  Users;
	map <int , string>  Users2;

	clients.clear();

	while(1)
	{
		//Заполняем множество сокетов
		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(listener, &readset);

		for (set<int>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			FD_SET(*it, &readset);
		}
		//Задаем таймаут
		timeval timeout;
		timeout.tv_sec = 36000;
		timeout.tv_usec = 0;
		//Ждем событияя в одном из сокетов
		int mx = max(listener, *max_element(clients.begin(), clients.end()));
		if(select(mx+1,&readset, NULL, NULL, &timeout) <= 0)
		{
			perror("select");
			exit(3);
		}
		//Определяем тип события и выполняем соответствующие действия
		if(FD_ISSET(listener, &readset))
		{
			//Поступил новый запрос на соединение, используем accept
			int  sock = accept(listener, NULL, NULL);

			if(sock < 0)
			{
				perror("accept");
				exit(3);
			}
		fcntl(sock, F_SETFL,O_NONBLOCK);

		clients.insert(sock);
		}

		map <int,unsigned long long> :: iterator SearchNums;
		map <int,unsigned long long> :: iterator SearchKeys;
		map <int,bool> :: iterator SearchUsers;
		map <int,string> :: iterator SearchUsers2;

	for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
	{

		if(FD_ISSET(*it, &readset))
		{	//поступили данные от клиента, читаем их


			//получение и обработка публичного ключа

      bytes_read = recv(*it, buf, 1024, 0);

			if(buf[0] == 'k'){//публичный ключ
			temp = buf;
			temp.erase(temp.begin());
			messs = stoi(temp);
 		 	Keys.insert(pair <int , unsigned int>(*it, messs));

			temp.clear();
			temp2.clear();

			string KeyP = 'k'+to_string(rsa_public.GetPublicKey());
			strncpy(buf, KeyP.c_str(), KeyP.length());
			buf[KeyP.length()] = 0;
			send(*it, buf, 1024, 0);
			buf[0] = 0;

		} else if(buf[0] == 'n'){//публичная Н
			temp = buf;
			temp.erase(temp.begin());
			messs = stoi(temp);
			Nums.insert(pair <int, unsigned int>(*it, messs));

			temp.clear();
			temp2.clear();

			string KeyN = 'n'+to_string(rsa_public.GetN());
			strncpy(buf,KeyN.c_str(), KeyN.length());
			buf[KeyN.length()] = 0;
			send(*it, buf, 1024, 0);
			buf[0] = 0;

		}else if(buf[0] == 'a'){//логин и пароль
			temp = buf;
			temp.erase(temp.begin());
			rsa_public.Decrtypt(temp);

			int Size;
			string Send;
			string Sende;
			Send = rsa_public.GetMessage();
			buf[0] = 0;
			SearchNums = Nums.find(*it);
			rsa.SetN(SearchNums->second);
			SearchKeys = Keys.find(*it);
			rsa.SetP(SearchKeys->second);



			auth.ConvertToUser(Send);//преобразование строки в структру Юзера
			cout << auth.User.login;
			if(auth.User.Authorisation)
			{
			rsa.Crypt(auth.User.login + ",Hi!");
			cout << " подключился!" << endl;
			Users.insert(pair <int, bool>(*it, auth.User.Authorisation));
			Users2.insert(pair <int, string>(*it, auth.User.login));
			Sende = '@'+rsa.GetCode();
			}
			else
			{
				rsa.Crypt(auth.User.login + " is no exist");
				cout << " не прошел авторизацию!" << endl;
				Sende = '!'+rsa.GetCode();
			}
			//отправка
			Size = Sende.length();
			strncpy(buf, Sende.c_str(), Size);
			send(*it, buf, 1024, 0);
			temp.clear();
			Send.clear();
			Sende.clear();
			buf[0] = 0;


			temp.clear();
		} else{

			temp = buf;
			rsa_public.Decrtypt(buf);

			if(bytes_read <= 0)
			{	//соединение разорвано, удаляем сокет из множества
				close(*it);
				clients.erase(*it);
				//обработать при отключении
				//SearchNums = Nums.find(*it);
				Keys.erase(*it);

				Nums.erase(*it);

				Users.erase(*it);
				SearchUsers2 = Users2.find(*it);
				cout << SearchUsers2->second <<" отключился!" << endl;
				Users2.erase(*it);

				continue;
			}


			for (set<int>::iterator itt = clients.begin(); itt != clients.end(); itt++){
				SearchUsers = Users.find(*itt);
			if (*it != *itt && SearchUsers->second){
				//зашифровка
				int Size;
				string Send;
				SearchUsers2 = Users2.find(*it);
				Send = SearchUsers2->second+": "+rsa_public.GetMessage();
				buf[0] = 0;
				SearchNums = Nums.find(*itt);
				rsa.SetN(SearchNums->second);

				SearchKeys = Keys.find(*itt);
				rsa.SetP(SearchKeys->second);
				rsa.Crypt(Send);
				//отправка
				Size = rsa.GetCode().length();
				strncpy(buf, rsa.GetCode().c_str(), Size);
				buf[Size] = 0;
				cout << "Послание: " << buf << endl;
				send(*itt, buf, sizeof(buf), 0);
      	buf[0]=0;
			} else buf[0]=0;
			}
			}
		}
	}

	}

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
