#include "fileEncryption.hpp"

fileEncryption::fileEncryption(Glib::ustring fileName)
{
    gameName = fileName;
}

fileEncryption::~fileEncryption()
{
    //dtor
}

bool fileEncryption::readFile(AgentDatabase *agent)
{
    try
    {
        ifstream myfile(gameName + ".txt");
        if(myfile.is_open())
        {
            ///Reads cipher number to offset all chars in the file
            int cipher;
            myfile >> cipher;
            ///Reading agent general info from saved game file
            agent->activeAgents = decryptFileInt(&myfile, cipher);
            agent->spycatcher = decryptFileInt(&myfile, cipher);
            agent->keygen->entropy = decryptFileInt(&myfile, cipher);

            ///Reading team info from saved game file
            int teamnum = decryptFileInt(&myfile, cipher);
            string temp;
            ///gets the "\n" out
            getline(myfile, temp);
            int i;
            for(i = 0; i < teamnum; i++)
            {
                AgentDatabase::Team team;
                getline(myfile, temp);
                team.teamName = decrypt(temp, cipher);
                getline(myfile, temp);
                team.signal = decrypt(temp, cipher);
                agent->teams.push_back(team);
            }

            ///Reads agent info from saved game file
            int agentnum = decryptFileInt(&myfile, cipher);
            for(i = 0; i < agentnum; i++)
            {
                Glib::ustring first = decryptFileString(&myfile, cipher);
                Glib::ustring last = decryptFileString(&myfile, cipher);
                int encryptSec = decryptFileInt(&myfile, cipher);
                int tapSec = decryptFileInt(&myfile, cipher);
                AgentDatabase::Agent myagent(last, first, encryptSec, tapSec);

                ///Reads team name, finds team signal, and copies team info into the agents info
                myagent.team = new AgentDatabase::Team();
                myagent.team->teamName = decryptFileString(&myfile, cipher);
                unsigned int x;
                for(x = 0; x < agent->teams.size(); x++)
                {
                    if(myagent.team->teamName == agent->teams[x].teamName)
                        myagent.team->signal = agent->teams[x].signal;
                }

                ///Finishes reading agent info
                myagent.id = decryptFileInt(&myfile, cipher);
                myagent.active = decryptFileInt(&myfile, cipher);
                myagent.marked = decryptFileInt(&myfile, cipher);
                myagent.cloaked = decryptFileInt(&myfile, cipher);
                myagent.cloakUsed = decryptFileInt(&myfile, cipher);
                myagent.encrypted = decryptFileInt(&myfile, cipher);
                myagent.struck = decryptFileInt(&myfile, cipher);
                myagent.surprise = decryptFileInt(&myfile, cipher);
                myagent.infiltrator = decryptFileInt(&myfile, cipher);
                myagent.overrides = decryptFileInt(&myfile, cipher);
                myagent.encryptee = decryptFileInt(&myfile, cipher);
                myagent.securitycode = decryptFileInt(&myfile, cipher);

                ///Gets a comma separated list of connections and tokenizes them. Then after some converting puts them into the connection set.
                getline(myfile, temp);
                getline(myfile, temp);
                temp = decrypt(temp, cipher);
                if(temp != "0")
                {
                    char* token = &temp[0];
                    char* conntemp = strtok(token, ",");;
                    while(conntemp != NULL)
                    {
                        int conn = atoi(conntemp);
                        myagent.codes.insert(conn);
                        conntemp = strtok(NULL, ",");
                    }
                }
                getline(myfile, temp);
                temp = decrypt(temp, cipher);

                ///Gets a comma separated list of codes and tokenizes them.  Then after some converting puts them into the codes set.
                char* token = &temp[0];
                char* codetemp = strtok(token, ",");;
                while(codetemp != NULL)
                {
                    int code = atoi(codetemp);
                    myagent.codes.insert(code);
                    codetemp = strtok(NULL, ",");
                }

                ///Adds agent to agent vector
                agent->agents.push_back(myagent);
            }

            return true;
        }
        else
        {
            cout << "Unable to open file";
            return false;
        }
    }
    catch(...)
    {
        cout << "Error reading file";
        return false;
    }
}

Glib::ustring fileEncryption::decryptFileString(ifstream *myfile, int cipher)
{
    Glib::ustring temp;
    *myfile >> temp;
    temp = decrypt(temp, cipher);
    return temp;
}

int fileEncryption::decryptFileInt(ifstream *myfile, int cipher)
{
    int tempint;
    Glib::ustring temp;
    *myfile >> temp;
    temp = decrypt(temp, cipher);
    tempint = atoi(temp.c_str());
    return tempint;
}

Glib::ustring fileEncryption::decrypt(Glib::ustring temp, int cipher)
{
    cipher += 3;
    //int check = temp.size();
    //char dc[temp.size() - 1];
    Glib::ustring dc = "";
    int count = 0;
    for(Glib::ustring::iterator it = temp.begin(); it!=temp.end(); ++it)
    {
        char ec = *it;
        ec -= cipher;
        ///Keeps it in range of output characters
        if((int)ec < 32)
        {
            ec += 93;
        }
        dc.append(sizeof(char), ec);
        count++;
    }
    return dc;
}

bool fileEncryption::encryptFile()
{
    try
    {
        ///Holds file contents
        Glib::ustring buffer = "";
        ifstream myfile(gameName + ".txt");
        if(myfile.is_open())
        {
            ///Read file into ustring
            while(!myfile.eof()){
                string temp;
                getline(myfile, temp);
                buffer += temp + "\n";
            }
            myfile.close();
            ofstream outfile(gameName + ".txt");
            if(outfile.is_open())
            {
                ///Gets a random number between 1 and 20 to use to offset each char in buffer
                int cipher;
                srand (time(NULL));
                cipher = rand() % 20 + 1;
                ///Writes the cipher number to the file, but adds 3 to it to use as the cipher so no one that reads the file can tell what number its offset by
                outfile << cipher << endl;
                cipher += 3;
                ///Iterates through the ustring and adds the cipher to the ascii value of each char
                for(Glib::ustring::iterator it = buffer.begin(); it!=buffer.end(); ++it)
                {
                    unsigned char temp = *it;
                    ///bypasses new lines
                    if(temp != '\n')
                    {
                        temp += cipher;
                        ///Keeps it in range of output characters
                        if((int)temp > 126)
                        {
                            temp -= 93;
                        }
                    }
                    outfile << temp;
                }
                outfile.close();
            }
            else
            {
                cout << "Unable to open file";
                return false;
            }
            return true;
        }
        else
        {
            cout << "Unable to open file";
            return false;
        }
    }
    catch(...)
    {
        cout << "Error reading file";
        return false;
    }
}

bool fileEncryption::saveFile(AgentDatabase agent)
{
    try
    {
        ///creates a file with the game name
        ofstream myfile(gameName + ".txt");
        if(myfile.is_open())
        {
            ///Saves active agents and spycatcher ints
            myfile << agent.activeAgents << endl;
            myfile << agent.spycatcher << endl;

            ///Saves the entropy number for reverse engineering
            myfile << agent.keygen->entropy << endl;

            /* Saves the number of teams and then team info to the file. Format is:
                # of teams
                Team Name
                Team Signal
                Team Name
                ect... (repeats the # of teams)
            */
            myfile << agent.teams.size() << endl;
            unsigned int i;
            for(i = 0; i < agent.teams.size(); i++)
            {
                myfile << agent.teams[i].teamName << "\n"
                       << agent.teams[i].signal << endl;
            }

            /* Saves the number of agents and agent info to the file. Format is:
                # of agents
                Agent First Name
                Agent Last Name
                Agent Encrypt Seconds
                Agent Tap Seconds
                Agent Team Name
                Agent ID
                Agent Active
                Agent Marked
                Agent Cloaked
                Agent Cloak Used
                Agent Encrypted
                Agent Struck
                Agent Surprise
                Agent Infiltrator
                Agent Overrides
                Agent Encryptee
                Agent Security Code
                Agent Set Connections
                Agent Set Codes
                Agent First Name
                ect... (repeats the # of teams)
            */

            myfile << agent.agents.size() << endl;
            for(i = 0; i < agent.agents.size(); i++)
            {

                ///Iterrates through each agent as saves all its relevent data to a file.
                myfile << agent.agents[i].firstName << "\n"
                       << agent.agents[i].lastName << "\n"
                       << agent.agents[i].encryptSeconds << "\n"
                       << agent.agents[i].tapSeconds << "\n"
                       << agent.agents[i].team->teamName << "\n"
                       << agent.agents[i].id << "\n"
                       << agent.agents[i].active << "\n"
                       << agent.agents[i].marked << "\n"
                       << agent.agents[i].cloaked << "\n"
                       << agent.agents[i].cloakUsed << "\n"
                       << agent.agents[i].encrypted << "\n"
                       << agent.agents[i].struck << "\n"
                       << agent.agents[i].surprise << "\n"
                       << agent.agents[i].infiltrator << "\n"
                       << agent.agents[i].overrides << "\n"
                       << agent.agents[i].encryptee << "\n"
                       << agent.agents[i].securitycode << endl;

                ///If the agents connections set isn't empty it copies it to the file with comma separation, or a '0' if empty
                if(agent.agents[i].connections.size() > 0)
                {
                    unsigned int count = 0;
                    std::set<int>::iterator iter;
                    for(iter = agent.agents[i].connections.begin(); iter != agent.agents[i].connections.end(); iter++)
                    {
                        if(count + 1 == agent.agents[i].codes.size())
                            myfile << *iter;
                        else
                            myfile << *iter << ",";
                        count++;
                    }
                }
                else
                    myfile << "0";
                myfile << endl;

                ///Same as above but with codes set
                if(agent.agents[i].codes.size() > 0)
                {
                    unsigned int count = 0;
                    std::set<int>::iterator iter2;
                    for(iter2 = agent.agents[i].codes.begin(); iter2 != agent.agents[i].codes.end(); iter2++)
                    {
                        if(count + 1 == agent.agents[i].codes.size())
                            myfile << *iter2;
                        else
                            myfile << *iter2 << ",";
                        count++;
                    }
                }
                else
                    myfile << "0";
                myfile << endl;
            }
            myfile.close();
            if(encryptFile() == false)
            {
                cout << "Unable to encrypt file";
            }
            return true;
        }
        else
        {
            cout << "Unable to open file";
            return false;
        }
    }
    catch(...)
    {
        cout << "Error saving file";
        return false;
    }
}
