/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include "datastruc.h"

int main()
{
    int f_des = open("../data/Users.dat", O_CREAT | O_RDWR, 0777);
    if (f_des == -1){
        printf("Can't create and open file.\n");
        exit(1);
    }
    lseek(f_des, 0, SEEK_SET);
    User User1 = 
    {
        .userID = 1,
        .pass_word = "home012",
        .acc_type = singleAccount,
        .account_id = 1,
    };
    unsigned int s1=sizeof(User1);
    write(f_des, &User1, s1);

    User User2 = {
        .userID = 2,
        .pass_word = "Home013",
        .acc_type = singleAccount,
        .account_id = 2,
    };
    unsigned int s2=sizeof(User2);
    write(f_des, &User2, s2);
    
    User User3 = {
        .userID = 3,
        .pass_word = "Home014",
        .acc_type = singleAccount,
        .account_id = 3,
    };
    unsigned int s3=sizeof(User3);
    write(f_des, &User3, s3);
    
    User User4 = {
        .userID = 4,
        .pass_word = "Home015",
        .acc_type = singleAccount,
        .account_id = 4,
    };
    unsigned int s4=sizeof(User4);
    write(f_des, &User4, s4);
    close(f_des);
    return 0;
}



