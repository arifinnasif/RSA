#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>
#include<conio.h>
#include<windows.h>
void delay(int sec)
{
	int ms=1000*sec;
	clock_t start_time=clock();
	while (clock()<start_time+ms);
}
long long modInverse(long long a,long long m)
{
	long long m0=m;
	long long y=0,x=1;
	if(m==1) return 0;
	while(a>1)
	{
		long long q=a/m;
		long long t=m;
		m=a%m,a=t;
		t=y;
		y=x-q*y;
		x=t;
	}
	if(x<0) x+=m0;
	return x;
}

long long GCD(long long a, long long b)
{
	if(a%b==0) return b;
	return GCD(b, a%b);
}

long long LCM(long long a,long long b)
{
	return (a*b)/GCD(a,b);
}


int *mark, nprime,nprimeall;
long long *prime,*primeall;
void sieve(long long a, long long b)
{
	mark=(int *)calloc((b+1),sizeof(int));
	prime=(long long *)calloc((b-a+1),sizeof(long long));
	primeall=(long long *)calloc((b+1)/3+2,sizeof(long long));
	int i,j,limit=(int)sqrt(b)+2;
	mark[1]=1;
	primeall[nprimeall++]=2;
	if(a<=2) prime[nprime++]=2;
	for(i=4;i<=b;i+=2) mark[i]=1;
	for(i=3;i<=b;i+=2)
		if(!mark[i])
		{
			primeall[nprimeall++]=i;
			if(a<=i)
			{
				prime[nprime++]=i;
			}
			if(i<=limit)
			{
				for(j=i*i;j<=b;j+=2*i) mark[j]=1;
			}
		}
}

long long p1,p2,l,e,d,n;
void keygen()
{
	time_t t;
	srand((unsigned) time(&t));
	sieve(1000,1000000);
	p1=prime[rand()%10];
	p2=prime[rand()%10];
	l=LCM(p1-1,p2-1);
	int i=0;
	do
	{
		e=primeall[12+i++];
	}while(l%e!=1);
	d=modInverse(e,l);
	n=p1*p2;
}

long long bigmod(long long a, long long b, long long M)
{
	if(b==0) return 1%M;
	long long x=bigmod(a,b/2,M);
	x=(x*x)%M;
	if(b%2==1) x=(x*a)%M;
	return x;
}

int main()
{
	char choice;
	int i;
	char text[1024],exitchoice;
	long long key1, key2,enc_out[1024];
	FILE *fp;
	if(fopen("log.lnk","r")==NULL)
	{
		fp=fopen("log.lnk","w");
		keygen();
		fprintf(fp,"%lld %lld %lld",d,e,n);
		fclose(fp);
	}
	xy:
	printf("ENTER YOUR CHOICE BY ENTERING THE CHARACTER IN THE PARENTHESIS\n\n");
	printf("1. ENCRYPT (e)\n2. DECRYPT (d)\n3. ENCRYPT WITH OPTION TO AUTHENTICATE (E)\n4. DECRYPT WITH OPTION TO AUTHENTICATE (D)\n5. SHOW PUBLIC KEYS (s)\n");
	choice=getch();
	//system("cls");
	if(choice=='e')
	{
		//option encrypt
		system("cls");
		printf("You have chosen to encrypt without the option to authenticate!\n\n");
		printf("\nENTER A TEXT: ");
		//gets(text);
		scanf(" %[^\n]",text);
		printf("\nENTER THE PUBLIC KEY 1 OF THE RECEIVER: ");
		scanf("%lld",&key1);
		printf("ENTER THE PUBLIC KEY 2 OF THE RECEIVER: ");
		scanf("%lld",&key2);
		for(i=0;i<strlen(text);i++)
		{
			enc_out[i]=bigmod((long long)text[i],key1,key2);
		}
		printf("Encrypted Output:\n");
		for(i=0;i<strlen(text);i++)
		{
			printf("LINE %d: %llX\n",i+1,enc_out[i]);
		}
	}
	else if(choice=='d')
	{
		//option decrypt
		system("cls");
		printf("You have chosen to decrypt without the option to authenticate!\n\n");
		int message_len;
		long long dec_input[1024],privatekey,publickey_pow,publickey_mod;
		char dec_output[1024];
		fp=fopen("log.lnk","r");
		fscanf(fp,"%lld %lld %lld",&privatekey,&publickey_pow,&publickey_mod);
		printf("ENTER MESSAGE LENGTH: ");
		scanf("%d",&message_len);
		printf("ENTER THE ENCRYPTED MESSAGE (%d HEX NUMBERS):\n",message_len);
		for(i=0;i<message_len;i++)
		{
			printf("LINE %d : ",i+1);
			scanf("%llX",&dec_input[i]);
			dec_output[i]=(char)bigmod(dec_input[i],privatekey,publickey_mod);
		}
		dec_output[i]='\0';
		printf("\nDECRYPTED MESSAGE: ");
		puts(dec_output);
		fclose(fp);
		
	}
	else if(choice=='s')
	{
		system("cls");
		long long KEY0,KEY1,KEY2;//KEY0 is private
		char showchoice;
		fp=fopen("log.lnk","r");
		fscanf(fp,"%lld %lld %lld",&KEY0,&KEY1,&KEY2);
		printf("\nPUBLIC KEY 1: %d\tPUBLIC KEY 2: %d\n\n",KEY1,KEY2);
		fclose(fp);
		printf("DO YOU WANT TO EXIT? (Y for YES | PRESS ANY KEY for NO)");
		showchoice=getch();
		if(showchoice=='y'||showchoice=='Y')
		{
			exit(0);
		}
		else
		{
			system("cls");
			goto xy;
		}
	}
	else if(choice=='E')
	{
		//encrypt with authentication
		system("cls");
		FILE *fp;
		long long privatekey, publickey1, publickey2;
		fp=fopen("log.lnk","r");
		fscanf(fp,"%lld %lld %lld",&privatekey,&publickey1,&publickey2);
		printf("You have chosen to encrypt with the option to authenticate!\n\n");
		printf("\nENTER A TEXT: ");
		//gets(text);
		scanf(" %[^\n]",text);
		printf("\nENTER THE PUBLIC KEY 1 OF THE RECEIVER: ");
		scanf("%lld",&key1);
		printf("ENTER THE PUBLIC KEY 2 OF THE RECEIVER: ");
		scanf("%lld",&key2);
		for(i=0;i<strlen(text);i++)
		{
			enc_out[i]=bigmod((long long)text[i],key1,key2);
			enc_out[i]=bigmod((long long)enc_out[i],privatekey,publickey2);
		}
		printf("Encrypted Output:\n");
		for(i=0;i<strlen(text);i++)
		{
			printf("LINE %d: %llX\n",i+1,enc_out[i]);
		}
		fclose(fp);
	}
	else if(choice=='D')
	{
		//decrypt with authentication
		system("cls");
		printf("You have chosen to decrypt with the option to authenticate!\n\n");
		int message_len;
		long long dec_input[1024],privatekey,publickey_pow,publickey_mod,senderkey1,senderkey2,t;
		char dec_output[1024];
		fp=fopen("log.lnk","r");
		fscanf(fp,"%lld %lld %lld",&privatekey,&publickey_pow,&publickey_mod);
		printf("ENTER THE PUBLIC KEY 1 OF THE SENDER: ");
		scanf("%lld",&senderkey1);
		printf("ENTER THE PUBLIC KEY 2 OF THE SENDER: ");
		scanf("%lld",&senderkey2);//decoy
		printf("ENTER MESSAGE LENGTH: ");
		scanf("%d",&message_len);
		printf("ENTER THE ENCRYPTED MESSAGE (%d HEX NUMBERS):\n",message_len);
		for(i=0;i<message_len;i++)
		{
			printf("LINE %d : ",i+1);
			scanf("%llX",&dec_input[i]);
			t=bigmod(dec_input[i],senderkey1,senderkey2);
			dec_output[i]=(char)bigmod(t,privatekey,publickey_mod);
		}
		dec_output[i]='\0';
		printf("\nDECRYPTED MESSAGE: ");
		puts(dec_output);
		printf("\n\n[IF THE OUTPUT IS GIBBERISH, THE MESSAGE MAY NOT BE FROM THE SENDER WHOSE PUBLIC KEY IS ENTERED!]");
		fclose(fp);
	}
	else
	{
		printf("\nYOU MAY HAVE ENTERED A WRONG KEY!");
		delay(1);
		system("cls");
		goto xy;
	}
	mg:
	printf("\nEXECUTE FOR ANOTHER TIME? (Y/N): ");
	scanf("%c",&exitchoice);
	if(exitchoice=='y'||exitchoice=='Y')
	{
		system("cls");
		goto xy;
	}
	else if(exitchoice=='n'||exitchoice=='N')
	{
		//do nothing
	}
	else
	{
		goto mg;
	}
	return 0;
}
//sodium_hexafluorosilicate
