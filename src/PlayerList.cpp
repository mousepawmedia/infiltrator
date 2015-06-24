#include "PlayerList.hpp"

PlayerList::PlayerList()
{
    //ctor
}

void PlayerList::addPlayer(Glib::ustring last, Glib::ustring first)
{
    Player newPlayer = Player(last, first);
    players.push(newPlayer);
}

PlayerList::Player PlayerList::getPlayer()
{
    PlayerList::Player player;
    if(players.size() > 0)
    {
        player = players.front();
        players.pop();
    }
    return player;
}

PlayerList::~PlayerList()
{
}
