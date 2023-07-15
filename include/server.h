/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#ifndef SERVER_H
#define SERVER_H

int balance(int client, int user_ID);
int deposit(int client, int user_ID);
int withdraw(int client, int user_ID);
int pwd_change(int client, int user_ID);
int view_det(int client, int user_ID);
int client_exit(int client, int user_ID);
int normal_user_login(int client);
int admn_login(int client);
int Login_handler_sys(int client);
int master_cont(int client, int user_ID, long cookie);
int mod_acc_type(int client);
int mod_admn_pwd(int client);
int add_acc(int client);
int view_acc_det(int client);
int del_acc(int client);
#endif /* !SERVER_H */
