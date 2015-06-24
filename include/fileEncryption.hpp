#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

//Third-party includes
#include <glibmm.h>
#include <iostream>
#include <fstream>

#include "agentDatabase.hpp"

using namespace std;


class fileEncryption
{
    public:
        fileEncryption(Glib::ustring);
        virtual ~fileEncryption();
        bool saveFile(AgentDatabase);
    protected:
        bool encryptFile();
        bool decryptFile();

        bool readFile();
    private:
        Glib::ustring gameName;
};

#endif // FILEENCRYPTION_H
