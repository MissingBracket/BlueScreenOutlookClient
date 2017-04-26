// BlueScreenOutlookClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
#include <iostream>
#include <string>

using namespace std;
void iterateclient(SOCKET wire);
void pootis();
//void Commands(SOCKET wire, int mode);

char *createpopmes(char *source, int k) {
	char *message = new char[8];
	for (int i = 0; i < 4; i++) {
		message[i] = source[i];
	}
	message[4] = ' ';
	message[5] = k + '0';
	message[6] = '\n';
	message[7] = '\0';
	//printf("%s", message);
	return message;
}

bool getcom(char*src, char* compare) {
	char dl[5] = "tits";
	for (int i = 0; i < 4; i++) {
		dl[i] = src[i];
	}dl[4] = '\0';
	cout << dl << endl;
	if (strcmp(dl, compare) == 0)return true;
	else return false;
}

bool GetData(SOCKET sock, void *buffer, int bufferLength)
{
	char *pbuffer = (char *)buffer;

	while (bufferLength > 0)
	{
		int n = recv(sock, pbuffer, bufferLength, 0);
		if (n == SOCKET_ERROR)
		{
			return false;
		}
		else if (n == 0)
			return false;

		pbuffer += n;
		bufferLength -= n;
	}

	return true;
}

bool GetLong(SOCKET sock, long *value)
{
	if (!GetData(sock, value, sizeof(value)))
		return false;
	*value = ntohl(*value);
	return true;
}

bool GetFile(SOCKET sock, FILE *f)
{
	long filesize;
	if (!GetLong(sock, &filesize))
		return false;
	if (filesize > 0)
	{
		char buffer[1024];
		do
		{
			int n = min(filesize, sizeof(buffer));
			if (!GetData(sock, buffer, n))
				return false;
			int offset = 0;
			do
			{
				size_t written = fwrite(&buffer[offset], 1, n - offset, f);
				if (written < 1)
					return false;
				offset += written;
			} while (offset < n);
			filesize -= n;
		} while (filesize > 0);
	}
	return true;
}
bool GetFile2(SOCKET sock)
{
	long filesize;
	if (!GetLong(sock, &filesize))
		return false;
	if (filesize > 0)
	{
		char buffer[1024];
		do
		{
			int n = min(filesize, sizeof(buffer));
			if (!GetData(sock, buffer, n))
				return false;
			cout << buffer;
			filesize -= n;
		} while (filesize > 0);
	}
	return true;
}

char &getmsg(SOCKET wire) {
	char b[128];
	if (recv(wire, b, sizeof(b), NULL) > 0)return *b;
	
}

void InBox(SOCKET wire) {
	char buffer[128] = { 0 };
	recv(wire, buffer, sizeof(buffer), NULL);	
	printf("%s\n", buffer);
	recv(wire, buffer, sizeof(buffer), NULL);
	printf("%s", buffer);
	recv(wire, buffer, sizeof(buffer), 0);// printf("3. %s\n", buffer);
	while (strcmp(buffer, "-ERR") != 0) {
		char inner[256] = { 0 };
		//	printf("First: %s\n", buffer);
		//		strcpy(buffer, "");
		recv(wire, inner, sizeof(inner), 0);
		printf("%s\n", inner);// *buffer = { 0 };
		if (strcmp(inner, "-ERR") == 0) { break; }
	}iterateclient(wire);
	//Commands(wire, 2);
	//if (strcmp(buffer, "+OK") == 0)cout << "would begin trnasfer"; exit(1);


}

void BInBox(SOCKET wire) {
	//system("cls");
	char MessBuff[128] = { 0 }; char t[8] = { 0 }; char MB2[128];
	recv(wire, MessBuff, sizeof(MessBuff), NULL);
	printf("1-%s-1\n", MessBuff);
	recv(wire, MB2, sizeof(MB2), NULL);//GetLimiter
	//cout <<t<< strcmp(t, "Sending") << endl;
	cout << "What this?: " << t;
	while (strcmp(MessBuff, "Sending")!=0) {
		//cout << "we in loop\n";
		recv(wire, MessBuff, sizeof(MessBuff), NULL);
		printf("%This is MESSBUFF: s[TermiNAtor]\n", MessBuff); *MessBuff = { 0 };
		//recv(wire, t, sizeof(t), NULL); //cout << t << endl;
		if (strcmp(MessBuff, "-Sending") == 0)cout << "ok, should leave\n";
		else cout << "SHit went wrong : " << MessBuff;
	}iterateclient(wire);
	//Commands(wire, 2);
	cout << "we left here\n";
}

void authenticate(SOCKET wire) {
	char MOTC[256] = { 0 };
	char pass[30], usr[30];
	printf("User:\t"); scanf("%s", &usr);
	printf("PassCode:\t"); scanf("%s", &pass);
	send(wire, usr, strlen(usr), NULL); char state[32] = { 0 };
	recv(wire, state, sizeof(state), NULL);
	send(wire, pass, strlen(pass), NULL);
	recv(wire, MOTC, sizeof(MOTC), NULL);
	//	MOTC[strlen(MOTC)] = '\0';
		/*if (strcmp(MOTC, "Welcome"))throw 1;
		else throw 0;*/	system("cls");
		//printf("%s", MOTC);
	if (strcmp(MOTC, "+OK"))
	{
		send(wire, "LIST", strlen("LIST"), NULL);
		InBox(wire);
	}
	else {printf("-ERR - Wrong Credentials");
	iterateclient(wire);
	}
}
void recmail(SOCKET wire) {
	char buffer[256];
	recv(wire, buffer, sizeof(buffer), NULL);
	if (strcmp(buffer, "+OK")==0){ 
		while (strcmp(buffer, "+0100") != 0) {
			recv(wire, buffer, sizeof(buffer), NULL);
			printf("%s", buffer);
			//if (strcmp(buffer, "+0100") == 0)return;
		}
	}
}
void printfile(char *f) {
	FILE*F = fopen(f, "r"); char messg[256];
	string es;
	while (!feof(F)) {
		fscanf(F,"%s", &messg);
		printf("%s ", messg);
	}fclose(F);
}
void iterateclient(SOCKET wire) {
	char nm[8] = { 0 }; int k;
	while (true) {
		printf("\nGive Input>> "); scanf("%s", &nm);
		if (strstr(nm, "QUIT") != NULL) {
			send(wire, "QUIT", strlen("QUIT"), NULL);
			char state[32] = { 0 };
			recv(wire, state, sizeof(state), NULL);
			printf("Server responded with: %s\n", state);
		}
		else if (strstr(nm, "APOP") != NULL) {
			send(wire, "APOP", strlen("APOP"), NULL);
			authenticate(wire);
		}
		else {
			//printf("Give ID>> ");
			scanf("%d", &k);
			send(wire, createpopmes(nm, k), strlen(createpopmes(nm, k)), NULL);
			if (strstr(nm, "RETR") != NULL) {
				char status[6] = { 0 };
				recv(wire, status, sizeof(status), NULL);
				if (strcmp(status, "+OK") == 0) {
					char name[25] = { 0 }; recv(wire, name, sizeof(name), NULL);
					FILE *file = fopen(name, "wb");
					if (file != NULL)
					{
						remove(name);
						GetFile(wire, file);
						fclose(file); printfile(name);
					}
					FILE *fil = fopen("rscautism.mp4", "wb");
					if (fil != NULL)
					{
						remove("rcautism.png");
						GetFile(wire, fil);
						fclose(fil);
					}
					
				}
				else { printf("Inexistant mail\n"); }
				
			}
			else if (strstr(nm, "DELE") != 0) {
				InBox(wire);
			}
			else if (strcmp(nm, "exit") == 0)exit(1);
		}
	}
}

/*void Commands(SOCKET wire, int mode) {
	string context; char comm[4] = { 0 }; //system("cls");
	while (true)
	{
		switch (mode) {
		case 1:
			while (true) {
				cout << "\n\tKlient Poczty POP3\n\tBlueScreenOutlook\n";
				cout << "Polecenia: \n"
					"\texit / QUIT\n"
					"\tlogin / APOP\n\n"
					"input?:\t"; cin >> context;
				if (context == "login" || context == "APOP") {
					system("cls");
					send(wire, "APOP", strlen("APOP"), NULL);
					authenticate(wire);
				}
				else if (context == "exit" || context == "Exit") {
				//	system("cls");
					send(wire, "QUIT", strlen("QUIT"), NULL);
					exit(1);
				}

			}break;
		case 2:
			cout << "\n\tBlueScreen Outlook\n\tInBox\n";
			cout << "Polecenia: \n"
				"RETR - odczytaj meila nr...\n"
				"DELE <int> - usun meila nr...\n"
				"QUIT - wyloguj\n\n"
				"input?:\t"; cin >> context; //int poot; char d[8]; scanf("%s", &d);
		//	if (getcom(d, "poot"))exit(0);
			if (context == "DELE") {
				send(wire, "DELE", strlen("DELE"), NULL);
				char *mes = "DELE"; int id;
				printf("Got Message DELE: input iD: ");
				scanf("%d", &id);
				//printf("Message to send %s\n", createpopmes(mes, id));
				send(wire, createpopmes(mes, id), strlen(createpopmes(mes, id)), NULL);
				exit(1);
				/*send(wire, "DELE", strlen("DELE"), NULL); Sleep(50);
				int k;
				char nm[8] = { 0 }; scanf("%d", &nm);
				send(wire, nm, strlen(nm),NULL);
				InBox(wire);*/
		/*	}
			else if (context == "RETR") { 
				char buf[64] = { 0 }; scanf("%s", &buf); char status[8] = { 0 };
				send(wire, "RETR", strlen("RETR"), NULL);
				send(wire, buf, strlen(buf), NULL);
				recv(wire, status, sizeof(status), NULL);
				if (strcmp(status, "+OK") == 0)
				{
					FILE *file = fopen(buf, "wb");
					if (file != NULL)
					{
						remove(buf);
						GetFile(wire, file);
						fclose(file); printfile(buf);
					}
					FILE *fil = fopen("rscautism.mp4", "wb");
					if (fil != NULL)
					{
						remove("rcautism.png");
						cout << "here";	GetFile(wire, fil); cout << "there";
						fclose(fil);
					}
				}
				
				else {
					printf("Inexistant MailName(-ERR)\n");Commands(wire, 2);}
				
			}
			else if (context == "dank") {
				cout << "here"; char beef[16] = { 0 };
				fgets(beef, 16, stdin); cout << "shite"<<beef;
				send(wire, beef, sizeof(beef), NULL);
			}
			else if (context=="rd"){
				send(wire, "rd", sizeof("rd"), NULL);
				char bf[16]; scanf("%s",&bf);
				send(wire, bf, sizeof(bf), NULL);
				recmail(wire);
				FILE *fil = fopen("rscautism.mp4", "wb");
				if (fil != NULL)
				{
					remove("rcautism.png");
					GetFile(wire, fil);
					fclose(fil);
				}
			}
			else if (context == "QUIT") { cout << "Logged Out"; Commands(wire, 1); }
			else { cout << "Unrecognised Command\n"; Commands(wire, 2); }
			break;
		}
	}
}*/
int main()
{
	
	//pootis();
	//FILE* RCVFILE;
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "WINSOCK At line 188", "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");//127.0.0.1 - 150.254.145.51
	addr.sin_port = htons(110);//1111/110
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, addrlen) != 0)
	{
		MessageBoxA(NULL, "WINSOCK At line 200", "ERROR", MB_OK | MB_ICONERROR); 
		return 0;

	}
	printf("Connection to server established\n");
	char MOTC[5] = { 0 };
	/*recv(Connection, MOTC, sizeof(MOTC), NULL);
	printf("%s\n", MOTC);*/
	
	recv(Connection, MOTC, sizeof(MOTC), NULL);
	if (strcmp(MOTC, "+OK") == 0)iterateclient(Connection);
		//Commands(Connection,1);
	/*try { authenticate(Connection); }
	catch (int i) {
		switch (i) {
		case 1:cout << "tits" << endl; break;
		case 0:cout << "nope.exe" << endl; break;
		}
	}*/
	
	return 0;
}

void GetCommand(char*dest, char*src) {
	for (int i = 0; i < 4; i++) {
		dest[i] = src[i];
	}dest[4] = '\0';
}

void pootis() {
	char tr[8];
	
	while (true) {
		scanf("%s", &tr);
		if (strstr(tr, "pootis") != NULL) {
			printf("Got a match!\n");
		}
	}
	/*string shell;
	char o[4];
	char*d;
	int a;
	while (true) {
		//getline(cin, shell);
		fgets(k, 8, stdin);
		a = k[5] - '0';
		GetCommand(o, k);
		printf("%d\n",a);
		printf(o);
		if (strcmp(o, "retr") == 0)printf("shite");
		//a = (int)shell[5];
		//cout << endl << shell << " " << a << endl;
	}return 0;*/
}
