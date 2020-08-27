/* Address Book
 * by Lawrence Manning 
 *
 * filename: settings.c
 */

#include "ab.h"
#include "externals.h"

/* This function reads the settings from the tool icon's tooltypes, and copies
 * the values into the settings structure.  If any of the values couldn't be
 * found, then the defaults will be used instead.  Also, if the program file
 * has no icon, then the defaults will be used. */
void read_tooltype_settings(void)
{
    struct DiskObject *icon_diskobject;
    UBYTE **icon_tooltypes; /* Points to an array of string pointers. */
    UBYTE *value;

    /* Reset settings to defaults so that any entires in the icons tooltypes
     * that are unused will be at there default value. */
    settings.main_window_left = default_settings.main_window_left;
    settings.main_window_top = default_settings.main_window_top;
    settings.main_window_width = default_settings.main_window_width;
    settings.edit_window_left = default_settings.edit_window_left;
    settings.edit_window_top = default_settings.edit_window_top;
    settings.edit_window_width = default_settings.edit_window_width;
    settings.dial_duration = default_settings.dial_duration;
    settings.dial_delay = default_settings.dial_delay;
    strcpy(settings.public_screen, default_settings.public_screen);
    settings.print_headers = default_settings.print_headers;
    settings.print_all = default_settings.print_all;
    settings.cx_priority = default_settings.cx_priority;
    strcpy(settings.cx_popkey, default_settings.cx_popkey);
    settings.cx_popup = default_settings.cx_popup;
    settings.old_sorting = default_settings.old_sorting;

    /* Read in the DiskObject. */
    if (icon_diskobject = GetDiskObject(program_filename))
    {     
        /* Copy the tooltype pointer. */
        icon_tooltypes = icon_diskobject->do_ToolTypes;

        /* Find the values in the tooltypes, and copy them into the
         * settings structure. */
        /* General tooltypes. */
        if (value = FindToolType(icon_tooltypes, "MAIN_WINDOW_LEFT"))
            settings.main_window_left = atol(value);
        if (value = FindToolType(icon_tooltypes, "MAIN_WINDOW_TOP"))
            settings.main_window_top = atol(value);
        if (value = FindToolType(icon_tooltypes, "MAIN_WINDOW_WIDTH"))
            settings.main_window_width = atol(value);
        if (value = FindToolType(icon_tooltypes, "EDIT_WINDOW_LEFT"))
            settings.edit_window_left = atol(value);
        if (value = FindToolType(icon_tooltypes, "EDIT_WINDOW_TOP"))
            settings.edit_window_top = atol(value);
        if (value = FindToolType(icon_tooltypes, "EDIT_WINDOW_WIDTH"))
            settings.edit_window_width = atol(value);
        if (value = FindToolType(icon_tooltypes, "DIAL_DURATION"))
            settings.dial_duration = atol(value);
        if (value = FindToolType(icon_tooltypes, "DIAL_DELAY"))
            settings.dial_delay = atol(value);
        if (value = FindToolType(icon_tooltypes, "PUBLIC_SCREEN"))
            strcpy(settings.public_screen, value);

        /* Settings menu tooltypes. */
        if (value = FindToolType(icon_tooltypes, "PRINT_HEADERS"))
            settings.print_headers = string_to_bool(value);
        if (value = FindToolType(icon_tooltypes, "PRINT_ALL"))
            settings.print_all = string_to_bool(value);

        /* Commodity tooltypes. */
        if (value = FindToolType(icon_tooltypes, "CX_PRIORITY"))
            settings.cx_priority = atol(value);
        if (value = FindToolType(icon_tooltypes, "CX_POPKEY"))
            strcpy(settings.cx_popkey, value);
        if (value = FindToolType(icon_tooltypes, "CX_POPUP"))
            settings.cx_popup = string_to_bool(value);

        /* Misc. tooltypes. */
        if (value = FindToolType(icon_tooltypes, "OLD_SORTING"))
            settings.old_sorting = string_to_bool(value);

        /* Free the DiskObject.  This is important! */
        FreeDiskObject(icon_diskobject);
    }
}
            
/* This function attempts to save the current settings to the tool icon's
 * tooltypes.  Returns TRUE for success, or FALSE for failure. */
BOOL write_tooltype_settings(void)
{
    struct DiskObject *icon_diskobject;
    UBYTE **old_icon_tooltypes;
    UBYTE *new_icon_tooltypes[WRITEABLE_TOOLTYPES + 1];
    BOOL success = FALSE;
    LONG count;
    UBYTE temp_string[GENERAL_STRING_SIZE];

    /* Allocate memory for the new tooltype strings. */
    for (count = 0; count < WRITEABLE_TOOLTYPES; count++)
    {
        if (!(new_icon_tooltypes[count] = AllocMem(GENERAL_STRING_SIZE, MEMF_CLEAR)))
            return (FALSE);
    }
    new_icon_tooltypes[WRITEABLE_TOOLTYPES] = NULL;

    /* Copy the position of the main window to the settings structure. */
    store_main_window_position();

    /* First see if there is a icon file on the disk. */
    if (!(icon_diskobject = GetDiskObject(program_filename)))
    {
        /* If not, then try and open the default icon for a tool. */
        icon_diskobject = GetDefDiskObject(WBTOOL);
    }

    /* If either of the above were successful, then write out the settings in
     * the icon's tooltypes. */
    if (icon_diskobject)
    {
        old_icon_tooltypes = icon_diskobject->do_ToolTypes;

        /* This tooltype is always writen. */
        sprintf(new_icon_tooltypes[0], "(DONOTWAIT)");

        /* General tooltypes. */
        sprintf(new_icon_tooltypes[1], "MAIN_WINDOW_LEFT=%d", settings.main_window_left);
        sprintf(new_icon_tooltypes[2], "MAIN_WINDOW_TOP=%d", settings.main_window_top);
        sprintf(new_icon_tooltypes[3], "MAIN_WINDOW_WIDTH=%d", settings.main_window_width);
        sprintf(new_icon_tooltypes[4], "EDIT_WINDOW_LEFT=%d", settings.edit_window_left);
        sprintf(new_icon_tooltypes[5], "EDIT_WINDOW_TOP=%d", settings.edit_window_top);
        sprintf(new_icon_tooltypes[6], "EDIT_WINDOW_WIDTH=%d", settings.edit_window_width);
        sprintf(new_icon_tooltypes[7], "DIAL_DURATION=%d", settings.dial_duration);
        sprintf(new_icon_tooltypes[8], "DIAL_DELAY=%d", settings.dial_delay);
        if (strlen(settings.public_screen))
            sprintf(new_icon_tooltypes[9], "PUBLIC_SCREEN=%s", settings.public_screen);
        else
            sprintf(new_icon_tooltypes[9], "(PUBLIC_SCREEN=<public screen>)");

        /* Settings menu tooltypes. */
        bool_to_string(temp_string, settings.print_headers);
        sprintf(new_icon_tooltypes[10], "PRINT_HEADERS=%s", temp_string);
        bool_to_string(temp_string, settings.print_all);
        sprintf(new_icon_tooltypes[11], "PRINT_ALL=%s", temp_string);

        /* Commodity tooltypes. */
        sprintf(new_icon_tooltypes[12], "CX_PRIORITY=%d", settings.cx_priority);
        if (strlen(settings.cx_popkey))
            sprintf(new_icon_tooltypes[13], "CX_POPKEY=%s", settings.cx_popkey);
        else
            sprintf(new_icon_tooltypes[13], "(CX_POPKEY=<hotkey>)");
        bool_to_string(temp_string, settings.cx_popup);
        sprintf(new_icon_tooltypes[14], "CX_POPUP=%s", temp_string);

        /* Misc. tooltypes. */
        bool_to_string(temp_string, settings.old_sorting);
        sprintf(new_icon_tooltypes[15], "OLD_SORTING=%s", temp_string);

        /* Point the disk object tooltypes to those constructed above. */
        icon_diskobject->do_ToolTypes = new_icon_tooltypes;

        /* Try and write them out. */
        success = PutDiskObject(program_filename, icon_diskobject);

        /* Restore the old tooltype pointer so we can free the DiskObject
         * properly. */
        icon_diskobject->do_ToolTypes = old_icon_tooltypes;
        FreeDiskObject(icon_diskobject);
    }
    else
        success = FALSE;

    /* Free memory used by the new tooltype strings. */
    for (count = 0; count < 5; count++)
    {
        if (new_icon_tooltypes[count])
            FreeMem(new_icon_tooltypes[count], GENERAL_STRING_SIZE);
    }

    return (success);
}

/* This function converts a string into it's boolean equivilant.  Passing the
 * string "YES" returns TRUE, else it returns FALSE. */
BOOL string_to_bool(UBYTE *string)
{
    if (strcmp(string, "YES") == 0)
        return (TRUE);
    else
        return (FALSE);
}

/* This function does the reverse of the one above.  ie. it converts a BOOL
 * into the equivilant string. */
void bool_to_string(UBYTE *string, BOOL boolean)
{
    if (boolean == TRUE)
        strcpy (string, "YES");
    else
        strcpy (string, "NO");
}
