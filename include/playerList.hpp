#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include <iostream>
#include <glibmm.h>
#include <algorithm>
#include <queue>

class PlayerList
{
    public:
        PlayerList();

        struct Player
        {
            Player(Glib::ustring last = "", Glib::ustring first = "")
            {
                lastName = last;
                firstName = first;
            }
            Glib::ustring lastName;
            Glib::ustring firstName;
        };

        void addPlayer(Glib::ustring, Glib::ustring);
        int getCount();
        Player getPlayer();

        virtual ~PlayerList();
    protected:
        std::queue<Player> players;
    private:
};

#endif // PLAYERLIST_H
