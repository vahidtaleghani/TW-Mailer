CC=g++

CFLAGS=-g -Wall -Wextra -O -std=c++2a -I /usr/local/include/gtest/ -pthread 
LIBS=-lldap -llber 
GTEST=/usr/local/lib/libgtest.a

rebuild: clean all
all: ./bin/myldap

clean:
	clear
	rm -f bin/* obj/*

./obj/myldap.o: myldap.cpp
	${CC} ${CFLAGS} -o obj/myldap.o myldap.cpp -c 

./obj/UserInput.o: UserInput.cpp
	${CC} ${CFLAGS} -o obj/UserInput.o UserInput.cpp -c 

./obj/LdapConnection.o: LdapConnection.cpp
	${CC} ${CFLAGS} -o obj/LdapConnection.o LdapConnection.cpp -c 

./bin/myldap: ./obj/myldap.o ./obj/UserInput.o ./obj/LdapConnection.o 
	${CC} ${CFLAGS} -o bin/myldap obj/UserInput.o obj/LdapConnection.o obj/myldap.o ${LIBS}