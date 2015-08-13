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
    set_margin_start(10);
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
