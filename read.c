#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
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
int main()
{
    struct customer db;
    int fd=open("customer",O_RDONLY,0644);
    lseek(fd,0,SEEK_SET);
    printf("Username\tPassword\tjointuser\tjointUserPass\tAccount_No.\t\tBalance\t\t\tAdmin\n");
    while(read(fd,&db,sizeof(db)))
    {printf("%s\t\t",db.username);
    printf("%s\t\t",db.password);
    printf("%s\t\t",db.username_2);
    printf("%s\t\t",db.password_2);
    printf("%s\t\t",db.account_number);
    printf("%f\t\t",db.balance);
    printf("%d\n",db.type);}
    close (fd);
    return 0;

}