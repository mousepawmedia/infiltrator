#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <glibmm.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>

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

class TitleLabel : public Gtk::Label
{
    public:
        TitleLabel();
        ~TitleLabel(){}
};

class RulesNotebook : public Gtk::Notebook
{
    public:
        RulesNotebook();
        ~RulesNotebook(){}

    private:
        class MarkupTextView : public Gtk::ScrolledWindow
        {
            public:
                MarkupTextView();
                Gtk::Label label;
                void set_markup(Glib::ustring str){label.set_markup(str);}
                ~MarkupTextView(){}
        };

        MarkupTextView view_about;
        MarkupTextView view_setup;
        MarkupTextView view_gameplay;
};



#endif // CUSTOMWIDGETS_H
