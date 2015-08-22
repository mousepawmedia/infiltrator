#include "primaryWindow.hpp"

primaryWindow::primaryWindow()
{
    //Start by nulling our pointers.
    db = 0;
    fe = 0;

    //Set the default size.
    set_default_size(500, 600);
    //Let's also give our window a nicer title.
    set_title("Infiltrator: The Game of Covert Operations");

    box_main.set_orientation(Gtk::ORIENTATION_VERTICAL);
    add(box_main);

    box_main.pack_start(lbl_title);

    btn_startgame.set_label("Create or Load Game");
    btn_startgame.signal_clicked().connect(sigc::mem_fun(this,
        &primaryWindow::showSetupScreen));
    btn_startgame.set_hexpand(false);
    btn_startgame.set_vexpand(false);
    box_main.pack_start(btn_startgame, Gtk::PACK_EXPAND_PADDING);

    show_all_children(true);
}

void primaryWindow::showSetupScreen()
{
    //Show the setup screen.
    win_setup = new setupWindow(&db, &fe, &newGame);
    win_setup->set_transient_for(*this);
    win_setup->set_modal();
    win_setup->signal_delete_event().connect(sigc::mem_fun(this,
        &primaryWindow::loadGame));
    win_setup->show();
}

bool primaryWindow::infoDone(GdkEventAny* event)
{
    if(win_info != 0)
    {
        //Delete the setup window instance.
        delete win_info;
        win_info = 0;
    }
    playGame();
    return false;
}

bool primaryWindow::loadGame(GdkEventAny* event)
{
    if(newGame && db != 0)
    {
        win_info = new infoWindow(db);
        win_info->set_transient_for(*this);
        win_info->set_modal();
        win_info->signal_delete_event().connect(sigc::mem_fun(this,
            &primaryWindow::infoDone));
        win_info->show();
    }
    else
    {
        playGame();
    }

    //We must allow the signal to propagate further, so return `false`.
    return false;
}

void primaryWindow::playGame()
{
    //If the setup window is currently defined...
    if(win_setup != 0)
    {
        //Delete the setup window instance.
        delete win_setup;
    }
    //If the database and filepath are defined (game data defined)...
    if(db != 0 && fe != 0)
    {
        //NOTE: Turn this off before release!
        db->printGameData();

        //Create a new game window with the database.
        win_game = new gameWindow(db, fe);
        //Make this modal and transient over the main window.
        win_game->set_transient_for(*this);
        win_game->set_modal();
        //Listen for when we close the game window.
        win_game->signal_delete_event().connect(sigc::mem_fun(this,
            &primaryWindow::unloadGame));

        //Deactivate the Start Game button.
        btn_startgame.set_sensitive(false);

        //Show the game window.
        win_game->show();
    }
}

bool primaryWindow::unloadGame(GdkEventAny* event)
{
    //Delete the agent database and the file encryption object.
    delete db;
    db = 0;
    delete fe;
    fe = 0;

    //Activate the Start Game button.
    btn_startgame.set_sensitive(true);

    //We must allow the signal to propagate further, so return `false`.
    return false;
}

primaryWindow::~primaryWindow()
{
    //Destructor
}
