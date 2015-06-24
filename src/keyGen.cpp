#include "keyGen.hpp"

KeyGen::KeyGen()
{
    //Obtain a seed from the system clock.
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    //Generate a random entropy key for Cantor pairing.
    infinite_improbability_drive = new std::minstd_rand(seed);
    entropy = abs((*infinite_improbability_drive)() / 1234567) + 3;
    srand(getRandom());

    gamemasterCode = getRandom();
}

int KeyGen::checkKey(int key)
{
    //Subtract entropy value from key to get actual pairing number (z).
    key = key - entropy;
    //Return the agent id (x) from the pairing (z). Ignore the key number (y).
    return getXFromSzu(key);
}

int KeyGen::generateKey(int agent, int number)
{
    int intermediary = szuPairing(agent, number);
    int key = intermediary + entropy;
    return key;
}

unsigned int KeyGen::getRandom()
{
    infinite_improbability_drive->discard(2);
    return (abs((*infinite_improbability_drive)() / 12345) + 3);
}

unsigned int KeyGen::rollDice(unsigned int minimum, unsigned int maximum)
{
    unsigned int r = rand() % maximum + minimum;

    //Validate random number.
    r > maximum ? r = maximum : r;
    r < minimum ? r = minimum : r;

    return r;
}

void KeyGen::sequenceReset(unsigned int size)
{
    sequence.clear();
    for(unsigned int i=1;i<=size;i++)
    {
        sequence.push_back(i);
    }
}

int KeyGen::sequenceNext()
{
    int r = 0;
    if(sequence.size() != 0)
    {
        int index = (rollDice(1, sequence.size())) - 1;
        r = sequence[index];
        sequence.erase(sequence.begin() + index);
    }
    return r;
}

bool KeyGen::sequenceEmpty()
{
    return (sequence.size() <= 0);
}

//Create Szudzik pairing.
int KeyGen::szuPairing(int x, int y)
{
    int z = (x >= y ? x * x + x + y : x + y * y);
    return z;
}

//Derive x from Szudzik pairing.
int KeyGen::getXFromSzu(int z)
{
    int zAbsRoot = abs(sqrt(z));
    int zMinus = z - pow(zAbsRoot,2);
    int x = (zMinus >= zAbsRoot ? zAbsRoot : zMinus);
    return x;
}

//Derive z from Szudzik pairing.
int KeyGen::getYFromSzu(int z)
{
    int zAbsRoot = abs(sqrt(z));
    int zMinus = z - pow(zAbsRoot,2);
    int y = (zMinus >= zAbsRoot ? zMinus - zAbsRoot : zAbsRoot);
    return y;
}

KeyGen::~KeyGen()
{
    delete infinite_improbability_drive;
}
