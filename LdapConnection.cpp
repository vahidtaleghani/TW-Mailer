#include "LdapConnection.h"
using namespace std;
// initialize Attributes 
LdapConnection::LdapConnection(){
    this->ldapUri = "ldap://ldap.technikum-wien.at:389";
    this->ldapVersion = LDAP_VERSION3;
    this->ldapSearchBaseDomainComponent = "dc=technikum-wien,dc=at";
    this->ldapSearchScope = LDAP_SCOPE_SUBTREE;
    this->rawLdapUserEnv = getenv("ldapuser");
}
// initialize search Attribute
void LdapConnection::ldapBindUserInitialize(string uid){
    this->rawLdapUser = uid;
    this->ldapBindUser  = "uid="+ this->rawLdapUser +",ou=people,dc=technikum-wien,dc=at";
    this->ldapSearchFilter = "(uid=" + this->rawLdapUser + ")";
}
// setup LDAP connection
// https://linux.die.net/man/3/ldap_initialize
void LdapConnection::ldapInitialize(){
    int rc = ldap_initialize(&this->ldapHandle, this->ldapUri);
    if (rc != LDAP_SUCCESS)
    {
      cout << "ldap_init failed" <<endl;
      exit(EXIT_FAILURE);
    }
   cout << "connected to LDAP server" << endl;
}
// set verison options
// https://linux.die.net/man/3/ldap_set_option
void LdapConnection::ldapSetOption(){
    int rc = ldap_set_option(
       this->ldapHandle,
       LDAP_OPT_PROTOCOL_VERSION, // OPTION:Sets/gets the protocol version.
       &this->ldapVersion);       // IN-Value:LDAP_VERSION3
   if (rc != LDAP_OPT_SUCCESS)
   {
      cout<< "ldap_set_option(PROTOCOL_VERSION):"<< ldap_err2string(rc)<<endl;
      ldap_unbind_ext_s(this->ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
   }
}
// start connection secure (initialize TLS)
// https://linux.die.net/man/3/ldap_start_tls_s
void LdapConnection::ldapStartTls(){
    int rc = ldap_start_tls_s(
       this->ldapHandle,
       NULL,
       NULL);
   if (rc != LDAP_SUCCESS)
   {
      cout<< "ldap_start_tls_s():"<<ldap_err2string(rc) << endl;
      ldap_unbind_ext_s(this->ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
   }
}
// bind credentials
// https://linux.die.net/man/3/lber-types
// SASL (Simple Authentication and Security Layer)
// https://linux.die.net/man/3/ldap_sasl_bind_s
// https://man7.org/linux/man-pages/man3/lber-types.3.html
// BerValue: commonly used as struct berval, is used to hold an arbitrary sequence of octets.  bv_val points to bv_len octets.
void LdapConnection::ldapBindCredentials(string password){
    this->ldapBindPassword = password;
    BerValue bindCredentials;
    bindCredentials.bv_val = (char *)ldapBindPassword.c_str();
    bindCredentials.bv_len = ldapBindPassword.length();
    BerValue *servercredp; // server's credentials
    int rc = ldap_sasl_bind_s(
       this->ldapHandle,
       this->ldapBindUser.c_str(),
       LDAP_SASL_SIMPLE,
       &bindCredentials,
       NULL, //LDAP control
       NULL, //LDAP control
       &servercredp);
    if (rc != LDAP_SUCCESS)
    {
      cout<<"LDAP bind error:"<< ldap_err2string(rc)<< endl;
      ldap_unbind_ext_s(this->ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
    }
}
// perform ldap search
// https://linux.die.net/man/3/ldap_search_ext_s
void LdapConnection::ldapSearch(){
    int rc = ldap_search_ext_s(
       this->ldapHandle,
       this->ldapSearchBaseDomainComponent,
       this->ldapSearchScope,
       this->ldapSearchFilter.c_str(),
       (char **)this->ldapSearchResultAttributes,
       0,
       NULL,
       NULL,
       NULL,
       500,
       &this->searchResult);
    if (rc != LDAP_SUCCESS)
    {
      cout << "LDAP search error:"<< ldap_err2string(rc)<<endl;
      ldap_unbind_ext_s(this->ldapHandle, NULL, NULL);
      exit(EXIT_FAILURE);
    }
    cout<<"Welcome To TW-Mailer"<< endl;

}
// get result of search
// https://linux.die.net/man/3/ldap_first_entry
// https://linux.die.net/man/3/ldap_next_entry
void LdapConnection::ldapSerachResultEntry(){
    LDAPMessage *searchResultEntry;
    searchResultEntry = ldap_first_entry(this->ldapHandle, this->searchResult);
    
      // Attributes
      BerElement *ber;
      char *searchResultEntryAttribute;
      for (searchResultEntryAttribute = ldap_first_attribute(this->ldapHandle, searchResultEntry, &ber);
           searchResultEntryAttribute != NULL;
           searchResultEntryAttribute = ldap_next_attribute(this->ldapHandle, searchResultEntry, ber))
      {
         BerValue **vals;
         if ((vals = ldap_get_values_len(this->ldapHandle, searchResultEntry, searchResultEntryAttribute)) != NULL)
         {
            for (int i = 0; i < ldap_count_values_len(vals); i++)
            {
               cout<<"\t"<<searchResultEntryAttribute <<":" << vals[i]->bv_val<< endl;
            }
            ldap_value_free_len(vals);
         }

         // free memory
         ldap_memfree(searchResultEntryAttribute);
      }
      // free memory
      if (ber != nullptr)
      {
         ber_free(ber, 0);
      }

      cout<<endl;
   
   // free memory
   ldap_msgfree(searchResult);
}
// close ldap
// https://linux.die.net/man/3/ldap_unbind_ext_s
void LdapConnection::ldapClose(){
    ldap_unbind_ext_s(this->ldapHandle, NULL, NULL);
} 
