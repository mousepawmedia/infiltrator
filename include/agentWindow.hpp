#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <glibmm.h>
#include <iostream>
#include <vector>

#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/entrybuffer.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/window.h>

#include "agentDatabase.hpp"
#include "customWidgets.hpp"

using std::vector;

class agentWindow : public Gtk::Window
{
    public:
        agentWindow(AgentDatabase*, int);
        ~agentWindow();
    private:
        AgentDatabase* db;
        int agentID;

        Gtk::Box box_main,
                 box_status,
                 box_statusB,
                 box_lr,
                 box_codes;

        Gtk::Grid grd_controls;

        Gtk::Button btn_newSecurity,
                    btn_submitCode,
                    btn_accuse,
                    btn_cloak,
                    btn_decomm,
                    btn_encrypt,
                    btn_intercept,
                    btn_logout,
                    btn_myCodes,
                    btn_transfer,
                    btn_teamInfo;

        NumberEntry txt_redeemCode,
                    txt_searchCode;

        Gtk::Label lbl_agentID,
                   lbl_agentName,
                   lbl_agentTeam,
                   lbl_agentSecurity,
                   lbl_overrides,
                   lbl_agentInfiltrator,
                   lbl_agentInterceptTime,
                   lbl_agentEncryptTime,
                   lbl_agentStatus,
                   lbl_connections;

        class AgentMdlCols : public Gtk::TreeModel::ColumnRecord
        {
            public:
                AgentMdlCols()
                {
                    add(col_id);
                    add(col_status);
                    add(col_last);
                    add(col_first);
                    add(col_conn);
                }
                Gtk::TreeModelColumn<int> col_id;
                Gtk::TreeModelColumn<Glib::ustring> col_last,
                                                    col_first,
                                                    col_status,
                                                    col_conn;
        };

        AgentMdlCols mdl_col_agents;
        Gtk::TreeView lst_agents;
        Glib::RefPtr<Gtk::ListStore> ref_mdl_agents;
        Glib::RefPtr<Gtk::TreeSelection> ref_sel_agents;

        Gtk::ScrolledWindow scrl_lst_agents,
                            scrl_lst_codes;

        class CodesMdlCols : public Gtk::TreeModel::ColumnRecord
        {
            public:
                CodesMdlCols()
                {
                    add(col_mine);
                    add(col_code);
                }
                Gtk::TreeModelColumn<bool> col_mine;
                Gtk::TreeModelColumn<int> col_code;
        };

        CodesMdlCols mdl_col_codes;
        Gtk::TreeView lst_codes;
        Glib::RefPtr<Gtk::ListStore> ref_mdl_codes;

        void actionAccuse();
        void actionCloak();
        void actionDecomm();
        void actionEncrypt();
        void actionIntercept();
        void actionNewSecurity();
        void actionTransfer();
        void addAgents();
        void addCodes();
        void displayDecommDialog();
        void displayInfiltratorWon();
        void viewTeamInfo();
        void viewCodes();
        void refreshAgents();
        void refreshButtons();
        void redeemCode();
        void logout();
};

#endif // MAINWINDOW_H
