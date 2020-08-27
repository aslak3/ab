/* Address Book
 * by Lawrence Manning 
 *
 * filename: maintestbed.c
 */

/* This file contains a stripped out mymain() which can be used to test specific
 * functions as they are writen.  Compile it as main.o and link as normal.
 * mymain() opens libraries and other stuff. */

#include "ab.h"

/* GLOBALS. */
/* Library bases. */
struct IntuitionBase *IntuitionBase = NULL;
struct Library *GadToolsBase = NULL;
struct Library *AslBase = NULL;
struct Library *IconBase = NULL;
struct GfxBase *GfxBase = NULL;

/* Screen information. */
struct Screen *pub_screen = NULL;
APTR pub_screen_vi = NULL;
struct DrawInfo *pub_screen_draw_info = NULL;

/* Main window information. */
LONG main_window_width = 0;
LONG main_window_height = 0;
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

/* Event information. */
LONG edit_window_result = 0;
LONG string_window_result = 0;
UBYTE search_string[GENERAL_STRING_SIZE];
LONG search_type = SEARCH_FOR_NAME; 
LONG current_search_type = SEARCH_FOR_NAME;
BOOL search_active = FALSE;

/* Address Book information. */
struct MinList *entry_list = NULL;
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

/* Settings information. */
struct settings_type settings;

/* Other globals. */
UBYTE *allocmem_failure_string = "Out of memory!";
UBYTE program_filename[FILENAME_STRING_SIZE];
BOOL workbench_loaded_flag;

UBYTE *version_string = "AB v1.1a (c) Lawrence Manning";
UBYTE *compilation_date_string = "25-1-96";

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
    close_audio_data();
    close_libraries();

    /* Print the erorr message, if there was one. */
    if (strlen(message))
        printf("%s\n", message);

    exit(return_code);
}

/* This is the CLI main entry point. */
void main(int argc, char **argv)
{
    mymain();
}

/* This is the WORKBENCH main entry point. */
void wbmain(struct WBStartup *wbmsg)
{
    mymain();
}

/* This is the common main function.  Called my main() and wbmain(). */
void mymain(void)
{
    open_libraries();
    open_audio_data();
    open_entry_list();
    open_pub_screen_information();
    
    make_gui_font_sensitive();

    open_main_window();
    open_edit_window();
    open_string_window();

    Delay(250);
        
    clean_up(RETURN_OK, "");
}
