#include "UserInput.h"
// Check the input character entered by the user (Only (a-z, 0-9) are allowed!)
bool UserInput::checkCharacter(string uid){
    for (long unsigned int i =0 ; i< uid.length(); i++){
        if(!((uid[i] >=  48 && uid[i] <= 57) || (uid[i] >= 97 && uid[i] <= 122))){
            cout<<"Only (a-z, 0-9) are allowed!"<<endl;
            return false;
        }
    }
    return true;
}
// Check the input length entered by the user (Less than 8 characters)
bool UserInput::checkLenght(string uid){
    if(uid.length() == 0){
            cout<<"Noting to read!" << endl;
            return false;
        }
    if(uid.length()>8){
            cout << "Input is too long!" << endl;
            return false;
    }
    return true;
}


