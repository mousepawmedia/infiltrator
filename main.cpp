#include <glibmm.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <iostream>

#include "agentWindow.hpp"
#include "agentDatabase.hpp"
#include "fileEncryption.hpp"
#include "gameWindow.hpp"
#include "keyGen.hpp"
#include "playerList.hpp"
#include "primaryWindow.hpp"
#include "setupWindow.hpp"

int main(int argc, char *argv[])
{
    /*PlayerList players;
    players.addPlayer("Llewellyn", "Ozymandius");
    players.addPlayer("Llewellyn", "Millicent");
    players.addPlayer("Aardvark", "Stephen");
    players.addPlayer("Pingo", "Autumn");
    players.addPlayer("Linkletter", "Jacob");
    players.addPlayer("Et", "Tiffany");
    players.addPlayer("Oven", "Bud");
    players.addPlayer("Hu", "Kaitlyn");
    players.addPlayer("Iddenstein", "Vincent");
    players.addPlayer("Xane", "Ursula");
    players.addPlayer("Wozwak", "Quincy");
    players.addPlayer("Nutley", "Yvette");

    AgentDatabase database(&players);*/
    /*AgentDatabase testing;

    //For testing save game purposes
    fileEncryption fe("Testing1234");
    fe.readFile(&testing);
    //fileEncryption de("Testing1234");
    fe.saveFile(testing);*/

    Glib::RefPtr<Gtk::Application> app =
        Gtk::Application::create(argc, argv, "com.mousepawgames.labs.infiltrator");

    primaryWindow window;

    // We'll pass our application's return code here.
    return app->run(window);
    ////return 0;
}
