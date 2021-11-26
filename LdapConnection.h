#ifndef LdapConnection_H
#define LdapConnection_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ldap.h>
#include "string"
#include <iostream>
#include <termios.h>
#include <unistd.h>


using namespace std;

class LdapConnection{
    public:
        LdapConnection();
        void ldapBindUserInitialize(string uid);
        void ldapInitialize();
        void ldapSetOption();
        void ldapStartTls();
        void ldapBindCredentials(string password);
        void ldapSearch();
        void ldapSerachResultEntry();
        void ldapClose();
        
    private:
        const char* ldapUri;
        const char* ldapSearchBaseDomainComponent;
        const char* rawLdapUserEnv;
        int ldapVersion;
        ber_int_t ldapSearchScope;
        const char * ldapSearchResultAttributes[3]{"uid", "cn", NULL};
        string ldapBindUser;
        string ldapBindPassword;
        string rawLdapUser;
        string ldapSearchFilter;
        LDAP *ldapHandle;
        LDAPMessage *searchResult; 
    
};

#endif // LdapConnection