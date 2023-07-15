/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include "client.h"
#include<arpa/inet.h>

#define PORT 4444
extern int errno;

void Exit_Manager(long ck, int sock_d){
    int opt = 6;
    write(sock_d, &ck, sizeof(ck));
    write(sock_d, &opt, sizeof(opt));
    close(sock_d);
    printf("It is disconnected from the server.\n");
    exit(0);
}

void Acc_Manager(long ck, int sock_d){
    int opt = 5;
    int acc_details[4] = {0,0,0,0}; 
    write(sock_d, &ck, sizeof(ck));
    write(sock_d, &opt, sizeof(opt)); 
    read(sock_d, acc_details, sizeof(acc_details)); 
    printf("User ID: %d\n", acc_details[0]);
    printf("Account Number: %d\n", acc_details[1]);
    printf("Account Type: %d\n", acc_details[2]);
    printf("Account Balance: %d\n", acc_details[3]);
}

void Pwd_Manager(long ck, int sock_d){
    int opt = 4; char new_pwd[50];
    int serv_R;
    printf("Please input your new pwd: ");
    scanf("%s", new_pwd);
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt));
    write(sock_d, new_pwd, sizeof(new_pwd));

    read(sock_d, &serv_R, sizeof(serv_R));
    if (serv_R > 0){
        printf("The pwd is changed successfully.\n");
    }
    else{
        printf("Pwd change unsuccessfull.\n");
    }
}

void Bal_Manager(long ck, int sock_d){
    int serv_R, opt = 3;
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt));

    read(sock_d, &serv_R, sizeof(serv_R));
    if (serv_R > 0){
        printf("The current balance is: %d\n", serv_R);
    }
    else{
        printf("Balance can't be displayed.\n");
    }
}
void Dep_Manager(long ck, int sock_d){
    int dep, serv_R, opt = 1;
    printf("Please input the amount to be deposited: ");
    scanf("%d", &dep);
    
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt)); 
    write(sock_d, &dep, sizeof(dep));

    read(sock_d, &serv_R, sizeof(serv_R));
    if (serv_R!=1) {
        printf("Money couldn't be deposited.\n");
    }
    else {
        printf("Money was deposited successfully.\n");
    }
}

void Withd_Manager(long ck, int sock_d){
    int withdraw, serv_R, opt = 2;
    printf("PLease input the amount to be withdrawn: ");
    scanf("%d", &withdraw);
    
    write(sock_d, &ck, sizeof(ck));write(sock_d, &opt, sizeof(opt));
    write(sock_d, &withdraw, sizeof(withdraw));

    read(sock_d, &serv_R, sizeof(serv_R));
    if (serv_R == 1){
        printf("Money withdrawn successfully.\n");
    }
    else{
        printf("Money withdraw unsuccessfull.\n");
    }
}

int opt_Menu(long ck, int sock_d){
    write(1, "1. Deposit\n", sizeof("1. Deposit\n")); 
    write(1, "2. Withdraw\n", sizeof("2. Withdraw\n")); 
    write(1, "3. Balance Enquiry\n", sizeof("3. Balance Enquiry\n")); 
    write(1, "4. Password Change\n", sizeof("4. Password Change\n")); 
    write(1, "5. Account Details\n", sizeof("5. Account Details\n")); 
    write(1, "6. Exit\n", sizeof("6. Exit\n")); 
    int resp;
    scanf("%d", &resp);
    switch(resp){
        case 1:
            Dep_Manager(ck, sock_d);break;
        case 2:
            Withd_Manager(ck, sock_d);break;
        case 3:
            Bal_Manager(ck, sock_d);break;
        case 4:
            Pwd_Manager(ck, sock_d);break;
        case 5:
            Acc_Manager(ck, sock_d);break;
        case 6:
            Exit_Manager(ck, sock_d);break;
    }
    opt_Menu(ck, sock_d);
}

int adminopt_Menu(long ck, int sock_d, char *u_name){
    write(1, "1. Add Account\n", sizeof("1. Add Account\n")); 
    write(1, "2. Delete Account\n", sizeof("1. Delete Account\n")); 
    write(1, "3. Modify Account Type\n", sizeof("3. Modify Account Type\n")); 
    write(1, "5. Account Details\n", sizeof("5. Account Details\n")); 
    write(1, "4. Password Change\n", sizeof("4. Password Change\n")); 
    write(1, "6. Exit\n", sizeof("6. Exit\n")); 
    int resp;
    scanf("%d", &resp);
    printf("The recorded response: %d\n", resp);
    switch(resp){
        case 1:
            AddAcc_Manager(ck, sock_d);break;
        case 2:
            DeleteAcc_Manager(ck, sock_d);break;
        case 3:
            Mod_AccType_Manager(ck, sock_d);break;
        case 4:
            Mod_AdmnPwd_Manager(ck, sock_d, u_name);break;
        case 5:
            view_AccDet_Manager(ck, sock_d);break;
        case 6:
            Exit_Manager(ck, sock_d);break;
    }
    adminopt_Menu(ck, sock_d, u_name);
}

void view_AccDet_Manager(long ck, int sock_d){
    int user_ID, opt = 5;
    printf("To check, input the user ID: ");
    scanf("%d", &user_ID);
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt));
    write(sock_d, &user_ID, sizeof(user_ID));
    printf("Everything written\n"); 
    int acc_details[4] = {0,0,0,0}; 
    
    read(sock_d, acc_details, sizeof(acc_details)); 
    printf("User ID: %d\n", acc_details[0]);
    printf("Account number: %d\n", acc_details[1]);
    printf("Account Type: %d\n", acc_details[2]);
    printf("Account Balance: %d\n", acc_details[3]);

}

void Mod_AdmnPwd_Manager(long ck, int sock_d, char *u_name){
    char newpass[50];
    int opt = 4, temp;
    printf("Please input your new admn acc. pwd: ");
    scanf("%s", newpass);
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt));
    write(sock_d, newpass, sizeof(newpass)); write(sock_d, u_name, sizeof(u_name));
    read(sock_d, &temp, sizeof(temp));
    if (temp == 1){
        printf("The admn pwd has changed successfully.\n");
    }
    else{
        printf("The pwd change failed.\n");
    }
}
void Mod_AccType_Manager(long ck, int sock_d){
    int acc_num, acc_type, opt = 3, temp;
    printf("Please input the acc_num to be modified: ");
    scanf("%d", &acc_num);
    printf("The New Account type:\n1. Single Account\n2. Joint Account\n");
    scanf("%d", &acc_type);
    write(sock_d, &ck, sizeof(ck));
    write(sock_d, &opt, sizeof(opt));
    write(sock_d, &acc_num, sizeof(acc_num));
    write(sock_d, &acc_type, sizeof(acc_type));
    read(sock_d, &temp, sizeof(temp));
    if (temp == 1){
        printf("The account no:%d's type is successfully set.\n", acc_num);
    }
    else{
        printf("Account type change unsuccessfull.\n");
    }
}
void DeleteAcc_Manager(long ck, int sock_d){
    int acc_num, opt = 2, temp;
    printf("Please input the account number to be deleted: ");
    scanf("%d", &acc_num);
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt));
    write(sock_d, &acc_num, sizeof(acc_num));

    read(sock_d, &temp, sizeof(temp));
    if (temp == 1){
        printf("The account no: %d is delelted successfully.\n", acc_num);
    }
    else{
        printf("Failed to delete account.\n");
    }
}

void AddAcc_Manager(long ck, int sock_d){
    int acc_num, opt = 1, temp;
    printf("Input acc_num to be added: ");
    scanf("%d", &acc_num);
    
    write(sock_d, &ck, sizeof(ck)); write(sock_d, &opt, sizeof(opt));
    write(sock_d, &acc_num, sizeof(acc_num));

    read(sock_d, &temp, sizeof(temp));
    if (temp == 1){
        printf("The account no: %d is added successfully.\n", acc_num);
    }
    else{
        printf("Adding the account failed.\n");
    }
}

int login_tog(int sock_d){
    write(1, "0. RegularLogin\n", sizeof("1. RegularLogin\n")); 
    write(1, "1. AdminLogin\n", sizeof("1. AdminLogin\n")); 
    int resp;
    scanf("%d", &resp);
    switch(resp){
        case 0:
            normal_user_login(sock_d);break;
        case 1:
            admn_login(sock_d);break;
    }

}
int normal_user_login(int sock_d){
    enum LoginRequest type = NormalLoginRequest;
    int temp = write(sock_d, &type, sizeof(type));
    
    Credentials *request = malloc(sizeof(Credentials));
    int userID;
    char pass_word[50];
    printf("/->Login<-/\n");
    printf("userID: ");
    scanf("%d", &request->userID);
    printf("pass_word: ");
    scanf("%s", pass_word);
    strcpy(request->pass_word, pass_word);
    temp = write(sock_d, request, sizeof(Credentials));
    if (temp < 0){
        int err = errno;	
        fprintf(stderr, "Can't write to server: %s\n",strerror(err));
        return -1;
    }
    long ck;
    read(sock_d, &ck, sizeof(ck));
    if (ck == -1)
        login_tog(sock_d);
    else
        opt_Menu(ck, sock_d); 
        return 1;
}

int admn_login(int sock_d){
    enum LoginRequest type = AdminLoginRequest;
    int temp = write(sock_d, &type, sizeof(type));
    AdminCredentials *request = malloc(sizeof(AdminCredentials));
    char u_name[50];
    char pass_word[50];
    printf("/->Admn Login<-/\n");
    printf("u_name: ");
    scanf("%s", u_name);
    printf("pass_word: ");
    scanf("%s", pass_word);
    strcpy(request->pass_word, pass_word);
    strcpy(request->u_name, u_name);
    temp = write(sock_d, request, sizeof(AdminCredentials));
    if (temp < 0){
        int e = errno;	
        fprintf(stderr, "Can't write to the server: %s\n",strerror(e));
        return -1;
    }
    long ck;
    read(sock_d, &ck, sizeof(ck));
    if (ck == -1)
        login_tog(sock_d);
    else
        adminopt_Menu(ck, sock_d, u_name); 
        return 1;
}

int main(){
    struct sockaddr_in server;
    int sock_d, connection;
    char b[5];
    sock_d = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    connection = connect(sock_d, (struct sockaddr *)&server, sizeof(server));
    if (connection< 0){
        int e = errno;	
        fprintf(stderr, "Can't connect to the server: %s\n",strerror(e));
        return -1;
    }
    long ck = login_tog(sock_d); 
    printf("Have returned to Main.\n");
    return 0;
}
