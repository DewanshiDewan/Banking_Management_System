/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#include <stdio.h>
#include <unistd.h>
#include<errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "db_oper.h"
extern int errno;

User get_User(int user_ID){
    User authenticate; 
    int sa = user_ID -1;
    int f_des = open("./data/Users.dat", O_RDONLY,0744);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc;
    lc.l_type = F_RDLCK; lc.l_start = sa*sizeof(User);
    lc.l_whence = SEEK_SET; lc.l_len = sizeof(User);
    lc.l_pid = getpid();
    int temp = fcntl(f_des, F_SETLK, &lc);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Write lock can't be set. %s\n",strerror(e));
        return authenticate;
    }
    lseek(f_des,sa*sizeof(authenticate),SEEK_SET); read(f_des, &authenticate, sizeof(authenticate));
    lc.l_type = F_UNLCK;
    fcntl(f_des, F_SETLK, &lc); close(f_des);
    return authenticate;
}

int get_Acc_ID(int user_ID){
    User user = get_User(user_ID);
    return user.account_id;
}

int db_Dep(int deposit, int user_ID){
    int account_id = get_Acc_ID(user_ID);
    Account update;
    int sa = (account_id-1); 
    int f_des = open("./data/Accounts.dat", O_RDWR);
    if(f_des == -1) 
    {
      printf("Can't open the file.\n");
      return (-1);
    }
    static struct flock lc;
    lc.l_type = F_WRLCK; lc.l_start = sa*sizeof(update);
    lc.l_whence = SEEK_SET; lc.l_len = sizeof(update);
    lc.l_pid = getpid();

    int temp = fcntl(f_des, F_SETLKW, &lc);
    if (temp == -1) {
      printf("Write lock can't be set.\n");
      return -1;
    }
    
    lseek(f_des,sa*sizeof(update),SEEK_SET);
    read(f_des, &update, sizeof(update));
    printf("The initial balance is: %d\n", update.balance);

    update.balance += deposit;
    lseek(f_des, sa*(sizeof(update)), SEEK_SET); write(f_des, &update, sizeof(update));
    lseek(f_des, sa*(sizeof(update)), SEEK_SET); read(f_des, &update, sizeof(update));
    printf("The final balance is: %d\n",update.balance);
    lc.l_type = F_UNLCK;
    fcntl(f_des,F_SETLKW, &lc); close(f_des);
    return (1);
}

int dbWithdraw(int withdraw, int user_ID){
    int account_id = get_Acc_ID(user_ID), sa;
    Account update;
    sa = account_id -1;  int f_des = open("./data/Accounts.dat", O_RDWR);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc;
    lc.l_type = F_WRLCK; lc.l_start = sa*sizeof(update);
    lc.l_whence = SEEK_SET; lc.l_len = sizeof(update);
    lc.l_pid = getpid();

    int temp = fcntl(f_des, F_SETLKW, &lc);
    if (temp == -1) {
      printf("Write lock can't be set.\n");
      return -1;
    }
    
    lseek(f_des,sa*sizeof(update),SEEK_SET); read(f_des, &update, sizeof(update));
    printf("The initial balance is: %d\n", update.balance);
    printf("Withdraw the following amount: %d\n", withdraw);
    if (update.balance < withdraw){
        printf("Can't withdraw-> Funds insufficient.\n");
        return -1;
    }
    update.balance -= withdraw;
    lseek(f_des, sa*(sizeof(update)), SEEK_SET); write(f_des, &update, sizeof(update));
    lseek(f_des, sa*(sizeof(update)), SEEK_SET); read(f_des, &update, sizeof(update));
    printf("The final balance is : %d\n",update.balance);
    lc.l_type = F_UNLCK; fcntl(f_des,F_SETLKW, &lc); close(f_des);
    return 1;
}
int *db_Acc_det(int user_ID){
    static int Acc_Det[4] = {0,0,0,0}; 
    User user = get_User(user_ID);
    int Acc_Bal = dbBalance(user_ID);
    printf("User ID: %d\n", user_ID);
    printf("Account Number: %d\n", user.account_id);
    printf("Account Type: %d\n", user.acc_type);
    printf("Account Balance: %d\n", Acc_Bal);
    Acc_Det[0] = user_ID; Acc_Det[1] = user.account_id;
    Acc_Det[2] = user.acc_type; Acc_Det[3] = Acc_Bal;
    return Acc_Det;
}
int db_Pwd_change(char *newpass, int user_ID){
    User authenticate; 
    int sa = user_ID -1; 
    int f_des = open("./data/Users.dat", O_RDWR);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      return -1;
    }
    static struct flock lc;
    lc.l_type = F_WRLCK; lc.l_start = sa*sizeof(authenticate);
    lc.l_whence = SEEK_SET; lc.l_len = sizeof(authenticate);
    lc.l_pid = getpid();

    int temp = fcntl(f_des, F_SETLKW, &lc);
    if (temp == -1) {
        printf("Write lock can't be set.\n");
        return -1;
    }
    lseek(f_des,sa*sizeof(authenticate),SEEK_SET);
    read(f_des, &authenticate, sizeof(authenticate));
    printf("The old pwd is: %s\n", authenticate.pass_word);
    
    strcpy(authenticate.pass_word, newpass);
    lseek(f_des,sa*sizeof(authenticate),SEEK_SET);
    write(f_des, &authenticate, sizeof(authenticate));
    
    lseek(f_des,sa*sizeof(authenticate),SEEK_SET);
    read(f_des, &authenticate, sizeof(authenticate));
    printf("The new pwd is: %s\n", authenticate.pass_word);
    lc.l_type = F_UNLCK;
    fcntl(f_des, F_SETLKW, &lc); close(f_des);
    return 1;
}

int dbBalance(int user_ID){
    int account_id = get_Acc_ID(user_ID), sa, f_des;
    Account update;
    sa = account_id -1; 
    f_des = open("./data/Accounts.dat", O_RDWR);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      return -1;
    }
    static struct flock lc;

    lc.l_type = F_RDLCK;
    lc.l_start = sa*sizeof(update);
    lc.l_whence = SEEK_SET;
    lc.l_len = sizeof(update);
    lc.l_pid = getpid();

    int temp = fcntl(f_des, F_SETLKW, &lc);
    if (temp == -1) {
      printf("Write lock can't be set.\n");
      return -1;
    }
    
    lseek(f_des,sa*sizeof(update),SEEK_SET);
    read(f_des, &update, sizeof(update));
    printf("The balance is: %d\n", update.balance);
    
    lc.l_type = F_UNLCK; fcntl(f_des,F_SETLKW, &lc);
    close(f_des); return update.balance;
}
int authorize_Login(Credentials *loginatt) {
    int user_ID = loginatt->userID; 
    User authenticate = get_User(user_ID); 
    int final;
    if (authenticate.userID == loginatt->userID)
    {
        if (strcmp(authenticate.pass_word, loginatt->pass_word) == 0)
        {
            final = 1;
        }
        else
        {
            final = -1;
        }
    }
    else
    {
        final = -1;
    }
    return final;
} 


int authorize_Admn_Login(AdminCredentials *loginatt) {
    printf("Now inside the authorize Admn login.\n"); 
    AdminCredentials authenticate; 
    int sa = 0, f_des; 
    f_des = open("./data/Admins.dat", O_RDONLY,0744);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc;
    
    lc.l_type = F_RDLCK; lc.l_start =0; 
    lc.l_whence = SEEK_SET; lc.l_len =0; 
    lc.l_pid = getpid();
    
    int temp = fcntl(f_des, F_SETLK, &lc);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Write lock can't be set. %s\n",strerror(e));
        return -1;
    }
    int final = -1;
    lseek(f_des,0,SEEK_SET); 
    printf("The entered username is: %s\n", loginatt->u_name);
    printf("The entered pwd is: %s\n", loginatt->pass_word);
    while(read(f_des, &authenticate, sizeof(authenticate))){
        if (strcmp(authenticate.u_name, loginatt->u_name) == 0){
            if (strcmp(authenticate.pass_word, loginatt->pass_word) == 0){
                return 1;
            }}}
    lc.l_type = F_UNLCK;
    fcntl(f_des, F_SETLK, &lc); close(f_des);
    return final;
} 

int db_Del_Acc(int acc_no){
    Account account;
    Account delete = {
        .userID = -1,
        .balance = -1
    };
    int f_des = open("./data/Accounts.dat", O_RDWR,0744);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc;
    lc.l_type = F_WRLCK; lc.l_start =0; 
    lc.l_whence = SEEK_SET; lc.l_len =0; 
    lc.l_pid = getpid();
    
    int temp = fcntl(f_des, F_SETLK, &lc);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Write lock can't be set. %s\n",strerror(e));
        return -1;
    }
    int final = -1;
    lseek(f_des,0,SEEK_SET);
    while(read(f_des, &account, sizeof(account))){
        if (account.userID == acc_no){
            lseek(f_des,-sizeof(account), SEEK_CUR);
            write(f_des, &delete, sizeof(delete));
            final = 0; 
        }
    }
    lc.l_type = F_UNLCK;
    fcntl(f_des, F_SETLK, &lc);
    close(f_des);
    
    User user;
    f_des = open("./data/Users.dat", O_RDWR,0744);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc1;
    
    lc1.l_type = F_WRLCK; lc1.l_start =0; 
    lc1.l_whence = SEEK_SET; lc1.l_len =0; 
    lc1.l_pid = getpid();
    
    temp = fcntl(f_des, F_SETLK, &lc1);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Write lock can't be set. %s\n",strerror(e));
        return -1;
    }
    
    lseek(f_des,0,SEEK_SET);
    while(read(f_des, &user, sizeof(user))){
        if (user.account_id == acc_no){
            user.account_id = -1;
            lseek(f_des,-sizeof(user), SEEK_CUR);
            write(f_des, &user, sizeof(user));
            final = 1; 
        }
    }
    lc1.l_type = F_UNLCK; fcntl(f_des, F_SETLK, &lc1);
    close(f_des); return final;
}

int db_Mod_Acc_type(int acc_no, int acc_type){
    User user;
    acc_type = acc_type -1;
    printf("in the db_Mod_Acc_type function\n");
    int f_des = open("./data/Users.dat", O_RDWR,0744);
    int final;
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc1;
    lc1.l_type = F_WRLCK;
    lc1.l_start =0; 
    lc1.l_whence = SEEK_SET;
    lc1.l_len =0; 
    lc1.l_pid = getpid();
    

    int temp = fcntl(f_des, F_SETLK, &lc1);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Write lock can't be set. %s\n",strerror(e));
        return -1;
    }
    
    lseek(f_des,0,SEEK_SET);
    while(read(f_des, &user, sizeof(user))){
        if (user.account_id == acc_no){
            user.acc_type = acc_type;
            lseek(f_des,-sizeof(user), SEEK_CUR); write(f_des, &user, sizeof(user));
            final = 1; 
            break;
        }
    }
    if (final != 1)
    {
        final = -1;
    }
    lc1.l_type = F_UNLCK;
    fcntl(f_des, F_SETLK, &lc1); close(f_des);
    return final;
}

int db_Mod_Admn_pwd(char *newpass, char *u_name){
    AdminCredentials authenticate; 
    printf("Npw inside the db_Mod_Admn_pwd function.\n");
    int f_des = open("./data/Admins.dat", O_RDWR,0744);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      exit(1);
    }
    static struct flock lc;
    lc.l_type = F_WRLCK; lc.l_start =0; 
    lc.l_whence = SEEK_SET; lc.l_len =0; 
    lc.l_pid = getpid();
    int temp = fcntl(f_des, F_SETLK, &lc);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Write lock can't be set. %s\n",strerror(e));
        return -1;
    }
    lseek(f_des,0,SEEK_SET); 
    while(read(f_des, &authenticate, sizeof(authenticate))){
        if (strcmp(authenticate.u_name, u_name) == 0){
            strcpy(authenticate.pass_word, newpass);
            lseek(f_des,-sizeof(authenticate), SEEK_CUR);
            write(f_des, &authenticate, sizeof(authenticate));
            lc.l_type = F_UNLCK;
            fcntl(f_des, F_SETLK, &lc); close(f_des);
            return 1;
        }
    }
    lc.l_type = F_UNLCK;
    fcntl(f_des, F_SETLK, &lc);
    close(f_des);
    return -1;
}
int db_Add_Acc(int acc_no){
    Account update = {
        .userID = acc_no,
        .balance = 0,
    };
    int f_des = open("./data/Accounts.dat", O_RDWR | O_APPEND);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      return -1; 
    }
    static struct flock lc;
    lc.l_type = F_WRLCK; lc.l_start = 0;
    lc.l_whence = SEEK_SET; lc.l_len = 0;
    lc.l_pid = getpid();

    int temp = fcntl(f_des, F_SETLKW, &lc);
    if (temp == -1) {
      printf("Write lock can't be set.\n");
      return -1;
    }
    
    write(f_des, &update, sizeof(update));
    lc.l_type = F_UNLCK;
    fcntl(f_des,F_SETLKW, &lc);
    close(f_des);
    printf("account created\n"); 
    User new_User;
    f_des = open("./data/Users.dat", O_RDWR,0744);
    if(f_des == -1) {
      printf("Can't open the file.\n");
      return -1;
    }
    static struct flock lc1;

    lc1.l_type = F_WRLCK;
    lc1.l_start = 0;
    lc1.l_whence = SEEK_SET;
    lc1.l_len =0; 
    lc1.l_pid = getpid();
    

    temp = fcntl(f_des, F_SETLK, &lc1);
    if (temp == -1) {
        int e = errno;
        fprintf(stderr, "Attempt to set write lc1 failed %s\n",strerror(e));
        return -1;
    }
    
    int max_user_ID;
    while(read(f_des, &new_User, sizeof(new_User))){
        if (new_User.userID > max_user_ID)
            max_user_ID = new_User.userID;
    }
    
    printf("The max_user_ID is: %d\n", max_user_ID); 
    new_User.userID = max_user_ID+1; 
    new_User.account_id =acc_no; 
    new_User.acc_type = singleAccount;
    char pass[50] = "dummy_pwd012";
    strcpy(new_User.pass_word, pass);
    
    temp = write(f_des, &new_User, sizeof(new_User)); 
    if (temp < 0){
        printf("New user can't be written.\n");
        return -1;
    }
    printf("Have written the user though still in the critical section.\n"); 
    lc1.l_type = F_UNLCK;
    fcntl(f_des, F_SETLK, &lc1);
    close(f_des);
    printf("New user and designated account created successfully.\n");
    return 1;
}
