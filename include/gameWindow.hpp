#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>

#include <glibmm.h>
#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>

#include <agentDatabase.hpp>
#include <agentWindow.hpp>
#include <customWidgets.hpp>
#include <fileEncryption.hpp>

class gameWindow : public Gtk::Window
{
    public:
        /**Defines a new game window for logging in and viewing scores.
        \param a pointer to the agent database instance we're using
        \param a pointer to the file encryption object we're using*/
        gameWindow(AgentDatabase*, fileEncryption*);

        /**Save the game.*/
        void saveGame();

        /**Update the contents for the scores (game status) list, lst_scores.*/
        void updateScores();

        ~gameWindow();
    private:
        /**The pointer to the agent database instance that the whole game is
        using.*/
        AgentDatabase* db;

        /**The pointer to the file encryption object we're using.*/
        fileEncryption* fe;

        /**The main box containing stuff.*/
        Gtk::Box box_main;

        Gtk::Box box_scores;

        /**The grid containing the login stuff. More efficient in this scenario
        than using a whole ton of boxes.*/
        Gtk::Grid grd_login;

        /**The button for logging in as an agent with the information entered.*/
        Gtk::Button btn_login;

        /**The numbers-only text entry for entering an agent's id.*/
        NumberEntry txt_id;

        /**The numbers-only text entry for entering an agent's security code.*/
        NumberEntry txt_security;

        /**The label containing the game's fancy graphical title! Ooh, pretty!*/
        TitleLabel lbl_title;

        /**The label prompting for the user ID.*/
        Gtk::Label lbl_id;

        /**The label prompting for the user's security code.*/
        Gtk::Label lbl_security;

        /**The label telling us when we were unable to login, because popup
        boxes are SO last year. (Actually, I just like how this looks. Whatevs.*/
        Gtk::Label lbl_login_msg;

        /**The label introducing the game status/high scores sector in a fancy
        way. More prettiness!*/
        Gtk::Label lbl_scores_header;

        Gtk::Notebook nbk_main;
        RulesNotebook nbk_rules;

        /**A pointer to the agent window, for when we dynamically allocate and
        define a new one on the login() function.*/
        agentWindow* agentWin;

        /**A class defining the columns for the model for the scores list. Gtkmm
        can be so incredibly non-trivial at times!*/
        class ScoreMdlCols : public Gtk::TreeModel::ColumnRecord
        {
            public:
                ScoreMdlCols()
                {
                    add(col_id);
                    add(col_status);
                    add(col_last);
                    add(col_first);
                    add(col_rank);
                    add(col_title);
                }
                Gtk::TreeModelColumn<int> col_id,
                                          col_rank;
                Gtk::TreeModelColumn<Glib::ustring> col_status,
                                                    col_last,
                                                    col_first,
                                                    col_title;
        };

        /**The scroll window for the scores list.*/
        Gtk::ScrolledWindow scrl_lst_scores;

        /**The model for the scores list.*/
        ScoreMdlCols mdl_col_score;

        /**The list of scores/game status.*/
        Gtk::TreeView lst_scores;

        /**A reference to the liststore used by the list of scores.*/
        Glib::RefPtr<Gtk::ListStore> ref_mdl_scores;

        /**Populate the scores (game status) list, lst_scores.*/
        void addScores();

        /**Login using the ID and security code entered on the interface.*/
        void login();

        /**Handle the signal that the agent window opened by login() was closed.
        \param gtkmm demands a sacrifice, and it selects it itself. (The handler
        will deal with this.
        \return false, so we don't prevent other handlers from working with this
        signal*/
        bool winClosed(GdkEventAny*);
};

#endif // GAMEWINDOW_H
