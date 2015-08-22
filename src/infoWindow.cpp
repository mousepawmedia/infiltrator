#include "infoWindow.hpp"

infoWindow::infoWindow(AgentDatabase* adb)
{
    /*Fought with a segfault for an hour and a half, and then I realized
    that I had forgotten this essential little piece of code. Umm...yeah. -JCM*/
    db = adb;

    //Set the default size.
    set_default_size(500, 600);
    //Let's also give our window a nicer title.
    set_title("New Game Info");

    grd_main.set_column_spacing(10);
    grd_main.set_row_spacing(10);
    add(grd_main);

    signal_delete_event().connect(sigc::mem_fun(this, &infoWindow::winClose));
    //Add top label.
    lbl_info1.set_text("Write down the following information. Give each player \
his/her ID and security code. Write the gamemaster code down in a secure \
location. Do NOT give it out or lose it!");
    lbl_info1.set_line_wrap();
    grd_main.attach(lbl_info1, 0, 0, 2, 1);

    //Display gamemaster code.
    int intcode = db->getGamemasterCode();
    Glib::ustring gamemastercode = AgentDatabase::int_to_ustring(intcode);
    lbl_code.set_markup("<b>Gamemaster Code:</b> " + gamemastercode);
    grd_main.attach(lbl_code, 0, 1, 2, 1);

    //Add the scores list to the scrollable box for it.
    scrl_lst_agents.add(lst_agents);
    //Add the scrollable box for the scores list to the main box.
    grd_main.attach(scrl_lst_agents, 0, 2, 2, 4);

    //Define the agent list and set its model.
    ref_mdl_agents = Gtk::ListStore::create(mdl_col_agents);
    lst_agents.set_model(ref_mdl_agents);

    //The agent list should WANT to resize horizontally...
    lst_agents.set_hexpand(true);
    //...and vertically.
    lst_agents.set_vexpand(true);

    //Append the status column...
    lst_agents.append_column("ID", mdl_col_agents.col_id);
    //...the last name column...
    lst_agents.append_column("Last", mdl_col_agents.col_last);
    //..first name column...
    lst_agents.append_column("First", mdl_col_agents.col_first);
    //...and security column.
    lst_agents.append_column("Security", mdl_col_agents.col_security);

    //Populate the list.
    addAgents();

    lbl_info2.set_markup("Make sure you have <b>written down all this \
information!</b> You cannot get it back later. When you're ready, enter \
the gamemaster code in the box and click <u>Continue</u> to proceed.");
    lbl_info2.set_line_wrap();
    grd_main.attach(lbl_info2, 0, 6, 2, 1);

    txt_code.set_placeholder_text("Enter Gamemaster Code");
    txt_code.signal_changed().connect(sigc::mem_fun(this, &infoWindow::typeCode));
    grd_main.attach(txt_code, 0, 7, 1, 1);

    btn_done.set_sensitive(false);
    btn_done.set_label("Continue");
    btn_done.signal_clicked().connect(sigc::mem_fun(this, &infoWindow::clickContinue));
    grd_main.attach(btn_done, 1, 7, 1, 1);

    show_all_children();
}

void infoWindow::addAgents()
{
    Gtk::TreeModel::Row row;
    for(unsigned int a = 0; a < db->agents.size(); a++)
    {
        row = *(ref_mdl_agents->append());
        row[mdl_col_agents.col_id] = db->agents[a].id;
        row[mdl_col_agents.col_security] = db->agents[a].securitycode;
        row[mdl_col_agents.col_first] = db->agents[a].firstName;
        row[mdl_col_agents.col_last] = db->agents[a].lastName;
    }
}

void infoWindow::typeCode()
{
    if(AgentDatabase::ustring_to_int(txt_code.get_text()) == db->keygen->gamemasterCode)
    {
        btn_done.set_sensitive(true);
    }
}

void infoWindow::clickContinue()
{
    allowClose = true;
    close();
}

bool infoWindow::winClose(GdkEventAny* event)
{
    /*Return the opposite of allowClose (if true, return false to close; if
    false, return true to prevent close.)*/
    return !allowClose;
}
