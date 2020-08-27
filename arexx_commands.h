/* Address Book
 * by Lawrence Manning 
 *
 * filename: arexx_commands.h
 */

/* This structure describes the ARexx command set for AB.  The first entry
 * is the command name, and the second is the address of the function to be
 * called for this ARexx command. */
struct arexx_command_type arexx_commands[] =
{
    {"HIDE", arexx_hide},
    {"SHOW", arexx_show},
    {"FIRST_ENTRY", arexx_goto_first_entry},
    {"PREV_ENTRY", arexx_goto_prev_entry},
    {"NEXT_ENTRY", arexx_goto_next_entry},
    {"LAST_ENTRY", arexx_goto_last_entry},
    {"DELETE_ENTRY", arexx_delete_entry},
    {"DIAL_ENTRY", arexx_dial_entry},
    {"DIAL_NUMBER", arexx_dial_number},
    {"NEW_PROJECT", arexx_new_project},
    {"OPEN_PROJECT", arexx_open_project},
    {"SAVE_PROJECT", arexx_save_project},
    {"SAVE_AS_PROJECT", arexx_save_as_project},
    {"SEARCH", arexx_search_entries},
    {"SEARCH_AGAIN", arexx_search_entries_again},
    {"SAVE_SETTINGS", arexx_save_settings},
    {"GET_ENTRY_DETAILS", arexx_get_entry_details},
    {"NEW_ENTRY", arexx_new_entry},
    {"PUT_ENTRY_DETAILS", arexx_put_entry_details},
    {NULL, NULL}
};
