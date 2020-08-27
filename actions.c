/* Address Book
 * by Lawrence Manning 
 *
 * filename: actions.c
 */

#include "ab.h"
#include "externals.h"

/* All of these functions are run when the user presses on a gadget, or selects
 * a menu option.  So if the user presses the Prev button then goto_prev_entry()
 * will be called.  Or, selecting About runs show_about_requester.  All the
 * functions here take no parameters, and return nothing.  Error messages
 * are displayed by the functions in this file. */
/* GADGET ACTIONS. */
/* This function is different from the rest because it takes some parametrs.
 * It takes the desired entry to display and a flag.  If the flag is TRUE,
 * then the buttons at the bottom of the main window are updated.  If it's
 * FALSE then they are not.  It's ok to pass an invalid entry_number. */
void goto_any_entry(LONG entry_node_number, BOOL update_buttons_flag)
{
    struct entry_node_type *entry_node;
    BOOL found = FALSE;

    entry_node = (struct entry_node_type *) entry_list->mlh_Head;

    /* Move through the list until we get to the required entry. */
    while (entry_node->node.mln_Succ)
    {
        /* Check to see if this is the right entry. */
        if (entry_node_number == entry_node->entry_node_number)
        {
            viewed_entry_node = entry_node;
            found = TRUE;
        }

        entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
    }

    if (found)
    {
        update_main_window_display();
        if (update_buttons_flag) 
            update_main_window_buttons();
    }
}

/* Goes to the first entry in a project. */
void goto_first_entry(void)
{
    /* Only works if we are not already at the start of the list. */
    if (viewed_entry_node != (struct entry_node_type *)
        entry_list->mlh_Head && viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) entry_list->mlh_Head;
        update_main_window_display();
        update_main_window_buttons();
    }
}

/* Goes to the previous entry. */
void goto_prev_entry(void)
{
    /* Only works if we are not already at the start of the list. */
    if (viewed_entry_node != (struct entry_node_type *)
        entry_list->mlh_Head && viewed_entry_node)
    {
         viewed_entry_node = (struct entry_node_type *) viewed_entry_node->node.mln_Pred;
         update_main_window_display();
         update_main_window_buttons();
    }
}

/* Goes to the next entry. */
void goto_next_entry(void)
{
    /* Only works if we are not already at the end of the list. */
    if (viewed_entry_node != (struct entry_node_type *)
        entry_list->mlh_TailPred && viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) viewed_entry_node->node.mln_Succ;
        update_main_window_display();
        update_main_window_buttons();
    }
}

/* Goes to the last entry. */
void goto_last_entry(void)
{
    /* Only works if we are not already at the end of the list. */
    if (viewed_entry_node != (struct entry_node_type *)
        entry_list->mlh_TailPred && viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) entry_list->mlh_TailPred;
        update_main_window_display();
        update_main_window_buttons();
    }
}

/* Opens the edit window to allow the user to edit the current entry. */
void edit_entry(void)
{
    if (viewed_entry_node)
    {
        /* Open a edit window and display the current entry in it. */
        open_edit_window();
        edit_window_updater();
        edit_window_events();

        /* If the entry details were ok'd then... */
        if (edit_window_result == EDIT_WINDOW_RESULT_OK)
        {
            /* Remove the old entry, and free the memory */
            remove_entry_node_from_list(viewed_entry_node);
            close_entry_node(viewed_entry_node);

            /* Now copy the current edit window into a new entry. */
            viewed_entry_node = open_entry_node();
            copy_edit_window_to_entry_node(viewed_entry_node);
            link_entry_node_to_list(viewed_entry_node);

            update_main_window_display();
            update_main_window_buttons();

            project_changed = TRUE;
        }
    }

    store_edit_window_position(); 
    close_edit_window();
}

/* Opens the edit window to let the user create a new entry. */
void new_entry(void)
{
    open_edit_window();
    edit_window_events();

    /* Add this node to the list, if the Ok gadget was selected. */
    if (edit_window_result == EDIT_WINDOW_RESULT_OK)
    {
        viewed_entry_node = open_entry_node();
        copy_edit_window_to_entry_node(viewed_entry_node);
        link_entry_node_to_list(viewed_entry_node);

        update_main_window_display();
        update_main_window_buttons();

        project_changed = TRUE;
    }

    store_edit_window_position();
    close_edit_window();
}

/* Deletes the current entry. */
void delete_entry(void)
{
    struct entry_node_type *temp_entry_node;

    /* The node can only be deleted if there is one here to start with. */
    if (viewed_entry_node)
    {
        /* See if this is the only node in the list. */
        if (viewed_entry_node == (struct entry_node_type *) entry_list->mlh_Head &&
            viewed_entry_node == (struct entry_node_type *) entry_list->mlh_TailPred)
        {
            temp_entry_node = NULL;
        }
        else
        {
            /* There is more then one node.  Now see if there are nodes before
             * the currently displayed node.  This is done by seeing if the
             * currently displayed node is not the head node. */
            if (viewed_entry_node != (struct entry_node_type *)
                entry_list->mlh_Head)
            {
                temp_entry_node = (struct entry_node_type *)
                    viewed_entry_node->node.mln_Pred;
            }
            /* If there are no nodes before the current one (ie. this is the
             * first in the list) then make the one after it the new current
             * one. */
            else
            {
                temp_entry_node = (struct entry_node_type *)
                    viewed_entry_node->node.mln_Succ;
            }
        }
 
        /* Remove this one from the list. */
        remove_entry_node_from_list(viewed_entry_node);
        close_entry_node(viewed_entry_node);

        /* Now update the new displayed node, according to the above. */
        viewed_entry_node = temp_entry_node;

        update_main_window_display();
        update_main_window_buttons();

        project_changed = TRUE;
    }
}

/* Tone-dails the current entry. */
void dial_entry(void)
{
    if (viewed_entry_node)
    {
        /* See if this entry has a phone number set. */
        if (viewed_entry_node->phone)
        {
            /* Can we open the audio.device? */
            if (open_audio_device())
            {
                /* If so, try and idal the number. */
                if (!(tone_dial_number(viewed_entry_node->phone)))
                    message_requester("Unable to dial number.", FALSE);

                /* Whether the number was dialed or not, close audio.device. */
                close_audio_device();
            }
            else
                message_requester("Unable to open audio device.\n"
                    "(Channels are probably in use by\n"
                    "another application.)", FALSE);
        }
    }
}

/* MENU ACTIONS. */
/* Makes a new project. */
void new_project(void)
{
    if (!IsListEmpty((struct List *) entry_list))
    {
        /* Confirm action. */
        if (message_requester("Do you really want to\nstart a new project?", TRUE))
        {
            /* Clear the entry list and create a new one. */
            close_entry_list();
            open_entry_list();
            viewed_entry_node = NULL;

            update_main_window_display();
            update_main_window_buttons();

            /* Make the project filename empty so we will ask the user to
             * enter a filename if they select Save. */
            strcpy(project_filename, "");
            search_active = FALSE;

            project_changed = FALSE;
        }
    }
    else
        message_requester("No project to delete.", FALSE);
}

/* Opens a project from disk. */
void open_project(void)
{
    UBYTE filename[FILENAME_STRING_SIZE];

    if (show_file_requester(filename, "Select File to Open", FALSE))
    {
        /* Update the project filename. */
        strcpy(project_filename, filename);

        /* Delete the current list, and make a new one. */
        close_entry_list();
        open_entry_list();

        /* Load it in. */
        if (load_entry_list(project_filename))
        {
            /* Just in case we have loaded in an empty list, make the current
             * displayed pointer NULL. */
            if (!(IsListEmpty((struct List *) entry_list)))
                viewed_entry_node = (struct entry_node_type *) entry_list->mlh_Head;
            else
                viewed_entry_node = NULL;

            /* Clear the search parameters. */
            search_active = FALSE;
        }
        /* If we had problems loading, then make a new list.  Unfortunately,
         * this removes the old address book entries from memory.  I will have
         * to fix this by using a temporary list during the loading, and then
         * copying this over to the proper list if the load was successful.
         * That way, if there was an error, I could simply restore the old
         * entry list. */
        else
        {
            message_requester("Unable to open project.", FALSE);

            close_entry_list();
            open_entry_list();

            viewed_entry_node = NULL;

            strcpy(project_filename, "");
        }

        update_main_window_display();
        update_main_window_buttons();

        project_changed = FALSE;
    }
}

/* Saves a project with the current name. */
void save_project(void)
{
    UBYTE filename[FILENAME_STRING_SIZE];

    if (!(IsListEmpty((struct List *) entry_list)))
    {
        /* See if there is a project_filename. */
        if (strlen(project_filename))
        {
            /* Save the project under this filename. */
            if (!(save_entry_list(project_filename)))
                message_requester("Unable to save project.", FALSE);
            else
                project_changed = FALSE;
        }
        /* No project_filename setup yet. */
        else
        {
            /* Open a file requester. */
            if (show_file_requester(filename, "Select File to Save", TRUE))
            {
                /* If Ok is selected, then copy this filename to the requester
                 * filename, and save. */
                strcpy(project_filename, filename);

                if (!(save_entry_list(project_filename)))
                    message_requester("Unable to save project.", FALSE);
                else
                    project_changed = FALSE;
            }
        }
    }
    else
        message_requester("No project to save.", FALSE);
}

/* Saves the project with a new name. */
void save_as_project(void)
{
    UBYTE filename[FILENAME_STRING_SIZE];

    if (!(IsListEmpty((struct List *) entry_list)))
    {
        if (show_file_requester(filename, "Select File to Save", TRUE))
        {
            strcpy(project_filename, filename);

            /* Save it with the new filename specified. */
            if (!(save_entry_list(project_filename)))
                message_requester("Unable to save project.", FALSE);
            else
                project_changed = FALSE;
        }
    }
    else
        message_requester("No project to save.", FALSE);
}

/* Prints the project. */
void print_project(void)
{
    struct entry_node_type *entry_node;
    BOOL printer_error = FALSE;
    UBYTE header_string[GENERAL_STRING_SIZE];
    LONG max_entry_count = 0;

    if (!(IsListEmpty((struct List *) entry_list)))
    {
        /* See if we can open the printer file. */
        if (open_printer_file())
        {
            /* Start at the head of the list. */
            entry_node = (struct entry_node_type *) entry_list->mlh_Head;
 
            /* Print out each entry, and check for errors. */
            while (entry_node->node.mln_Succ && !printer_error)
            {
                sprintf(header_string, "--- Entry %d of %d ---\n",
                    entry_node->entry_node_number, number_of_entry_nodes);

                /* See if this entry should be printed.  Depends on weather
                 * all entries should be printed, otherwise this entry is
                 * only printed if it is the viewed entry. */
                if (settings.print_all || entry_node == viewed_entry_node)
                {
                    printer_error = !write_entry_to_printer_file(entry_node,
                        header_string);
                }

                /* Go to next entry, */
                entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
            }

            close_printer_file();

            /* Print a message if there was an error while printing. */
            if (printer_error)
            {
                message_requester("Unable to print.  (Printer\n"
                    "has probably run out of paper.)", FALSE);
            }
        }
        else
        {
            message_requester("Unable to open printer device.\n"
                "(Printer is probably being used\n"
                "by another application.)", FALSE);
        }
    }
    else
        message_requester("No project to print.", FALSE);
}

/* Opens the About window. */
void show_about_requester(void)
{
    UBYTE about_text[1000];
    LONG byte_count = 0;
    struct entry_node_type *entry_node;
    LONG address_count;

    /* Start at the head of the list. */
    entry_node = (struct entry_node_type *) entry_list->mlh_Head;

    while (entry_node->node.mln_Succ)
    {
        /* For each entry, add together the memory used to hold the strings. */
        if (entry_node->name)
            byte_count += strlen(entry_node->name);

        for (address_count = 0; address_count < 5; address_count++)
        {
            if (entry_node->address[address_count])
                byte_count += strlen(entry_node->address[address_count]);
        }

        if (entry_node->phone)
            byte_count += strlen(entry_node->phone);

        if (entry_node->email)
            byte_count += strlen(entry_node->email);

        if (entry_node->comment)
            byte_count += strlen(entry_node->comment);

        entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
    }

    /* Construct a string holding the about requester text. */
    sprintf(about_text,
        "%s\n"
        "Compilation date: %s\n"
        "\n"
        "This program is FREEWARE!\n"
        "\n"
        "E-mail: manningl@ee.port.ac.uk\n"
        "\n"
        "ARexx port name: %s\n"
        "\n"
        "Project contains %d entries, with\n"
        "%d bytes used.",
        version_string, compilation_date_string, AREXX_PORT_NAME,
            number_of_entry_nodes, byte_count);

    message_requester(about_text, FALSE);
}

/* Opens the search window and performs a search. */
void search_entries(void)
{
    struct entry_node_type *temp_entry_node;

    /* See if there are entries in the list. */
    if (!(IsListEmpty((struct List *) entry_list)))
    {
        /* Open the window. */
        open_string_window();

        /* See if there are valid search params to show it in the cycle and
         * string gadgets. */
        if (search_active)
        {
            GT_SetGadgetAttrs(string_window_gadget_list[GADID_TYPE_CYCLE], string_window,
                NULL, GTCY_Active, search_type, TAG_DONE);
            GT_SetGadgetAttrs(string_window_gadget_list[GADID_STRING_STRING],
                 string_window, NULL, GTST_String, search_string, TAG_DONE);
        }
        else
        {
            search_type = SEARCH_FOR_NAME;
            strcpy(search_string, "");
        }

        /* Call the event handler.  Continue when the user has Ok'd or Cancel'd
         * the requester window. */
        string_window_events();

        /* Now see if the user selected Ok. */
        if (string_window_result == STRING_WINDOW_RESULT_OK)
        {
            /* Is there a string in the string gadget?  Should be, as we
             * checked in the string window's event handler, but just in
             * case... */
            if (strlen_gadget(string_window_gadget_list[GADID_STRING_STRING]))
            {
                strcpy_gadget(string_window_gadget_list[GADID_STRING_STRING], 
                    search_string);

                /* Note: current_search_type is updated by 
                 * string_window_gadgets(). */
                search_type = current_search_type;

                search_active = TRUE;

                /* Perform the search. */
                temp_entry_node = entry_search(viewed_entry_node, FALSE);

                /* Match found? */
               if (temp_entry_node)
               {
                   viewed_entry_node = temp_entry_node;
                   update_main_window_display();
                   update_main_window_buttons();
               }
               else
                   DisplayBeep(pub_screen);
            }
        }
        close_string_window();
    }
    /* No entries then... */
    else
        message_requester("No project to search.", FALSE);
}

/* Repeats a search. */
void search_entries_again()
{
    struct entry_node_type *temp_entry_node;

    /* See if there are entries in the list. */
    if (!(IsListEmpty((struct List *) entry_list)))
    {
        /* See if there is a valid search set up. */
        if (search_active)
        {
            /* Perform the search. */
            temp_entry_node = entry_search(viewed_entry_node, TRUE);
 
            /* Match found? */
            if (temp_entry_node)
            {
                viewed_entry_node = temp_entry_node;
                update_main_window_display();
                update_main_window_buttons();

            }
            /* Nothing found. */
            else
                DisplayBeep(pub_screen);
        }
        /* No search defined yet. */
        else
            message_requester("No search defined.", FALSE);
    }
    /* No entries to search. */
    else
        message_requester("No project to search.", FALSE);
}

/* This function simply calls the save settings function. */
void save_settings(void)
{
    if (!(write_tooltype_settings()))
        message_requester("Unable to save settings.", FALSE);
}
