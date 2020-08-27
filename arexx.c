/* Address Book
 * by Lawrence Manning 
 *
 * filename: arexx.c
 */

#include "ab.h"
#include "externals.h"
#include "arexx_commands.h"

/* Opens a message port with the name AREXX_PORT_NAME.  Checks first for a
 * port of that name existing in the system. */
void open_arexx_port(void)
{
    UBYTE error_message[GENERAL_STRING_SIZE];

    /* We need to disable multitasking now. */
    Forbid();

    /* See if there is already a message port of the required name.  If there
     * isn't then try to open one. */
    if (!(FindPort(AREXX_PORT_NAME)))
    {
        if (!(arexx_port = CreatePort(AREXX_PORT_NAME, 0)))
        {
            Permit();
            clean_up(RETURN_FAIL, "Unable to create arexx port.");
        }
    }
    /* If the message port already exists, then we have to quit. */
    else
    {
        Permit();
        sprintf(error_message, "An arexx port called %s is already in use.",
            AREXX_PORT_NAME);
        clean_up(RETURN_FAIL, error_message);
    }

    /* Renable multitasking. */
    Permit();
}

/* This function closes the message port.  It removes any messages waiting
 * at the port before doing so. */
void close_arexx_port(void)
{
    struct RexxMsg *arexx_message;

    /* See if there is a message port to remove. */
    if (arexx_port)
    {
        /* Disable multitasking to stop any new messages arriving. */
        Forbid();

        /* Remove any messages. */
        while (arexx_message = (struct RexxMsg *) GetMsg(arexx_port))
        {
            arexx_message->rm_Result1 = RC_FATAL;
            arexx_message->rm_Result2 = (LONG) NULL;
            ReplyMsg((struct Message *) arexx_message);
        }

        /* Now delete the port itself. */
        DeletePort(arexx_port);

        /* Now we can renable multitasking. */
        Permit();

        arexx_port = NULL;
    }
}

/* Command to hide AB's interface. */
void arexx_hide(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    make_hidden = TRUE;

    *result1 = RC_OK;
}

/* Command to show AB's interface. */
void arexx_show(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    make_shown = TRUE;

    *result1 = RC_OK;
}

/* The following functions carry out simular functions to the functions
 * in actions.c. */
/* Gotos the first entry. */
void arexx_goto_first_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* Only works if we are not already at the start of the list. */
    if (viewed_entry_node != (struct entry_node_type *) entry_list->mlh_Head &&
        viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) entry_list->mlh_Head;
        update_main_window_display();
        update_main_window_buttons();

        *result1 = RC_OK;
    }
    else
        *result1 = RC_WARN;;
}

/* Gotos the previos entry. */
void arexx_goto_prev_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* Only works if we are not already at the start of the list. */
    if (viewed_entry_node != (struct entry_node_type *) entry_list->mlh_Head &&
        viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) viewed_entry_node->node.mln_Pred;
         update_main_window_display();
         update_main_window_buttons();

        *result1 = RC_OK;
    }
    else
        *result1 = RC_WARN;
}

/* Gotos the next entry. */
void arexx_goto_next_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* Only works if we are not already at the end of the list. */
    if (viewed_entry_node != (struct entry_node_type *) entry_list->mlh_TailPred &&
        viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) viewed_entry_node->node.mln_Succ;
        update_main_window_display();
        update_main_window_buttons();

        *result1 = RC_OK;
    }
    else
        *result1 = RC_WARN;
}

/* Gotos the last entry. */
void arexx_goto_last_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* Only works if we are not already at the end of the list. */
    if (viewed_entry_node != (struct entry_node_type *) entry_list->mlh_TailPred &&
        viewed_entry_node)
    {
        viewed_entry_node = (struct entry_node_type *) entry_list->mlh_TailPred;
        update_main_window_display();
        update_main_window_buttons();

        *result1 = RC_OK;
    }
    else
        *result1 = RC_WARN;
}

/* Deletes the current entry. */
void arexx_delete_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    struct entry_node_type *temp_entry_node;

    strcpy(result2, "");

    /* All of this code comes from the delete_entry() function in actions.c */
    if (viewed_entry_node)
    {
        if (viewed_entry_node == (struct entry_node_type *) entry_list->mlh_Head &&
            viewed_entry_node == (struct entry_node_type *) entry_list->mlh_TailPred)
        {
            temp_entry_node = NULL;
        }
        else
        {
            if (viewed_entry_node != (struct entry_node_type *) entry_list->mlh_Head)
            {
                temp_entry_node = (struct entry_node_type *)
                    viewed_entry_node->node.mln_Pred;
            }
            else
            {
                temp_entry_node = (struct entry_node_type *)
                    viewed_entry_node->node.mln_Succ;
            }
        }
 
        remove_entry_node_from_list(viewed_entry_node);
        close_entry_node(viewed_entry_node);

        viewed_entry_node = temp_entry_node;

        update_main_window_display();
        update_main_window_buttons();

        project_changed = TRUE;

        *result1 = RC_OK;
    }
    else
        *result1 = RC_WARN;
}

/* Tone-dials the current entry. */
void arexx_dial_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    if (viewed_entry_node)
    {
        /* See if this entry has a phone number set. */
        if (viewed_entry_node->phone)
        {
            /* Can we open the audio.device? */
            if (open_audio_device())
            {
                /* If so, try and idal the number. */
                if (tone_dial_number(viewed_entry_node->phone))
                    *result1 = RC_OK;
                else
                    *result1 = RC_WARN;;

                /* Whether the number was dialed or not, close audio.device. */
                close_audio_device();
            }
            else
                *result1 = RC_WARN;
        }
        else
            *result1 = RC_WARN;
    }
    else
        *result1 = RC_WARN;
}

/* This arexx function tone dials a number specified in the argument. */
void arexx_dial_number(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* See if an argument was specified. */
    if (strlen(arguments))
    {
        /* Can we open the audio.device? */
        if (open_audio_device())
        {
            /* If so, try and idal the number. */
            if (tone_dial_number(arguments))
                *result1 = RC_OK;
            else
                *result1 = RC_WARN;;

            /* Whether the number was dialed or not, close audio.device. */
            close_audio_device();
        }
        else
            *result1 = RC_WARN;
    }
    else
        *result1 = RC_ERROR;
}
    
/* Makes a new project. */
void arexx_new_project(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    if (!IsListEmpty((struct List *) entry_list))
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

        *result1 = RC_OK;
    }
    else
        *result1 = RC_WARN;

}

/* Opens a project stored on disk. */
void arexx_open_project(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* In this function, the argument is the filename. */
    if (strlen(arguments) > 0)
    {
        /* Update the project filename. */
        strcpy(project_filename, arguments);

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

            *result1 = RC_OK;
        }
        /* If we had problems loading, then make a new list. */
        else
        {
            close_entry_list();
            open_entry_list();

            viewed_entry_node = NULL;

            strcpy(project_filename, "");

            *result1 = RC_WARN;
        }

        update_main_window_display();
        update_main_window_buttons();

        project_changed = FALSE;
    }
    /* This error occures if we have not been given a filename in the 
     * argument. */
    else
        *result1 = RC_ERROR;
}

/* Saves the current project under its current name. */
void arexx_save_project(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    if (!(IsListEmpty((struct List *) entry_list)))
    {
        /* See if there is a project_filename. */
        if (strlen(project_filename))
        {
            /* Save the project under this filename. */
            if (save_entry_list(project_filename))
                *result1 = RC_OK;
            else
                *result1 = RC_WARN;
        }
        /* No project_filename setup yet. */
        else
            *result1 = RC_WARN;
    }
    else
        *result1 = RC_WARN;
}

/* Saves the project under a new name. */
void arexx_save_as_project(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* In this function, the argument is the filename. */
    if (strlen(arguments) > 0)
    {
        if (!(IsListEmpty((struct List *) entry_list)))
        {
            strcpy(project_filename, arguments);

            /* Save it with the new filename specified. */
            if (save_entry_list(project_filename))
                *result1 = RC_OK;
            else
                *result1 = RC_WARN;
        }
        /* Error caused by there being an empty project. */
        else
            *result1 = RC_WARN;
    }
    /* This error caused by there being no argument. */
    else
       *result1 = RC_ERROR;
}

/* Searches for an entry. */
void arexx_search_entries(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    struct entry_node_type *temp_entry_node;

    strcpy(result2, "");

    /* See if the arguments are ok. */
    if (strlen(arguments) > 3 && arguments[0] >= '1' && arguments[0] <= '3')
    {
        /* See if there are entries in the list. */
        if (!(IsListEmpty((struct List *) entry_list)))
        {
            /* Now we can set up the search. */
            strcpy(search_string, &arguments[2]);
            search_type = arguments[0] - '0' - 1;
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
            {
                /* If the search was unsuccesful, then copy a string to
                 * Result2. */
                strcpy(result2, "Not found");
            }

            /* Regardless of weather the search was succesful or not, set
             * result1 to RC_OK.  We can seee if the search was succesful by
             * looking at the result variable in the ARexx script. */
            *result1 = RC_OK;
        }
        else
            *result1 = RC_WARN;
    }
    else
        *result1 = RC_ERROR;
}

/* Repeats a search. */
void arexx_search_entries_again(LONG *result1, UBYTE *result2,
    UBYTE *arguments)
{
    struct entry_node_type *temp_entry_node;

    strcpy(result2, "");

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
                strcpy(result2, "Not found");

            *result1 = RC_OK;
        }
        /* No search defined yet. */
        else
            *result1 = RC_WARN;
    }
    /* No entries to search. */
    else
        *result1 = RC_WARN;
}

/* Saves the current settings to the program's tooltypes. */
void arexx_save_settings(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    if (write_tooltype_settings())
        *result1 = RC_OK;
    else
        *result2 = RC_WARN;
}

/* Gets the field details for the current entry. */
void arexx_get_entry_details(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    LONG field_number;

    strcpy(result2, "");

    /* In this function, the argument is the field number. */
    if (strlen(arguments) > 0 && arguments[0] >= '1' && arguments[0] <= '9')
    {
        if (!(IsListEmpty((struct List *) entry_list)))
        {
            /* Convert the field number in char form to an integer. */
            field_number = arguments[0] - '0';

            /* Copy the relevant field to result2, if it contains text. */
            if (field_number == 1)
            {
                if (viewed_entry_node->name)
                    strcpy(result2, viewed_entry_node->name);
            }

            if (field_number >=2 && field_number <= 6)
            {
                if (viewed_entry_node->address[field_number - 2])
                    strcpy(result2, viewed_entry_node->address[field_number - 2]);
            }

            if (field_number == 7)
            {
                if (viewed_entry_node->phone)
                    strcpy(result2, viewed_entry_node->phone);
            }

            if (field_number == 8)
            {
                if (viewed_entry_node->email)
                    strcpy(result2, viewed_entry_node->email);
            }

            if (field_number == 9)
            {
                if (viewed_entry_node->comment)
                    strcpy (result2, viewed_entry_node->comment);
            }

            /* If the field was empty, then set Result2 to indicate this. */
            if (strlen(result2) == 0)
                strcpy(result2, "Field empty");

            *result1 = RC_OK;
        }
        else
            /* This error is for there being no entries in the project. */
            *result1 = RC_WARN;
    }
    else
        /* This error is caused by there being no entries to search. */
        *result1 = RC_ERROR;
}

/* Makes a new entry with the given name. */
void arexx_new_entry(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    strcpy(result2, "");

    /* In this function, the argument is the text to copy into the name field
     * of the new entry. */
    if (strlen(arguments) > 0)
    {
        /* Make a new node. */
        viewed_entry_node = open_entry_node();

        /* Allocate memory for the string and copy it across. */
        if (!(viewed_entry_node->name = AllocMem(strlen(arguments) + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);

        strcpy(viewed_entry_node->name, arguments);        

        /* Link it into the project list and update the main window. */
        link_entry_node_to_list(viewed_entry_node);

        update_main_window_display();
        update_main_window_buttons();
        project_changed = TRUE;

        *result1 = RC_OK;
    }
    else
        *result1 = RC_ERROR;
}

/* Changes a specific field in the current entry. */
void arexx_put_entry_details(LONG *result1, UBYTE *result2, UBYTE *arguments)
{
    struct entry_node_type *entry_node = NULL;
    LONG field_number;
    UBYTE *field_text;

    strcpy(result2, "");

    /* See if the arguments are ok.  Note that the check for trying to clear
     * the name field in an entry is a fudge, and should be improved. */
    if (arguments[0] >= '1' && arguments[0] <= '9' && strlen(arguments) >= 3 &&
        strcmp(arguments, "1 Field empty") != 0)
    {
        /* Now see if there is an entry to use. */
        if (viewed_entry_node)
        {
            /* Find the field number and the field text. */
            field_number = arguments[0] - '0';
            field_text = &arguments[2];

            /* Names are dealt with differently from the rest. */
            if (field_number == 1)
            {
                /* Temporarily remove the entry from the list. */
                remove_entry_node_from_list(viewed_entry_node);

                /* Copy the new text over the old.  Note that the entry should
                 * always have a name, but it's checked here anyway. */
                if (viewed_entry_node->name)
                {
                    FreeMem(viewed_entry_node->name, strlen(entry_node->name) + 1);
                    viewed_entry_node->name = NULL;
                }

                if (!(viewed_entry_node->name = AllocMem(strlen(field_text) + 1,
                    MEMF_CLEAR)))
                {
                    clean_up(RETURN_FAIL, allocmem_failure_string);
                }
           
                strcpy(viewed_entry_node->name, field_text);
                    
                /* Now put the entry node back into the list. */
                link_entry_node_to_list(viewed_entry_node);
            }

            /* Addresses. */
            if (field_number >= 2 && field_number <= 6)
            {
                /* Free text memory, if an address line is already present. */
                if (viewed_entry_node->address[field_number - 2])
                {
                    FreeMem(viewed_entry_node->address[field_number - 2],
                        strlen(viewed_entry_node->address[field_number - 2]) + 1);
                    viewed_entry_node->address[field_number - 2] = NULL;
                }

                /* See if the string should be left empty. */
                if (strcmp(field_text, "Field empty") != 0)
                {
                    /* Copy the new address text into the entry node. */
                    if (!(viewed_entry_node->address[field_number - 2] =
                        AllocMem(strlen(field_text) + 1, MEMF_CLEAR)))
                    {
                        clean_up(RETURN_FAIL, allocmem_failure_string);
                    }

                    strcpy(viewed_entry_node->address[field_number - 2],
                        field_text);
                }
            }

            /* Phone numbers. */
            if (field_number == 7)
            {
                /* Free text memory, if a phone number is already present. */
                if (viewed_entry_node->phone)
                {
                    FreeMem(viewed_entry_node->phone,
                        strlen(viewed_entry_node->phone) + 1);
                    viewed_entry_node->phone = NULL;
                }

                /* See if the string should be left empty. */
                if (strcmp(field_text, "Field empty") != 0)
                {
                    /* Copy the new phone number into the entry node. */
                    if (!(viewed_entry_node->phone =
                        AllocMem(strlen(field_text) + 1, MEMF_CLEAR)))
                    {
                        clean_up(RETURN_FAIL, allocmem_failure_string);
                    }

                    strcpy(viewed_entry_node->phone, field_text);
                }
            }

            /* E-mail address. */
            if (field_number == 8)
            {
                /* Free text memory, if a E-mail address is already present. */
                if (viewed_entry_node->email)
                {
                    FreeMem(viewed_entry_node->email,
                         strlen(viewed_entry_node->email) + 1);
                    viewed_entry_node->email = NULL;
                }

                /* See if the string should be left empty. */
                if (strcmp(field_text, "Field empty") != 0)
                {
                    /* Copy the new phone number into the entry node. */
                    if (!(viewed_entry_node->email =
                        AllocMem(strlen(field_text) + 1, MEMF_CLEAR)))
                    {
                        clean_up(RETURN_FAIL, allocmem_failure_string);
                    }

                    strcpy(viewed_entry_node->email, field_text);
                }
            }

            /* Comments. */
            if (field_number == 9)
            {
                /* Free text memory, if a comment is already present. */
                if (viewed_entry_node->comment)
                {
                    FreeMem(viewed_entry_node->comment,
                         strlen(viewed_entry_node->comment) + 1);
                    viewed_entry_node->comment= NULL;
                }

                /* See if the string should be left empty. */
                if (strcmp(field_text, "Field empty") != 0)
                {
                    /* Copy the new phone number into the entry node. */
                    if (!(viewed_entry_node->comment =
                        AllocMem(strlen(field_text) + 1, MEMF_CLEAR)))
                    {
                        clean_up(RETURN_FAIL, allocmem_failure_string);
                    }

                    strcpy(viewed_entry_node->comment, field_text);
                }
            }

            update_main_window_display();
            update_main_window_buttons();
            project_changed = TRUE;

            *result1 = RC_OK;
        }
        else
            /* This failure caused by the project being empty. */
            *result1 = RC_WARN;
    }
    else
        /* This failure caused by the arguments being invalid. */
        *result1 = RC_ERROR;
}
