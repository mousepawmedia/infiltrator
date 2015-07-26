#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

//Third-party includes
#include <glibmm.h>
#include <iostream>
#include <fstream>
#include <set>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "agentDatabase.hpp"

using namespace std;


class fileEncryption
{
    public:
        /**A constructor for fileEncryption.
        \param the Glib::ustring of the game name*/
        fileEncryption(Glib::ustring);
        virtual ~fileEncryption();
        /**A utility function that saves all agentDatabase info to a file for import later.
        \param the AgentDatabase class object to retieve data from
        \return the resulting boolean*/
        bool saveFile(AgentDatabase);
        /**A utility function that reads game data from a file.
        \param the AgentDatabase class object to save data to
        \return the resulting boolean*/
        bool readFile(AgentDatabase*);
    private:
        ///The game name which is also the filename data is saved to.
        Glib::ustring gameName;
        /**A utility function that encrypts the saved data file.
        \return the resulting boolean*/
        bool encryptFile();
        /**A utility function that decrypts the saved data file strings.
        \return the resulting ustring*/
        Glib::ustring decryptFileString(ifstream*, int);
        /**A utility function that decrypts the saved data file ints.
        \return the resulting int*/
        int decryptFileInt(ifstream*, int);
        /**A utility function that offsets chars based on cipher.
        \return the resulting ustring*/
        Glib::ustring decrypt(Glib::ustring, int);
};

#endif // FILEENCRYPTION_H
