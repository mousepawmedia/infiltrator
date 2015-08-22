#ifndef INFOWINDOW_H
#define INFOWINDOW_H

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

#include <agentDatabase.hpp>
#include <customWidgets.hpp>

class infoWindow : public Gtk::Window
{
    public:
        infoWindow(AgentDatabase*);
        ~infoWindow(){}
    private:
        AgentDatabase* db;

        bool allowClose = false;

        Gtk::Grid grd_main;

        Gtk::Label lbl_info1,
                   lbl_info2,
                   lbl_code;

        NumberEntry txt_code;
        Gtk::Button btn_done;

        /**A class defining the columns for the model for the agents list.*/
        class AgentMdlCols : public Gtk::TreeModel::ColumnRecord
        {
            public:
                AgentMdlCols()
                {
                    add(col_id);
                    add(col_last);
                    add(col_first);
                    add(col_security);
                }
                Gtk::TreeModelColumn<int> col_id,
                                          col_security;
                Gtk::TreeModelColumn<Glib::ustring> col_last,
                                                    col_first;
        };

        /**The scroll window for the agents list.*/
        Gtk::ScrolledWindow scrl_lst_agents;

        /**The model for the agents list.*/
        AgentMdlCols mdl_col_agents;

        /**The list of agents.*/
        Gtk::TreeView lst_agents;

        /**A reference to the liststore used by the list of agents.*/
        Glib::RefPtr<Gtk::ListStore> ref_mdl_agents;

        void addAgents();
        bool winClose(GdkEventAny*);
        void typeCode();
        void clickContinue();
};

#endif // INFOWINDOW_H
