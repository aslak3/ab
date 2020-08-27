/* Address Book
 * by Lawrence Manning 
 *
 * filename: gui.c
 */

#include "ab.h"
#include "externals.h"
#include "gui_menus.h"
#include "gui_gadgets.h"

/* SCREENS. */
/* This function gets the public screen's information.  If the correct tooltype
 * was specified, then try and get that screen.  If not, try the Workbench. */
void open_pub_screen_information(void)
{
    /* See if a public screen was specified. */
    if (strlen(settings.public_screen))
    {
        /* Try and lock the public screen specified in the tool types. */
        pub_screen = LockPubScreen(settings.public_screen);
    }

    /* If no screen was specified, or the attempted lock failed, then try
     * to lock the default public screen. */
    if (!(pub_screen))
    {
        if (!(pub_screen = LockPubScreen(NULL)))
            clean_up(RETURN_FAIL, "Couldn't lock default public screen.");
    }

    /* Find the visual info for this screen.  Needed for menus. */
    if (!(pub_screen_vi = GetVisualInfo(pub_screen, TAG_DONE)))
        clean_up(RETURN_FAIL, "Unable to obtain visual info for screen");

    /* Find the address of the screens DrawInfo.  This describes the preference
     * settings.  Needed because we need the Amiga key image and colours and
     * things like that. */
    if (!(pub_screen_draw_info = GetScreenDrawInfo(pub_screen)))
        clean_up(RETURN_FAIL, "Unable to obtain draw info for screen.");
}

/* Free up all the things opened in the previos function, and reset the pointers
 * to those things to NULL. */
void close_pub_screen_information(void)
{
    if (pub_screen_draw_info)
    {
        FreeScreenDrawInfo(pub_screen, pub_screen_draw_info);
        pub_screen_draw_info = NULL;
    }

    if (pub_screen_vi)
    {
        FreeVisualInfo(pub_screen_vi);
        pub_screen_vi = NULL;
    }

    if (pub_screen)
    {
        UnlockPubScreen(NULL, pub_screen);
        pub_screen = NULL;
    }
}

/* GUI FONT SENSITIZING ROUTINES. */
void make_gui_font_sensitive(void)
{
    /* GENERAL FONT SENSITIZING VARIABLES. */
    struct RastPort *pub_screen_rp = &pub_screen->RastPort;
    LONG window_width, window_height;
    LONG min_window_width; /* The minimum width of main and edit windows. */
    LONG gadget_height;
    LONG gadget_counter;
    LONG x_counter, y_counter;

    /* MAIN WINDOW VARIABLES. */
    UBYTE *main_window_entry_details_labels[] = {"Name:", "Address:", "Phone:", "E-mail:",
        "Comment:", NULL};
    UBYTE *main_window_button_labels[] = {"Edit", "New", "Delete", "Dial", NULL};
    LONG main_window_entry_details_left, main_window_entry_details_width;
    LONG main_window_button_width; /* All buttons are the same width. */

    /* EDIT_WINDOW_VARIABLES. */
    UBYTE *edit_window_entry_details_labels[] = {"Name:", "Address:", "Phone:", "E-mail:",
        "Comment:", NULL};
    UBYTE *edit_window_button_labels[] = {"Ok", "Cancel", NULL};
    LONG edit_window_entry_details_left, edit_window_entry_details_width;
    LONG edit_window_button_width; /* Both buttons are the same width. */

    /* STRING WINDOW VARIABLES. */
    UBYTE *string_window_cycle_label[] = {"Search type:", NULL};
    UBYTE *string_window_cycle_states[] = {"Name", "Phone", "E-mail", NULL};
    UBYTE *string_window_button_labels[] = {"Ok", "Cancel", NULL};
    LONG string_window_cycle_left, string_window_cycle_width;
    LONG string_window_button_width;

    /* GENERAL FONT SENSITIZING CODE. */
    /* First we can work out the height that all the gadgets will be.  This is
     * simply the (screen text) font height plus six. */
    gadget_height = pub_screen_rp->Font->tf_YSize + 6;

    /* MAIN WINDOW CODE. */
    /* Work out the width of the buttons.  The width is the length of the
     * longest string, plus sixteen pixels. */
    main_window_button_width = length_of_longest_string(main_window_button_labels) + 16;

    /* Now we can work out the minimum window width.  This is the length of all
     * four buttons, plus the two window borders, plus the gap between the
     * buttons.  This width is also the default edit window width. */
    min_window_width = (8 * 2) + (main_window_button_width * 4) + (8 * 3);

    /* Now we see if the width specified in the tooltypes is more then this
     * window width.  If it is, then we set the window width to this value,
     * and adjust the width of the button gadgets.  This is not entiraly
     * satisfactory method, and could be improved. */
    if (settings.main_window_width > min_window_width)
    {
        main_window_button_width += (settings.main_window_width - min_window_width) / 4;
        window_width = settings.main_window_width;
    }
    else
        window_width = min_window_width;

    /* The left edge of the entry detail gadggets is worked out by finding
     * the length of the longest label and adding 16.  (8 for the border, and
     * 8 for the gap between the text and the gadget. */
    main_window_entry_details_left = 
        length_of_longest_string(main_window_entry_details_labels) + 8 + 8;

    /* The width of the entry gadgets can now be calculated. */
    main_window_entry_details_width = window_width - main_window_entry_details_left - 8;

    x_counter = 8; y_counter = 4;

    /* We can now position the entry detail gadgets. */
    for (gadget_counter = GADID_NAME_TEXT; gadget_counter <= GADID_COMMENT_TEXT;
        gadget_counter++)
    {
        main_window_new_gadgets[gadget_counter].ng_LeftEdge =
            main_window_entry_details_left;
        main_window_new_gadgets[gadget_counter].ng_TopEdge = y_counter;
        main_window_new_gadgets[gadget_counter].ng_Width =
            main_window_entry_details_width;
        main_window_new_gadgets[gadget_counter].ng_Height = gadget_height;

        /* See what vertical gap there should be between the gadgets. */
        if (gadget_counter >= GADID_ADDRESS_ONE_TEXT &&
             gadget_counter <= GADID_ADDRESS_FOUR_TEXT)
        {
            /* This positions the gadgets with no gap. */
            y_counter += gadget_height;
        }
        else
        {
            /* This postions the gadgets with a two pixel gap. */
            y_counter += gadget_height + 2;
        }
    }

    /* There should be an additional 4 pixel gap between the entry detail
     * gadgets and the buttons. */
    y_counter += 2;

    /* Now we can position the movement scroller. */
    main_window_new_gadgets[GADID_MOVEMENT_SCROLLER].ng_LeftEdge = x_counter;
    main_window_new_gadgets[GADID_MOVEMENT_SCROLLER].ng_TopEdge = y_counter;
    main_window_new_gadgets[GADID_MOVEMENT_SCROLLER].ng_Width = window_width - (8 * 2);
    main_window_new_gadgets[GADID_MOVEMENT_SCROLLER].ng_Height = gadget_height;

    y_counter += gadget_height + 4;
    x_counter = 8;

    for (gadget_counter = GADID_EDIT_BUTTON; gadget_counter <= GADID_DIAL_BUTTON;
        gadget_counter++)
    {
        main_window_new_gadgets[gadget_counter].ng_LeftEdge = x_counter;
        main_window_new_gadgets[gadget_counter].ng_TopEdge = y_counter;
        main_window_new_gadgets[gadget_counter].ng_Width = main_window_button_width;
        main_window_new_gadgets[gadget_counter].ng_Height = gadget_height;

        x_counter += main_window_button_width + 8;
    }

    window_height = y_counter + gadget_height + 4;

    /* Fill in the window dimension globals so open_main_window knows what
     * size the window should be. */
    main_window_width = window_width;
    main_window_height = window_height;

    /* EDIT WINDOW CODE. */
    /* First work out the size of the two buttons at the bottom of the
     * window. */
    edit_window_button_width = length_of_longest_string(edit_window_button_labels) + 16;

    /* The base edit window width is the minimum calculated window width of
     * the main window. (not neccesary the final width) */
    window_width = min_window_width;

    /* Now see if the settings indicate a bigger width. */
    if (settings.edit_window_width > window_width)
        window_width = settings.edit_window_width;

    /* We can borrow the values calculated for the entry window here. */
    edit_window_entry_details_left = main_window_entry_details_left;
    edit_window_entry_details_width = window_width - edit_window_entry_details_left - 8;

    x_counter = 8; y_counter = 4;

    /* We can now position the entry detail gadgets. */
    for (gadget_counter = GADID_NAME_STRING; gadget_counter <= GADID_COMMENT_STRING;
        gadget_counter++)
    {
        edit_window_new_gadgets[gadget_counter].ng_LeftEdge =
            edit_window_entry_details_left;
        edit_window_new_gadgets[gadget_counter].ng_TopEdge = y_counter;
        edit_window_new_gadgets[gadget_counter].ng_Width =
            edit_window_entry_details_width;
        edit_window_new_gadgets[gadget_counter].ng_Height = gadget_height;

        /* See what vertical gap there should be between the gadgets. */
        if (gadget_counter >= GADID_ADDRESS_ONE_STRING &&
            gadget_counter <= GADID_ADDRESS_FOUR_STRING)
        {
            y_counter += gadget_height;
        }
        else
        {
            y_counter += gadget_height + 2;
        }
    }

    /* There should be an additional 4 pixel gap between the entry detail
     * gadgets and the buttons. */
    y_counter += 2;

    for (gadget_counter = GADID_OK_BUTTON; gadget_counter <=
        GADID_CANCEL_BUTTON; gadget_counter++)
    {
        edit_window_new_gadgets[gadget_counter].ng_LeftEdge = x_counter;
        edit_window_new_gadgets[gadget_counter].ng_TopEdge = y_counter;
        edit_window_new_gadgets[gadget_counter].ng_Width =
            edit_window_button_width;
        edit_window_new_gadgets[gadget_counter].ng_Height = gadget_height;

        x_counter += window_width - (2 * 8) - edit_window_button_width;
    }

    window_height = y_counter + gadget_height + 4;

    /* Fill in the window dimension globals so open_edit_window knows what
     * size the window should be. */
    edit_window_width = window_width;
    edit_window_height = window_height;

    /* STRING WINDOW CODE. */
    /* Find width of the Ok and Cancel buttons. */
    string_window_button_width =
        length_of_longest_string(string_window_button_labels) + 16;

    /* Find the left position of the cycle gadget. */
    string_window_cycle_left =
        length_of_longest_string(string_window_cycle_label) + 8 + 8;

    /* Find width of cylce gadget.  This is based on the length of the cycle
     * state strings, with an offset for the border, and an offset for the gap
     * between the gadget text and the gadget box.  The 32 is because we want
     * this gadget (and therefore the window) to be fairly big.  The 22 is
     * the width of the cylce logo. */
    string_window_cycle_width =
        length_of_longest_string(string_window_cycle_states) + 22 + 32;

    /* We can work out the width of the window now.  It is the sum of the
     * above two dimensions, plus the window border on the rh side. */
    window_width = string_window_cycle_left + string_window_cycle_width + 8;

    x_counter = 8; y_counter = 4;

    /* Now we can fill in the placing and size of the cylce and string gads. */
    string_window_new_gadgets[GADID_TYPE_CYCLE].ng_LeftEdge = string_window_cycle_left;
    string_window_new_gadgets[GADID_TYPE_CYCLE].ng_TopEdge = y_counter;
    string_window_new_gadgets[GADID_TYPE_CYCLE].ng_Width = string_window_cycle_width;
    string_window_new_gadgets[GADID_TYPE_CYCLE].ng_Height = gadget_height;

    y_counter += gadget_height + 2;

    string_window_new_gadgets[GADID_STRING_STRING].ng_LeftEdge = x_counter;
    string_window_new_gadgets[GADID_STRING_STRING].ng_TopEdge = y_counter;
    string_window_new_gadgets[GADID_STRING_STRING].ng_Width = window_width - 8 - 8;
    string_window_new_gadgets[GADID_STRING_STRING].ng_Height = gadget_height;

    y_counter += gadget_height + 4;

    for (gadget_counter = GADID_OK2_BUTTON; gadget_counter <= GADID_CANCEL2_BUTTON;
        gadget_counter++)
    {
        string_window_new_gadgets[gadget_counter].ng_LeftEdge = x_counter;
        string_window_new_gadgets[gadget_counter].ng_TopEdge = y_counter;
        string_window_new_gadgets[gadget_counter].ng_Width =
            string_window_button_width;
        string_window_new_gadgets[gadget_counter].ng_Height = gadget_height;

        x_counter += window_width - (2 * 8) - string_window_button_width;
    }

    window_height = y_counter + gadget_height + 4;

    /* Fill in the window dimension globals so open_edit_window knows what
     * size the window should be. */
    string_window_width = window_width;
    string_window_height = window_height;
}

/* This function returns the length of the longest string in pixels using the
 * current screen font for the public screen. */
LONG length_of_longest_string(UBYTE **strings)
{
    struct RastPort *pub_screen_rp = &pub_screen->RastPort;
    UBYTE **string_pointer = strings;
    LONG this_length;
    LONG greatest_length = 0;

    while (*string_pointer != NULL)
    {
        /* Find the length of this string, and see if it is the largest so
         * far. */
        this_length = TextLength(pub_screen_rp, *string_pointer, strlen(*string_pointer));
        if (this_length > greatest_length)
            greatest_length = this_length;

        /* Go to the next string. */
        string_pointer++;
    }

    return (greatest_length);
}
    
/* MAIN WINDOW FUNCTIONS. */
/* The main window is where the user sees the information for each person. */
void open_main_window(void)
{
    WORD screen_width = pub_screen->Width;
    WORD screen_height = pub_screen->Height;
    BYTE screen_bar_height = pub_screen->BarHeight;
    BYTE window_border_left = pub_screen->WBorLeft;
    BYTE window_border_right = pub_screen->WBorRight;
    BYTE window_border_bottom = pub_screen->WBorBottom;
    BYTE offset_left, offset_top;
    struct Gadget *previous_gadget, *context_gadget;
    struct NewGadget newgadget;
    LONG count = 0, t_count = 0;

    /* First fill in the zoom position for the main window. */
    main_window_zoom_position[0] = settings.main_window_left;
    main_window_zoom_position[1] = settings.main_window_top;
    main_window_zoom_position[2] = MAIN_WINDOW_ZOOMED_WIDTH;
    main_window_zoom_position[3] = screen_bar_height + 1;

    /* Set up some offset vairables to save some typing. */
    offset_left = window_border_left; offset_top = screen_bar_height + 1;

    /* Create context gadget. */
    main_window_first_gadget = NULL;
    context_gadget = CreateContext(&main_window_first_gadget);
    previous_gadget = context_gadget;

    /* Create the gadgets in one for loop! */
    for (count = 0; count < NO_OF_MAIN_WINDOW_GADGETS; count++)
    {
        CopyMem(&main_window_new_gadgets[count], &newgadget, sizeof(struct NewGadget));

        /* Fill in the visual info field first, and the offsets. */
        newgadget.ng_VisualInfo = pub_screen_vi;
        newgadget.ng_LeftEdge += offset_left;
        newgadget.ng_TopEdge += offset_top;

        /* Now create the gadget! */
        main_window_gadget_list[count] = CreateGadgetA(main_window_gadget_kinds[count],
            previous_gadget, &newgadget,
            (struct TagItem *) &main_window_gadget_tags[t_count]);

        previous_gadget = main_window_gadget_list[count];

        /* Move t_count to the start of the next gadget. */
        while (main_window_gadget_tags[t_count] != TAG_DONE)
            t_count++;

        t_count++;
    }

    /* main_window_first_gadget will be NULL if there was an error when making
     * the gadgets for the control window. */
    if (main_window_first_gadget == NULL)
        clean_up(RETURN_FAIL, "Unable to create gadgets for main window.");

    /* Set up the menus.  First create them using GadTools. */
    if (!(main_window_menu = CreateMenus(new_menus,
        GTMN_FrontPen, pub_screen_draw_info->dri_Pens[BARDETAILPEN],
        GTMN_Checkmark, pub_screen_draw_info->dri_CheckMark,
        GTMN_AmigaKey, pub_screen_draw_info->dri_AmigaKey,
        TAG_DONE)))
    {
        clean_up(RETURN_FAIL, "Unable to crate menus for main window.");
    }

    /* Now layout the menu: */
    LayoutMenus(main_window_menu, pub_screen_vi, TAG_DONE);

    /* Now open the main window, and refresh it to show the gadgets. */
    if (!(main_window = OpenWindowTags(NULL,
        WA_ScreenTitle, version_string,
        WA_Left, settings.main_window_left,
        WA_Top, settings.main_window_top,
        WA_Width, main_window_width + window_border_left + window_border_right,
        WA_Height, main_window_height + screen_bar_height + 1 + window_border_bottom,
        WA_Gadgets, main_window_first_gadget,
        WA_DragBar, TRUE,
        WA_CloseGadget, TRUE,
        WA_DepthGadget, TRUE,
        WA_Zoom, main_window_zoom_position,
        WA_SmartRefresh, TRUE,
        WA_Activate, TRUE,
        WA_NewLookMenus, TRUE,
        WA_PubScreen, pub_screen,
        WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_GADGETDOWN | IDCMP_GADGETUP |
            IDCMP_MOUSEMOVE | IDCMP_VANILLAKEY | IDCMP_MENUPICK | IDCMP_REFRESHWINDOW,
        TAG_DONE)))
    {
        clean_up(RETURN_FAIL, "Unable to open main window.");
    }

    /* Attach menu to main window. */
    if (!(main_window_menu_attached = SetMenuStrip(main_window, main_window_menu)))
        clean_up(RETURN_FAIL, "Unable to attach menu to main window.");

    update_main_window_display();
    update_main_window_buttons();
    update_main_window_menus();

    GT_RefreshWindow(main_window, NULL);
}

/* This funtions closes the window opened above. */
void close_main_window(void)
{
    if (main_window_menu_attached)
    {
        ClearMenuStrip(main_window);
        main_window_menu_attached = FALSE;
    }

    if (main_window_menu)
    {
        FreeMenus(main_window_menu);
        main_window_menu = NULL;
    }

    if (main_window)
    {
        CloseWindow(main_window);
        main_window = NULL;
    }

    if (main_window_first_gadget)
    {
        FreeGadgets(main_window_first_gadget);
        main_window_first_gadget = NULL;
    }
}

/* This little function updates the settings structure to show the coordinates
 * of the main window. */
void store_main_window_position(void)
{
    /* Main window must be open. */
    if (main_window)
    {
        settings.main_window_left = main_window->LeftEdge;
        settings.main_window_top = main_window->TopEdge;
    }
}

/* This is the main window master updater.  It displays the currently viewed
 * node.  Also, this function updates the tilte bar to show the number of
 * the currently displayed entry. */
void update_main_window_display(void)
{
    LONG viewed_entry_node_number;

    /* Main window must be open to update it, so we can safely call this 
     * function and not worry about the main window being open or not. */
    if (main_window)
    {
        /* Get the current entry number, or 0 if there isn't one. */
        if (viewed_entry_node)
            viewed_entry_node_number = viewed_entry_node->entry_node_number;
        else
            viewed_entry_node_number = 0;

        /* If there is a valid node, then show it. Else clear all the text
         * fields.  Do this by writing a "" to them. */
        if (viewed_entry_node)
        {
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_NAME_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->name, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_ONE_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->address[0], TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_TWO_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->address[1], TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_THREE_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->address[2], TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_FOUR_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->address[3], TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_FIVE_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->address[4], TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_PHONE_TEXT], 
                main_window, NULL, GTTX_Text,viewed_entry_node->phone, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_EMAIL_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->email, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_COMMENT_TEXT],
                main_window, NULL, GTTX_Text, viewed_entry_node->comment, TAG_DONE);
        }
        else
        {
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_NAME_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_ONE_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_TWO_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_THREE_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_FOUR_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_ADDRESS_FIVE_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_PHONE_TEXT], 
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_EMAIL_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_COMMENT_TEXT],
                main_window, NULL, GTTX_Text, NULL, TAG_DONE);
        }

        /* Now update the titlebar. */
        sprintf(main_window_title, "AB (entry %d of %d)",
            viewed_entry_node_number, number_of_entry_nodes);
        SetWindowTitles(main_window, main_window_title, (UBYTE *) -1);
    }
}

/* This function updates the buttons in the main window. */
void update_main_window_buttons(void)
{
    LONG viewed_entry_node_number;

    if (main_window)
    {
        /* Get the current entry number, or 0 if there isn't one. */
        if (viewed_entry_node)
            viewed_entry_node_number = viewed_entry_node->entry_node_number;
        else
            viewed_entry_node_number = 0;

        /* Now we update the movement scroller. */
        GT_SetGadgetAttrs(main_window_gadget_list[GADID_MOVEMENT_SCROLLER],
            main_window, NULL, GTSC_Top, viewed_entry_node_number - 1,
            GTSC_Total, number_of_entry_nodes + 1, TAG_DONE);

        /* Update the four bottom buttons, depending on weather or not
         * there is a current entry.  Also update the Dial button.  If the
         * current entry has a phone number set, then this button is enabled. */
        if (!viewed_entry_node)
        {
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_MOVEMENT_SCROLLER],
                main_window, NULL, GA_Disabled, TRUE, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_EDIT_BUTTON],
                main_window, NULL, GA_Disabled, TRUE, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_DELETE_BUTTON],
                main_window, NULL, GA_Disabled, TRUE, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_DIAL_BUTTON],
                main_window, NULL, GA_Disabled, TRUE, TAG_DONE);
        }
        else
        {
            /* If not, then we can enable the three bottom gadgets, and the top
             * ones. */
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_MOVEMENT_SCROLLER],
                main_window, NULL, GA_Disabled, FALSE, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_EDIT_BUTTON],
                main_window, NULL, GA_Disabled, FALSE, TAG_DONE);
            GT_SetGadgetAttrs(main_window_gadget_list[GADID_DELETE_BUTTON],
                main_window, NULL, GA_Disabled, FALSE, TAG_DONE);

            /* Now we see if we should enable the Dial gadget.  If the
             * viewed entry contains a phone number then it should be
             * enabled, otherwise we disable it. */
            if (viewed_entry_node->phone)
            {    
                GT_SetGadgetAttrs(main_window_gadget_list[GADID_DIAL_BUTTON], main_window,
                    NULL, GA_Disabled, FALSE, TAG_DONE);
            }
            else
            {
                GT_SetGadgetAttrs(main_window_gadget_list[GADID_DIAL_BUTTON],
                    main_window, NULL, GA_Disabled, TRUE, TAG_DONE);
            }
        }
    }
}

/* This function updates the settings menu to display the settings stored in
 * the global settings structure.  Assumes main window menus are available and
 * attached to the main window. */
void update_main_window_menus(void)
{
    struct MenuItem *menu_item; /* Needed by the SET_MENU_STATE macro. */

    /* Main window must be open. */
    if (main_window)
    {
        SET_MENU_STATE(main_window_menu, MENUID_SETTINGS, MENUID_PRINT_HEADERS, NOSUB,
            settings.print_headers);
        SET_MENU_STATE(main_window_menu, MENUID_SETTINGS, MENUID_PRINT_ALL, NOSUB,
            settings.print_all);
    }
}
    
/* EDIT WINDOW FUNCTIONS. */
/* This function opens the edit window. */
void open_edit_window(void)
{
    WORD screen_width = pub_screen->Width;
    WORD screen_height = pub_screen->Height;
    BYTE screen_bar_height = pub_screen->BarHeight;
    BYTE window_border_left = pub_screen->WBorLeft;
    BYTE window_border_right = pub_screen->WBorRight;
    BYTE window_border_bottom = pub_screen->WBorBottom;
    BYTE offset_left, offset_top;
    struct Gadget *previous_gadget, *context_gadget;
    struct NewGadget newgadget;
    LONG count = 0, t_count = 0;

    /* Set up some offset vairables to save some typing. */
    offset_left = window_border_left; offset_top = screen_bar_height + 1;

    /* Create context gadget. */
    edit_window_first_gadget = NULL;
    context_gadget = CreateContext(&edit_window_first_gadget);
    previous_gadget = context_gadget;

    /* Create the gadgets in one for loop! */
    for (count = 0; count < NO_OF_EDIT_WINDOW_GADGETS; count++)
    {
        CopyMem(&edit_window_new_gadgets[count], &newgadget, sizeof(struct NewGadget));

        /* Fill in the visual info field first, and the offsets. */
        newgadget.ng_VisualInfo = pub_screen_vi;
        newgadget.ng_LeftEdge += offset_left;
        newgadget.ng_TopEdge += offset_top;

        /* Now create the gadget! */
        edit_window_gadget_list[count] = CreateGadgetA(edit_window_gadget_kinds[count],
            previous_gadget, &newgadget,
            (struct TagItem *) &edit_window_gadget_tags[t_count]);

        previous_gadget = edit_window_gadget_list[count];

        /* Move t_count to the start of the next gadget. */
        while (edit_window_gadget_tags[t_count] != TAG_DONE)
            t_count++;

        t_count++;
    }

    /* edit_window_first_gadget will be NULL if there was an error when making
     * the gadgets for the control window. */
    if (edit_window_first_gadget == NULL)
        clean_up(RETURN_FAIL, "Unable to create gadgets for edit window.");

    /* Now open the edit window, and refresh it to show the gadgets. */
    if (!(edit_window = OpenWindowTags(NULL,
        WA_Title, EDIT_WINDOW_TITLE,
        WA_Left, settings.edit_window_left,
        WA_Top, settings.edit_window_top,
        WA_Width, edit_window_width + window_border_left + window_border_right,
        WA_Height, edit_window_height + screen_bar_height + 1 +
            window_border_bottom,
        WA_Gadgets, edit_window_first_gadget,
        WA_DragBar, TRUE,
        WA_CloseGadget, TRUE,
        WA_DepthGadget, TRUE,
        WA_Activate, TRUE,
        WA_SmartRefresh, TRUE,
        WA_NewLookMenus, TRUE,
        WA_PubScreen, pub_screen,
        WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_VANILLAKEY |
            IDCMP_ACTIVEWINDOW | IDCMP_REFRESHWINDOW,
        TAG_DONE)))
    {
        clean_up(RETURN_FAIL, "Unable to open edit window.");
    }

    GT_RefreshWindow(edit_window, NULL);
}

/* This function closes the edit window. */
void close_edit_window(void)
{
    if (edit_window)
    {
        CloseWindow(edit_window);
        edit_window = NULL;
    }

    if (edit_window_first_gadget)
    {
        FreeGadgets(edit_window_first_gadget);
        edit_window_first_gadget = NULL;
    }
}

/* This little function up the settings structure to show the coordinates
 * of the edit window.  It is called when the edit window is closed. */
void store_edit_window_position(void)
{
    if (edit_window)
    {
        settings.edit_window_left = edit_window->LeftEdge;
        settings.edit_window_top = edit_window->TopEdge;
    }
}

/* This function sets up the string gadgets to contain the current strings
 * for the current node. */
void edit_window_updater(void)
{
    if (edit_window)
    {
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_NAME_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->name, TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_ADDRESS_ONE_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->address[0], TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_ADDRESS_TWO_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->address[1], TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_ADDRESS_THREE_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->address[2], TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_ADDRESS_FOUR_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->address[3], TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_ADDRESS_FIVE_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->address[4], TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_PHONE_STRING],
           edit_window, NULL, GTST_String, viewed_entry_node->phone, TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_EMAIL_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->email, TAG_DONE);
        GT_SetGadgetAttrs(edit_window_gadget_list[GADID_COMMENT_STRING],
            edit_window, NULL, GTST_String, viewed_entry_node->comment, TAG_DONE);
    }
}

/* STRING WINDOW FUNCTIONS. */
/* This function opens up a window for the user to type a search string into. 
 * This window is used for searching the address book entries.  The window
 * opened is placed in the center of the screen.  NOTE: This is called the
 * string window because it used to be just a window with a string gadget in.
 * It should be called the search window, but it would take too long to change
 * all references of string window to search window... */
void open_string_window(void)
{
    WORD screen_width = pub_screen->Width;
    WORD screen_height = pub_screen->Height;
    BYTE screen_bar_height = pub_screen->BarHeight;
    BYTE window_border_left = pub_screen->WBorLeft;
    BYTE window_border_right = pub_screen->WBorRight;
    BYTE window_border_bottom = pub_screen->WBorBottom;
    LONG window_left, window_top;
    BYTE offset_left, offset_top;
    struct Gadget *previous_gadget, *context_gadget;
    struct NewGadget newgadget;
    LONG count = 0, t_count = 0;

    /* Set up some offset vairables to save some typing. */
    offset_left = window_border_left; offset_top = screen_bar_height + 1;

    /* Create context gadget. */
    string_window_first_gadget = NULL;
    context_gadget = CreateContext(&string_window_first_gadget);
    previous_gadget = context_gadget;

    /* Create the gadgets in one for loop! */
    for (count = 0; count < NO_OF_STRING_WINDOW_GADGETS; count++)
    {
        CopyMem(&string_window_new_gadgets[count], &newgadget,
            sizeof(struct NewGadget));

        /* Fill in the visual info field first, and the offsets. */
        newgadget.ng_VisualInfo = pub_screen_vi;
        newgadget.ng_LeftEdge += offset_left;
        newgadget.ng_TopEdge += offset_top;

        /* Now create the gadget! */
        string_window_gadget_list[count] = CreateGadgetA(
            string_window_gadget_kinds[count], previous_gadget, &newgadget,
            (struct TagItem *) &string_window_gadget_tags[t_count]);

        previous_gadget = string_window_gadget_list[count];

        /* Move t_count to the start of the next gadget. */
        while (string_window_gadget_tags[t_count] != TAG_DONE)
            t_count++;

        t_count++;
    }

    /* string_window_first_gadget will be NULL if there was an error when
     * making the gadgets for the control window. */
    if (string_window_first_gadget == NULL)
        clean_up(RETURN_FAIL, "Unable to create gadgets for string window.");

    /* Calculate left and top coord for the string window. */
    window_left = (screen_width / 2) - ((string_window_width + window_border_left +
        window_border_right) / 2);
    window_top = (screen_height / 2) - ((string_window_height + screen_bar_height +
        window_border_bottom) / 2); 

    if (!(string_window = OpenWindowTags(NULL,
        WA_Title, STRING_WINDOW_TITLE,
        WA_Left, window_left,
        WA_Top, window_top,
        WA_Width, string_window_width + window_border_left + window_border_right,
        WA_Height, string_window_height + screen_bar_height + 1 + window_border_bottom,
        WA_Gadgets, string_window_first_gadget,
        WA_DragBar, TRUE,
        WA_CloseGadget, TRUE,
        WA_DepthGadget, TRUE,
        WA_Activate, TRUE,
        WA_SmartRefresh, TRUE,
        WA_NewLookMenus, TRUE,
        WA_PubScreen, pub_screen,
        WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_GADGETUP | IDCMP_VANILLAKEY |
            IDCMP_ACTIVEWINDOW | IDCMP_REFRESHWINDOW,
        TAG_DONE)))
    {
        clean_up(RETURN_FAIL, "Unable to open control window.");
    }

    GT_RefreshWindow(string_window, NULL);
}

/* This function closes the window opened above. */
void close_string_window(void)
{
    if (string_window)
    {
        CloseWindow(string_window);
        string_window = NULL;
    }

    if (string_window_first_gadget)
    {
        FreeGadgets(string_window_first_gadget);
        string_window_first_gadget = NULL;
    }
}

/* HIDE and SHOW routines. */
/* Hide routine closes all open windows and frees screen information. */
void hide_ab(void)
{
    /* Only hide if we are not hidden already. */
    if (ab_status != HIDDEN)
    {
        /* Store the screen pointer so we can tell if AB has moved screens. */
        previous_pub_screen = pub_screen;

        store_main_window_position();
        close_main_window();
        store_edit_window_position();
        close_edit_window();
        close_pub_screen_information();

        ab_status = HIDDEN;
    }
}

/* Show routine reopens the screen information and opens the main window again.
 * It is possible that it reopens on different screen to the one it was on
 * before. */
void show_ab(void)
{
    /* Only reapear if we are not already shown. */
    if (ab_status != SHOWN)
    {
        open_pub_screen_information();
        ScreenToFront(pub_screen);

        /* See if we need to re-do the font sensitising because AB has moved
         * to a new screen.  This also occures if this is the first time
         * that we have called this function. (previous_pub_screen is NULL) */
        if (pub_screen != previous_pub_screen);
            make_gui_font_sensitive();

        open_main_window();

        ab_status = SHOWN;
    }
    else
    {
        /* If the user wants to show the AB window and it is already open,
         * make it the front most window on the screen, and make it the
         * active window.  Also, if it is zoomed, then unzoom the window. */
        WindowToFront(main_window);
        ActivateWindow(main_window);
        if (main_window->Flags & WFLG_ZOOMED)
            ZipWindow(main_window);
    }
}
        
/* OTHER GUI ROUTINES. */
/* Turns the busy pointer on or off, depending on parameter.  TRUE turns the
 * busy pointer on, FALSE turns it off.  New v1.2a: For compatibility with 2.0
 * this function no longer works.  I'll change this if and when I find out
 * how to change the pointer under 2.0. */
void control_busy_pointer(BOOL on_flag)
{
    /* SetWindowPointer(main_window, WA_BusyPointer, on_flag, TAG_DONE); */
    ;
}

/* This routine produces a message requester.  It takes two arguments, the
 * string to display, and a flag.  If this flag is TRUE then a Ok/Cancel
 * response is required, else the requester opened has only an Ok button. */
BOOL message_requester(UBYTE *message, BOOL yes_and_no_flag)
{
    struct EasyStruct message_requester;
    LONG return_value;

    /* Set up the structure. */
    message_requester.es_StructSize = sizeof(struct EasyStruct);
    message_requester.es_Flags = 0;
    message_requester.es_Title = "Message";
    message_requester.es_TextFormat = message;
    if (!yes_and_no_flag)
        message_requester.es_GadgetFormat = "Ok";
    else
        message_requester.es_GadgetFormat = "Ok|Cancel";

    /* Call the funcion to generate the requester. */
    return_value = EasyRequest(main_window, &message_requester, NULL, NULL);

    return ((BOOL) return_value);
}

/* This routine shows a file requester to the user.  If they select Ok, then
 * it returns TRUE, else it returns FALSE.  Also returns FALSE if there were
 * problems opening the file requester.  The selected filename will be in
 * filename string.  Assumes main window is open. */
BOOL show_file_requester(UBYTE *filename, UBYTE *title, BOOL saveing_flag)
{
    struct FileRequester *file_requester = NULL;
    BOOL result;

    /* Allocate a requester structure.  Return FALSE if failure. */
    if (!(file_requester = (struct FileRequester *) AllocAslRequestTags(ASL_FileRequest,
        TAG_DONE)))
    {
        return (FALSE);
    }

    /* Open the file requester. */
    if (AslRequestTags(file_requester,
        ASLFR_Window, main_window,
        ASLFR_TitleText, title,
        ASLFR_DoSaveMode, saveing_flag,
        ASLFR_RejectIcons, TRUE,
        TAG_DONE))
    {
        /* If the user pressed Ok, then construct a complete filename.  Ignore
         * AddPart() return value.  Must fix this bit. */
        AddPart(file_requester->fr_Drawer, file_requester->fr_File, FILENAME_STRING_SIZE);

        /* Copy this filename across, and set the return value. */
        strcpy(filename, file_requester->fr_Drawer);
        result = TRUE;
    }
    else
    {
        /* If the user selected cancel, then copy a blank string and set the
         * return value to FALSE. */
        strcpy(filename, "");
        result = FALSE;
    }

    /* Either way, free the asl requester structure from memory. */
    FreeAslRequest(file_requester);

    return (result);
}
