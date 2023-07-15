/* Student Name: Dewanshi Dewan
   Roll Number: IMT2020549
   Date: 08/05/2022 */

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

enum LoginRequest{
    NormalLoginRequest,
    AdminLoginRequest,
};

enum AdminRequestType{
    AdminDummyRequest,
    Add_Acc_rq,
    Del_Acc_rq,
    Mod_AccType_rq,
    Mod_pwd_rq,
    Acc_det_rq,
    Admn_exit_rq,
};
enum RequestType{
    DummyRequest,
    Deposit_rq,
    Withdrawal_rq,    
    Balance_rq,
    Pwd_change_rq,
    Det_View_rq,
    Exit_rq,
};

enum AccountType{
    singleAccount,
    jointAccount
};

enum TransactionType{
    Deposit,
    Withdrawal
};

typedef struct Credentials{
    int userID;
    char pass_word[50];
}Credentials;

typedef struct AdminCredentials{
    char u_name[50];
    char pass_word[50];
}AdminCredentials;

typedef struct Account{
    int userID;
    int balance;
}Account;

typedef struct User{
    int userID;
    char pass_word[50];
    int account_id;
    enum AccountType acc_type;
}User;

typedef struct Transaction{
    int userID;
    int acc_id;
    int user_id;
    int opening_balance;
    int closing_balance;
    enum TransactionType transaction_type;
}Transaction;

#endif /* !DATASTRUCTURES_H */
