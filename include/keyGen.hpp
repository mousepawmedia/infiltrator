#ifndef KEYGEN_H
#define KEYGEN_H

#include <iomanip>
#include <iostream>
#include <chrono>
#include <random>

#include <glibmm.h>

/*Algorithm:
Cantor-Pair Agent ID w/ Key Number
Cantor-Pair Result w/ Entropy*/

class KeyGen
{
    public:
        /** Create a new instance of the code generator, complete
        with a fresh random number generator and entropy key.*/
        KeyGen();

        /** Generate a key for an agent.
        \param The agent ID.
        \param The key number.
        \return The key.*/
        int generateKey(int, int);

        /** Return the identity of the agent for a key.
        \param The code.
        \return The agent ID.*/
        int checkKey(int);

        unsigned int getRandom();
        unsigned int rollDice(unsigned int, unsigned int);

        void sequenceReset(unsigned int);
        int sequenceNext();
        bool sequenceEmpty();

        unsigned int gamemasterCode;

        unsigned int entropy;

        ~KeyGen();
    private:
        int szuPairing(int, int);
        int getXFromSzu(int);
        int getYFromSzu(int);

        unsigned int seed;
        std::vector<unsigned int> sequence;
        std::minstd_rand* infinite_improbability_drive;

};

#endif // KEYGEN_H
