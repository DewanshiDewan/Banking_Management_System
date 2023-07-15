/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#ifndef DBOPERATIONS_H
#define DBOPERATIONS_H
#include "datastruc.h"
int authorize_Login(Credentials *loginattm);
int authorize_Admn_Login(AdminCredentials *loginattm);
int db_Dep(int deposit, int user_ID);
int dbWithdraw(int withdraw, int user_ID);
int dbBalance(int user_ID);
int db_Pwd_change(char *newpass, int user_ID);
int *db_Acc_det(int user_ID);
int db_Del_Acc(int validate);
int db_Mod_Acc_type(int acc_no, int acc_type);
int db_Mod_Admn_pwd(char *newpass, char *u_name);
int db_Add_Acc(int acc_no);
#endif /* !DBOPERATIONS_H */

