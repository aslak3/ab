/* Address Book
 * by Lawrence Manning 
 *
 * filename: gui_gadgets.c
 */

/* NewGadgets for main window comming up... */
struct NewGadget main_window_new_gadgets[NO_OF_MAIN_WINDOW_GADGETS] =
{
    {
        0, 0, 0, 0,             /* make_gui_font_sensitive() fills this in. */
        "Name:", NULL,          /* Use Screen font. */
        GADID_NAME_TEXT,
        PLACETEXT_LEFT,
        NULL,                   /* Fill out VisualInfo later. */
        NULL
    },
    {   
        0, 0, 0, 0,
        "Address:", NULL,
        GADID_ADDRESS_ONE_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_TWO_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_THREE_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_FOUR_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_FIVE_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "Phone:", NULL,
        GADID_PHONE_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "E-mail:", NULL,
        GADID_EMAIL_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "Comment:", NULL,
        GADID_COMMENT_TEXT,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_MOVEMENT_SCROLLER,
        NULL,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "_Edit", NULL,
        GADID_EDIT_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "_New", NULL,
        GADID_NEW_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "_Delete", NULL,
        GADID_DELETE_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "D_ial", NULL,
        GADID_DIAL_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    }
};

/* This is list of the kinds of gadgets used in the main window. */
ULONG main_window_gadget_kinds[NO_OF_MAIN_WINDOW_GADGETS] =
{
    TEXT_KIND,      /* Name. */
    TEXT_KIND,      /* Address one. */
    TEXT_KIND,      /* Address two. */
    TEXT_KIND,      /* Address three. */
    TEXT_KIND,      /* Address four. */
    TEXT_KIND,      /* Address five. */
    TEXT_KIND,      /* Phone. */
    TEXT_KIND,      /* Email. */
    TEXT_KIND,      /* Comment. */
    SCROLLER_KIND,  /* Movement. */
    BUTTON_KIND,    /* Edit. */
    BUTTON_KIND,    /* New. */
    BUTTON_KIND,    /* Delete. */
    BUTTON_KIND     /* Dial. */
};

/* Here is a list of the TAGs used in the main window gadgets. */
Tag main_window_gadget_tags[] =
{
    GTTX_Border, TRUE, TAG_DONE,        /* Name. */
    GTTX_Border, TRUE, TAG_DONE,        /* Address one. */
    GTTX_Border, TRUE, TAG_DONE,        /* Address two. */
    GTTX_Border, TRUE, TAG_DONE,        /* Address three. */
    GTTX_Border, TRUE, TAG_DONE,        /* Address four. */
    GTTX_Border, TRUE, TAG_DONE,        /* Address five. */
    GTTX_Border, TRUE, TAG_DONE,        /* Phone. */
    GTTX_Border, TRUE, TAG_DONE,        /* Email. */
    GTTX_Border, TRUE, TAG_DONE,        /* Comment. */
    GA_Immediate, TRUE, GA_RelVerify, TRUE, GTSC_Visible, 2, TAG_DONE,
                                        /* Movement. */
    GT_Underscore, '_', TAG_DONE,       /* Edit. */
    GT_Underscore, '_', TAG_DONE,       /* New. */
    GT_Underscore, '_', TAG_DONE,       /* Delete. */
    GT_Underscore, '_', TAG_DONE        /* Dial. */
};

struct NewGadget edit_window_new_gadgets[NO_OF_EDIT_WINDOW_GADGETS] =
{
    {
        0, 0, 0, 0,
        "Name:", NULL,
        GADID_NAME_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_ADDRESS_ONE_STRING
    },
    {   
        0, 0, 0, 0,
        "Address:", NULL,
        GADID_ADDRESS_ONE_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_ADDRESS_TWO_STRING
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_TWO_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_ADDRESS_THREE_STRING
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_THREE_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_ADDRESS_FOUR_STRING
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_FOUR_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_ADDRESS_FIVE_STRING
    },
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_ADDRESS_FIVE_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_PHONE_STRING
    },
    {
        0, 0, 0, 0,
        "Phone:", NULL,
        GADID_PHONE_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_EMAIL_STRING
    },
    {
        0, 0, 0, 0,
        "E-mail:", NULL,
        GADID_EMAIL_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_COMMENT_STRING
    },
    {
        0, 0, 0, 0,
        "Comment:", NULL,
        GADID_COMMENT_STRING,
        PLACETEXT_LEFT,
        NULL,
        (APTR) GADID_NAME_STRING
    },
    {
        0, 0, 0, 0,
        "_Ok", NULL,
        GADID_OK_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "_Cancel", NULL,
        GADID_CANCEL_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    }
};

/* This is list of the kinds of gadgets used in the edit window. */
ULONG edit_window_gadget_kinds[NO_OF_EDIT_WINDOW_GADGETS] =
{
    STRING_KIND,      /* Name. */
    STRING_KIND,      /* Address one. */
    STRING_KIND,      /* Address two. */
    STRING_KIND,      /* Address three. */
    STRING_KIND,      /* Address four. */
    STRING_KIND,      /* Address code. */
    STRING_KIND,      /* Phone. */
    STRING_KIND,      /* Email. */
    STRING_KIND,      /* Comment. */
    BUTTON_KIND,      /* Ok. */
    BUTTON_KIND,      /* Cancel. */
};

/* Here is a list of the TAGs used in the edit window gadgets. */
Tag edit_window_gadget_tags[] =
{
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Name. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Address one. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Address two. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Address three. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Address four. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Address code. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Phone. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Email. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,       /* Comment. */
    GT_Underscore, '_', TAG_DONE,                       /* Ok. */
    GT_Underscore, '_', TAG_DONE                        /* Cancel. */
};

/* This is the labels used by the search type cycle gadget. */
UBYTE *search_type_labels[] =
{
    "Name", "Phone", "E-mail", NULL
};

/* These are the gadgets used by the string window. */
struct NewGadget string_window_new_gadgets[NO_OF_STRING_WINDOW_GADGETS] =
{
    {
        0, 0, 0, 0,
        "_Search type:", NULL,
        GADID_TYPE_CYCLE,
        PLACETEXT_LEFT,
        NULL,
        NULL
    },        
    {
        0, 0, 0, 0,
        NULL, NULL,
        GADID_STRING_STRING,
        NULL,
        NULL,
        NULL
    },
    {
        0, 0, 0, 0,
        "_Ok", NULL,
        GADID_OK2_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL,
    },
    {
        0, 0, 0, 0,
        "_Cancel", NULL,
        GADID_CANCEL2_BUTTON,
        PLACETEXT_IN,
        NULL,
        NULL
    }
};

/* These are the kinds of gadgets used in the string window. */
ULONG string_window_gadget_kinds[NO_OF_STRING_WINDOW_GADGETS] =
{
    CYCLE_KIND,         /* Type cycle gadget. */
    STRING_KIND,        /* Main string gadget. */
    BUTTON_KIND,        /* Ok button. */
    BUTTON_KIND         /* Cancel button. */
};

/* These are the tags used in the string window gadgets. */
Tag string_window_gadget_tags[] =
{
    GTCY_Labels, (Tag) search_type_labels, GT_Underscore,
        '_', TAG_DONE,                              /* Type cycle gadget. */
    GTST_MaxChars, GENERAL_STRING_SIZE, TAG_DONE,   /* Main string gadget. */
    GT_Underscore, '_', TAG_DONE,                   /* Ok button. */
    GT_Underscore, '_', TAG_DONE                    /* Cancel button. */
};
