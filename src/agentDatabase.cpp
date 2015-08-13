#include "agentDatabase.hpp"

const Glib::ustring AgentDatabase::greek_letters[AgentDatabase::GREEK_LETTER_CNT] =
                                                   {"ALPHA", "BETA", "GAMMA",
                                                    "DELTA", "EPSILON", "ZETA",
                                                    "THETA", "IOTA", "KAPPA",
                                                    "LAMDA", "XI", "OMIKRON",
                                                    "PI", "SIGMA", "TAU",
                                                    "OMEGA"};

const Glib::ustring AgentDatabase::number_text[AgentDatabase::NUMBER_TEXT_CNT] =
                                                 {"1", "2", "3",
                                                  "4", "5", "6",
                                                  "7", "8", "9",
                                                  "10", "11", "12",
                                                  "13", "14", "15",
                                                  "16", "17", "18",
                                                  "19", "20"};

const Glib::ustring AgentDatabase::signals[AgentDatabase::SIGNALS_CNT] =
                           {"Triple-wink.",
                            "Rub your ear.",
                            "Tap your cheek twice.",
                            "Tap your shoulder twice.",
                            "Knock your knuckles together twice.",
                            "Tap chin, then nose.",
                            "Tap both temples at same time twice.",
                            "Hand signal 2 - 5 - fist.",
                            "Hand signal 3 - fist - 3.",
                            "Hand signal 1 - 2 - 1.",
                            "Tap your palm with other hand twice.",
                            "Rub chin, tap nose.",
                            "Tap both shoulders at same time.",
                            "Cross and tap your wrists together twice.",
                            "Tap your knee twice.",
                            "Cross arms and tap elbows."};

AgentDatabase::AgentDatabase(PlayerList* players, int defEncryptSeconds,
    int defTapSeconds)
{
    //Define a new key generator.
    keygen = new KeyGen();

    //Start with 0 agents. This will get adjusted by generateAgents.
    activeAgents = 0;

    //Save the defaults.
    defaultEncryptSeconds = defEncryptSeconds;
    defaultTapSeconds = defTapSeconds;

    //Generate the agents. (NOTE: This uses the above defaults.)
    generateAgents(players);

    /*The spycatcher is 0 by default, meaning the infiltrator is not yet caught.*/
    spycatcher = 0;
}

AgentDatabase::AgentDatabase()
{
    keygen = new KeyGen();
}

AgentDatabase::Agent::Agent(Glib::ustring last, Glib::ustring first,
    unsigned int defEncryptSeconds, unsigned int defTapSeconds)
{
    lastName = last;
    firstName = first;
    active = true;
    marked = false;
    struck = false;
    surprise = false;
    infiltrator = false;

    //Initially, the account has never had a login from gamemaster.
    overrides = 0;

    //Cloak and cloakUsed are off by default as well. ;)
    cloaked = false;
    cloakUsed = false;

    //Encryption is off.
    encryptee = 0;
    encrypted = false;

    encryptSeconds = defEncryptSeconds;
    tapSeconds = defTapSeconds;
}

void AgentDatabase::generateAgents(PlayerList* players)
{
    PlayerList::Player player;
    //We do NOT want more than 85 players! Even 85 is pushing it.
    for(int i0 = 0; i0 < 85; i0++)
    {
        player = players->getPlayer();

        if(player.firstName == "" && player.lastName == "")
        {
            //We're done with the loop.
            break;
        }

        //Otherwise, if we have some data, define a new player.
        Agent agent(player.lastName, player.firstName, defaultEncryptSeconds,
            defaultTapSeconds);
        agents.push_back(agent);
        activeAgents++;
    }
    /*Select the infiltrator. (Variable named for 1600s double agent
    Samuel Morland in the War of the Three Kingdoms)*/
    int morland = (keygen->rollDice(1, agents.size())) - 1;
    agents[morland].infiltrator = true;

    /*Assign agent IDs (1-size) using keygen's sequence.*/
    keygen->sequenceReset(agents.size());
    for(unsigned int i1 = 0; i1 < agents.size(); i1++)
    {
        int num = keygen->sequenceNext();
        agents[i1].id = num;

        //While we're in that loop, assign the security code.
        agents[i1].securitycode = keygen->getRandom();
    }

    //Assign the field codes.
    for(unsigned int i2 = 0; i2 < agents.size(); i2++)
    {
        //Use the sequencer to randomly fetch unique numbers between 1-99.
        keygen->sequenceReset(99);
        //Assign the field codes. Always give five extras.
        for(unsigned int c = 0; c < (5+agents.size()); c++)
        {
            /*Create the code using the agent id and a random number from the
            sequencer.*/
            int code = keygen->generateKey(agents[i2].id, keygen->sequenceNext());
            agents[i2].codes.insert(code);
            codesmaster.insert(code);
        }
    }

    /*Resort vector using std::sort (<algorithm>) and the overloaded <
    operator defined in Agent. http://stackoverflow.com/a/1380496/472647*/
    std::sort(agents.begin(), agents.end(), std::less<Agent>());

    /*Determine how many people to a team. If we have a lot of people, the
    minimum should be one quarter the number of players. If we have 8 or less,
    the minimum should always be 2.
    For the max, we never want more than 4 people to a team, but if we have
    less than 8 players, the max MUST be half the number of players.*/
    int teamSize = keygen->rollDice(
        (agents.size() > 8 ? agents.size() / 4 : 2),
        (agents.size() > 8 ? 4 : agents.size()/2));

    /*The number of teams is the number of players divided by the size of the
    teams. Any extra will be distributed in.*/
    int teamNum = agents.size() / teamSize;

    //However, we never want more than 16 teams or less than 2.
    teamNum > 16 ? teamNum = 16 : teamNum;
    teamNum < 2 ? teamNum = 2 : teamNum;

    //Generate the teams.
    generateTeams(teamNum);

    /*Loop through the teams, assigning <teamSize> players using
    keygen->sequenceNext().*/
    keygen->sequenceReset(agents.size());
    for(unsigned int t=0;t<teams.size();t++)
    {
        for(int ts=0; ts<teamSize; ts++)
        {
            int ta = keygen->sequenceNext();
            agents[ta-1].team = &teams[t];
        }
    }

    /*Then, if keygen->sequenceEmpty() is false,
    loop through again, assigning one player via keygen->sequenceNext()
    to each until keygen->sequenceEmpty() returns true.*/
    while(!keygen->sequenceEmpty())
    {
        for(unsigned int t1=0;t1<teams.size();t1++)
        {
            int ta = keygen->sequenceNext();
            agents[ta-1].team = &teams[t1];
            if(keygen->sequenceEmpty())
            {
                break;
            }
        }
    }
}

bool AgentDatabase::accuseAgent(int agentID, int accuseID)
{
    if(agents[accuseID-1].infiltrator)
    {
        agents[accuseID-1].active = false;
        agents[accuseID-1].struck = true;
        spycatcher = agentID;

        //Deactivate all of the infiltrator's remaining codes.
        for(auto c : agents[accuseID-1].codes)
        {
            codesused.insert(c);
        }

        return true;
    }
    else
    {
        agents[agentID-1].active = false;

        //If there are still active agents, decrease the count thereof.
        activeAgents > 0 ? activeAgents-- : 0;
        checkInfiltratorWon();

        return false;
    }
}

bool AgentDatabase::checkLogin(unsigned int id, unsigned int pass)
{
    bool r = false;

    //Check if the id is valid.
    if(id > 0 && id <= agents.size())
    {
        //Validate agent ID and security code.
        r = (agents[id-1].securitycode == pass);
        //If that failed...
        if(!r)
        {
            r = (pass==keygen->gamemasterCode);
            agents[id-1].overrides++;
        }
        return r;
    }
    else
    {
        return r;
    }
}

void AgentDatabase::checkInfiltratorWon()
{
    //If there's only one active agent left and we haven't caught the infiltrator...
    if(activeAgents == 1 && spycatcher == 0)
    {
        //We can infer that the infiltrator won!
        //Since we don't track the infiltrator externally, for each agent...
        for(unsigned int a = 0; a < agents.size(); a++)
        {
            //Double check that the agent is both infiltrator and active...
            if(agents[a].infiltrator && agents[a].active)
            {
                //If so, they're the spycatcher!
                spycatcher = agents[a].id;
                //Be sure to let them know.
                agents[a].surprise = true;
            }
        }
    }
}

void AgentDatabase::decommAgent(int id)
{
    //If the agent is indeed marked...
    if(agents[id-1].marked)
    {
        //Lower the boom!
        agents[id-1].active = false;
        //Be sure to notify them of the bad news!
        agents[id-1].struck = true;
        //If there are still active agents, decrease the count thereof.
        activeAgents > 0 ? activeAgents-- : 0;
        //Check if the infiltrator won!
        checkInfiltratorWon();
    }
}

void AgentDatabase::newSecurityCode(int id)
{
    agents[id-1].securitycode = keygen->getRandom();
}

AgentDatabase::RedemptionCode AgentDatabase::redeemCode(int id, int code)
{
    if(codesmaster.count(code) == 0)
    {
        return AgentDatabase::INVALID;
    }
    else if(agents[id-1].codes.count(code) > 0)
    {
        return AgentDatabase::OWN;
    }
    else if(codesused.count(code) > 0)
    {
        return AgentDatabase::USED;
    }
    else
    {
        codesused.insert(code);
        int fromID = keygen->checkKey(code);
        if(agents[fromID-1].infiltrator)
        {
            //If they're from the same team...
            if(agents[id-1].team->teamName == agents[fromID-1].team->teamName)
            {
                //Add as a connection to cover up the horrible truth.
                agents[id-1].connections.insert(agents[fromID-1].id);
                //Mark the agent redeeming the code.
                agents[id-1].marked = true;
                return AgentDatabase::MARK;
            }
            else
            {
                //Add as a connection to confuse things.
                agents[id-1].connections.insert(agents[fromID-1].id);
                //Decommission the agent redeeming the code.
                agents[id-1].active = false;
                //If there are still active agents, decrease the count thereof.
                activeAgents > 0 ? activeAgents-- : 0;
                checkInfiltratorWon();
                return AgentDatabase::POISON;
            }
        }
        else if(agents[id-1].connections.count(fromID) > 0)
        {
            return AgentDatabase::REPEAT;
        }
        else
        {
            //Add the agent as a connection.
            agents[id-1].connections.insert(agents[fromID-1].id);
            return AgentDatabase::SUCCESS;
        }
    }
    return AgentDatabase::SUCCESS;
}

int AgentDatabase::checkCode(int code)
{
    return keygen->checkKey(code);
}

void AgentDatabase::generateTeams(int numTeams)
{
    keygen->sequenceReset(16);
    for(int i0 = 0; i0 < numTeams; i0++)
    {
        Team team;
        int g = keygen->sequenceNext();
        team.teamName = greek_letters[g-1];
        teams.push_back(team);
    }

    keygen->sequenceReset(20);
    for(int i1 = 0; i1 < numTeams; i1++)
    {
        int n = keygen->sequenceNext();
        teams[i1].teamName += "-" + number_text[n-1];
    }

    keygen->sequenceReset(16);
    for(int i2 = 0; i2 < numTeams; i2++)
    {
        int s = keygen->sequenceNext();
        teams[i2].signal = signals[s-1];
    }

    /*std::cout << "\nTeams:" << teams.size() << std::endl;

    for(int t=0;t<teams.size();t++)
    {
        Team* printTeam = &teams[t];
        std::cout << printTeam->teamName << " " << printTeam->signal << std::endl;
    }*/
}

void AgentDatabase::printGameData(bool printAllCodes)
{
    std::cout << keygen->gamemasterCode << std::endl;
    for(unsigned int a = 0; a < agents.size(); a++)
    {
        std::cout << agents[a].id << ". " << agents[a].firstName
                  << " " << agents[a].lastName
                  << ": " << agents[a].team->teamName << " ["
                  << agents[a].infiltrator << "] - "
                  << agents[a].securitycode << "\nCodes:\n";
        for(auto f : agents[a].codes)
        {
            std::cout << "   " << f << "\n";
        }
        std::cout << std::endl;
    }

    if(printAllCodes)
    {
        std::cout << "\nMASTER CODES:" << std::endl;
        for(auto f : codesmaster)
        {
            std::cout << "   " << f << std::endl;
        }

    }
}

Glib::ustring AgentDatabase::int_to_ustring(int num)
{
    Glib::ustring str = Glib::ustring::format(num);
    return str;
}

int AgentDatabase::ustring_to_int(const Glib::ustring &txt)
{
    return atoi(txt.c_str());
}

AgentDatabase::~AgentDatabase()
{
    //dtor
}
