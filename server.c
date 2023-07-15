/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include "server.h"
#include "datastruc.h"
#include "db_oper.h"

#define PORT 4444
extern int errno;

int master_cont(int client,int user_ID, long ck){
    printf("Entered the master controller.\n");
    enum RequestType type;
    long ck_rcv;
    
    read(client, &ck_rcv, sizeof(ck_rcv));
    read(client, &type, sizeof(type));

    if (ck_rcv == ck){ 
        printf("The cookie has matched.\n");
        printf("Type we've recieved: %d\n", type);
        switch(type){
            case Deposit_rq:
                deposit(client, user_ID);break;
            case Balance_rq:
                balance(client, user_ID);break;
            case Pwd_change_rq:
                pwd_change(client, user_ID);break;
            case Withdrawal_rq:
                withdraw(client, user_ID);break;
            case Exit_rq:
                client_exit(client, user_ID);break;
            case Det_View_rq:
                view_det(client, user_ID);break;
        } 
    }
    master_cont(client, user_ID, ck);

}

int admn_cont(int client, long ck){
    printf("Entered the admn controller.\n");
    enum AdminRequestType type;
    long ck_rcv;
    
    read(client, &ck_rcv, sizeof(ck_rcv));
    read(client, &type, sizeof(type));

    if (ck_rcv == ck){ 
        printf("The cookie has matched.\n");
        printf("Type we've recieved: %d\n", type);
        switch(type){
            case Add_Acc_rq:
                add_acc(client);
                break;
            case Del_Acc_rq:
                del_acc(client);
                break;
            case Acc_det_rq:
                view_acc_det(client);
                break;
            case Admn_exit_rq:
                client_exit(client, -1);
                break;
            case Mod_AccType_rq:
                mod_acc_type(client);
                break;
            case Mod_pwd_rq:
                mod_admn_pwd(client);
                break;
            default:
                printf("Didn't catch anything.\n");
                break;
        } 
    }
    admn_cont(client, ck);
}

int view_acc_det(int client){
    int user_ID;
    read(client, &user_ID, sizeof(user_ID));
    view_det(client, user_ID);
    return 1;

}
int mod_admn_pwd(int client){
    char newpass[50], u_name[50];
    printf("Now inside the modify admn pwd.\n");
    read(client, newpass, sizeof(newpass));
    read(client, u_name, sizeof(u_name));
    printf("The newpass recieved is: %s\n", newpass);
    printf("The username recieved is: %s\n", u_name);
    int temp = db_Mod_Admn_pwd(newpass, u_name);//ret
    write(client, &temp, sizeof(temp)); 
    if (temp == 1){
        printf("Password changed successfully.\n");
    }
    else{
        printf("Password change unsuccessfull.\n");
    }
    return temp;
}

int mod_acc_type(int client){
    int acc_no;
    int acc_type; 
    printf("Now inside the function for modifying account type.\n");
    read(client, &acc_no, sizeof(acc_no));
    read(client, &acc_type, sizeof(acc_type)) ;
    printf("The account number which is read: %d\n", acc_no);
    printf("The account type which is read: %d\n", acc_type);
    int temp = db_Mod_Acc_type(acc_no, acc_type);
    write(client, &temp, sizeof(temp)); 
    if (temp == 1){
        printf("Password changed successfully.\n");
    }
    else
        printf("The account type couldn't be modified.\n");
    return temp;
}

int del_acc(int client){
    int acc_no;
    read(client, &acc_no, sizeof(acc_no));
    int temp = db_Del_Acc(acc_no);
    write(client, &temp, sizeof(temp));
    if (temp == 1){
        printf("Account deleted successfully.\n");
    }
    else{
        printf("Account delete unsuccessfull.\n");
    }
    return temp;
}

int add_acc(int client){
    printf("Now in the add_acc function\n");
    int acc_no;

   read(client, &acc_no, sizeof(acc_no));
   printf("The account number which is read:%d\n", acc_no);
   int temp = db_Add_Acc(acc_no);
   write(client, &temp, sizeof(temp));
   return temp;
}

int client_exit(int client,int user_ID){
    close(client);
    if (client == -1)
        printf("Connection with the admin ended.\n");
    else
        printf("Connection with the client%d closed\n", client);
    exit(0);
}

int pwd_change(int client, int user_ID){
    char newpass[50];
    int temp;
    read(client, newpass, sizeof(newpass));
    temp = db_Pwd_change(newpass, user_ID);
    write(client, &temp, sizeof(temp));
    return temp;
}

int view_det(int client, int user_ID){
    printf("in the view_det function\n");
    int *det = malloc(sizeof(int)*4);
    det = db_Acc_det(user_ID); 
    write(client, det, sizeof(int)*4);
    return 1;
} 

int balance(int client, int user_ID){
    int balance = dbBalance(user_ID);
    write(client, &balance, sizeof(balance));
    if (balance <= 0)
        return (-1);
    else
        return (1);
}


int deposit(int client, int user_ID){
    int deposit_t ;
    read(client, &deposit_t, sizeof(deposit));
    int temp = deposit(deposit_t, user_ID);
    write(client, &temp, sizeof(temp));
    return temp;
}

int withdraw(int client, int user_ID){
    int withdraw;
    printf("Now inside the withdraw func.\n");
    read(client, &withdraw, sizeof(withdraw));
    int temp = dbWithdraw(withdraw, user_ID);
    write(client, &temp, sizeof(temp));
    return temp;
}

long GenerateCookie(int val){
    if (val!=1)
    {
        return -1;
    }
    else 
    {
        return random();
    } 
}

int normal_user_login(int client){
    Credentials *loginatt = malloc(sizeof(Credentials));
    printf("Waiting for the login credentials.\n");
    int bytes = read(client, loginatt, sizeof(Credentials));
    int validate = authorize_Login(loginatt);
    long ck = GenerateCookie(validate);

    write(client, &ck, sizeof(ck));  
    if (validate!=1){
        Login_handler_sys(client);
    }
    else {
        int user_ID = loginatt->userID;
        master_cont(client, user_ID,  ck);
    }
    return 0;
} 

int admn_login(int client){
    AdminCredentials *loginatt = malloc(sizeof(AdminCredentials));
    printf("Waiting for the admin login credentials.\n");
    int bytes = read(client, loginatt, sizeof(AdminCredentials));
    printf("The bytes read are: %d\n", bytes);
    int validate = authorize_Admn_Login(loginatt);
    long ck = GenerateCookie(validate);

    write(client, &ck, sizeof(ck));  
    if (validate!=1){ 
        Login_handler_sys(client);
    }
    else {
        admn_cont(client, ck);
    } 
    return 0;
}

int Login_handler_sys(int client){
    enum LoginRequest type;
    printf("Now in the login handler system.\n");
    int temp; 
    temp = read(client, &type, sizeof(type));
    printf("The bytes read are: %d\n", temp);
    if (temp < 0){
        printf("Here first query isn't doing anything.\n");
        return -1;
    }
    switch(type){
        case AdminLoginRequest:
            admn_login(client);
            break;
        case NormalLoginRequest:
            normal_user_login(client);
            break;
    } 
    return 0;
}

int Connecting_sock(){
    int sock_d;
    struct sockaddr_in serveraddr, clientaddr;
    int opt = 1;

    sock_d = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_d == 0){
        int e = errno;	
        fprintf(stderr, "Creation of socket at server end unsuccessfull: %s\n", strerror(e));
        return -1;
    }
    setsockopt(sock_d, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    if(bind(sock_d, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
        int e = errno;	
        fprintf(stderr, "Binding at server end unsuccessfull: %s\n", strerror(e));
        return -1;
    }
    return sock_d;
}

int Connecting_client(int sock_d)  {
    // handling 10 users at once max
    struct sockaddr_in clientaddr;
	socklen_t addr_size = sizeof(clientaddr);
    listen(sock_d, 10);
    int child_pID;
    while(1){
        int client = accept(sock_d, (struct sockaddr *)&clientaddr,&addr_size);
        if (client < 0){
            int e = errno;	
            fprintf(stderr, "Connection unsuccessfull: %s\n", strerror(e));
            return -1;
        }
        printf("The connection is accepted from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        if ((child_pID=fork()) != 0){
            close(client);
        }
        else {
            close(sock_d);
            Login_handler_sys(client);    
            return 1;
        }
    }
return 0;
}

int main()
{
    srandom(42); 
    int sock_d = Connecting_sock();
    Connecting_client(sock_d);
    return 0;
}
