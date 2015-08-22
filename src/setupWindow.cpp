#include "setupWindow.hpp"

setupWindow::setupWindow(AgentDatabase** adb, fileEncryption** afe, bool* newGame)
{
    //Store the database pointer.
    db = adb;
    //Store the file read/write object pointer.
    fe = afe;

    newGameFlag = newGame;

    //Set the default size.
    set_default_size(500, 600);
    //Let's also give our window a nicer title.
    set_title("Game Setup");

    //This first box should be vertical.
    box_main.set_orientation(Gtk::ORIENTATION_VERTICAL);

    //Set the margins to give some space in the interface.
    box_main.set_margin_top(30);
    box_main.set_margin_left(10);
    box_main.set_margin_right(10);
    box_main.set_margin_bottom(10);

    //Space out the widgets in the box.
    box_main.set_spacing(20);

    //Add the box to the window.
    add(box_main);

    //The title label (lbl_title) was already set up in customWidgets

    //Add the label to the box.
    box_main.pack_start(lbl_title, Gtk::PACK_SHRINK);

    //Add the three grids to the notebook, and create their respective tabs.
    nbk_main.append_page(grd_new, "New Game");
    nbk_main.append_page(grd_load, "Load Game");
    nbk_main.append_page(nbk_rules, "About");

    //Add the notebook to the box.
    box_main.pack_start(nbk_main, Gtk::PACK_EXPAND_WIDGET);

    //Set up the "New Game" tab.
    grd_new.set_column_spacing(5);
    grd_new.set_row_spacing(5);
    //grd_new.set_row_homogeneous(true);

    //Save path text field and selector button.
    txt_savepath.set_sensitive(false);
    grd_new.attach(txt_savepath, 0, 0, 4, 1);
    btn_save.set_label("Save As...");
    btn_save.set_vexpand(false);
    btn_save.signal_clicked().connect(sigc::mem_fun(this, &setupWindow::browseSave));
    grd_new.attach(btn_save, 4, 0, 1, 1);

    //Add the scores list to the scrollable box for it.
    scrl_lst_players.add(lst_players);
    //Add the scrollable box for the scores list to the main box.
    grd_new.attach(scrl_lst_players, 0, 2, 5, 1);

    //Define the players list and set its model.
    ref_mdl_players = Gtk::ListStore::create(mdl_col_players);
    lst_players.set_model(ref_mdl_players);

    //The player list should WANT to resize horizontally...
    lst_players.set_hexpand(true);
    //...and vertically...
    lst_players.set_vexpand(true);

    //Append the last name column...
    lst_players.append_column("Last", mdl_col_players.col_last);
    //...and the first name column.
    lst_players.append_column("First", mdl_col_players.col_first);

    //Store the reference to the agent list's selection object for later use.
    ref_sel_players = lst_players.get_selection();

    //Sort by last name.
    ref_mdl_players->set_sort_column(mdl_col_players.col_last, Gtk::SORT_ASCENDING);

    //Set up signal for when the selection is changed.
    ref_sel_players->signal_changed().connect(sigc::mem_fun(this,
        &setupWindow::playerListRefresh));

    //Remove Player button...
    btn_remove.set_label("Remove Player");
    btn_remove.set_vexpand(false);
    //This should be deactivated by default, and activated when a row is selected.
    btn_remove.set_sensitive(false);
    btn_remove.signal_clicked().connect(sigc::mem_fun(this, &setupWindow::removePlayer));
    //This should appear towards the right of its own row.
    grd_new.attach(btn_remove, 4, 3, 1, 1);

    //First/Last fields and Add Player button.
    txt_firstname.set_placeholder_text("First name");
    grd_new.attach(txt_firstname, 0, 4, 2, 1);
    txt_lastname.set_placeholder_text("Last name");
    grd_new.attach_next_to(txt_lastname, txt_firstname, Gtk::POS_RIGHT, 2, 1);
    btn_add.set_label("Add Player");
    btn_add.set_vexpand(false);
    btn_add.signal_clicked().connect(sigc::mem_fun(this, &setupWindow::addPlayer));
    grd_new.attach_next_to(btn_add, txt_lastname, Gtk::POS_RIGHT, 1, 1);

    lbl_intercept.set_text("Intercept (sec): ");
    grd_new.attach(lbl_intercept, 0, 5, 1, 1);

    //Intercept time field. (Default 2 min)
    //Increment by 1 second per tick, or 10 per page.
    spn_intercept.set_increments(1, 10);
    //Integers only, no decimals.
    spn_intercept.set_digits(0);
    //The value must be between 15 seconds and 6 hours.
    spn_intercept.set_range(15, 21600);
    //The default is 2 minutes.
    spn_intercept.set_value(120);
    //Attach it!
    grd_new.attach_next_to(spn_intercept, lbl_intercept, Gtk::POS_RIGHT, 1, 1);

    lbl_encrypt.set_text("Encrypt (sec): ");
    grd_new.attach_next_to(lbl_encrypt, spn_intercept, Gtk::POS_RIGHT, 1, 1);

    //Encrypt time field. (Default 2 min)
    //Increment by 1 second per tick, or 10 per page.
    spn_encrypt.set_increments(1, 10);
    //Integers only, no decimals.
    spn_encrypt.set_digits(0);
    //The value must be between 15 seconds and 5 minutes.
    spn_encrypt.set_range(15, 300);
    //The default is 2 minutes.
    spn_encrypt.set_value(120);
    //Attach it!
    grd_new.attach_next_to(spn_encrypt, lbl_encrypt, Gtk::POS_RIGHT, 1, 1);

    //We need a label primarily for spacing.
    lbl_msgnew.set_text("\nThe game is afoot...");
    grd_new.attach(lbl_msgnew, 1, 6, 3, 1);

    //Start game button.
    btn_startnew.set_label("START NEW GAME");
    btn_startnew.set_vexpand(false);

    btn_startnew.signal_clicked().connect(sigc::mem_fun(this, &setupWindow::startNewGame));
    //This should be extra large, and towards the right of its own row.
    grd_new.attach(btn_startnew, 1, 7, 3, 1);

    //Set up the "Load Game" tab.
    grd_load.set_column_spacing(5);
    grd_load.set_row_spacing(5);

    //Load path text field and selector button.
    txt_loadpath.set_sensitive(false);
    txt_loadpath.set_hexpand(true);
    grd_load.attach(txt_loadpath, 0, 0, 4, 1);
    btn_load.set_label("Browse...");
    btn_load.set_vexpand(false);
    btn_load.signal_clicked().connect(sigc::mem_fun(this, &setupWindow::browseLoad));
    grd_load.attach(btn_load, 4, 0, 1, 1);

    //We need a label primarily for spacing.
    lbl_msgload.set_text("\nOnce more into the breach, dear friends...");
    grd_load.attach(lbl_msgload, 1, 6, 3, 1);

    //Load game button.
    btn_startload.set_label("RESUME GAME");
    btn_startload.set_vexpand(false);
    btn_startload.signal_clicked().connect(sigc::mem_fun(this, &setupWindow::loadGame));
    //This should be extra large, and towards the right of its own row.
    grd_load.attach(btn_startload, 1, 7, 3, 1);

    //Show everything on this window.
    show_all_children(true);
}

void setupWindow::addPlayer()
{
    //Create the variable to hold the row.
    Gtk::TreeModel::Row row;

    //If we have don't have BOTH a first name and last name
    if(txt_firstname.get_text().length() <= 0 || \
       txt_lastname.get_text().length() <= 0)
    {
        Gtk::MessageDialog dlg_name_incomplete(*this, "Duplicate Player",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_name_incomplete.set_secondary_text("You must enter both a first \
name and a last name for each player.");
        dlg_name_incomplete.run();
    }
    //Else if the new entry is a duplicate of an existing entry...
    else if(findPlayer(txt_firstname.get_text(), txt_lastname.get_text()))
    {
        Gtk::MessageDialog dlg_name_repeat(*this, "Incomplete Name",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_name_repeat.set_secondary_text("You already added this player to. \
the list. Each player name needs to be different.");
        dlg_name_repeat.run();
    }
    //Otherwise, we're safe to proceed...
    else
    {
        //Define a new row in the players model.
        row = *(ref_mdl_players->append());
        //Add the player's first name...
        row[mdl_col_players.col_first] = txt_firstname.get_text();
        //...and the last name...
        row[mdl_col_players.col_last] = txt_lastname.get_text();
    }

    //Rebuild the player list.
    flag_plstbuild = true;
}

void setupWindow::browseLoad()
{
    txt_loadpath.set_text(dlgBrowseLoad());
}

void setupWindow::browseSave()
{
    txt_savepath.set_text(dlgBrowseSave());
}

Glib::ustring setupWindow::dlgBrowseLoad()
{
    Glib::ustring path = "";
    Gtk::FileChooserDialog dlg_loadpath("Select game file.",
        Gtk::FILE_CHOOSER_ACTION_OPEN);
    dlg_loadpath.set_transient_for(*this);
    dlg_loadpath.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dlg_loadpath.add_button("Select", Gtk::RESPONSE_OK);

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Infiltrator Games");
    filter_text->add_pattern("*.agents");

    dlg_loadpath.add_filter(filter_text);

    int result = dlg_loadpath.run();
    switch (result)
    {
        case Gtk::RESPONSE_OK:
        {
            path = dlg_loadpath.get_filename();
            break;
        }
        default:
        {
            path = "";
            break;
        }
        return path;
    }
}

Glib::ustring setupWindow::dlgBrowseSave()
{
    Glib::ustring path = "";
    Gtk::FileChooserDialog dlg_savepath("Select save file.",
        Gtk::FILE_CHOOSER_ACTION_SAVE);
    dlg_savepath.set_transient_for(*this);
    dlg_savepath.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dlg_savepath.add_button("Select", Gtk::RESPONSE_OK);
    dlg_savepath.set_current_name("UntitledGame.agents");

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Infiltrator Games");
    filter_text->add_pattern("*.agents");

    dlg_savepath.add_filter(filter_text);

    int result = dlg_savepath.run();
    switch (result)
    {
        case Gtk::RESPONSE_OK:
        {
            path = dlg_savepath.get_filename();
            break;
        }
        default:
        {
            path = "";
            break;
        }
    }
    return path;
}

bool setupWindow::findPlayer(Glib::ustring firstname, Glib::ustring lastname)
{
    //Create the variable to hold the row.
    Gtk::TreeModel::Row row;
    //Typedef to minimize code length.
    typedef Gtk::TreeModel::Children type_children;
    //Iterate through the data model...
    type_children children = ref_mdl_players->children();
    for(type_children::iterator iter = children.begin();
        iter != children.end(); ++iter)
    {
        row = *iter;
        //If we have an exact match...
        if(row[mdl_col_players.col_first] == firstname && \
           row[mdl_col_players.col_last] == lastname)
        {
            //Return true.
            return true;
        }
    }
    //If we didn't get an exact match, return false.
    return false;
}

void setupWindow::playerListRefresh()
{
    Gtk::TreeModel::iterator iter = ref_sel_players->get_selected();
    //If something is selected...
    if(iter)
    {
        //Activate the "remove player" button.
        btn_remove.set_sensitive(true);
    }
    //Otherwise...
    else
    {
        //Deactivate the "remove player" button.
        btn_remove.set_sensitive(false);
    }
}

void setupWindow::removePlayer()
{
    Gtk::TreeModel::iterator iter = ref_sel_players->get_selected();
    //If something is selected...
    if(iter)
    {
        //Delete that something (a player/row) from the list.
        ref_mdl_players->erase(iter);
    }
    //Rebuild the player list.
    flag_plstbuild = true;
}

void setupWindow::buildPlayerListObject()
{
    //If we aren't supposed to rebuilt the plst...
    if(!flag_plstbuild)
    {
        //Drop everything NOW.
        return;
    }

    //Reset the flag.
    flag_plstbuild = false;

    if(plst != 0)
    {
        //We delete the existing PlayerList...
        delete plst;
        /*...AND THEN WE SET THE POINTER TO NULL! Otherwise, we get an uber-pesky
        Heisenbug in which some (but not all) runs of "delete plst" trigger a
        SEGFAULT, due to the attempted deletion of an invalid memory address.
        No one knows why, so we do it this way to save ourselves a lot of
        misery. http://stackoverflow.com/a/381749/472647*/
        plst = 0;
    }

    //If we have a null pointer (no current PlayerList defined herein)...
    if(plst == 0)
    {
        //Allocate a new PlayerList.
        plst = new PlayerList;
    }

    //Create the variable to hold the row.
    Gtk::TreeModel::Row row;
    //Typedef to minimize code length.
    typedef Gtk::TreeModel::Children type_children;
    //Iterate through the data model. For each row..
    type_children children = ref_mdl_players->children();
    for(type_children::iterator iter = children.begin();
        iter != children.end(); ++iter)
    {
        row = *iter;
        //Transfer the data to the PlayerList object.
        plst->addPlayer(row[mdl_col_players.col_last],
                       row[mdl_col_players.col_first]);
    }
}

void setupWindow::startNewGame()
{
    //Convert the list of players to the PlayerList object.
    buildPlayerListObject();
    //If < 4 players, error.
    if(plst->getCount() < 4)
    {
        flag_plstbuild = true;
        //Throw an error message.

        /*Technically, there is no upper limit on the number of players. Imagine
        this game at a corporate retreat or convention, lasting several days!*/
        Gtk::MessageDialog dlg_game_toofew(*this, "Too Few Players!",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_game_toofew.set_secondary_text("You need at least four players \
for <i>Infiltrator</i>, and the more the merrier!");
        dlg_game_toofew.run();
    }
    //Else if the save path is undefined...
    else if(txt_savepath.get_text().length() <= 0)
    {
        Gtk::MessageDialog dlg_game_nosavepath(*this, "No Save Path",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_game_nosavepath.set_secondary_text("You must define a file location \
to save your game!");
        dlg_game_nosavepath.run();
    }
    //Otherwise...
    else
    {
        /*Get the intercept and encrypt default time values from the interface
        and convert them to integers.*/
        int intercept = static_cast<int>(spn_intercept.get_value());
        int encrypt = static_cast<int>(spn_encrypt.get_value());

        //Create a new database at the db pointer we stored earlier.
        *db = new AgentDatabase(plst, intercept, encrypt);
        //Save the filepath.
        filepath = txt_savepath.get_text();
        //Create the file read/write object at the fe pointer we stored earlier.
        *fe = new fileEncryption(filepath);

        *newGameFlag = true;

        //Delete the PlayerList object.
        delete plst;
        /*I'm just doing this to prevent an impossible repeat of the Heisenbug
        I described up in "buildPlayerListObject()". Don't mind me.*/
        plst = 0;

        //Close window.
        close();
    }
}

void setupWindow::loadGame()
{
    if(txt_loadpath.get_text().length() <= 0)
    {
        Gtk::MessageDialog dlg_game_noloadpath(*this, "No Game Selected",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_game_noloadpath.set_secondary_text("You must select a game file \
to load from.");
        dlg_game_noloadpath.run();
    }
    //Otherwise...
    else
    {
        //Save the filepath.
        filepath = txt_loadpath.get_text();
        //Create the file read/write object at the fe pointer we stored earlier.
        *fe = new fileEncryption(filepath);
        //Create a new database at the db pointer we stored earlier.
        *db = new AgentDatabase;
        *newGameFlag = false;
        //Load into the database.
        if((*fe)->readFile(*db))
        {
            //We were successful.
            //Close the setup window.
            close();
        }
        else
        {
            //Throw an error.
            Gtk::MessageDialog dlg_loaderror(*this, "Error Loading Game",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            dlg_loaderror.set_secondary_text((*fe)->getError());
            dlg_loaderror.run();
        }


    }
}

setupWindow::~setupWindow()
{
    //dtor
}
