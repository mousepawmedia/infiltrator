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

    view_objectives.set_markup(
        "The first objective in <i>Infiltrator</i> is to connect with the \
most agents by entering one code from each. The individual who \
makes the most connections wins the title of <b>Networker</b>.\n\n\
However, one player is an infiltrator. If you enter a code from them, \
you are <u>decommissioned</u>, meaning you are out of the game. \
If you think you know who the infiltrator is, you can accuse them. \
If you're wrong, you're decommissioned, but if you are right, you \
win the title of <b>Spycatcher</b>. If the infiltrator manages to \
decommission ALL of the other players, they win that title."
    );
    append_page(view_objectives, "Objectives");

    view_setup.set_markup(
        "You can create a new game, or load a game from a '.agents' file. \
Click <b>Create or Load Game</b> on the main screen to bring up the \
setup screen.\n\n\
--New Game--\n\
Click the <u>New Game</u> tab to create a new <i>Infiltrator</i> game. \
At the top of the screen, click <u>Save As...</u> and browse to the \
location where you want to save your new game. Give it a name, and make \
sure it ends with '.agents', so you can open it later.\n\n\
Enter names using the <u>First name</u> and <u>Last name</u> boxes \
and the <u>Add Player</u> button. Names you have added will be added \
to the player list in the middle of the screen. You can also remove \
players from that list by clicking their name and clicking the \
<u>Remove Player</u> button. You must have at least four players to \
start the game.\n\n\
<b>CAUTION:</b> You cannot add players after the game is started.\n\n\
Using the <u>Intercept (sec)</u> and <u>Encrypt (sec)</u> boxes, select \
how many seconds of intercept and encrypt time you want each player to \
start the game with. The default is 120 seconds (2 minutes). Intercept \
and Encrypt time should be scarce enough to encourage players to \
carefully weigh their use of it.\n\
When you have all the settings the way you want them and are ready to \
start the game, click <u>START NEW GAME</u>.\n\n\
The next screen will give you a gamemaster code and the login \
information for all agents. Be sure to write down the gamemaster \
code securely and save it - you cannot recover it later. The \
gamemaster code will allow you to log into any agent's account, in \
case they forget or lose their security code.\n\n\
The screen also shows you each player's ID and security code. Write \
down each player's information and give it to them, so they \
can log in privately.\n\n\
Once you've written down all of the information, enter the gamemaster \
code at the bottom of the screen and click <b>Continue</b>.\n\n\
--Load Game--\n\
To resume a game you were playing earlier, click the <u>Load Game</u> \
tab. Click <u>Browse</u> and browse to the location of your game's save \
file, which ends with '.agents'. Once the file is selected, click \
<u>RESUME GAME</u> to load the file and resume the game."
    );
    append_page(view_setup, "Setup");

    view_gamescreen.set_markup(
        "The main game screen allows players to log into their personal \
agent dashboards. It also shows a list of all agents and some \
status information about them.\n\n\
An agent's <u>Status</u> is on the left side of the list. A '✓' symbol \
means the agent is active, while '✗' means they are decommissioned. \
The <u>Rank</u> shows how many connections the agent has made. An \
agent's <u>Title</u> shows whether the agent has won as Spycatcher.\n\n\
You can login to your agent dashboard using the form at the top of the \
game screen. Enter your agent ID number in the top box, and your \
security code in the bottom box. Then, click <u>Login</u>.\n\n\
<b>CAUTION:</b> Make sure no one is looking over your shoulder when \
you log in! All of your secret information is found on the agent \
dashboard."
    );
    append_page(view_gamescreen, "Game Screen");

    view_gettingstarted.set_markup(
        "When you first log into your agent dashboard, you are presented with \
all of the information you need to play the game. At the top is your \
ID number, name, team name, and security code. <u>Overrides</u> lists \
the number of times the gamemaster has logged into your account.\n\n\
If you believe someone has your security code, you can request a new \
one by clicking on <u>New Security Code</u> in the upper right corner.\n\n\
The second line shows you how much time you have for Intercepts and \
Encrypts. If you are the infiltrator, this line will also display a \
message about that.\n\n\
Towards the center bottom of the screen is the <u>My Team Info</u> \
button. Click on this to view your team's top secret hand signal, which \
you can use in combination with the team name to find other teammates.\n\n\
Below that button is the <u>My Codes</u> button. Click on this to view \
all of your unused codes. Give these codes to other agents so they can \
connect with you. Each one can only be redeemed once.\n\n\
When you are ready to walk away from the computer, always remember to \
click the <u>Logout</u> button at the bottom of the screen."
    );
    append_page(view_gettingstarted, "Getting Started");

    view_codes.set_markup(
        "Every agent has more than enough codes for everyone to connect with \
them. Each unique code can only be used once. When a code is used, \
it will appear in the list on the right side of each agent dashboard. \
You can search through used codes by typing the code in the box \
above the list.\n\n\
<b>To redeem a code</b>, enter the code in the white box towards the \
upper left of the agent dashboard, and click <u>Submit Code</u>. \
You will be notified of the outcome immediately.\n\n\
If you connect with a new agent, they will be listed either as \
<u>Connected</u> or <u>Teammate</u> in the Connection column of \
the agent list on the left side of your agent dashboard. Agents \
you have not connected with will simply display <u>---</u> in the \
Connection column.\n\n\
<b>REMEMBER:</b> If you enter a code from the infiltrator, you will \
be decommissioned, which means you can no longer enter codes, take \
game actions, or give any clues or information about who the \
infiltrator is. You are allowed to announce ONLY that you were \
decommissioned (but not how), and continue to give out your \
own codes."
    );
    append_page(view_codes, "Codes and Connections");

    view_winning.set_markup(
        "In order to win as Networker, you must have the most connections. \
Even if you are decommissioned, if you have more connections than \
anyone else, you win. However, in case of a tie between an active and \
decommissioned agent, the active agent wins.\n\n\
In order to win as Spycatcher, you must accuse the infiltrator correctly \
using your agent dashboard. Click their name in the list and \
click <b>Accuse</b>. Be careful, though - if you accuse incorrectly, \
you will be decommissioned! If you are correct, the infiltrator will be \
decommissioned, and all of his or her codes will be removed from the \
game."
    );
    append_page(view_winning, "Winning the Game");

    view_tricks.set_markup(
        "Another way to make a connection with an agent is to <b>intercept</b> \
one of their codes. On the agent dashboard, click any player you \
have not connected with and click the <u>Intercept</u> button. On the \
window that appears, enter how many seconds you want the intercept to \
last for. You have a limited number of seconds for intercepts, so use \
the time wisely! If a code from that agent is entered while your \
intercept is in effect, you will anonymously receive the connection \
instead of the person who entered it.\n\n\
Be careful, however! If you intercept the infiltrator's code, you \
will not be immediately decommisioned. The infiltrator can decommission \
you at will using their agent dashboard.\n\n\
Another trick is to go undercover by <b>Cloaking</b>. When you are \
cloaked, you appear as decommissioned on all the agent lists. This \
may be useful in throwing people off the trail of the infiltrator, \
tricking them into accusing incorrectly and getting decommissioned, or \
to help you gather information. While you are cloaked, you cannot redeem \
codes, and you can only use your cloak once. You can engage your \
cloak by clicking <u>Engage Cloak</u> button on the bottom of the stack of \
buttons in the middle of your agent dashboard. When you're done with \
the cloak, use the <u>Uncloak</u> button in the same place to \
disengage your cloak."
    );
    append_page(view_tricks, "Sneaky Tricks");

    view_teams.set_markup(
        "The game randomly assigned other players to be on the same team as \
you. There are two special tricks that teams can use: Encrypt and \
Transfer.\n\n\
When you <b>Encrypt</b> a teammate, intercepts will not work as long \
as your encryption lasts. This is useful in helping to ensure that \
others are able to connect to your teammate. You have a limited number \
of seconds for encryption, so use them wisely. To encrypt a teammate, \
click their name in the agent list and click <u>Encrypt</u>. Enter \
the number of seconds you want to encrypt them for, and click <u>OK</u>.\n\n\
You can <b>Transfer</b> interception time to a teammate, allowing them \
to set longer interceptions than would be otherwise possible alone. \
To transfer time to a teammate, click their name in the agent list and \
click <u>Transfer</u>. Enter the number of seconds you want to transfer \
to them, and click <u>OK</u>.\n\n\
<b>TIP:</b> If you're trying to confirm a player is on your team \
before you connect with them, try saying the first half of your team \
name to them, and let them say the second half. Use your team \
signal carefully, as an observant rival agent could copy it to fool \
you into thinking they're on your team.\n\n\
<b>CAUTION:</b> If the infiltrator is on your team and you connect with \
them, they will appear as a normal teammate on your list. However, they \
can decommission you whenever they want using their agent dashboard!"
    );
    append_page(view_teams, "Teams");

    view_infiltrator.set_markup(
        "If you are the infiltrator, you have a hard task ahead of you. You must \
decommission all of the other agents by getting them to enter one of \
your codes or tricking them into accusing incorrectly, all while \
avoiding detection yourself.\n\n\
However, you have a unique trick available to you. You have been \
assigned to a team, just like any other agent. If any of your teammates\
enter one of your codes, you will appear in their list only as a \
teammate. They are now <b>marked</b>, meaning that you can \
decommission them any time you want to! When an agent is marked, an \
arrow (➳) will appear next to their name in the list on your \
agent dashboard. To decommission a marked agent, click on their name \
and click the 'Decommission' button at the top of the button stack \
in the middle of your screen. You cannot undo a decommission, so use \
it wisely!\n\
This ability to decommission your teammates when you want allows you \
to use them to your advantage. Since they do not know you are the \
infiltrator, you can time their decommissioning to make another agent \
appear to be the infiltrator, or use them to spread your codes around. \
Be careful, however - if they figure out that you're the infiltrator, \
they can still accuse you and win the game!\n\
If an agent intercepts one of your codes, they will also be 'marked' \
instead of being immediately decommissioned. You can decommission them \
the same way you would a teammate.\n\n\
All of the other game actions are the same for you as for another \
agent. Use them to cover up your actions. Cloaking is an especially \
useful tool, as you can use it to divert suspicions!\n\
Use your tricks and tools wisely. If you are caught, you will be \
decommissioned and announced to all the players, and all of your \
codes will be removed from the game."
    );
    append_page(view_infiltrator, "For the Infiltrator");

    view_tech.set_markup(
        "The Gamemaster is in charge of the entire game. Their unique code can \
be used to log in as any agent, though the agent dashboard records \
how many times the gamemaster has logged in. Usually, the gamemaster \
should only log in when an agent has lost or forgotten his or her \
security code.\n\n\
The game automatically saves every time an agent logs in and logs out, \
and when the game is closed. This means that the game can always be \
reloaded from its '.agents' file.\n\n\
The '.agents' file contains all of the game information, encrypted. \
Technically, the code can be broken with a little bit of effort. \
However, the gamemaster reserves the right to throw anyone off the \
computer, so if you get caught trying to crack the game file, it won't \
go so well for you!"
    );
    append_page(view_tech, "Gamemaster");
}
