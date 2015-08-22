#include "agentWindow.hpp"

agentWindow::agentWindow(AgentDatabase* adb, int id)
{
    //Store pointer and id.
    db = adb;
    agentID = id;

    //Set the default size.
    set_default_size(700, 300);
    //Let's also give our window a nicer title.
    set_title("AGENT " + db->int_to_ustring(id));

    box_main.set_orientation(Gtk::ORIENTATION_VERTICAL);
    add(box_main);

    box_status.set_hexpand(false);
    box_status.set_spacing(30);
    box_status.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box_main.pack_start(box_status, Gtk::PACK_SHRINK);
    box_main.set_spacing(10);

    //Define the status labels.
    lbl_agentID.set_text("ID: " + AgentDatabase::int_to_ustring(id));
    lbl_agentName.set_text(db->agents[id-1].lastName + ", " +
                           db->agents[id-1].firstName);
    lbl_agentTeam.set_text(db->agents[id-1].team->teamName);
    lbl_agentSecurity.set_text(AgentDatabase::int_to_ustring
                              (db->agents[id-1].securitycode));
    lbl_overrides.set_text("Overrides: " + AgentDatabase::int_to_ustring
                                (db->agents[id-1].overrides));

    //Add the labels to box_status.
    box_status.pack_start(lbl_agentID, Gtk::PACK_SHRINK);
    box_status.pack_start(lbl_agentName, Gtk::PACK_SHRINK);
    box_status.pack_start(lbl_agentTeam, Gtk::PACK_SHRINK);
    box_status.pack_start(lbl_agentSecurity, Gtk::PACK_SHRINK);
    box_status.pack_start(lbl_overrides, Gtk::PACK_SHRINK);

    //Define button for getting a new security code.
    btn_newSecurity.set_label("New Security Code");
    //Attach the signal.
    btn_newSecurity.signal_clicked().connect(sigc::mem_fun
        (this, &agentWindow::actionNewSecurity));
    //Add the button to the status bar.
    box_status.pack_start(btn_newSecurity, Gtk::PACK_SHRINK);

    box_statusB.set_hexpand(false);
    box_statusB.set_spacing(30);
    box_statusB.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box_main.pack_start(box_statusB, Gtk::PACK_SHRINK);

    //Define encrypts left label.
    lbl_agentEncryptTime.set_text("Encrypt Time Left: " +
        AgentDatabase::int_to_ustring(db->agents[id-1].encryptSeconds) +
        "sec");
    //Add to status B box.
    box_statusB.pack_start(lbl_agentEncryptTime, Gtk::PACK_SHRINK);

    //Define intercept time label.
    lbl_agentInterceptTime.set_text("Intercept Time Left: " +
        AgentDatabase::int_to_ustring(db->agents[id-1].tapSeconds) +
        "sec");
    //Add to status B box.
    box_statusB.pack_start(lbl_agentInterceptTime, Gtk::PACK_SHRINK);

    if(db->agents[id-1].infiltrator)
    {
        lbl_agentInfiltrator.set_text("You're the infiltrator.");
        box_statusB.pack_start(lbl_agentInfiltrator, Gtk::PACK_SHRINK);
    }

    box_lr.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box_lr.set_spacing(30);
    box_main.pack_start(box_lr);;

    box_lr.pack_start(grd_controls, Gtk::PACK_EXPAND_WIDGET);
    grd_controls.set_row_spacing(10);

    //Add the text entry box with its default text...
    txt_redeemCode.set_text("Enter code here.");
    txt_redeemCode.set_hexpand(false);
    txt_redeemCode.set_vexpand(false);
    grd_controls.attach(txt_redeemCode, 0, 0, 1, 1);

    //...and the submit button thereof.
    btn_submitCode.set_label("Submit Code");
    btn_submitCode.set_hexpand(false);
    txt_redeemCode.set_vexpand(false);
    btn_submitCode.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::redeemCode));
    grd_controls.attach_next_to(btn_submitCode, txt_redeemCode, Gtk::POS_RIGHT, 1, 1);

    //Create the connection count label.
    lbl_connections.set_text("Connections: " +
        AgentDatabase::int_to_ustring(db->agents[id-1].connections.size()) +
        " of " + AgentDatabase::int_to_ustring(db->agents.size()));
    grd_controls.attach(lbl_connections, 0, 1, 3, 1);

    //Add the scrollable box for the agent list.
    grd_controls.attach(scrl_lst_agents, 0, 2, 2, 7);
    scrl_lst_agents.add(lst_agents);

    //Define the agent list.
    ref_mdl_agents = Gtk::ListStore::create(mdl_col_agents);
    lst_agents.set_model(ref_mdl_agents);

    //This should WANT to resize horizontally...
    lst_agents.set_hexpand(true);
    //...and vertically.
    lst_agents.set_vexpand(true);

    //Store the reference to the agent list's selection object for later use.
    ref_sel_agents = lst_agents.get_selection();

    //Set up signal for when the selection is changed.
    ref_sel_agents->signal_changed().connect(sigc::mem_fun(this,
        &agentWindow::refreshButtons));

    //Append the status column to the agent list.
    lst_agents.append_column("S", mdl_col_agents.col_status);
    //Make the column sortable.
    lst_agents.get_column(0)->set_sort_column(mdl_col_agents.col_status);

    //Repeat for the last name column...
    lst_agents.append_column("Last", mdl_col_agents.col_last);
    lst_agents.get_column(1)->set_sort_column(mdl_col_agents.col_last);

    //..first name column...
    lst_agents.append_column("First", mdl_col_agents.col_first);
    lst_agents.get_column(2)->set_sort_column(mdl_col_agents.col_first);

    //...and connection column.
    lst_agents.append_column("Connection", mdl_col_agents.col_conn);
    lst_agents.get_column(3)->set_sort_column(mdl_col_agents.col_conn);

    /*Set default sort column to sort by last name, NOT id.
    (No sense in giving any clues for hacking others' accounts.)*/
    ref_mdl_agents->set_sort_column(mdl_col_agents.col_last, Gtk::SORT_ASCENDING);

    //Populate the list.
    addAgents();

    //If the agent is the infiltrator...
    if(db->agents[id-1].infiltrator)
    {
        //Top button should allow decommissioning marked agents.
        btn_decomm.set_label("< Decommission");
        //Deactive by default.
        btn_decomm.set_sensitive(false);
        btn_decomm.set_hexpand(false);
        btn_decomm.set_vexpand(false);
        btn_decomm.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::actionDecomm));
        grd_controls.attach(btn_decomm, 2, 2, 1, 1);
    }
    //If the agent is NOT the infiltrator...
    else
    {
        //Top button should allow accusing an agent of being the infiltrator.
        btn_accuse.set_label("< Accuse");
        //Deactive by default.
        btn_accuse.set_sensitive(false);
        btn_accuse.set_hexpand(false);
        btn_accuse.set_vexpand(false);
        btn_accuse.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::actionAccuse));
        grd_controls.attach(btn_accuse, 2, 2, 1, 1);
    }

    //Define, add, and deactivate the button to place an intercept on an agent.
    btn_intercept.set_label("< Intercept");
    btn_intercept.set_sensitive(false);
    btn_intercept.set_hexpand(false);
    btn_intercept.set_vexpand(false);
    btn_intercept.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::actionIntercept));
    grd_controls.attach(btn_intercept, 2, 3, 1, 1);

    //Do the same for the button to transfer intercept time to an agent.
    btn_transfer.set_label("< Transfer Time");
    btn_transfer.set_sensitive(false);
    btn_transfer.set_hexpand(false);
    btn_transfer.set_vexpand(false);
    btn_transfer.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::actionTransfer));
    grd_controls.attach(btn_transfer, 2, 4, 1, 1);

    //Do the same for the button to prevent another agent from being intercepted.
    btn_encrypt.set_label("< Encrypt");
    btn_encrypt.set_sensitive(false);
    btn_encrypt.set_hexpand(false);
    btn_encrypt.set_vexpand(false);
    btn_encrypt.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::actionEncrypt));
    grd_controls.attach(btn_encrypt, 2, 5, 1, 1);

    //Do the same for to view team info. (ALWAYS ACTIVE)
    btn_teamInfo.set_label("My Team Info");
    btn_teamInfo.set_hexpand(false);
    btn_teamInfo.set_vexpand(false);
    btn_teamInfo.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::viewTeamInfo));
    grd_controls.attach(btn_teamInfo, 2, 6, 1, 1);

    //Do the same for to view my codes. (ALWAYS ACTIVE FOREVER!)
    btn_myCodes.set_label("My Codes");
    btn_myCodes.set_hexpand(false);
    btn_myCodes.set_vexpand(false);
    btn_myCodes.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::viewCodes));
    grd_controls.attach(btn_myCodes, 2, 7, 1, 1);

    //The label here is just for a default. refreshButtons will override.
    btn_cloak.set_label("Engage Cloak");
    btn_cloak.set_hexpand(false);
    btn_cloak.set_vexpand(false);
    btn_cloak.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::actionCloak));
    grd_controls.attach(btn_cloak, 2, 8, 1, 1);

    //Right side - used code list and search.
    box_codes.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_codes.set_spacing(10);
    box_lr.pack_start(box_codes);

    //Codes box
    box_codes.pack_start(txt_searchCode, Gtk::PACK_SHRINK);

    //Add the scrollable box for the agent list.
    box_codes.pack_start(scrl_lst_codes, Gtk::PACK_EXPAND_WIDGET);
    scrl_lst_codes.add(lst_codes);

    //Define the agent list.
    ref_mdl_codes = Gtk::ListStore::create(mdl_col_codes);
    lst_codes.set_model(ref_mdl_codes);

    //This should NOT WANT to resize horizontally...
    lst_codes.set_hexpand(false);
    //...but it SHOULD WANT to resize vertically.
    lst_codes.set_vexpand(true);

    //Make it searchable by the codes column.
    lst_codes.set_search_entry(txt_searchCode);
    lst_codes.set_search_column(mdl_col_codes.col_code);

    //Append the "my code?" column to the code list.
    lst_codes.append_column("", mdl_col_codes.col_mine);
    //Lock the column width to be just wide enough.
    lst_codes.get_column(0)->set_fixed_width(30);
    //Make the column sortable.
    lst_codes.get_column(0)->set_sort_column(mdl_col_codes.col_mine);

    //Append the code column to the code list.
    lst_codes.append_column("Code", mdl_col_codes.col_code);
    //Make the column sortable.
    lst_codes.get_column(1)->set_sort_column(mdl_col_codes.col_code);

    //Set default sort column to sort by code.
    ref_mdl_codes->set_sort_column(mdl_col_codes.col_code, Gtk::SORT_ASCENDING);

    //Populate the list.
    addCodes();

    lst_codes.show();

    //Bottom row: Status and logout button.
    lbl_agentStatus.set_hexpand(false);
    grd_controls.attach(lbl_agentStatus, 0, 9, 1, 1);
    if(db->agents[id-1].active)
    {
        lbl_agentStatus.set_label("Active");
    }
    else
    {
        lbl_agentStatus.set_label("Decommissioned");
    }

    //Define the logout button. (ALWAYS ACTIVE!)
    btn_logout.set_label("Logout");
    btn_logout.set_hexpand(false);
    btn_logout.set_vexpand(false);

    //When the button is clicked, run logout function.
    btn_logout.signal_clicked().connect(sigc::mem_fun(this, &agentWindow::logout));

    //Add to grid.
    grd_controls.attach(btn_logout, 1, 9, 1, 1);

    /*Refresh the interface to reflect current status.*/
    refreshButtons();

    //Show all children.
    show_all_children(true);

    //If the agent has been decommissioned from a mark, but doesn't know...
    if(db->agents[agentID-1].struck)
    {
        //Unmark the flag.
        db->agents[agentID-1].struck = false;
        //Display the message.
        displayDecommDialog();
    }

    //If the agent is the infiltrator and won the game...
    if(db->agents[agentID-1].surprise)
    {
        displayInfiltratorWon();
    }

    //If the agent had a successful intercept.
    if(db->agents[agentID-1].intercept_success)
    {
        displayInterceptSuccess();
    }

    //Update the encrypts and intercepts.
    db->updateTimer();
}

void agentWindow::actionAccuse()
{
    Gtk::TreeModel::iterator iter = ref_sel_agents->get_selected();

    //If something is selected.
    if(iter)
    {
        //Get the ID from the row.
        int id = (*iter)[mdl_col_agents.col_id];

        Gtk::MessageDialog dlg_accuse(*this, "Accuse Agent?",
            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE, true);
        dlg_accuse.set_secondary_text("Are you sure you want to ACCUSE " +
        db->agents[id-1].firstName + " " + db->agents[id-1].lastName +
        "of being the infiltrator? \n<b><span foreground=\"red\">WARNING:</span> \
If you are wrong, you will be immediately decommissioned!</b>", true);
        dlg_accuse.add_button("No", 0);
        dlg_accuse.add_button("Yes", 1);
        if(dlg_accuse.run() == 1)
        {
            bool result = db->accuseAgent(agentID, id);
            //If the guess was correct...
            if(result)
            {
                Gtk::MessageDialog dlg_spycatcher(*this, "Spycatcher!",
                    false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
                dlg_spycatcher.set_secondary_text("You accused correctly! \
The infiltrator has been decommissioned, and all his/her codes have been \
deactivated. You have earned the title of SPYCATCHER!", true);
                dlg_spycatcher.run();
            }
            //If the guess was incorrect.
            else
            {
                Gtk::MessageDialog dlg_wrong(*this, "Decommissioned!",
                    false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
                dlg_wrong.set_secondary_text("You accused wrongly and have \
been deactivated! <b>You may continue to pass out your codes</b>, but you may \
<b>do NOTHING ELSE.</b> \n <span foreground=\"red\"><b>Give NO CLUES about the \
identity of the infiltrator</b></span>, lest you ruin the game for everyone \
else.", true);
                dlg_wrong.run();
            }
        }
    }

    //Update the interface.
    refreshAgents();
    refreshButtons();
}

void agentWindow::actionCloak()
{
    if(db->agents[agentID-1].cloakUsed == false)
    {
        Gtk::MessageDialog dlg_cloak_on(*this, "Engage Cloak?",
            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE, true);
        dlg_cloak_on.set_secondary_text("Are you sure you want to ENGAGE your \
cloak? You will appear as decommissioned to everyone else when it is on. \
\n<b><span foreground=\"red\">WARNING:</span> You CANNOT redeem codes while it \
is on, and you can only use it one time!</b>", true);
        dlg_cloak_on.add_button("No", 0);
        dlg_cloak_on.add_button("Yes", 1);
        if(dlg_cloak_on.run() == 1)
        {
            db->agents[agentID-1].cloaked = true;
            db->agents[agentID-1].cloakUsed = true;
        }
    }
    else if(db->agents[agentID-1].cloaked)
    {
        Gtk::MessageDialog dlg_cloak_off(*this, "Disable Cloak?",
            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE, true);
        dlg_cloak_off.set_secondary_text("Are you sure you want to DISABLE your \
cloak? You will appear as active to everyone else, and can redeem codes again. \
\n<b><span foreground=\"red\">WARNING:</span> You CANNOT turn your cloak back \
on!</b>", true);
        dlg_cloak_off.add_button("No", 0);
        dlg_cloak_off.add_button("Yes", 1);
        if(dlg_cloak_off.run() == 1)
        {
            db->agents[agentID-1].cloaked = false;
            db->agents[agentID-1].cloakUsed = true;
        }
    }

    refreshButtons();
}

void agentWindow::actionDecomm()
{
    Gtk::TreeModel::iterator iter = ref_sel_agents->get_selected();

    //If something is selected.
    if(iter)
    {
        //Get the ID from the row.
        int id = (*iter)[mdl_col_agents.col_id];

        Gtk::MessageDialog dlg_decomm(*this, "Decommission Agent?",
            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE, true);
        dlg_decomm.set_secondary_text("Are you sure you want to DECOMMISSION " +
        db->agents[id-1].firstName + " " + db->agents[id-1].lastName +
        "? <b><span foreground=\"red\">You cannot undo this!</span></b>", true);
        dlg_decomm.add_button("No", 0);
        dlg_decomm.add_button("Yes", 1);
        if(dlg_decomm.run() == 1)
        {
            db->decommAgent(id);
            refreshAgents();
            refreshButtons();
            if(db->spycatcher == agentID)
            {
                displayInfiltratorWon();
            }
        }
    }
}

void agentWindow::actionEncrypt()
{
    Gtk::TreeModel::iterator iter = ref_sel_agents->get_selected();

    //If something is selected.
    if(iter)
    {
        //Get the ID from the row.
        int id = (*iter)[mdl_col_agents.col_id];
        //Creates an instance of a new popup window to enter intercept time.
        timeWindow = new TimeSelectWindow("Protect Agent from Intercept", "How many seconds would \
you like to spend to protect " + db->agents[id-1].firstName + " " +
db->agents[id-1].lastName + " from being intercepted?", db,
TimeSelectWindow::TIME_ENCRYPT, id, agentID);

        /*The popup window should be modal, meaning we can't use this
        while it is open.*/
        timeWindow->set_modal(true);

        //Keep the new popup window on top.
        timeWindow->set_transient_for(*this);

        /*Connect the signal that the popup window closed to the winClosed
        function, so we can perform essential tasks and keep the game going!*/
        timeWindow->signal_delete_event().connect(sigc::mem_fun(this, &agentWindow::winClosed));

        //Now we're ready...show the popup window!
        timeWindow->show();
    }
}

void agentWindow::actionIntercept()
{
    Gtk::TreeModel::iterator iter = ref_sel_agents->get_selected();

    //If something is selected.
    if(iter)
    {
        //Get the ID from the row.
        int id = (*iter)[mdl_col_agents.col_id];
        //Creates an instance of a new popup window to enter intercept time.
        timeWindow = new TimeSelectWindow("Intercept codes of an Agent", "How \
many seconds would you like to use for an intercept on Agent " +
db->agents[id-1].firstName + " " + db->agents[id-1].lastName + "?",
db, TimeSelectWindow::TIME_INTERCEPT, id, agentID);
        /*The popup window should be modal, meaning we can't use this
        while it is open.*/
        timeWindow->set_modal(true);
        //Keep the new popup window on top.
        timeWindow->set_transient_for(*this);
        /*Connect the signal that the popup window closed to the winClosed
        function, so we can perform essential tasks and keep the game going!*/
        timeWindow->signal_delete_event().connect(sigc::mem_fun(this, &agentWindow::winClosed));
        //Now we're ready...show the popup window!
        timeWindow->show();
    }
}

void agentWindow::actionNewSecurity()
{
    Gtk::MessageDialog dlg_new_security(*this, "New Security Code?",
        false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE, true);
    dlg_new_security.set_secondary_text("Are you sure you want to discard your \
current security code and get a new one? \
\n<b><span foreground=\"red\">WARNING:</span> You cannot undo this. \
is on, and you can only use it one time!</b>", true);
    dlg_new_security.add_button("No", 0);
    dlg_new_security.add_button("Yes", 1);
    if(dlg_new_security.run() == 1)
    {
        db->newSecurityCode(agentID);
        lbl_agentSecurity.set_text(AgentDatabase::int_to_ustring
            (db->agents[agentID-1].securitycode));
    }
}

void agentWindow::actionTransfer()
{
    Gtk::TreeModel::iterator iter = ref_sel_agents->get_selected();

    //If something is selected.
    if(iter)
    {
        //Get the ID from the row.
        int id = (*iter)[mdl_col_agents.col_id];
        //Creates an instance of a new popup window to enter intercept time.
        timeWindow = new TimeSelectWindow("Transfer time to a teammate", "How \
many seconds of intercept time would you like to transfer to Agent " +
db->agents[id-1].firstName + " " + db->agents[id-1].lastName + "?",
db, TimeSelectWindow::TIME_TRANSFER, id, agentID);
        /*The popup window should be modal, meaning we can't use this
        while it is open.*/
        timeWindow->set_modal(true);
        //Keep the new popup window on top.
        timeWindow->set_transient_for(*this);
        /*Connect the signal that the popup window closed to the winClosed
        function, so we can perform essential tasks and keep the game going!*/
        timeWindow->signal_delete_event().connect(sigc::mem_fun(this, &agentWindow::winClosed));
        //Now we're ready...show the popup window!
        timeWindow->show();
    }
}

bool agentWindow::winClosed(GdkEventAny* event)
{
    //If we have an agent window defined (the pointer is not null).
    if(timeWindow != 0)
    {
        //Delete the dynamically allocated window object.
        delete timeWindow;
        //Reset the pointer to null.
        timeWindow = 0;
    }

    //Update the interface.
    refreshButtons();

    //Do NOT prevent other handlers from working with the window's delete_event.
    return false;
}

void agentWindow::addAgents()
{
    Gtk::TreeModel::Row row;
    for(unsigned int a = 0; a < db->agents.size(); a++)
    {
        if(a == (agentID-1))
        {
            continue;
        }

        row = *(ref_mdl_agents->append());
        row[mdl_col_agents.col_id] = db->agents[a].id;
        row[mdl_col_agents.col_first] = db->agents[a].firstName;
        row[mdl_col_agents.col_last] = db->agents[a].lastName;
    }
    //Instead of duplicating the rest of the code, just call refreshAgents()!
    refreshAgents();
}

void agentWindow::addCodes()
{
    //Define the row variable we'll use over and over.
    Gtk::TreeModel::Row row;

    //We always have to clear first! Inefficient, but the only way. :(
    ref_mdl_codes->clear();

    //For each code in the codesused set...
    for(auto c : db->codesused)
    {
        //Add a new row to the table.
        row = *(ref_mdl_codes->append());
        //Write code to row.
        row[mdl_col_codes.col_code] = c;
        //Mark if this code is in the agent's codes set.
        row[mdl_col_codes.col_mine] = (db->agents[agentID-1].codes.count(c) > 0);
    }
}

void agentWindow::displayDecommDialog()
{
    //If the agent was the infiltrator and was "struck"...
    if(db->agents[agentID-1].infiltrator)
    {
        //They were accused correctly! Tell them so.
        Gtk::MessageDialog dlg_caught(*this, "You've Been Caught!",
        false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_caught.set_secondary_text("A-HA! You have been found out, \
infiltrator! You have been decommissioned. <b>Your codes have all been \
deactivated,</b> and you must <b>announce yourself to the others.</b>", true);
        dlg_caught.run();
    }
    //Else if the agent was NOT the infiltrator and was "struck"...
    else
    {
        //They were decommissioned by the infiltrator. Tell them so.
        Gtk::MessageDialog dlg_decomm(*this, "Decommissioned!",
        false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg_decomm.set_secondary_text("The infiltrator has decommissioned \
you! You entered his/her code a while back, and it was just activated. <b>You \
may continue to pass out your codes</b>, but you may <b>do NOTHING ELSE.</b> \
\n <span foreground=\"red\"><b>Give NO CLUES about the identity of the \
infiltrator</b></span>, lest you ruin the game for everyone else.", true);
        dlg_decomm.run();
    }
}

void agentWindow::displayInfiltratorWon()
{
    if(db->agents[agentID-1].infiltrator && db->spycatcher == agentID)
    {
        db->agents[agentID-1].surprise = false;
        //The infiltrator won!
        Gtk::MessageDialog dlg_won(*this, "You Won!",
        false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg_won.set_secondary_text("Congratulations, infiltrator! \
You have eliminated all the other agents. You won the title of SPYCATCHER! \
You may now <b>announce yourself to the others.</b>", true);
        dlg_won.run();
    }
}

void agentWindow::displayInterceptSuccess()
{
    if(db->agents[agentID-1].intercept_success)
    {
        db->agents[agentID-1].intercept_success = false;
        //The infiltrator won!
        Gtk::MessageDialog dlg_success(*this, "Intercept Successful",
        false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg_success.set_secondary_text("You successfully intercepted one or more \
codes. Well done, agent.", true);
        dlg_success.run();
    }
}

void agentWindow::refreshAgents()
{
    lbl_connections.set_text("Connections: " +
        AgentDatabase::int_to_ustring(db->agents[agentID-1].connections.size()) +
        " of " + AgentDatabase::int_to_ustring(db->agents.size()));

    int id;
    typedef Gtk::TreeModel::Children type_children; //Minimize code length.
    type_children children = ref_mdl_agents->children();
    for(type_children::iterator iter = children.begin();
        iter != children.end(); ++iter)
    {
        Gtk::TreeModel::Row row = *iter;
        id = row[mdl_col_agents.col_id];

        //If target agent is NOT active OR is cloaked....
        if(db->agents[id-1].active == false || db->agents[id-1].cloaked)
        {
            //Decommissioned. Use UTF-8 "Ballot X" U+2717.
            row[mdl_col_agents.col_status] = "✗";
        }
        else
        {
            //Active. Use UTF-8 "Check Mark" U+2713.
            row[mdl_col_agents.col_status] = "✓";

            //If infiltrator is logged in and target agent is marked...
            if(db->agents[id-1].marked && db->agents[agentID-1].infiltrator)
            {
                //Marked. Use UTF-8 "White-Feathered Rightwards Arrow" U+27B3.
                row[mdl_col_agents.col_status] = "➳";
            }
        }

        //We check connection separately from status, so we can see both.
        if(db->agents[agentID-1].connections.count(id) > 0)
        {
            if(db->agents[agentID-1].team->teamName == db->agents[id-1].team->teamName)
            {
                row[mdl_col_agents.col_conn] = "Teammate";
            }
            else
            {
                row[mdl_col_agents.col_conn] = "Connected";
            }
        }
        else
        {
            row[mdl_col_agents.col_conn] = "---";
        }
    }
}

void agentWindow::refreshButtons()
{
    //Docs: https://developer.gnome.org/gtkmm-tutorial/stable/sec-treeview-selection.html.en

    //Deactivate the relevant buttons before doing ANYTHING!
    txt_redeemCode.set_sensitive(false);
    btn_submitCode.set_sensitive(false);
    btn_accuse.set_sensitive(false);
    btn_cloak.set_sensitive(false);
    btn_decomm.set_sensitive(false);
    btn_intercept.set_sensitive(false);
    btn_encrypt.set_sensitive(false);
    btn_transfer.set_sensitive(false);

    //Update encrypt time label.
    lbl_agentEncryptTime.set_text("Encrypt Time Left: " +
        AgentDatabase::int_to_ustring(db->agents[agentID-1].encryptSeconds) +
        " sec");

    //Update intercept time label.
    lbl_agentInterceptTime.set_text("Intercept Time Left: " +
        AgentDatabase::int_to_ustring(db->agents[agentID-1].tapSeconds) +
        " sec");

    //If the agent is decommissioned...
    if(!db->agents[agentID-1].active)
    {
        //You can't do anything!
        lbl_agentStatus.set_label("Decommissioned");

        //Deactivate the agent list.
        lst_agents.set_sensitive(false);

        //Deactivate the team info button.
        btn_teamInfo.set_sensitive(false);

        return;
    }

    //Otherwise...
    lbl_agentStatus.set_label("Active");

    if(db->agents[agentID-1].cloakUsed == false)
    {
        btn_cloak.set_label("Engage Cloak");
        btn_cloak.set_sensitive(true);

        //Enable the code redemption features.
        txt_redeemCode.set_sensitive(true);
        btn_submitCode.set_sensitive(true);
    }
    else if(db->agents[agentID-1].cloaked)
    {
        btn_cloak.set_label("Uncloak");
        btn_cloak.set_sensitive(true);

        //Leave code redemption features disabled.
    }
    else
    {
        btn_cloak.set_label("Cloak Used");
        btn_cloak.set_sensitive(false);

        //Enable the code redemption features.
        txt_redeemCode.set_sensitive(true);
        btn_submitCode.set_sensitive(true);
    }

    //Get the current selection from the agent list.
    Gtk::TreeModel::iterator iter = ref_sel_agents->get_selected();

    //If something is selected.
    if(iter)
    {
        int id = (*iter)[mdl_col_agents.col_id];

        //If the infiltrator is not yet caught...
        if(db->spycatcher == 0)
        {
            /*The agent can be accused, even if they are inactive (otherwise,
            cloaked agents are revealed by the button's state.)
            Activate that button.*/
            btn_accuse.set_sensitive(true);
        }

        //If the selected agent is active...
        if(db->agents[id-1].active)
        {
            //If the selected agent is one of the current agent's connections...
            if(db->agents[agentID-1].connections.count(id) > 0)
            {
                //If the selected agent is on the current agent's team...
                if(db->agents[id-1].team->teamName == db->agents[agentID-1].team->teamName)
                {
                    //If the current agent has some encrypts left.
                    if(db->agents[agentID-1].encryptSeconds > 0)
                    {
                        //The agent can be protected/encrypted.
                        btn_encrypt.set_sensitive(true);
                    }

                    //If the current agent has interception time available...
                    if(db->agents[agentID-1].tapSeconds > 0)
                    {
                        //The agent can receive interception time. Activate that button.
                        btn_transfer.set_sensitive(true);
                    }
                }
            }
            //Otherwise, if the agent is NOT a connection...
            else
            {
                //If the current agent has interception time available...
                if(db->agents[agentID-1].tapSeconds > 0)
                {
                    //The selected agent can be intercepted. Activate that button.
                    btn_intercept.set_sensitive(true);
                }
            }
        }

        //If the current agent is the infiltrator AND the selected agent is marked.
        if(db->agents[agentID-1].infiltrator && db->agents[id-1].marked)
        {
            //The agent can be decommissioned on demand. Activate that button.
            btn_decomm.set_sensitive(true);
        }
    }
}

void agentWindow::redeemCode()
{
    int code = AgentDatabase::ustring_to_int(txt_redeemCode.get_text());
    AgentDatabase::RedemptionCode result = db->redeemCode(agentID, code);
    switch(result)
    {
        case AgentDatabase::INVALID:
        {
            Gtk::MessageDialog dlg_code_invalid(*this, "Invalid Code",
                false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg_code_invalid.set_secondary_text("The code you entered is \
invalid. Check the code and try again.");
            dlg_code_invalid.run();
            break;
        }
        case AgentDatabase::REPEAT:
        {
            Gtk::MessageDialog dlg_code_repeat(*this, "Already Connected",
                false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg_code_repeat.set_secondary_text("You are already connected to \
the agent this code is from. Please put the code back into play!");
            dlg_code_repeat.run();
            break;
        }
        case AgentDatabase::USED:
        {
            Gtk::MessageDialog dlg_code_used(*this, "Used Code",
                false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg_code_used.set_secondary_text("The code you entered was already \
redeemed by an agent. Please discard.");
            dlg_code_used.run();
            break;
        }
        case AgentDatabase::OWN:
        {
            Gtk::MessageDialog dlg_code_own(*this, "Your Code",
                false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg_code_own.set_secondary_text("You cannot redeem your own \
codes, agent.");
            dlg_code_own.run();
            break;
        }
        //If the agent is marked, make it look normal!
        case AgentDatabase::MARK:
        case AgentDatabase::SUCCESS:
        {
            refreshAgents();
            int newC = db->checkCode(code);
            Gtk::MessageDialog dlg_code_own(*this, "Success",
                false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
            dlg_code_own.set_secondary_text("You are now connected to " +
                db->agents[newC-1].firstName + " " + db->agents[newC-1].lastName);
            dlg_code_own.run();
            break;
        }
        case AgentDatabase::POISON:
        {
            Gtk::MessageDialog dlg_code_poison(*this, "Decommissioned!",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            dlg_code_poison.set_secondary_text("You just entered a code from \
the infiltrator! You are now decommissioned. <b>You may continue to pass out \
your codes</b>, but you may <b>do NOTHING ELSE.</b> \n<span foreground=\"red\">\
<b>Give NO CLUES about the identity of the infiltrator</b></span>, lest you \
ruin the game for everyone else.", true);
            dlg_code_poison.run();

            //Clear code boxes.
            txt_redeemCode.set_text("");
            txt_searchCode.set_text("");

            //Deactivate interface.
            refreshButtons();

            break;
        }
        case AgentDatabase::INTERCEPTED:
        {
            Gtk::MessageDialog dlg_code_intercept(*this, "Intercepted!",
                false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            dlg_code_intercept.set_secondary_text("The code you just entered \
was intercepted! You need to get another code from the agent to make a \
a connection from them now.", true);
            dlg_code_intercept.run();
            break;
        }
    }

    //Refresh that codes list!
    addCodes();
}

void agentWindow::logout()
{
    close();
}

void agentWindow::viewCodes()
{
    Gtk::MessageDialog dlg_codes(*this, "My Codes", false, Gtk::MESSAGE_INFO,
        Gtk::BUTTONS_OK, true);
    Glib::ustring text = "Unused Codes...";
    //For each of the agent's codes...
    for(auto c : db->agents[agentID-1].codes)
    {
        //If the code is not used...
        if(db->codesused.count(c) == 0)
        {
            //Add it with a newline to the text variable.
            text += "\n" + AgentDatabase::int_to_ustring(c);
        }
    }
    //Set the text.
    dlg_codes.set_secondary_text(text);
    //Display the dialog box.
    dlg_codes.run();
}

void agentWindow::viewTeamInfo()
{
    Gtk::MessageDialog dlg_team_info(*this, db->agents[agentID-1].team->teamName,
                false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg_team_info.set_secondary_text("TEAM SIGNAL: " +
        db->agents[agentID-1].team->signal);
    //Display the dialog box.
    dlg_team_info.run();
}

agentWindow::~agentWindow()
{
    //dtor
}
