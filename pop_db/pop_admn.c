/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include "datastruc.h"
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
    int f_des = open("../data/Admins.dat", O_CREAT | O_RDWR, 0777);
    if (f_des == -1){
        printf("Can't create and open file.\n");
        exit(1);
    }
    lseek(f_des, 0, SEEK_SET);
    AdminCredentials User1 = {
        .u_name = "Admn1",
        .pass_word = "bestadmn",
    };
    write(f_des, &User1, sizeof(User1));

    AdminCredentials User2 = {
        .u_name = "Admn2",
        .pass_word = "helloworld",
    };
    write(f_des, &User2, sizeof(User2));
    close(f_des);
    return 0;
}



