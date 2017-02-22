#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
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

bool millerTest(long long int d, long long int n)
{
	time_t t;
	srand((unsigned) time(&t));
	long long int a = 2+rand()%(n-4);

	long long int x = power(a,d,n);
	if (x == 1|| x == n-1)
		return true;

	while(d != n-1)
	{
		x = (x*x)%n;
		d *= 2;
		if (x == 1)
			return false;
		if (x == n-1)
			return true;
	}
	return false;
}

bool isPrime(long long int n)
{	
	if (n == 2 || n == 3)
		return true;
	if (n == 1 || n == 4)
		return false;
	if(n%2 == 0)
	{
		return false;
	}
	long long int d ;
	d = n -1;
	while(d%2 == 0)
		d = d/2;
	int k = 6;

	while(k)
	{
		if (millerTest(d,n) == false)
			return false;
		k--;
	}
	return true;
}

long long int permitiveRoot(long long int n)
{
	time_t t;
	srand((unsigned) time(&t));
	long long int trueN = n-1;
	long long int trueP = n;
	n = n-1;

	bool flag = true;
	long long int arr[1000000];
	
	
	int cnt = 0;

	while(n%2 == 0)
	{
		n = n/2;
		if(flag)
		{
			arr[cnt] = trueN/2;
			flag =false;
			cnt++;
		}
	}
	flag =true;
	for(long long int i = 3 ; i*i <= n;i++)
	{
		while(n%i == 0)
		{

			n=n/i;
			if(flag)
			{
				arr[cnt]=trueN/i;
				cnt++;
				flag = false;
			}
		}
		flag =true;
	}
	if(n>2)
	{
		arr[cnt] =trueN/n;
		cnt++;
	}

	
	bool flagP = true;
	long long int a; //random root
	while(1)
	{
		a = 2+rand()%(trueP-3);
		
		for(long long int i=0;i<cnt;i++)
		{
			
			long long int ans = power(a,arr[i],trueP);
			
			if(ans == 1)
			{
				flagP = false;
				break;
			}
		}
		if (flagP)
		{
			break;
		}
		flagP= true;
	}
	
	return a;
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
	int skt,num;
	struct sockaddr_in srvr;
	char arr[41]={' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ',','.','?','0','1','2','3','4','5','6','7','8','9','!'};
	char bffer[1024];
	FILE *fptr;
	fptr = fopen(argv[1],"r");
	
	skt = socket(AF_INET,SOCK_STREAM,0);
	
	if(skt<0)
	{
		
		printf("Failed to create socket...\n");
		exit(1);
	}
	memset((char *)&srvr,0,sizeof(srvr));
	
	srvr.sin_port = htons(12345);
	srvr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(srvr.sin_addr));
	

	printf("connected\n");
	
	if(connect(skt, (struct sockaddr *) &srvr,sizeof(srvr))<0)
	{
		perror("");
		printf("connection failed\n");
		close(skt);
		exit(1);
	}
	

	long long int n;
	time_t t;
	srand((unsigned) time(&t));
	while(1)
	{
		n = rand()%1000000000;
		if(isPrime(n))
		{
			break;
		}
	}
	long long int root = permitiveRoot(n);
	long long int x = 2+rand()%(n-3);
	long long int xa = power(root,x,n);
	printf("prime number -%lld permitiveRoot - %lld xa --- %lld\n",n,root,xa );
	memset(bffer,0,sizeof(bffer));
	sprintf(bffer,"%lld#%lld#%lld",n,root,xa);

	send(skt, bffer, sizeof(bffer), 0);
	memset(bffer,0,sizeof(bffer));
	recv(skt,bffer,1024,0);
	long long int xb = atoi(bffer);
	long long int key = power(xa,xb,n);
	key = key%41;
	printf("key-------%lld\n",key); 

	char *string = NULL;	
	size_t  size = 0;
	fseek(fptr,0,SEEK_END);
	size =ftell(fptr);
	rewind(fptr);
	string = malloc((size+1)*sizeof(*string));
	fread(string,size,1,fptr);
	string[size] ='\0';
	printf("%s\n", string);


	touupper(string);
	char output[1048]="";
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
	printf("sendtext---->%s\n",output);
	send(skt, output, sizeof(output), 0);
	return 0;
}

