/* Address Book
 * by Lawrence Manning 
 *
 * filename: misc.c
 */

#include "ab.h"
#include "externals.h"

/* Open intuition and gadtools librares, checking to see if they opened okay. */
void open_libraries(void)
{
    if (!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 39)))
        clean_up(RETURN_FAIL, "Unable to open intution.libary v39.");

    if (!(GadToolsBase = OpenLibrary("gadtools.library", 39)))
        clean_up(RETURN_FAIL, "Unable to open gadtools.library v39.");

    if (!(AslBase = OpenLibrary("asl.library", 37)))
        clean_up(RETURN_FAIL, "Unable to open asl.library v37.");

    if (!(IconBase = OpenLibrary("icon.library", 36)))
        clean_up(RETURN_FAIL, "Unable to open icon.library v36.");

    if (!(GfxBase = (struct GfxBase *) OpenLibrary("graphics.library", 0)))
        clean_up(RETURN_FAIL, "Unable to open any version of graphics.library.");

    if (!(RexxSysBase = (struct RxsLib *) OpenLibrary("rexxsyslib.library", 0)))
        clean_up(RETURN_FAIL, "Unable to open any version of rexxsyslib.library.");

    if (!(CxBase = OpenLibrary("commodities.library", 37)))
        clean_up(RETURN_FAIL, "Unable to open commodities.library v37.");
}

/* Close any libraries that are open, and reset their bases to NULL. */
void close_libraries(void)
{
    if (IntuitionBase)
    {
        CloseLibrary((struct Library *) IntuitionBase);
        IntuitionBase = NULL;
    }

    if (GadToolsBase)
    {
        CloseLibrary(GadToolsBase);
        GadToolsBase = NULL;
    }

    if (AslBase)
    {
        CloseLibrary(AslBase);
        AslBase = NULL;
    }

    if (IconBase)
    {
        CloseLibrary(IconBase);
        IconBase = NULL;
    }

    if (GfxBase)
    {
        CloseLibrary((struct Library *) GfxBase);
        GfxBase = NULL;
    }

    if (RexxSysBase)
    {
        CloseLibrary((struct Library *) RexxSysBase);
        RexxSysBase = NULL;
    }

    if (CxBase)
    {
        CloseLibrary(CxBase);
        CxBase = NULL;
    }
}

/* This function copies the contents of a string gadget to the buffer. */
void strcpy_gadget(struct Gadget *gadget, UBYTE *string)
{
    strcpy(string, ((struct StringInfo *) gadget->SpecialInfo)->Buffer);
}

/* This functon returns the number of chars used in a string gadget. */
LONG strlen_gadget(struct Gadget *gadget)
{
    return (strlen(((struct StringInfo *) gadget->SpecialInfo)->Buffer));
}

/* This function writes out a string to the file handle. */
BOOL write_string(UBYTE *string, BPTR file_handle)
{
    /* See if there is a valid string here. */
    if (string)
    {
        /* Write out the length of the the string as a BYTE. */
        if ((FPutC(file_handle, strlen(string))) == -1)
            return (FALSE);

        /* Write out the string, not including the end NULL. */
        if ((FPuts(file_handle, string)) == EOF)
            return (FALSE);
    }
    else /* If there isn't a string, then write out a 0. */
    {
        if ((FPutC(file_handle, 0)) == EOF)
            return (FALSE);
    }

    return (TRUE);
}

/* This function reads in a line from the file.  Returns TRUE for success or
 * FALSE for failure. */
BOOL read_string(UBYTE **string, BPTR file_handle)
{
    LONG length;

    /* Read in the length. */
    if ((length = FGetC(file_handle)) == -1)
        return (FALSE);

    /* If there is a valid string here, then read it in. */
    if (length)
    {
        /* Alloc memory. */
        if (!(*string = AllocMem(length + 1, MEMF_CLEAR)))
            return (FALSE);

        /* Read in the string. */
       if (!(FGets(file_handle, *string, length + 1)))
            return (FALSE);
    }

    return (TRUE);
}

/* This funciion is a case-insensitive version of strstr. */
BOOL stristr(UBYTE *string_one, UBYTE *string_two)
{
    UBYTE string_one_caps[GENERAL_STRING_SIZE];
    UBYTE string_two_caps[GENERAL_STRING_SIZE];
    LONG count;

    for (count = 0; count < strlen(string_one) + 1; count++)
        string_one_caps[count] = toupper(string_one[count]);

    for (count = 0; count < strlen(string_two) + 1; count++)
        string_two_caps[count] = toupper(string_two[count]);

    if (strstr(string_one_caps, string_two_caps))
        return (TRUE);
    else
        return (FALSE);
}

/* This funciion is called on startup and will open a project if the user
 * wants one opened automatically, either through a shell argument, or a
 * default tool in a project icon. */
void open_startup_project(void)
{
    /* First see if there is a startup project to open. */
    if (strlen(startup_project_filename))
    {
        /* Make a copy of this. */
        strcpy(project_filename, startup_project_filename);

        /* Try and load it in. */
        if (load_entry_list(startup_project_filename))
        {
            /* Set viewed_entry_node.  Will be NULL if there is no entries
             * in this file. */
            if (!(IsListEmpty((struct List *) entry_list)))
                viewed_entry_node = (struct entry_node_type *) entry_list->mlh_Head;
            else
                 viewed_entry_node = NULL;
        }
        else
        {
            /* Display warning message if workbench was used to load AB, else
             * quit. */
            if (workbench_loaded_flag)
            {
                message_requester("Unable to load project.", FALSE);
                close_entry_list();
                open_entry_list();
            }
            else
                clean_up(RETURN_FAIL, "Unable to load project.");
            
            viewed_entry_node = NULL;
        }
    }
}

