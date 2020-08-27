/* Address Book
 * by Lawrence Manning
 *
 * filename: externals.h
 */

/* EXTERNALS. */
/* Version strings. */
extern UBYTE *program_name_string;
extern UBYTE *version_string;
extern UBYTE *compilation_date_string;
extern UBYTE *VER;

/* Library bases. */
extern struct IntuitionBase *IntuitionBase;
extern struct Library *GadToolsBase;
extern struct Library *AslBase;
extern struct Library *IconBase;
extern struct GfxBase *GfxBase;
extern struct RxsLib *RexxSysBase;
extern struct Library *CxBase;

/* Screen information. */
extern struct Screen *pub_screen;
extern APTR pub_screen_vi;
extern struct DrawInfo *pub_screen_draw_info;
extern struct Screen *previous_pub_screen;

/* Main window information. */
extern UBYTE main_window_title[GENERAL_STRING_SIZE];
extern LONG main_window_width;
extern LONG main_window_height;
extern WORD main_window_zoom_position[4];
extern struct Window *main_window;
extern struct Gadget *main_window_gadget_list[NO_OF_MAIN_WINDOW_GADGETS];
extern struct Gadget *main_window_first_gadget;
extern struct Menu *main_window_menu;
extern BOOL main_window_menu_attached;

/* Edit window information. */
extern LONG edit_window_width;
extern LONG edit_window_height;
extern struct Window *edit_window;
extern struct Gadget *edit_window_gadget_list[NO_OF_EDIT_WINDOW_GADGETS];
extern struct Gadget *edit_window_first_gadget;

/* String window infromation. */
extern LONG string_window_width;
extern LONG string_window_height;
extern struct Window *string_window;
extern struct Gadget *string_window_gadget_list[NO_OF_STRING_WINDOW_GADGETS];
extern struct Gadget *string_window_first_gadget;

/* Other GUI information. */
extern BOOL make_hidden; extern BOOL make_shown;
extern LONG ab_status;

/* Event information. */
extern LONG edit_window_result;
extern LONG string_window_result;
extern UBYTE search_string[GENERAL_STRING_SIZE];
extern LONG search_type;
extern LONG current_search_type;
extern BOOL search_active;

/* Address Book information. */
extern struct MinList *entry_list;
extern LONG number_of_entry_nodes;
extern struct entry_node_type *viewed_entry_node;
extern UBYTE project_filename[FILENAME_STRING_SIZE];
extern BOOL project_changed;
extern UBYTE startup_project_filename[FILENAME_STRING_SIZE];

/* Printer information. */
extern BPTR printer_file;

/* Tone dial information. */
extern BYTE *sine_wave_data;
extern struct IOAudio *audio_io[2];
extern struct MsgPort *audio_port[2];
extern UBYTE allocate_which_channels[2][1]; 
extern BOOL audio_device_open[2];
extern LONG clock_rate;

/* Arexx information. */
extern struct MsgPort *arexx_port;

/* Commodities information. */
extern struct MsgPort *commodity_port;
extern CxObj *commodity_broker;
extern CxObj *commodity_filter;
extern BOOL hotkey_enabled;
extern UBYTE *commodity_name;
extern UBYTE *commodity_description;

/* Settings information. */
extern struct settings_type settings;
extern struct settings_type default_settings;

/* Other globals. */
extern UBYTE *allocmem_failure_string;
extern UBYTE program_filename[FILENAME_STRING_SIZE];
extern BOOL workbench_loaded_flag;
