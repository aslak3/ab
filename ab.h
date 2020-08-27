/* Address Book
 * by Lawrence Manning
 *
 * filename: ab.h
 */

/* For version history, see file version_history.doc */

/* Compiler setup. */
/* Lib bases are extern'd in extern.h. */
#define __NOLIBBAE__

/* UNIX style includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Amiga includes. */
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <intuition/screens.h>
#include <libraries/gadtools.h>
#include <libraries/asl.h>
#include <libraries/commodities.h>
#include <graphics/gfxbase.h>
#include <devices/audio.h>
#include <devices/inputevent.h>
#include <rexx/storage.h>
#include <rexx/errors.h>
#include <rexx/rxslib.h>
#include <workbench/workbench.h>

/* Amiga prototypes. */
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/asl.h>
#include <proto/commodities.h>
#include <proto/rexxsyslib.h>
#include <proto/icon.h>
#include <proto/alib.h>

/* Version defines. */
#define PROGRAM_NAME "AB v1.2b"
#define COMPILATION_DATE "6-4-97"
#define COPYRIGHT "\251 Lawrence Manning"

/* General defines. */
#define GENERAL_STRING_SIZE 200
#define FILENAME_STRING_SIZE 255
#define NO_OF_MAIN_WINDOW_GADGETS 14
#define NO_OF_EDIT_WINDOW_GADGETS 11
#define NO_OF_STRING_WINDOW_GADGETS 4
#define MAIN_WINDOW_ZOOMED_WIDTH 200
#define EDIT_WINDOW_TITLE "Edit person details"
#define EDIT_WINDOW_RESULT_OK 1
#define EDIT_WINDOW_RESULT_CANCEL 2
#define STRING_WINDOW_TITLE "Enter search details"
#define STRING_WINDOW_RESULT_OK 1
#define STRING_WINDOW_RESULT_CANCEL 2
#define SEARCH_FOR_NAME 0
#define SEARCH_FOR_PHONE 1
#define SEARCH_FOR_EMAIL 2
#define FILE_HEADER_STRING "AB (c) 1995 Lawrence Manning"
#define WRITEABLE_TOOLTYPES 16
#define CLOCK_RATE_PAL 3546895
#define CLOCK_RATE_NTSC 3579545
#define SINE_WAVE_SAMPLES 18
#define AREXX_PORT_NAME "AB_AREXX"
#define NOT_QUITTING 0
#define NORMAL_QUIT 1
#define NO_CONFIRMATION_QUIT 2
#define SHOWN 1
#define HIDDEN 2
#define SHIFT_QUALIFIER (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT)

/* GadgetID, and also gadget_list index. */
/* Main window gadgets. */
#define GADID_NAME_TEXT 0
#define GADID_ADDRESS_ONE_TEXT 1
#define GADID_ADDRESS_TWO_TEXT 2
#define GADID_ADDRESS_THREE_TEXT 3
#define GADID_ADDRESS_FOUR_TEXT 4
#define GADID_ADDRESS_FIVE_TEXT 5
#define GADID_PHONE_TEXT 6
#define GADID_EMAIL_TEXT 7
#define GADID_COMMENT_TEXT 8
#define GADID_MOVEMENT_SCROLLER 9
#define GADID_EDIT_BUTTON 10
#define GADID_NEW_BUTTON 11
#define GADID_DELETE_BUTTON 12
#define GADID_DIAL_BUTTON 13

/* Edit window. */
#define GADID_NAME_STRING 0
#define GADID_ADDRESS_ONE_STRING 1
#define GADID_ADDRESS_TWO_STRING 2
#define GADID_ADDRESS_THREE_STRING 3
#define GADID_ADDRESS_FOUR_STRING 4
#define GADID_ADDRESS_FIVE_STRING 5
#define GADID_PHONE_STRING 6
#define GADID_EMAIL_STRING 7
#define GADID_COMMENT_STRING 8
#define GADID_OK_BUTTON 9
#define GADID_CANCEL_BUTTON 10

/* String window. */
#define GADID_TYPE_CYCLE 0
#define GADID_STRING_STRING 1
#define GADID_OK2_BUTTON 2
#define GADID_CANCEL2_BUTTON 3

/* These are the menu numbers. */
/* Main window. */
#define MENUID_PROJECT 0
#define MENUID_NEW 0
#define MENUID_OPEN 1
#define MENUID_SAVE 2
#define MENUID_SAVE_AS 3
#define MENUID_PRINT 4
/*      -------------------- */
#define MENUID_ABOUT 6
/*      -------------------- */
#define MENUID_HIDE 8
#define MENUID_QUIT 9

#define MENUID_EDIT 1
#define MENUID_SEARCH 0
#define MENUID_SEARCH_AGAIN 1

#define MENUID_SETTINGS 2
#define MENUID_PRINT_HEADERS 0
#define MENUID_PRINT_ALL 1
/*      -------------------- */
#define MENUID_SAVE_SETTINGS 3

/* Datatypes. */
/* This structure represents the information used to store a person entry in
 * the database. */
struct entry_node_type
{
    struct MinNode node;
    LONG entry_node_number;
    UBYTE *name;
    UBYTE *address[5];
    UBYTE *phone;
    UBYTE *email;
    UBYTE *comment;
};

/* This structure represents the settings information. */
struct settings_type
{
    LONG main_window_left;      /* Window position for main window. */
    LONG main_window_top;       /* Ditto. */
    LONG main_window_width;     /* The minimum width for the main window. */
    LONG edit_window_left;      /* Window position for edit window. */
    LONG edit_window_top;       /* Ditto. */
    LONG edit_window_width;     /* The minimum width for the edit window. */
    LONG dial_duration;         /* Tone dial tone length in 50th second. */
    LONG dial_delay;            /* Length of pause between tones in 50th sec. */
    UBYTE public_screen[GENERAL_STRING_SIZE];
                                /* If valid, open on this public screen. */
    BOOL print_headers;         /* TRUE prints the "--- Entry x of y ---"
                                 * header when printing. */
    BOOL print_all;             /* TRUE prints all entries. */
    LONG cx_priority;           /* Commodity priority. */
    UBYTE cx_popkey[GENERAL_STRING_SIZE];
                                /* Commodity popkey. */
    BOOL cx_popup;              /* TRUE starts AB shown, FALSE hidden. */
    BOOL old_sorting;           /* TRUE uses first char of name for sorting. */
};

/* This typedef is a function pointer to an ARexx function. */
typedef void (*AREXX_FUNCTION)(LONG *result1, UBYTE *result2, UBYTE *arguments);

/* This structure descirbes an ARexx command. */
struct arexx_command_type
{
    UBYTE *arexx_command_name;
    AREXX_FUNCTION arexx_command_function;
};

/* MACROS. */
/* This macro combines ItemAddress with FULLMENUNUM. */
#define ITEM_ADDRESS(menu_strip, menu, item, subitem) \
    (ItemAddress(menu_strip, FULLMENUNUM(menu, item, subitem)))

/* This macro returns the state of a menu item's checkbox. */
#define MENU_CHECKBOX(menu_strip, menu, item, subitem) \
    ((ITEM_ADDRESS(menu_strip, menu, item, subitem)->Flags & CHECKED) != 0)

/* This macro sets the menu as being checked depending the value of variable.
 * If variable is TRUE the item is checked, else it is not checked.  The
 * variable menu_item (type struct MenuItem *) needs to be defined to use this
 * macro. */
#define SET_MENU_STATE(menu_strip, menu, item, subitem, variable) \
    menu_item = ITEM_ADDRESS(menu_strip, menu, item, subitem); \
    menu_item->Flags = (menu_item->Flags & ~CHECKED) | ((variable) ? CHECKED : 0)

/* These macros turns a menu on and off. */
#define ON_MENU(menu_strip, menu, item, subitem) \
    OnMenu(menu_strip, FULLMENUNUM(menu, item, subitem))
#define OFF_MENU(menu_strip, menu,item, subitem) \
    OffMenu(menu_strip, FULLMENUNUM(menu, item, subitem))

/* main.c prototypes. */
void clean_up(UBYTE return_code, UBYTE *message);
void mymain(void);

/* gui.c prototypes. */
void open_pub_screen_information(void);
void close_pub_screen_information(void);
void make_gui_font_sensitive(void);
LONG length_of_longest_string(UBYTE **strings);
void open_main_window(void);
void close_main_window(void);
void store_main_window_position(void);
void update_main_window_display(void);
void update_main_window_buttons(void);
void update_main_window_menus(void);
void open_edit_window(void);
void close_edit_window(void);
void store_edit_window_position(void);
void edit_window_updater(void);
void open_string_window(void);
void close_string_window(void);
void hide_ab(void);
void show_ab(void);
void control_busy_pointer(BOOL on_flag);
BOOL message_requester(UBYTE *message, BOOL yes_and_no_flag);
void dos_error_requester(void);
BOOL show_file_requester(UBYTE *filename, UBYTE *title, BOOL saveing_flag);

/* event.c prototypes. */
void main_window_events(void);
LONG main_window_arexx(UBYTE *arexx_command, UBYTE *arexx_arguments, LONG arexx_action,
    LONG *arexx_result1, UBYTE *arexx_result2);
LONG main_window_commodity(ULONG message_id);
LONG main_window_gadgets(ULONG message_class, UWORD gadgetid, UWORD message_code);
LONG main_window_vanilla_keypress(UBYTE keypress, UWORD message_qualifier);
LONG main_window_raw_keypress(UBYTE keypress, UWORD message_qualifier);
LONG main_window_menus(UWORD menu_number, UWORD item_number, UWORD subitem_number);
void edit_window_events(void);
BOOL edit_window_gadgets(ULONG message_class, UWORD gadgetid, UWORD message_code);
BOOL edit_window_keypress(UBYTE keypress, UWORD message_qualifier);
void string_window_events(void);
BOOL string_window_gadgets(ULONG message_class, UWORD gadgetid, UWORD message_code);
BOOL string_window_keypress(UBYTE keypress, UWORD message_qualifier);

/* actions.c prototypes. */
void goto_any_entry(LONG entry_node_number, BOOL update_buttons_flag);
void goto_first_entry(void);
void goto_prev_entry(void);
void goto_next_entry(void);
void goto_last_entry(void);
void edit_entry(void);
void new_entry(void);
void delete_entry(void);
void dial_entry(void);
void new_project(void);
void open_project(void);
void save_project(void);
void save_as_project(void);
void print_project(void);
void show_about_requester(void);
void search_entries(void);
void search_entries_again(void);
void save_settings(void);

/* entries.c prototypes. */
void open_entry_list(void);
void close_entry_list(void);
struct entry_node_type *open_entry_node(void);
void close_entry_node(struct entry_node_type *entry_node);
void link_entry_node_to_list(struct entry_node_type *entry_node);
void remove_entry_node_from_list(struct entry_node_type *entry_node);
void copy_edit_window_to_entry_node(struct entry_node_type *entry_node);
BOOL save_entry_list(UBYTE *filename);
BOOL load_entry_list(UBYTE *filename);
struct entry_node_type *entry_search(struct entry_node_type *start_entry_node, 
    BOOL skip_current_entry);
LONG compare_names(UBYTE *name_one, UBYTE *name_two);

/* printer.c prototypes. */
BOOL open_printer_file(void);
void close_printer_file(void);
BOOL write_entry_to_printer_file(struct entry_node_type *entry_node, UBYTE *header_string);

/* tonedial.c prototypes. */
void open_audio_data(void);
void close_audio_data(void);
BOOL open_audio_device(void);
void close_audio_device(void);
BOOL tone_dial_number(UBYTE *phone_number_string);

/* arexx.c prototypes. */
void open_arexx_port(void);
void close_arexx_port(void);
void arexx_hide(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_show(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_goto_first_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_goto_prev_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_goto_next_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_goto_last_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_delete_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_dial_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_dial_number(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_new_project(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_open_project(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_save_project(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_save_as_project(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_search_entries(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_search_entries_again(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_save_settings(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_get_entry_details(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_new_entry(LONG *result1, UBYTE *result2, UBYTE *arguments);
void arexx_put_entry_details(LONG *result1, UBYTE *result2, UBYTE *arguments);

/* commodity.c prototypes. */
void open_commodity(void);
void close_commodity(void);

/* settings.c prototypes. */
void read_tooltype_settings(void);
BOOL write_tooltype_settings(void);
BOOL string_to_bool(UBYTE *string);
void bool_to_string(UBYTE *string, BOOL boolean);

/* misc.c prototypes. */
void open_libraries(void);
void close_libraries(void);
void strcpy_gadget(struct Gadget *gadget, UBYTE *string);
LONG strlen_gadget(struct Gadget *gadget);
BOOL write_string(UBYTE *string, BPTR file_handle);
BOOL read_string(UBYTE **string, BPTR file_handle);
BOOL stristr(UBYTE *string_one, UBYTE *string_two);
void open_startup_project(void);

