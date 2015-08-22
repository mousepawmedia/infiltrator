#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <glibmm.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

#include <iostream>
#include <agentDatabase.hpp>

class NumberEntry : public Gtk::Entry
{
    public:
        NumberEntry(){}
        void on_insert_text(const Glib::ustring&, int*);
        ~NumberEntry(){}
    private:
        bool contains_only_numbers(const Glib::ustring&);
};

class TimeSelectWindow : public Gtk::Window
{
    public:
        enum TimeSelectMode
        {
            TIME_INTERCEPT,
            TIME_ENCRYPT,
            TIME_TRANSFER
        };

        /**A constructor for popupWindow
        \param the Glib::ustring of the window's title
        \param the Glib::ustring of the window's message
        \param true in intercepting, false if encrypting
        \param agent being targeted
        \param agent setting the intercept/encrypt*/
        TimeSelectWindow(Glib::ustring, Glib::ustring, AgentDatabase*, TimeSelectMode, int, int);

    private:
        AgentDatabase *db;
        int targetID;
        int fromID;
        Gtk::Button btn_OK,
                    btn_Cancel;
        /*NumberEntry txt_time;*/
        Gtk::SpinButton spn_time;
        Gtk::Label lbl_message;
        Gtk::Box box_main,
                 box_message,
                 box_time,
                 box_buttons;
        /**closes the popup window*/
        void closeWindow();
        /**Takes intercept time and starts a countdown to intercept codes from selected person.*/
        void processIntercept();
        /**Takes encrypt time and starts a countdown to protect against intercepts for selected person*/
        void processEncrypt();
        /**Takes transfer time and transfer it to the selected teammate.*/
        void processTransfer();
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
