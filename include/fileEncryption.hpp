#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

//Third-party includes
#include <glibmm.h>
#include <iostream>
#include <fstream>
#include <set>

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
        bool readFile();
    private:
        ///The game name which is also the filename data is saved to.
        Glib::ustring gameName;
        /**A utility function that decrypts the saved data file.
        \return the resulting boolean*/
        bool encryptFile();
        /**A utility function that encrypts the saved data file.
        \return the resulting boolean*/
        bool decryptFile();
};

#endif // FILEENCRYPTION_H
