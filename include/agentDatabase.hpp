#ifndef AGENTDATABASE_H
#define AGENTDATABASE_H

//STL includes
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

//Third-party includes
#include <glibmm.h>

//In-project includes
#include "PlayerList.hpp"
#include "keyGen.hpp"

class AgentDatabase
{
    public:
        /**Creates a new agent database.
        \param A pointer to the game's key generator (KeyGen)
        \param the number of encryptions to start with
        \param the number of interception seconds to start with
        */
        AgentDatabase(PlayerList*, int=180, int=180);

        /**The struct defining a team.*/
        struct Team
        {
            /**The team name.*/
            Glib::ustring teamName;
            /**The team's secret signal.*/
            Glib::ustring signal;
        };

        /**The struct defining an agent and his/her game info.*/
        struct Agent
        {
            /**Creates a new agent struct.
            \param the agent's last name
            \param the agent's first name
            \param the number of encryptions to start with
            \param the number of interception seconds to start with*/
            Agent(Glib::ustring, Glib::ustring, unsigned int, unsigned int);

            /**Overload the less-than (<) operator, for sorting agents by ID.*/
            bool operator < (const Agent& str) const
            {
                return (id < str.id);
            }

            /**Is the agent active (true) or decommissioned (false)?*/
            bool active;

            /**Is the agent marked to be decommissioned on-demand by the
            infiltrator?*/
            bool marked;

            /**Is the agent presently cloaked?*/
            bool cloaked;

            /**Has the agent used their cloak? (They only get one use!)*/
            bool cloakUsed;

            /**Is the agent being protected against interception by someone?*/
            bool encrypted;

            /**Has the agent been decommissioned while they were logged out?
            This applies both to being switched from marked to decommissioned,
            and to if the infiltrator was accused correctly.*/
            bool struck;

            ///Has the agent had something good happen while they were out?
            bool surprise;

            /**How many times the gamemaster has logged in as the agent.*/
            unsigned int overrides;

            /**The agent's id number.*/
            unsigned int id;

            /**How many seconds the agent has left for interception.*/
            int tapSeconds;

            /**The id of the agent that this agent is protecting (encrypting).*/
            unsigned int encryptee;

            /**How many times the agent can protect (encrypt) the agent.*/
            int encryptSeconds;

            /**Is the agent the infiltrator?*/
            bool infiltrator;

            /**The agent's security code, which serves as their login passcode.*/
            unsigned int securitycode;

            /**The agent's last name. Defined by the constructor.*/
            Glib::ustring lastName;

            /**The agent's first name. Defined by the constructor.*/
            Glib::ustring firstName;

            /**The pointer to the agent's team.*/
            Team* team;

            /**The IDs of the agents this agent is connected to.*/
            std::set<int> connections;

            /**The codes this agent has, which he/she can give to others.*/
            std::set<int> codes;
        };

        enum RedemptionCode
        {
            ///The code was invalid.
            INVALID = 0,
            ///The redeeming agent is already connected to the code's agent.
            REPEAT = 1,
            ///The code was already redeemed.
            USED = 2,
            ///The code belongs to the redeeming agent.
            OWN = 3,
            ///The code was successfully redeemed.
            SUCCESS = 4,
            ///The code was from the infiltrator.
            POISON = 5,
            /**The code was from the infiltrator, but the redeeming agent is
            being marked instead of immediately decommissioned.*/
            MARK = 6,
        };

        ///The vector of teams that are defined in the game.
        std::vector<Team> teams;
        ///The vector of agents that are defined in the game.
        std::vector<Agent> agents;
        ///The master list (set) of ALL codes.
        std::set<int> codesmaster;
        ///The master list (set) of USED codes.
        std::set<int> codesused;

        ///The default number of encrypts per agent.
        unsigned int defaultEncryptSeconds;
        ///The default number of interception seconds.
        unsigned int defaultTapSeconds;

        /**The agent ID of the person to catch the infiltrator, or the
        infiltrator him/herself if all the other agents were decommissioned.*/
        int spycatcher;

        /**The number of agents still active.*/
        unsigned int activeAgents;

        /**Accuse an agent of being the infiltrator, and trigger the
        consequences.
        \param the id of the agent doing the accusing
        \param the id of the agent being accused
        \return true if the accusation was correct, else false*/
        bool accuseAgent(int, int);

        /**Check the login information for the agent.
        \param the id of the agent to login as
        \param the security code for the agent, or the gamemaster override
        \return true if login is valid, false otherwise*/
        bool checkLogin(unsigned int, unsigned int);

        /**Get the id of the agent that the code is from.
        \param the code to check
        \return the agent id that the code is from*/
        int checkCode(int);

        /**Check if the infiltrator won "spycatcher" by default.*/
        void checkInfiltratorWon();

        /**Decommission a marked agent.
        \param the unlucky agent to decommission*/
        void decommAgent(int);

        /**Create a new security code for an agent.
        \param the agent to create a new code for*/
        void newSecurityCode(int);

        /**Redeem a code, and trigger the consequences.
        \param the id of the agent redeeming the code
        \param the code being redeemed
        \return the RedemptionCode for the result*/
        RedemptionCode redeemCode(int, int);

        /**A utility function converting an integer to a Glib::ustring.
        \param the integer to convert
        \return the resulting Glib::ustring*/
        static Glib::ustring int_to_ustring(int);

        /**A utility function converting a Glib::ustring to an integer.
        \param the Glib::ustring to convert
        \return the resulting integer*/
        static int ustring_to_int(const Glib::ustring&);

        ~AgentDatabase();
    private:
        ///The pointer storing the key generator instance for the game.
        KeyGen* keygen;

        /**Generate the agents from the PlayerList, as well as their teams.
        ONLY CALL THIS AT GAME START!
        \param the player list to generate agents and teams from*/
        void generateAgents(PlayerList*);

        /**Generate the teams for the game. This populates the teams vector.
        \param the number of teams to create.*/
        void generateTeams(int);

        /**Assigns given number of agents to the given team. Requires
        that keyGen has an active sequence.
        \param the number of agents to assign
        \param the team (pointer) to assign to*/
        void pickTeam(int, Team*);

        //Selected Greek letter names for teams.
        static const unsigned int GREEK_LETTER_CNT = 16;
        static const Glib::ustring greek_letters[GREEK_LETTER_CNT];
        //Also, select a number between 1-20 for each.
        static const unsigned int NUMBER_TEXT_CNT = 20;
        static const Glib::ustring number_text[NUMBER_TEXT_CNT];
        //Team signals.
        static const unsigned int SIGNALS_CNT = 16;
        static const Glib::ustring signals[SIGNALS_CNT];

        /**Print all players, their login info, teams, codes, and whether they're
        the infiltrator.
        \param whether to print out a master list of all the codes*/
        void printGameData(bool=false);
};

#endif // AGENTDATABASE_H
