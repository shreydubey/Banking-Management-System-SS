#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <fcntl.h>
#define PORT 8000
struct customer
{
char username[1024];
char password[1024];
char username_2[1024];
char password_2[1024];
char account_number[1024];
double balance;
bool type;
};
struct date
{
    int mon,date,year;
};
struct transaction_log
{
struct date dob;
char account_number[1024];
double balance;
char type[30];
};



void service_cli(int sock);
int signup(int sock);
int signupadmin(int sock);
int customer_operation(int sock,int fd,int cnt,int second);
int Deposit(int sock,int fd,int cnt);
int Withdraw(int sock,int fd,int cnt);
int Balance_Enquiry(int sock,int fd,int cnt);
int Password_change(int sock,int fd,int cnt,int second);
int Admin_operation(int sock,int fd,int cnt);
int Add_account(int sock,int fd,int cnt); 
int Delete_account(int sock,int fd,int cnt);
int Search(int sock,int fd,int cnt);
int Modify_Account(int sock,int fd,int cnt);
//int Transaction(int sock,int fd,int cnt,int second);



void service_cli(int sock){
	int func_id;
	printf("Client [%d] connected\n", sock);
	while(1){		
		printf("Reading option\n");
		read(sock, &func_id, sizeof(int));
		printf("Read %d\n",func_id);
		if(func_id==1) {signup(sock);}
        else if(func_id==2){signupadmin(sock);}
		else break;
	}
	close(sock);
	printf("Client disconnected\n");
}
int signup(int sock)
{
    write(sock,"Enter Details",14);
    int fd,valid=0,cnt=0,second;
    char name[20],pass_word[20];
    read(sock,&name,sizeof(name));
    read(sock,&pass_word,sizeof(pass_word));
    printf("User_name %s\n",name);
    printf("user_password %s\n",pass_word);
    struct customer c;
    fd=open("customer",O_RDONLY|O_RDWR,0644);
    if(fd==-1)
    {
        perror("File doesn't open");
        exit(EXIT_FAILURE);
    }
    lseek(fd,0,SEEK_SET);
    while(read(fd,&c,sizeof(c)))
    {
        cnt++;
        if((c.type==0&&!strcmp(c.username,name)&& !strcmp(c.password,pass_word)))
            {valid=1;second=1;break;}
        if((c.type==0&&!strcmp(c.username_2,name)&& !strcmp(c.password_2,pass_word)))
        {valid=1;second=2;break;}
    }
    write(sock,&valid,sizeof(valid));
    if(valid==1)
    {

        customer_operation(sock,fd,cnt,second);
    }
    return 3;
}



int signupadmin(int sock)
{
    write(sock,"Enter Details",14);
    int fd,valid=0,cnt=0;
    char name[20],pass_word[20];
    read(sock,&name,sizeof(name));
    read(sock,&pass_word,sizeof(pass_word));
    printf("User_name %s\n",name);
    printf("user_password %s\n",pass_word);
    struct customer c;
    fd=open("customer",O_RDONLY|O_RDWR,0644);
    if(fd==-1)
    {
        perror("File doesn't open");
        exit(EXIT_FAILURE);
    }
    lseek(fd,0,SEEK_SET);
    while(read(fd,&c,sizeof(c)))
    {
        cnt++;
        if((c.type==1&& !strcmp(c.username,name)&& !strcmp(c.password,pass_word))||(c.type==1&&!strcmp(c.username_2,name)&& !strcmp(c.password_2,pass_word)))
            {valid=1;break;}
    }
    write(sock,&valid,sizeof(valid));
    if(valid==1)
    {
        Admin_operation(sock,fd,cnt);
       
    }
    return 3;
}






int customer_operation(int sock,int fd,int cnt,int second)
{
    while(1)
   { 
    int choice;
    read(sock,&choice,sizeof(choice));
    if(choice==1)
        Deposit(sock,fd,cnt);
   else if(choice==2)
        Withdraw(sock,fd,cnt);
    else if(choice==3)
        Balance_Enquiry(sock,fd,cnt);
    else if(choice==4)
		Password_change(sock,fd,cnt,second);
    else if(choice==5)
        //Transaction(sock,fd,cnt,second);
        continue;
    else 
        break;
    }
    return 0;
}



int Deposit(int sock,int fd,int cnt)
{
    struct customer temp;
   struct transaction_log tlog;
    int amt,file,dd,mm,yy;
    char name[20];
    //read(sock,&name,sizeof(name));
    read(sock,&amt,sizeof(&amt));
    read(sock,&dd,sizeof(dd));
    read(sock,&mm,sizeof(mm));
    read(sock,&yy,sizeof(yy));

    cnt--;
   // printf("%d\n",cnt);

   printf("%s\n",name);
   printf("%d\n",amt);
   printf("%d-%d-%d\n",dd,mm,yy);



 
    lseek(fd,cnt*sizeof(struct customer),SEEK_SET);
    //write(fd,&c,sizeof(c));
    read(fd,&temp,sizeof(temp));
    strcpy(name,temp.username);
    temp.balance=temp.balance+amt;
    file=open(name,O_RDWR,0644);
    lseek(file,0,SEEK_END);
    tlog.dob.date=dd;
    tlog.dob.mon=mm;
    tlog.dob.year=yy;
    strcpy(tlog.account_number,temp.account_number);
    tlog.balance=temp.balance;
    strcpy(tlog.type,"Deposit");
    lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
    write(fd,&temp,sizeof(temp));
    write(file,&tlog,sizeof(tlog));
    write(sock,"Money Deposited Successfully",29);
    return 0;
}


int Withdraw(int sock,int fd,int cnt)
{
   
    struct customer temp;
    struct transaction_log tlog;
    int amt,file,dd,yy,mm;
    read(sock,&amt,sizeof(&amt));
    read(sock,&dd,sizeof(dd));
    read(sock,&mm,sizeof(mm));
    read(sock,&yy,sizeof(yy));
    char name[20];
    cnt--;
    lseek(fd,cnt*sizeof(struct customer),SEEK_SET);
    //write(fd,&c,sizeof(c));
    read(fd,&temp,sizeof(temp));
    temp.balance=temp.balance-amt;
    strcpy(name,temp.username);
    file=open(name,O_RDWR,0644);
    lseek(file,0,SEEK_END);
    tlog.dob.date=dd;
    tlog.dob.mon=mm;
    tlog.dob.year=yy;
    strcpy(tlog.account_number,temp.account_number);
    tlog.balance=temp.balance;
    strcpy(tlog.type,"Withdraw");
    lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
    write(fd,&temp,sizeof(temp));
    write(file,&tlog,sizeof(tlog));
    write(sock,"Money Withdraw Successfully",29);
    return 0; 
}

int Balance_Enquiry(int sock,int fd,int cnt)
{
    struct customer temp;
    double x;
    cnt--;
    lseek(fd,cnt*sizeof(struct customer),SEEK_SET);  
    read(fd,&temp,sizeof(temp));
    x=temp.balance;
    write(sock,&x,sizeof(x));
    return 0;
}


int Password_change(int sock,int fd,int cnt,int second)
{
    struct customer temp;
   int confirm,wrong=5;
   bool flag=false;
    char buf1[30]="Enter the old password";
    char buf2[30]="Enter the new password";
    char oldpass[20],newpass[20];
   read(sock,&confirm,sizeof(confirm));
   if(confirm)
   {
       read(sock,&oldpass,sizeof(oldpass));
       printf("%s\n",oldpass);
       cnt--;
       lseek(fd,cnt*sizeof(struct customer),SEEK_SET);  
        read(fd,&temp,sizeof(temp));
 
       if((!strcmp(temp.password,oldpass))||(!strcmp(temp.password_2,oldpass)))
       {
           flag=true;
            write(sock,&flag,sizeof(flag));
           read(sock,&newpass,sizeof(newpass));
           if(second==1)
           strcpy(temp.password,newpass);
           if(second==2)
           strcpy(temp.password_2,newpass);
           lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
           write(fd,&temp,sizeof(temp));
    
       }
       else write(sock,&flag,sizeof(flag));
   }
   return 0; 
}















/**************************ADMIN MODULE**************************/

int Admin_operation(int sock,int fd,int cnt)
{
   while(1)
    {
    int choice;
   read(sock,&choice,sizeof(choice));
   if(choice==1)Add_account(sock,fd,cnt); 
   if(choice==2)Delete_account(sock,fd,cnt);
   if(choice==3)Modify_Account(sock,fd,cnt);
   if(choice==4)Search(sock,fd,cnt);
   if(choice==5)break;
   }
}

int Add_account(int sock,int fd,int cnt)
{
    struct transaction_log tlog;
   	char name[20],pass_word[20],acc_no[20],name_1[20],pass_word_1[20],check[20];
	double bal;
	bool type;
    int file,dd,mm,yy;
    read(sock,&name,sizeof(name));
    read(sock,&pass_word,sizeof(pass_word));
    read(sock,&acc_no,sizeof(acc_no));
    read(sock,&type,sizeof(type));
    read(sock,&name_1,sizeof(name_1));
	read(sock,&pass_word_1,sizeof(pass_word_1));

    read(sock,&dd,sizeof(dd));
    read(sock,&mm,sizeof(mm));
    read(sock,&yy,sizeof(yy));

    printf("%s\t\t",name);
    printf("%s\t\t",pass_word);
    printf("%s\t\t",name_1);
    printf("%s\t\t",pass_word_1);
    printf("%s\t\t",acc_no);
    printf("%f\t\t",bal);
    printf("%d\n",type);
    struct customer temp;
    lseek(fd,0*sizeof(struct customer),SEEK_END);
    strcpy(temp.username,name);
    strcpy(temp.account_number,acc_no);
    strcpy(temp.password,pass_word);
    strcpy(temp.account_number,acc_no);
    strcpy(temp.password_2,pass_word_1);
    strcpy(temp.username_2,name_1);
    temp.type=type;
    temp.balance=0.00;
    write(fd,&temp,sizeof(temp));
    strcpy(check,name);
    file=open(check,O_CREAT|O_RDWR,0644);
    tlog.dob.date=dd;
    tlog.dob.mon=mm;
    tlog.dob.year=yy;
    strcpy(tlog.account_number,acc_no);
    tlog.balance=0.00;
    strcpy(tlog.type,"Created");
    write(file,&tlog,sizeof(tlog));
    close(file);
    write(sock,"Account added Successfully",27);
    return 0;
}



int Delete_account(int sock,int fd,int cnt)
{
    char name[20],buf1[30]="Account deleted Successfully";
    char buf2[30]="No such Account exists";
    bool flag=false;
    int confirm;
    struct customer temp;
    read(sock,&name,sizeof(name));
    read(sock,&confirm,sizeof(confirm));
    lseek(fd,0,SEEK_SET);
    printf("Final call for delete %d\n",confirm);
  if(confirm)
  { 
     while(read(fd,&temp,sizeof(temp)))
    {
        if(!strcmp(temp.username,name))
       { 
           strcpy(temp.username,"Deleted");
           strcpy(temp.account_number,"Deleted");
           flag=true;
           lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
           write(fd,&temp,sizeof(temp));
           break;
       }
    }
    if(flag==true)
     write(sock,&buf1,sizeof(buf1));   
    else
        write(sock,&buf2,sizeof(buf2));    
    }

}




int Search(int sock,int fd,int cnt)
{
    char buf[30],name[20],pass_word[20];
    bool flag=false;
    double bal;
    read(sock,&buf,sizeof(buf));
    printf("%s\n",buf);
    lseek(fd,0,SEEK_SET);
    struct customer temp;
    while(read(fd,&temp,sizeof(temp)))
    {
        if(!strcmp(temp.account_number,buf)||!strcmp(temp.username,buf))
        {
             flag=true;
             strcpy(name,temp.username);
             strcpy(pass_word,temp.password);
             bal=temp.balance;
            write(sock,&flag,sizeof(flag));
            write(sock,&name,sizeof(name));
            write(sock,&pass_word,sizeof(pass_word));
            write(sock,&bal,sizeof(bal));
            write(sock,&temp.account_number,sizeof(temp.account_number));
            write(sock,&temp.type,sizeof(temp.type));
           
            break;
        }
    }
}

int Modify_Account(int sock,int fd,int cnt)
{
    while(1)
    {
    struct customer temp;
    int option;
    bool flag=false;
    char name[20],name_2[20],pass_word[20],pass_word_2[20];
    read(sock,&option,sizeof(option));
    read(sock,&name,sizeof(name));
    if(option==1)
    {
        read(sock,&pass_word,sizeof(pass_word));
        lseek(fd,0,SEEK_SET);
        while(read(fd,&temp,sizeof(temp)))
        {
            if(!strcmp(temp.username,name))
            {
                strcpy(temp.password,pass_word);
                lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
                write(fd,&temp,sizeof(temp));
                flag=true;
                break;
            }
            if(!strcmp(temp.username_2,name_2))
            {
                strcpy(temp.password_2,pass_word);
                lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
                write(fd,&temp,sizeof(temp));
                flag=true;
                break;
            }
        }
        if(flag)
            write(sock,"Password modified Successfully",31);
        else
            write(sock,"No such Username Exists",25);
    }
    else if(option==2)
    {
        read(sock,&name_2,sizeof(name_2));
        read(sock,&pass_word_2,sizeof(pass_word_2));
        lseek(fd,0,SEEK_SET);
        while(read(fd,&temp,sizeof(temp)))
        {
            if(!strcmp(temp.username,name))
            {
                strcpy(temp.username_2,name_2);
                strcpy(temp.password_2,pass_word_2);
                lseek(fd,-1*sizeof(struct customer),SEEK_CUR);
                write(fd,&temp,sizeof(temp));
                flag=true;
                break;
            }
        }
        if(flag)
            write(sock,"Joint User Added",17);
        else
            write(sock,"No such user exists",20);
    }
    else break;
    }
    return 0;
}






























int main()
{
        int sockfd;
        struct sockaddr_in servaddr;
        int newsocket;
        struct sockaddr_in newaddr;
        char buf[1024];
        int addr_size=sizeof(newaddr);
        printf("\nInitializing connection...\n");
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sockfd < 0)
        {
            printf("socket Creation failed\n");
            exit(0);
        }
    int optval = 1;
	int optlen = sizeof(optval);
	/*
	to close socket automatically while terminating process
	SOL_SOCKET : to manipulate option at API level o/w specify level
	*/
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, optlen)==-1){
		printf("set socket options failed\n");
		exit(EXIT_FAILURE);
	}
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;
        printf("Binding socket...\n");
        if(bind(sockfd, (void*)&servaddr, sizeof(servaddr))==-1)
        {
            printf("Bind failed\n");
            exit(EXIT_FAILURE);
        }
        if(listen(sockfd, 100)==-1)
        {
            printf("Listen failed\n");
            exit(EXIT_FAILURE);
        }
        printf("Listening...\n");
	while(1)
	{
        newsocket = accept(sockfd, (void*)&newaddr, &addr_size);
		if(fork()==0)
		{
			service_cli(newsocket);
			exit(1);
        }
		
	}
    close(sockfd);
    printf("Connection closed\n");
    return 0;
}
                                              
