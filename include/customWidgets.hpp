#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <glibmm.h>
#include <gtkmm/entry.h>

#include <iostream>

class NumberEntry : public Gtk::Entry
{
    public:
        NumberEntry(){}
        void on_insert_text(const Glib::ustring&, int*);
        ~NumberEntry(){}
    private:
        bool contains_only_numbers(const Glib::ustring&);
};



#endif // CUSTOMWIDGETS_H
