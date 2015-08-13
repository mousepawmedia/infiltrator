#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <iostream>

#include <glibmm.h>
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

#include <agentDatabase.hpp>
#include <customWidgets.hpp>
#include <fileEncryption.hpp>


class setupWindow : public Gtk::Window
{
    public:
        /**Defines a new setup window for starting a game.
        \param a pointer to the database pointer.
        \param a pointer to the filename string pointer.
        */
        setupWindow(AgentDatabase**, fileEncryption**);

        ~setupWindow();
    private:
        ///Pointer to the main database pointer.
        AgentDatabase** db;
        ///Pointer to the file read/write object pointer,.
        fileEncryption** fe;

        Glib::ustring filepath;

        /**Pointer for the PlayerList object, used temporarily to generate
        a new game.*/
        PlayerList* plst = 0;

        bool flag_plstbuild = false;

        /**The main box containing stuff.*/
        Gtk::Box box_main;

        ///Button to add the player to the list.
        Gtk::Button btn_add;
        ///Button to remove the player from the list.
        Gtk::Button btn_remove;
        ///Button to browse for load file.
        Gtk::Button btn_load;
        ///Button to browse for save file.
        Gtk::Button btn_save;
        ///Button to start new game.
        Gtk::Button btn_startnew;
        ///Button to load game.
        Gtk::Button btn_startload;

        ///Load path text field.
        Gtk::Entry txt_loadpath;
        ///Save path text field.
        Gtk::Entry txt_savepath;
        ///First name text field.
        Gtk::Entry txt_firstname;
        ///Last name text field.
        Gtk::Entry txt_lastname;

        /**The layout grid for About.*/
        Gtk::Grid grd_about;
        /**The layout grid for New Game.*/
        Gtk::Grid grd_new;
        /**The layout grid for Load Game.*/
        Gtk::Grid grd_load;

        Gtk::Label lbl_encrypt;
        Gtk::Label lbl_intercept;
        Gtk::Label lbl_msgnew;
        Gtk::Label lbl_msgload;

        /**The notebook with the New Game and Load Game tabs.*/
        Gtk::Notebook nbk_main;

        /**The spin button for encrypt seconds.*/
        Gtk::SpinButton spn_encrypt;
        /**The spin button for intercept seconds.*/
        Gtk::SpinButton spn_intercept;

        /**The label containing the game's fancy graphical title! Ooh, pretty!*/
        TitleLabel lbl_title;

        /**The notebook containing the game's rules and info.*/
        RulesNotebook nbk_rules;

        /**A class defining the columns for the model for the player list. Gtkmm
        can be so incredibly non-trivial at times!*/
        class PlayerMdlCols : public Gtk::TreeModel::ColumnRecord
        {
            public:
                PlayerMdlCols()
                {
                    add(col_last);
                    add(col_first);
                }
                Gtk::TreeModelColumn<Glib::ustring> col_first,
                                                    col_last;
        };

        /**The scroll window for the players list.*/
        Gtk::ScrolledWindow scrl_lst_players;

        /**The model for the players list.*/
        PlayerMdlCols mdl_col_players;

        /**The list of players status.*/
        Gtk::TreeView lst_players;

        /**A reference to the liststore used by the list of players.*/
        Glib::RefPtr<Gtk::ListStore> ref_mdl_players;
        /**A reference to the selected row in the list of players.*/
        Glib::RefPtr<Gtk::TreeSelection> ref_sel_players;

        /**Add a new player using the contents of the first and last name
        fields.*/
        void addPlayer();

        void browseLoad();
        void browseSave();

        Glib::ustring dlgBrowseLoad();
        Glib::ustring dlgBrowseSave();

        /**Searches for a player in the list.
        \param the first name of the player
        \param the last name of the player
        \return true if player is in list, false otherwise.*/
        bool findPlayer(Glib::ustring, Glib::ustring);

        /**Start a new game using the entered data.*/
        void startNewGame();

        /**Load a game using the selected file.*/
        void loadGame();

        /**Build the player list object from */
        void buildPlayerListObject();

        /**Refresh the interface buttons based on the selection.*/
        void playerListRefresh();

        /**Removes the selected player.*/
        void removePlayer();
};

#endif // SETUPWINDOW_H
