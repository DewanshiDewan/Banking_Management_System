/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include "datastruc.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(){
    int f_des = open("../data/Accounts.dat", O_CREAT | O_RDWR, 0777);
    if (f_des == -1){
        printf("Can't create and open file.\n");
        exit(1);
    }
    lseek(f_des, 0, SEEK_SET);

    Account acc1 = {
        .userID = 1,
        .balance = 62500,//balance
    };

    write(f_des, &acc1, sizeof(acc1));

    Account acc2 = {
        .userID = 2,
        .balance = 40500,
    };

    write(f_des, &acc2, sizeof(acc2));
    
    Account acc3 = {
        .userID = 3,
        .balance = 80001,
    };

    write(f_des, &acc3, sizeof(acc3));
    
    Account acc4 = {
        .userID = 4,
        .balance = 87961,
    };

    write(f_des, &acc4, sizeof(acc4));
    close(f_des);
}



