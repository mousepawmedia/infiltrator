#include "customWidgets.hpp"

//SOURCE: http://stackoverflow.com/a/10282948/472647
void NumberEntry::on_insert_text(const Glib::ustring& text, int* position)
{
    //Allow only numbers to be entered.
    if (contains_only_numbers(text))
    {
        Gtk::Entry::on_insert_text(text, position);
    }
}

bool NumberEntry::contains_only_numbers(const Glib::ustring& text)
{
    for (int i = 0; i < text.length(); i++)
    {
        if (Glib::Unicode::isdigit(text[i]) == false)
        {
            return false;
        }
    }

    return true;
}

TimeSelectWindow::TimeSelectWindow(Glib::ustring title, Glib::ustring message,
    AgentDatabase *adb, TimeSelectMode mode, int id_to, int id_from)
{
    db = adb;
    targetID = id_to;
    fromID = id_from;

    //Set the default size.
    set_default_size(400, 100);
    //Let's also give our window a nicer title.
    set_title(title);

    //Set up main area
    box_main.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_main.set_spacing(10);
    //Set the margins to give some space in the interface.
    box_main.set_margin_top(30);
    box_main.set_margin_left(20);
    box_main.set_margin_right(20);
    box_main.set_margin_bottom(10);
    add(box_main);

    //Set up message area
    box_message.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_main.pack_start(box_message, Gtk::PACK_SHRINK);

    //Set up time area
    box_time.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_main.pack_start(box_time, Gtk::PACK_SHRINK);

    //Set up buttons area
    box_buttons.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box_buttons.set_halign(Gtk::ALIGN_CENTER);
    box_buttons.set_spacing(10);
    box_main.pack_start(box_buttons, Gtk::PACK_SHRINK);

    //Add label to window
    lbl_message.set_text(message);
    lbl_message.set_alignment(Gtk::ALIGN_CENTER);
    box_message.pack_start(lbl_message, Gtk::PACK_SHRINK);

    //Add text box to window
    /*txt_time.set_text("Enter time in seconds.");
    txt_time.set_margin_left(200);
    txt_time.set_margin_right(200);
    box_time.pack_start(txt_time, Gtk::PACK_SHRINK);*/

    spn_time.set_digits(0);
    spn_time.set_numeric();
    spn_time.set_snap_to_ticks();
    spn_time.set_increments(1, 10);

    /*Only allow entering between five seconds and the maximum intercept time
    available for the user.*/
    double max_time = ((mode == TIME_INTERCEPT || mode == TIME_TRANSFER) ?
        static_cast<double>(db->agents[fromID].tapSeconds) :
        static_cast<double>(db->agents[fromID].encryptSeconds));
    spn_time.set_range(5, max_time);
    box_time.pack_start(spn_time, Gtk::PACK_SHRINK);

    //Add buttons to window
    btn_OK.set_label("OK");
    if(mode == TIME_INTERCEPT)
    {
        btn_OK.signal_clicked().connect(sigc::mem_fun(this, &TimeSelectWindow::processIntercept));
    }
    else if(mode == TIME_ENCRYPT)
    {
        btn_OK.signal_clicked().connect(sigc::mem_fun(this, &TimeSelectWindow::processEncrypt));
    }
    else if(mode == TIME_TRANSFER)
    {
        btn_OK.signal_clicked().connect(sigc::mem_fun(this, &TimeSelectWindow::processTransfer));
    }
    btn_Cancel.set_label("Cancel");
    btn_Cancel.signal_clicked().connect(sigc::mem_fun(this, &TimeSelectWindow::closeWindow));
    box_buttons.pack_start(btn_OK, Gtk::PACK_SHRINK);
    box_buttons.pack_start(btn_Cancel, Gtk::PACK_SHRINK);

    show_all_children(true);
}

void TimeSelectWindow::closeWindow()
{
    close();
}

void TimeSelectWindow::processIntercept()
{
    //Update the encrypts and intercepts before we do anything.
    db->updateTimer();

    //Remove the time from the intercepting agent.
    db->agents[fromID - 1].tapSeconds -= spn_time.get_value_as_int();

    //Create the new intercept object.
    AgentDatabase::Intercept newIntercept;
    newIntercept.ownerID = fromID;
    newIntercept.secondsLeft = spn_time.get_value_as_int();
    //Add the intercept to the target agent.
    db->agents[targetID - 1].intercepts.push_back(newIntercept);
    //Close the window.
    close();
}

void TimeSelectWindow::processEncrypt()
{
    //Update the encrypts and intercepts before we do anything.
    db->updateTimer();

    //Remove the time from the encrypting agent.
    db->agents[fromID - 1].encryptSeconds -= spn_time.get_value_as_int();

    //Create the new encrypt object.
    AgentDatabase::Encrypt newEncrypt;
    newEncrypt.ownerID = fromID;
    newEncrypt.secondsLeft = spn_time.get_value_as_int();

    //Add the encrypt to the target agent.
    db->agents[targetID - 1].encrypts.push_back(newEncrypt);
    close();
}

void TimeSelectWindow::processTransfer()
{
    //Update the encrypts and intercepts before we do anything.
    db->updateTimer();

    //Remove the time from the transferring agent.
    db->agents[fromID - 1].tapSeconds -= spn_time.get_value_as_int();
    //Add the time to the receiving agent.
    db->agents[targetID - 1].tapSeconds += spn_time.get_value_as_int();
    //Close the window.
    close();
}

TitleLabel::TitleLabel()
{
    //We are going to use Pango to make it really fancy.
    set_use_markup(true);
    //Center the text.
    set_justify(Gtk::JUSTIFY_CENTER);
    /*The title "INFILTRATOR" should be large and bold. Make the A stand out
    by making it red and raising it above the rest, thus representing the
    infiltrator player. In the subtitle, make covert italic.*/
    set_markup("<big><b>INFILTR\
<span foreground=\"red\" rise=\"5000\">A</span>TOR</b></big>\
\nThe Game of <i>Covert</i> Operations");
}

RulesNotebook::MarkupTextView::MarkupTextView()
{
    label.set_halign(Gtk::ALIGN_START);
    label.set_valign(Gtk::ALIGN_START);
    label.set_line_wrap(true);
    //set_margin_start(10);
    set_margin_top(10);
    add(label);
}

RulesNotebook::RulesNotebook()
{
    //The rules notebook should always have the tabs to the left.
    set_tab_pos(Gtk::POS_LEFT);

    view_about.set_markup(
"<i>Infiltrator</i> is an alternative reality game. The majority of game play \
takes place in a real-life, face-to-face environment, such as a party. \
The Infiltrator computer program manages the game, tracks progress, \
and allows players to take certain in-game actions.\n\n\
<i>Infiltrator</i> was created by Jason C. McDonald and Jared Hutton for \
MousePaw Games."
    );
    append_page(view_about, "About");

    view_setup.set_markup(
        "Setup"
    );
    append_page(view_setup, "Setup");

    view_gameplay.set_markup(
        "Gameplay"
    );
    append_page(view_gameplay, "Gameplay");
}
