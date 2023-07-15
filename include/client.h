/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#ifndef CLIENT_H
#define CLIENT_H
#include "datastruc.h"

int normal_user_login(int client);
int admn_login(int client); 
void DeleteAcc_Manager(long ck, int sock_d);
void AddAcc_Manager(long ck, int sock_d);
void Mod_AdmnPwd_Manager(long ck, int sock_d, char *username);
void Mod_AccType_Manager(long ck, int sock_d);
void view_AccDet_Manager(long ck, int sock_d);
#endif /* !CLIENT_H */
