#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
long long int power(long long int a, long long int b, long long int n)
{
	long long int ans = 1;
	a = a%n;
	while(b>0)
	{
		if(b & 1)
			ans = (ans*a) % n;

		b = b>>1;
		a = (a*a)%n;
	}
	return ans;
}
void touupper(char *string)
{
	for(int i = 0;i<strlen(string);i++)
	{
		if(string[i]>='a' && string[i]<='z')
		{
			string[i] = string[i]-'a'+65;
		}
	}
}

int main(int argc ,char * argv[])
{
	FILE *fptr;
	fptr = fopen("output.txt","w");
	if (fptr == NULL)
	{
		printf("File does not exits\n");
		return 0;
	}
	int skt,acptskt,portnumber,num,path_fname,pid;//rval;
	struct sockaddr_in srvr,clnt;
	char arr[41]={' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ',','.','?','0','1','2','3','4','5','6','7','8','9','!'};
	socklen_t addr_size;
	char bffer[1024];
	portnumber=12345;

	skt = socket(AF_INET,SOCK_STREAM,0);

	if(skt<0)
	{
		
		printf("Failed to create socket...\n");
		exit(1);
	}
	printf("Server start\n");
	memset((struct sockaddr*)&srvr,0,sizeof(srvr));


	srvr.sin_family = AF_INET;
	srvr.sin_addr.s_addr = INADDR_ANY;
	srvr.sin_port = htons(portnumber);



	if(bind(skt,(struct sockaddr *)&srvr,sizeof(srvr))<0)
	{
		printf("Bind failed\n");
		perror(" ");
		exit(1);
	}

	listen(skt,10);

	addr_size = sizeof(clnt);
	acptskt = accept(skt,(struct sockaddr*)&clnt, &addr_size);
	if(acptskt==-1)
	{
		
		printf("accept failed\n");
		perror(" ");
		exit(1);
	}
	


	memset(bffer,0,sizeof(bffer));

	recv(acptskt,bffer,1024,0);
	long long int xa;
	long long int n ;
	long long int root;
	sscanf(bffer,"%lld#%lld#%lld",&n,&root,&xa);
/*
	printf("%s\n",bffer );

	printf(" prime-- %lld ",n);

	printf(" root-- %lld ",root);

	printf(" xa--%lld \n",xa);*/

	long long int x = 2+rand()%(n-3);
	long long int xb = power(root,x,n);
	long long int key = power(xa,xb,n);
	key = key%41;
	printf("key-------%lld\n",key); 

	memset(bffer,0,sizeof(bffer));
	sprintf(bffer,"%lld",xb);
	send(acptskt, bffer, sizeof(bffer), 0);

	char string[1048];	
	memset(string,0,sizeof(string));
	recv(acptskt,string,1024,0);
	printf("recived---->%s\n",string);
	key = 41-key;
	touupper(string);
	char output[1048]="";
	memset(output,0,sizeof(output));
	int temp;
	for(int i=0;i<strlen(string);i++)
	{
		if(string[i] == ' ')
		{
			temp = (0+key)%41;
			output[i] = arr[temp];
		}
		if(string[i] == ',')
		{
			temp = (27+key)%41;
			output[i] = arr[temp];
		}
		
		if(string[i] == '.')
		{
			temp = (28+key)%41;
			output[i] = arr[temp];

		}
		
		if(string[i] == '?')
		{
			temp = (29+key)%41;
			output[i] = arr[temp];		
		}
		
		if(string[i] == '!')
		{
			temp = (40+key)%41;
			output[i] = arr[temp];		
		}
		
		if( string[i]>='0' && string[i]<='9' )
		{
			temp = (string[i]-'0'+30+key)%41;
			output[i] = arr[temp];
		}
		
		if( string[i]>='A' && string[i]<='Z' )
		{
			temp = (string[i]-'A'+1+key)%41;
			output[i] = arr[temp];			
		}
	}
	printf("decrypted--->%s\n",output);
	fprintf(fptr, "%s\n",output);
	close(acptskt);
	close(skt);
	fclose(fptr);
	return 0;
}
