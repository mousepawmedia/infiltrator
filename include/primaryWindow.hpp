#ifndef PRIMARYWINDOW_H
#define PRIMARYWINDOW_H

#include <iostream>
#include <glibmm.h>
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include "agentDatabase.hpp"
#include "customWidgets.hpp"
#include "fileEncryption.hpp"
#include "gameWindow.hpp"
#include "setupWindow.hpp"

class primaryWindow : public Gtk::Window
{
    public:
        primaryWindow();
        ~primaryWindow();
    protected:
    private:
        AgentDatabase* db;
        //Glib::ustring* filepath;
        fileEncryption* fe;

        /**The label containing the game's fancy graphical title! Ooh, pretty!*/
        TitleLabel lbl_title;

        Gtk::Box box_main;

        Gtk::Button btn_startgame;

        setupWindow* win_setup;
        gameWindow* win_game;

        void showSetupScreen();
        bool loadGame(GdkEventAny*);
        bool unloadGame(GdkEventAny*);
};

#endif // PRIMARYWINDOW_H
