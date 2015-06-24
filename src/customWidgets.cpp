#include "customWidgets.hpp"

//SOURCE: http://stackoverflow.com/a/10282948/472647
void NumberEntry::on_insert_text(const Glib::ustring& text, int* position)
{
    //Allow only numbers to be entered.
    if(contains_only_numbers(text))
    {
        Gtk::Entry::on_insert_text(text, position);
    }
}

bool NumberEntry::contains_only_numbers(const Glib::ustring& text)
{
    for(int i = 0; i < text.length(); i++)
    {
        if(Glib::Unicode::isdigit(text[i]) == false)
            return false;
    }

    return true;
}
