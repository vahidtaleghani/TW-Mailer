#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include<iostream>
#include "LdapConnection.h"
#include "UserInput.h"

using namespace std;

int main(int argc, char *argv[])
{
    
    if (argc ==2  && strcmp(argv[1], "start") == 0)
   { 
    UserInput *ui = new UserInput();
    LdapConnection *lc = new LdapConnection();
    
    //connect to LDAP
    lc->ldapInitialize();
    lc->ldapSetOption();
    lc->ldapStartTls();
    //take uid and password 
    string uid ;  
    while(true){

        uid.clear();;
        cout << "please Enter your username"<< endl;
        getline(cin, uid);

        if(ui->checkLenght(uid) == false || ui->checkCharacter(uid) == false){
            continue;
        }
        break;
    }
    lc->ldapBindUserInitialize(uid);
    string password=getpass("Please Enter your password"); 
    //check uid and password in ldap 
    lc->ldapBindCredentials(password);
    //search for find information of uid and show them 
    lc->ldapSearch();
    lc->ldapSerachResultEntry();
    // close ldap
    lc->ldapClose();
   }

    return 0;
}
