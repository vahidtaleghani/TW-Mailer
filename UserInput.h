#ifndef UserInput_H
#define UserInput_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ldap.h>
#include "string"
#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

class UserInput{
    public:
        bool checkLenght(string uid);
        bool checkCharacter(string uid);
 
    private:
  
};

#endif 