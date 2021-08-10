#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include<stdlib.h>
#include <string.h>
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
void main() {
struct customer db;
int fd=open("customer",O_RDWR|O_CREAT,0644);
//strcpy(db.username,"Neeraj");
//strcpy(db.password,"2797");
//strcpy(db.username_2,"NULL");
//strcpy(db.password_2,"NULL");
//strcpy(db.account_number,"MT2020079");
//db.balance=599.65;
//db.type=false;
//write(fd,&db,sizeof(db));
strcpy(db.username,"Shubham");
strcpy(db.password,"ABC");
strcpy(db.account_number,"MT2020080");
strcpy(db.username_2,"NULL");
strcpy(db.password_2,"NULL");
db.balance=729.65;
db.type=true;
write(fd,&db,sizeof(db));
//strcpy(db.username,"Mayank");
//strcpy(db.password,"143");
//strcpy(db.username_2,"Aman");
//strcpy(db.password_2,"Malviya");
//strcpy(db.account_number,"MT2020081");
//db.balance=4299.65;
//db.type=false;
//write(fd,&db,sizeof(db));
close(fd);
}
//