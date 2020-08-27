/* Address Book
 * by Lawrence Manning 
 *
 * filename: main.c
 */

#include "ab.h"

/* GLOBALS. */
/* Version strings. */
/* This general version string is used by commodities so needs to be less then
 * forty charecters. */
UBYTE *program_name_string = PROGRAM_NAME;
UBYTE *version_string = PROGRAM_NAME " " COPYRIGHT;
UBYTE *compilation_date_string = COMPILATION_DATE;
UBYTE *VER = "$VER: " PROGRAM_NAME " (" COMPILATION_DATE ")";

/* Library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *GadToolsBase = NULL;
struct Library *AslBase = NULL;
struct Library *IconBase = NULL;
struct GfxBase *GfxBase = NULL;
struct RxsLib *RexxSysBase = NULL;
struct Library *CxBase = NULL;

/* Screen information. */
struct Screen *pub_screen = NULL;
APTR pub_screen_vi = NULL;
struct DrawInfo *pub_screen_draw_info = NULL;
struct Screen *previous_pub_screen = NULL;

/* Main window information. */
UBYTE main_window_title[GENERAL_STRING_SIZE] = "";
LONG main_window_width = 0; /* Internal width (sans border). */
LONG main_window_height = 0; /* Internal hight. */
WORD main_window_zoom_position[4] = {0, 0, 0, 0};
struct Window *main_window = NULL;
struct Gadget *main_window_gadget_list[NO_OF_MAIN_WINDOW_GADGETS];
struct Gadget *main_window_first_gadget = NULL;
struct Menu *main_window_menu = NULL;
BOOL main_window_menu_attached = FALSE;

/* Edit window information. */
LONG edit_window_width = 0;
LONG edit_window_height = 0;
struct Window *edit_window = NULL;
struct Gadget *edit_window_gadget_list[NO_OF_EDIT_WINDOW_GADGETS];
struct Gadget *edit_window_first_gadget = NULL;

/* String window infromation. */
LONG string_window_width = 0;
LONG string_window_height = 0;
struct Window *string_window = NULL;
struct Gadget *string_window_gadget_list[NO_OF_STRING_WINDOW_GADGETS];
struct Gadget *string_window_first_gadget = NULL;

/* Other GUI information. */
BOOL make_hidden = FALSE; BOOL make_shown = FALSE;
LONG ab_status = HIDDEN;

/* Event information. */
LONG edit_window_result = 0;
LONG string_window_result = 0;
UBYTE search_string[GENERAL_STRING_SIZE];
LONG search_type = SEARCH_FOR_NAME;
LONG current_search_type = SEARCH_FOR_NAME;
BOOL search_active = FALSE;

/* Address Book information. */
struct entry_list_type *entry_list = NULL;
LONG number_of_entry_nodes = 0;
struct entry_node_type *viewed_entry_node = NULL;
UBYTE project_filename[FILENAME_STRING_SIZE] = "";
BOOL project_changed = FALSE;
UBYTE startup_project_filename[FILENAME_STRING_SIZE] = "";

/* Printer information. */
BPTR printer_file = NULL;

/* Tone dial information. */
BYTE *sine_wave_data = NULL;
struct IOAudio *audio_io[2] = {NULL, NULL};
struct MsgPort *audio_port[2] = {NULL, NULL};
UBYTE allocate_which_channels[2][1] = { {1}, {2} }; 
BOOL audio_device_open[2] = {FALSE, FALSE};
LONG clock_rate = 0;

/* Arexx information. */
struct MsgPort *arexx_port = NULL;

/* Commodities information. */
struct MsgPort *commodity_port = NULL;
CxObj *commodity_broker = NULL;
CxObj *commodity_filter = NULL;
BOOL hotkey_enabled = FALSE;
UBYTE *commodity_name = "AB";
UBYTE *commodity_description = "An easy to use address book program";

/* Settings information. */
struct settings_type settings;
struct settings_type default_settings =
{
    50, 25, 0,                      /* Main window position. */
    100, 50, 0,                     /* Edit window position. */
    10, 5,                          /* Tone-dial settings. */
    "",                             /* Public screen to use. */
    TRUE,                           /* Print heaaders? */
    TRUE,                           /* Print all entries? */
    0,                              /* Commodity prioirty. */
    "ctrl alt a",                   /* Commodity popkey. */
    TRUE,                           /* Start AB shown. */
    FALSE                           /* Use new sorting method. */
};

/* Other globals. */
UBYTE *allocmem_failure_string = "Out of memory!";
UBYTE program_filename[FILENAME_STRING_SIZE];
BOOL workbench_loaded_flag;

/* This function closes down everything. */
void clean_up(UBYTE return_code, UBYTE *message)
{
    close_audio_device(); /* In case it is open when we quit. */
    close_printer_file(); /* Ditto. */
    close_edit_window(); /* Ditto. */
    close_string_window(); /* Ditto. */

    close_main_window();
    close_pub_screen_information();
    close_entry_list();
    close_arexx_port();
    close_commodity();
    close_audio_data();

    /* Print the erorr message, if there was one. */
    if (strlen(message))
    {
        /* If intuition is available, and if AB was loaded using the Workbench,
         * then display a message requester.  Otherwise, just print the
         * mesage. */
        if (workbench_loaded_flag && IntuitionBase != NULL)
            message_requester(message, FALSE);
        else
            printf("%s\n", message);
    }

    close_libraries();

    exit(return_code);
}

/* This is the CLI main entry point. */
int main(int argc, char **argv)
{
    /* Make a copy of the program's filename. */
    if (argc >= 1)
        strcpy(program_filename, *argv);
    else
        clean_up(RETURN_FAIL, "");

    /* See if there is an argument for a filename, or maybe a queery on the
     * usage. */
    if (argc >= 2)
    {
        if (strcmp(argv[1], "?") == 0)
        {
            printf("%s\n", version_string);
            printf("Compilation date: %s\n", compilation_date_string);
            printf("Usage: %s [<filename>]\n", program_filename);
            clean_up(RETURN_OK, "");
        }

        strcpy(startup_project_filename, *(argv + 1));
    }

    workbench_loaded_flag = FALSE;

    mymain();
}

/* This is the WORKBENCH main entry point. */
void wbmain(struct WBStartup *wbmsg)
{
    /* Make a copy of the program's filename. */
    if (wbmsg->sm_NumArgs >= 1)
        strcpy(program_filename, wbmsg->sm_ArgList->wa_Name);
    else
        clean_up(RETURN_FAIL, "");

    /* See if there is an argument, passed in the second element of
     * sm_ArgList. */
    if (wbmsg->sm_NumArgs >= 2)
    {
        /* First extract directory name from the wa_Lock discriptor. */
        NameFromLock(((struct WBArg *) wbmsg->sm_ArgList + 1)->wa_Lock,
            startup_project_filename, FILENAME_STRING_SIZE);

        /* Now add in the filename to make a complete path. */
        AddPart(startup_project_filename, (((struct WBArg *)
            wbmsg->sm_ArgList + 1)->wa_Name), FILENAME_STRING_SIZE);
    }

    workbench_loaded_flag = TRUE;

    mymain();
}

/* This is the common main function.  Called my main() and wbmain(). */
void mymain(void)
{
    open_libraries();
    read_tooltype_settings();
    open_audio_data();
    open_commodity();
    open_arexx_port();
    open_entry_list();
    open_startup_project();

    if (settings.cx_popup)
        show_ab();

    main_window_events();

    clean_up(RETURN_OK, "");
}
