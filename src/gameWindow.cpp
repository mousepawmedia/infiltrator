#include "gameWindow.hpp"

gameWindow::gameWindow(AgentDatabase* adb)
{
    db = adb;

    //Set the default size.
    set_default_size(500, 600);
    //Let's also give our window a nicer title.
    set_title("Infiltrator");

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

    //Add the title label...

    //We are going to use Pango to make it really fancy.
    lbl_title.set_use_markup(true);
    //Center the text.
    lbl_title.set_justify(Gtk::JUSTIFY_CENTER);
    /*The title "INFILTRATOR" should be large and bold. Make the A stand out
    by making it red and raising it above the rest, thus representing the
    infiltrator player. In the subtitle, make convert italic.*/
    lbl_title.set_markup("<big><b>INFILTR\
<span foreground=\"red\" rise=\"5000\">A</span>TOR</b></big>\
\nThe Game of <i>Covert</i> Operations");

    //Add the label to the box.
    box_main.pack_start(lbl_title, Gtk::PACK_SHRINK);

    //Add the login controls grid to the box.
    box_main.pack_start(grd_login, Gtk::PACK_SHRINK);

    //Define the agent ID prompt label.
    lbl_id.set_text("Agent ID: ");
    //Add the agent ID prompt label to the grid.
    grd_login.attach(lbl_id, 0, 0, 1, 1);

    //Since we never have more than 99 players, limit id to 2 chars.
    txt_id.set_max_length(2);
    //When we hit enter, run the login function (just like if we clicked LOGIN).
    txt_id.signal_activate().connect(sigc::mem_fun(this, &gameWindow::login));
    //Add the agent ID input box to the grid.
    grd_login.attach_next_to(txt_id, lbl_id, Gtk::POS_RIGHT, 1, 1);

    //Define the security code prompt label.
    lbl_security.set_text("Security Code: ");
    //Add the security code prompt label to the grid.
    grd_login.attach(lbl_security, 0, 1, 1, 1);

    //Hide the security code for data entry.
    txt_security.set_visibility(false);
    //When we hit enter, run the login function (just like if we clicked LOGIN).
    txt_security.signal_activate().connect(sigc::mem_fun(this, &gameWindow::login));
    //Add the security code input box to the grid.
    grd_login.attach_next_to(txt_security, lbl_security, Gtk::POS_RIGHT, 1, 1);

    //Define the login message label.
    lbl_login_msg.set_text("");
    //Add the login message label to the grid.
    grd_login.attach(lbl_login_msg, 0, 2, 2, 1);

    //Define the login button.
    btn_login.set_label("Login");
    //Connect the login button's clicked signal to the login command.
    btn_login.signal_clicked().connect(sigc::mem_fun(this, &gameWindow::login));
    //Add the login button to the grid.
    grd_login.attach_next_to(btn_login, lbl_login_msg, Gtk::POS_RIGHT, 1, 1);

    /*
    //NOTE: Temporary override for testing ONLY!
    txt_id.set_text("1");
    txt_security.set_text(AgentDatabase::int_to_ustring(db->agents[0].securitycode));
    login();*/
    //END OF OVERRIDE

    //Let's use Pango for the game status header label as well.
    lbl_scores_header.set_use_markup(true);
    /*Define the game status header label. Make sure the As are always red and
    raised above the rest of the text. This should NOT be as big as the main
    game header, and the rise should not be AS pronounced as such.*/
    lbl_scores_header.set_markup("<b>G\
<span foreground=\"red\" rise=\"3000\">A</span>ME ST\
<span foreground=\"red\" rise=\"3000\">A</span>TUS...</b>");
    //Center the game status (scores) header label.
    lbl_scores_header.set_justify(Gtk::JUSTIFY_CENTER);
    //Add the game status label to the main box.
    box_main.pack_start(lbl_scores_header, Gtk::PACK_SHRINK);

    //Add the scores list to the scrollable box for it.
    scrl_lst_scores.add(lst_scores);
    //Add the scrollable box for the scores list to the main box.
    box_main.pack_start(scrl_lst_scores);

    //Define the agent list and set its model.
    ref_mdl_scores = Gtk::ListStore::create(mdl_col_score);
    lst_scores.set_model(ref_mdl_scores);

    //The agent list should WANT to resize horizontally...
    lst_scores.set_hexpand(true);
    //...and vertically.
    lst_scores.set_vexpand(true);

    //Append the status column...
    lst_scores.append_column("", mdl_col_score.col_status);
    //...the rank column...
    lst_scores.append_column("Rank", mdl_col_score.col_rank);
    //...the last name column...
    lst_scores.append_column("Last", mdl_col_score.col_last);
    //..first name column...
    lst_scores.append_column("First", mdl_col_score.col_first);
    //...and title column.
    lst_scores.append_column("Title", mdl_col_score.col_title);

    //Populate the list.
    addScores();

    //Sort by last name initially to mask the agent IDs.
    ref_mdl_scores->set_sort_column(mdl_col_score.col_last, Gtk::SORT_ASCENDING);
    //...then by status...
    ref_mdl_scores->set_sort_column(mdl_col_score.col_status, Gtk::SORT_ASCENDING);
    //...and then sort by rank from here on out.
    ref_mdl_scores->set_sort_column(mdl_col_score.col_rank, Gtk::SORT_DESCENDING);

    //Show everything on this window.
    show_all_children(true);

}

void gameWindow::addScores()
{
    Gtk::TreeModel::Row row;

    //For each agent in the database...
    for(unsigned int a = 0; a < db->agents.size(); a++)
    {
        //Define a new row in the scores model.
        row = *(ref_mdl_scores->append());
        //Add the agent ID...
        row[mdl_col_score.col_id] = db->agents[a].id;
        //...the first name...
        row[mdl_col_score.col_first] = db->agents[a].firstName;
        //...and the last name...
        row[mdl_col_score.col_last] = db->agents[a].lastName;
    }
    //Instead of duplicating the rest of the code, just call updateScores().
    updateScores();
}

void gameWindow::login()
{
    /*Fetch and convert the contents of the id text entry to an integer.
    This is the agent id.*/
    unsigned int id = AgentDatabase::ustring_to_int(txt_id.get_text());

    /*Fetch and convert the contents for the security code text entry to an
    integer. This is the agent's security code.*/
    unsigned int sc = AgentDatabase::ustring_to_int(txt_security.get_text());

    /*Try to log in using the id and security code.
    If successful (returned true)...*/
    if(db->checkLogin(id, sc))
    {
        /*Define a new agent window, providing the database pointer and the id
        number.*/
        agentWin = new agentWindow(db, id);

        //Clear the login message, since we were successful!
        lbl_login_msg.set_text("");

        //Clear the id text entry, so as to not give anyone any clues...
        txt_id.set_text("");
        //...and the security text entry, for security reasons.
        txt_security.set_text("");

        /*The new agent window should be modal, meaning we can't use this
        while it is open.*/
        agentWin->set_modal(true);
        //Keep the new agent window on top.
        agentWin->set_transient_for(*this);
        /*Connect the signal that the agent window closed to the winClosed
        function, so we can perform essential tasks and keep the game going!*/
        agentWin->signal_delete_event().connect(sigc::mem_fun(this, &gameWindow::winClosed));
        //Now we're ready...show the agent window!
        agentWin->show();
    }
    //Else, if login was not successful (returned false)...
    else
    {
        //Tell the agent that it was not successful and let them try again.
        lbl_login_msg.set_text("Invalid login.");

        /*We don't clear the text entries here, so they can potentially fix
        a mistake.*/
    }
}

void gameWindow::updateScores()
{
    //Declare a typedef for this bit, to minimize code length.
    typedef Gtk::TreeModel::Children type_children;

    //Get the children (elements) of the scores list model.
    type_children children = ref_mdl_scores->children();

    //For each child (element)...
    for(type_children::iterator iter = children.begin();
        iter != children.end(); ++iter)
    {
        //Get the row using the iterator.
        Gtk::TreeModel::Row row = *iter;
        //Get the id from the row and store it separately, to save time/CPU.
        int id = row[mdl_col_score.col_id];

        //Update rank (connection count).
        row[mdl_col_score.col_rank] = db->agents[id-1].connections.size();

        //If the agent was defined in the database as the spycatcher...
        if(id == db->spycatcher)
        {
            //If they were the infiltrator...
            if(db->agents[id-1].infiltrator)
            {
                //They must have knocked out everyone else. Indicate that.
                row[mdl_col_score.col_title] = "Spycatcher/Infiltrator";
            }
            //Otherwise, if they are NOT the infiltrator...
            else
            {
                //They caught the infiltrator and earned the title. Show that.
                row[mdl_col_score.col_title] = "Spycatcher";
            }
        }
        /*Otherwise, if there is a spycatcher (thus, the infiltrator was caught),
        and the agent is the infiltrator.*/
        else if(db->spycatcher > 0 && db->agents[id-1].infiltrator)
        {
            //The player is the infiltrator, and has been caught. Show that.
            row[mdl_col_score.col_title] = "Infiltrator (CAUGHT)";
        }

        //If the agent is decommissioned or cloaked...
        if(db->agents[id-1].active == false || db->agents[id-1].cloaked)
        {
            //Decommissioned. Use UTF-8 "Ballot X" U+2717.
            row[mdl_col_score.col_status] = "✗";
        }
        //Otherwise, if the agent is active...
        else
        {
            //Active. Use UTF-8 "Check Mark" U+2713.
            row[mdl_col_score.col_status] = "✓";
        }
    }
}

bool gameWindow::winClosed(GdkEventAny* event)
{
    //If we have an agent window defined (the pointer is not null).
    if(agentWin != 0)
    {
        //Delete the dynamically allocated window object.
        delete agentWin;
    }

    //Update the scores.
    updateScores();

    //Do NOT prevent other handlers from working with the window's delete_event.
    return false;
}

gameWindow::~gameWindow()
{
    //dtor
}
