#include <unistd.h>
#include <sys/socket.h>


#include <string.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

//dn domain name
//length the length of string dn
//ipadd result holder
//return -1 error
//return 0 ok
//return 1 end of addrinfo list


//3 cases

//usage 1
//dn!=0 normal use

//usage 2
//dn==0, length >= 0
//move to the #length entry
//start with 0


//usage 3 
//dn==0, length < 0
//free up the memory

//notice when changing a new stun server,
//you should always free up the memory by 
//calling usage 3
int get_stun_addr(const char* dn, int length, struct in_addr* ipadd)
{
	struct addrinfo* aip;
	static struct addrinfo* ailist = 0;
	struct sockaddr_in* sinp;
	
	
	
	//case 3
	if(dn == 0 && length < 0)
	{
		
		if(ailist)
			freeaddrinfo(ailist);
		ailist=0;
		return 0;
	}
	

	
	//case 1
	if(dn!=0)
	{
		
		int i=0;
		if(ailist)
		{
			//warning, older ipadd may be wasted
			printf("memory leak may occur\n");
			//don't worry, these code recover the memory 
			freeaddrinfo(ailist);
		}
		
		while(dn[i]!=0)
		{
			i++;
			if(i>=length)
			{
				//error, string abnormal
				printf("Stun Server name error\n");
				exit(0);
			}
		}
		
		if(getaddrinfo(dn, 0, 0, &ailist)<0)
		{
			//error, function call abnormal
			printf("failed to get ip infomation\n");
			exit(0);
		}
		
		
		
		aip=ailist;
		/*
		sinp=(struct sockaddr_in*)aip->ai_addr;
		ipadd=&sinp->sin_addr;
		return 0;
		*/
	}
	
	//case 2
	else if(dn == 0 && length>=0)
	{
		
		//a bit redundent with case 1
		aip=ailist;
		
		for(int ct=0; ct<=length-1; ct++)
		{
			aip=aip->ai_next;
			if(aip==NULL)
			{
				//not found
				//end of ailist reached
				return 1;
			}
		}
		
		/*
		//still redundent part with case 1
		sinp=(struct sockaddr_in*)aip->ai_addr;
		ipadd=&sinp->sin_addr;
		return 0;
		*/
	}
	
	
	else
	{
		//unexpected para
		printf("unexpected para\n");
		exit(0);
	}
	
	
	//for both case 1 and 2
	sinp=(struct sockaddr_in*)aip->ai_addr;
	
	//a substitude
	//*ipadd=sinp->sin_addr;
	memcpy(ipadd, &(sinp->sin_addr), sizeof(struct in_addr));
	
	
	return 0;

}


int main()
{
	
	const char* addr;
	char abuf[INET_ADDRSTRLEN];
	struct sockaddr_in hello;
	get_stun_addr("www.google.com",15, &(hello.sin_addr));
	addr=inet_ntop(AF_INET, &(hello.sin_addr), abuf, INET_ADDRSTRLEN);
	printf("%s\n", addr);
		
}

