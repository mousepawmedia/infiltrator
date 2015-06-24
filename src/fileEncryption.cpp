#include "fileEncryption.hpp"

fileEncryption::fileEncryption(Glib::ustring fileName)
{
    gameName = fileName;
}

fileEncryption::~fileEncryption()
{
    //dtor
}

bool fileEncryption::readFile()
{
    try
    {
        ofstream myfile (gameName + ".txt");
        if (myfile.is_open())
        {

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

bool fileEncryption::encryptFile()
{
    return true;
}

bool fileEncryption::decryptFile()
{
    return true;
}

bool fileEncryption::saveFile(AgentDatabase agent)
{
    try
    {
        //creates a file with the game name
        ofstream myfile (gameName + ".txt");
        if (myfile.is_open())
        {
            //Saves active agents and spycatcher ints
            myfile << agent.activeAgents << endl;
            myfile << agent.spycatcher << endl;

            //Saves the entropy number for reverse engineering
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
                Agent Tap Seconds
                Agent Encryptee
                Agent Encrypt Seconds
                Agent Security Code
                Agent Set Connections
                Agent Set Codes
                Agent First Name
                ect... (repeats the # of teams)
            */
            myfile << agent.agents.size() << endl;
            for(i = 0; i < agent.agents.size(); i++)
            {
                //Iterrates through each agent as saves all its relevent data to a file.
                myfile << agent.agents[i].firstName << "\n"
                       << agent.agents[i].lastName << "\n"
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
                       << agent.agents[i].tapSeconds << "\n"
                       << agent.agents[i].encryptee << "\n"
                       << agent.agents[i].encryptSeconds << "\n"
                       << agent.agents[i].securitycode << endl;

                //If the agents connections set isn't empty it copies it to the file with comma separation, or a '0' if empty
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
                //Same as above but with codes set
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
