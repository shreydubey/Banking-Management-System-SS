#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdbool.h>
#include <fcntl.h>
#define PORT 8000


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


						//Function Declaration
int entry_point(int sock);
int customer_operation(int sock);
int Deposit(int sock);
int Withdraw(int sock);
int Balance_Enquiry(int sock);
int Password_change(int sock);
int Admin_operation(int sock);
int Add_account(int sock);
int Delete_account(int sock);
int Search(int sock);
int Modify_account(int sock);
int Transaction(int sock);



						//Function Defination
int entry_point(int sock)
{
	while(1)
	{   
	int choice,valid;
    system("clear");
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("\t1. Sign In as Customer\n");
	printf("\t2. Sign In as Admin\n");
	printf("\t3. Exit\n");
	printf("Enter Your Choice\n");
	scanf("%d", &choice);

    while(choice > 3 || choice < 1)
	{
		printf("Invalid Choice!\n");
		printf("Enter Your Choice\n");
		scanf("%d", &choice);
	}

    write(sock,&choice,sizeof(choice));
    if(choice==1)
    {
        char buf[20],name[20],pass_word[20];
        read(sock,buf,sizeof(buf));
        printf("%s\n",buf);
		
        printf("Enter the username :");
        scanf("%s",name);
		strcpy(pass_word,getpass("Enter password: "));
        write(sock,&name,sizeof(name));
		write(sock,&pass_word,sizeof(pass_word));
		read(sock,&valid,sizeof(valid));
		if(valid==1)
		{

			printf("******************Login Successful**************************\n");
			sleep(2);
			customer_operation(sock);
		}
		else
		{
			printf("**********************Login failed **************************\n");
			sleep(2);
		}
    }
    else if(choice==2)
	{
		char buf[20],name[20],pass_word[20];
        read(sock,buf,sizeof(buf));
        printf("%s\n",buf);
		
        printf("Enter the username :");
        scanf("%s",name);
		strcpy(pass_word,getpass("Enter password: "));
        write(sock,&name,sizeof(name));
		write(sock,&pass_word,sizeof(pass_word));
		read(sock,&valid,sizeof(valid));
		if(valid==1)
		{
			printf("******************************Login Successful**********************************\n");
			sleep(2);
			Admin_operation(sock);
		}
		else
		{
			printf("*******************************Login failed************************************* \n");
			sleep(2);
		}
	}
	else break;
}
return 3;
}



int customer_operation(int sock)
{
	while(1)
	{	
	int choice;
	system("clear");
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("Choose the options\n");
	printf("\t1.Deposit\n\t2.Withdraw\n\t3.Balance Enquiry\n\t4.Password change\n\t5.Transaction\n\t6.Back\n");
	printf("Enter your choice :");
	scanf("%d",&choice);
	printf("\n");
	while(choice<1||choice>6)
	{
		printf("INVALID option\n");
		printf("Select option between 1 to 5\n");
		scanf("%d",&choice);
	}
	write(sock,&choice,sizeof(choice));
	if(choice==1)
		Deposit(sock);
	else if(choice==2)
		Withdraw(sock);
	else if(choice==3)
		Balance_Enquiry(sock);
	else if(choice==4)
		Password_change(sock);
	else if(choice==5)
		Transaction(sock);
	else 
		break;
	}
		return 0;
}



int Deposit(int sock)
{
	int amt,date,month,year;
	char buf[30],name[20];
	//printf("Enter the username again :");
	//scanf("%s",name);
	printf("Enter the amount you want to deposit :");
	scanf("%d",&amt);
	printf("-------------------------------------------------------------------------\n");	
	printf("Enter Date like this 01,07,26..... \n");
	printf("Enter Month Like this 01,02,03,....,10,11,12\n");
	printf("Enter Year like this 2020,2021,2022....\n");
	printf("-------------------------------------------------------------------------\n");
	printf("Enter Today's date :");
	scanf("%d",&date);
	printf("Enter current Month :");
	scanf("%d",&month);
	printf("Enter the current year :");
	scanf("%d",&year);

	//write(sock,&name,sizeof(name));
	write(sock,&amt,sizeof(amt));
	write(sock,&date,sizeof(date));
	write(sock,&month,sizeof(month));
	write(sock,&year,sizeof(year));

	read(sock,buf,sizeof(buf));
	printf("%s\n",buf);
	printf("---------------------SERVER BUSY-------------------\n");
	sleep(5);
}


int Withdraw(int sock)
{
	int amt,date,month,year;
	char buf[30];
	printf("Enter the amount you want to withdraw\n");
	scanf("%d",&amt);


	printf("-------------------------------------------------------------------------\n");	
	printf("Enter Date like this 01,07,26..... \n");
	printf("Enter Month Like this 01,02,03,....,10,11,12\n");
	printf("Enter Year like this 2020,2021,2022....\n");
	printf("-------------------------------------------------------------------------\n");
	printf("Enter Today's date :");
	scanf("%d",&date);
	printf("Enter current Month :");
	scanf("%d",&month);
	printf("Enter the current year :");
	scanf("%d",&year);


	write(sock,&amt,sizeof(amt));
	write(sock,&date,sizeof(date));
	write(sock,&month,sizeof(month));
	write(sock,&year,sizeof(year));
	read(sock,buf,sizeof(buf));
	printf("%s\n",buf);	
	printf("---------------------SERVER BUSY-------------------\n");
	sleep(5);
}


int Balance_Enquiry(int sock)
{
	double buf;
	read(sock,&buf,sizeof(buf));
	printf("Your Current Balance is %0.2f \n",buf);
	printf("---------------------SERVER BUSY-------------------\n");
	sleep(5);
}


int Password_change(int sock)
{
	int confirm,wrong=5;
	char buf1[30],buf2[30],oldpass[20],newpass[20];
	bool flag,flag_1;
	printf("Are you sure you want to change Password\n");
	printf("Press 0 to not confirm \n press 1 to confirm  \n");
	scanf("%d",&confirm);
	write(sock,&confirm,sizeof(confirm));
	//read(sock,&confirm,sizeof(confirm));
	if(confirm)
	{
		
		strcpy(oldpass,getpass("Enter old Password:"));
		write(sock,&oldpass,sizeof(oldpass));
		read(sock,&flag,sizeof(flag));
		
		if(flag)
		{
			strcpy(newpass,getpass("Enter new Password:"));
			write(sock,&newpass,sizeof(newpass));
		}
		else
			printf("your password is incorrect. \n");
		sleep(3);
		
	}
	return 0;
}


int Transaction(int sock)
{
 char name[20];
 struct transaction_log tlog;
 printf("Enter the username :");
 scanf("%s",name);
 int fp=open(name,O_RDONLY,0644);
 lseek(fp,0,SEEK_SET);
 printf("-----------------MINI STATEMENT ---------------\n");
 printf("Date\t\t Account_No.\tBalance\n");
 printf("-------------------------------------------------\n");
 while(read(fp,&tlog,sizeof(tlog)))
 {
	 printf("%d-%d-%d\t",tlog.dob.date,tlog.dob.mon,tlog.dob.year);
	 printf("%s\t",tlog.account_number);
	 printf("%f\t",tlog.balance);
	 printf("%s\n",tlog.type);
 }
printf("-------------------------------------------------\n");
sleep(5);
}












/***********************************ADMIN MODULE************************/

int Admin_operation(int sock)
{
	while(1)
	{
	int choice;
	system("clear");
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("\n\n");
	printf("-----------------------  WELCOME ADMIN  ---------------------\n");
	printf("Choose the option\n");
	printf("\t1.Add Account\n\t2.Delete Account\n\t3.Modify an account\n\t4.Search\n\t5.Exit\n");
	printf("Enter your choice:    ");
	scanf("%d",&choice);
	printf("\n");
	while(choice<1 && choice >5)
	{
		printf("Enter your choice between 1 to 4");
		scanf("%d",&choice);
		printf("\n");
	}
	write(sock,&choice,sizeof(choice));
	if(choice==1)Add_account(sock);
	if(choice==2)Delete_account(sock);
	if(choice==3)Modify_account(sock);
	if(choice==4)Search(sock);
	if(choice==5){printf("Have a great day\n");sleep(2);break;}

	}

}




int Add_account(int sock)
{
	char name[20],pass_word[20],acc_no[20],buf[30],name_1[20],pass_word_1[20];
	double bal;
	bool type;
	int x,date,month,year;
	system("clear");
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("\n");
	printf("----------------IMPORTANT----------------\n");
	printf("* Username must be at most length 20\n");
	printf("* Password must be at most length 20\n");
	printf("* Account number must be at most length 20\n");
	printf("----------------------------------------\n");
	printf("\n");
	printf("Enter the username: ");
	scanf("%s",name);
	strcpy(pass_word,getpass("Enter password: "));
	printf("Enter the account number :");
	scanf("%s",acc_no);
	printf("Enter the type of user \n");
	printf("\tpress 0 for Customer\n\tpress 1 for Admin\n ");
	scanf("%d",&x);
	type=x;
	printf("\n");
	printf("-----------------------------IMPORTANT-----------------------------------\n");
	printf("If you want Joint account type name and password\nIf you don't want Joint Account Type NULL in username and Password\n");
	printf("-------------------------------------------------------------------------\n");
	printf("Enter the username :");
	scanf("%s",name_1);
	strcpy(pass_word_1,getpass("Enter password: "));
printf("\n");

	printf("-------------------------------------------------------------------------\n");	
	printf("Enter Date like this 01,07,26..... \n");
	printf("Enter Month Like this 01,02,03,....,10,11,12\n");
	printf("Enter Year like this 2020,2021,2022....\n");
	printf("-------------------------------------------------------------------------\n");
	printf("\n");
	printf("Enter Today's date :");
	scanf("%d",&date);
	printf("Enter current Month :");
	scanf("%d",&month);
	printf("Enter the current year :");
	scanf("%d",&year);

	write(sock,&name,sizeof(name));
	write(sock,&pass_word,sizeof(pass_word));
	write(sock,&acc_no,sizeof(acc_no));
	write(sock,&type,sizeof(type));
	write(sock,&name_1,sizeof(name_1));
	write(sock,&pass_word_1,sizeof(pass_word_1));
	write(sock,&date,sizeof(date));
	write(sock,&month,sizeof(month));
	write(sock,&year,sizeof(year));


	read(sock,&buf,sizeof(buf));
	printf("%s\n",buf);
	printf("---------------------SERVER BUSY-------------------\n");
	sleep(5);
	return 0;
}

int Delete_account(int sock)
{
	char name[20],buf[30];
	int confirm;
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("\n");
	printf("Enter the username\n");
	scanf("%s",name);
	printf("Are u sure you want to delete\n");
	printf("Press 1 for YES\t\t\t PRESS 0 for NO\n");
	scanf("%d",&confirm);
	write(sock,&name,sizeof(name));
	write(sock,&confirm,sizeof(confirm));
	read(sock,buf,sizeof(buf));
	printf("%s\n",buf);
	printf("---------------------SERVER BUSY-------------------\n");
	sleep(5);
}



int Search(int sock)
{
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("\n");
	int option;
	char name[20],acc_no[20],pass_word[20];
	double bal;
	bool flag;
	printf("On what column you want to search \n");
	printf("Press 0 for Account number \nPress 1 for Username\n");
	scanf("%d",&option);
	if(option)
	{
	printf("Enter the username :");
	scanf("%s",name);
	write(sock,&name,sizeof(name));
	}
	else
	{
		printf("Enter the account number :");
		scanf("%s",acc_no);
		write(sock,&acc_no,sizeof(acc_no));
	}
	read(sock,&flag,sizeof(flag));
	if(flag)
	{
		read(sock,&name,sizeof(name));
		printf("Username %s\n",name);
		read(sock,&pass_word,sizeof(pass_word));
		printf("Password %s\n",pass_word);
		read(sock,&bal,sizeof(bal));
		printf("Balance %0.2f\n",bal);
		read(sock,&acc_no,sizeof(acc_no));
		printf("Account_Number %s\n",acc_no);
		read(sock,&flag,sizeof(flag));
		if(flag==1)printf("Customer Type\n");
		else printf("Admin Type\n");
		sleep(3);
		
	}
	
	
	else 
		printf("No such Username or Account Number Exist\n");
	return 0;

}



int Modify_account(int sock)
{
	while(1)
	{	
	system("clear");
	printf("*************************WELCOME TO SCAM BANK*******************************\n");
	printf("\n");
	printf("------------------  MODIFY_ACCOUNT  --------------------\n");
	printf("Choose option\n");
	printf("1.Modify user Account Password\n");
	printf("2.Joint Account\n");
	printf("3.Exit\n");
	int option;
	char name[20],name_2[20],pass_word_2[20],pass_word[20],buf[35];
	printf("Choose option - ");
	scanf("%d",&option);
	printf("\n");
	while(option<1 || option>3)
	{
		printf("INVALID option\n");
		printf("Choose option between 1 and 2 ");
		scanf("%d",&option);
		printf("\n");
	}
	write(sock,&option,sizeof(option));
	if(option==1)
	{
		printf("Enter the username :");
		scanf("%s",name);
		write(sock,&name,sizeof(name));
		strcpy(pass_word,getpass("Enter new password: "));
		write(sock,&pass_word,sizeof(pass_word));
		read(sock,&buf,sizeof(buf));
		printf("%s\n",buf);
		printf("-------------------SERVER BUSY---------------\n");
		sleep(6);
	}
	else if(option==2)
	{
		printf("Enter the username :");
		scanf("%s",name);
		write(sock,&name,sizeof(name));
		printf("Enter the joint username\n");
		scanf("%s",name_2);
		strcpy(pass_word_2,getpass("Enter new password for jointuser :  "));	
		write(sock,&name_2,sizeof(name_2));
		write(sock,&pass_word_2,sizeof(pass_word_2));
		read(sock,&buf,sizeof(buf));
		printf("%s\n",buf);
		printf("-------------------SERVER BUSY---------------\n");
		sleep(6);
	}
	else break;
	}
	return 0;
}





















int main(int argc, char * argv[]){
	int cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buf[80];
	if(cli_fd == -1){
		printf("socket creation failed\n");
		exit(0);
	}
	struct sockaddr_in ca;
	ca.sin_family=AF_INET;
	ca.sin_port= htons(PORT);
	ca.sin_addr.s_addr = INADDR_ANY;
	if(connect(cli_fd, (struct sockaddr *)&ca, sizeof(ca))==-1){
		printf("connect failed\n");
		exit(0);
	}
	printf("connection established\n");
	
	while(entry_point(cli_fd)!=3);
	close(cli_fd);
	return 0;
}