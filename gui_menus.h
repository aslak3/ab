/* Address Book
 * by Lawrence Manning 
 *
 * filename: gui_menus.h
 */

struct NewMenu new_menus[] =
{
    {
        NM_TITLE, "Project", NULL,
        0, 0, NULL
    },
    {
        NM_ITEM, "New", "N",
        0, 0, NULL
    },
    {
        NM_ITEM, "Open...", "O",
        0, 0, NULL
    },
    {
        NM_ITEM, "Save", "S",
        0, 0, NULL
    },
    {
        NM_ITEM, "Save As...", "A",
        0, 0, NULL
    },
    {
        NM_ITEM, "Print", "P",
        0, 0, NULL
    },
    {
        NM_ITEM, NM_BARLABEL, NULL,
        0, 0, NULL
    },
    {
        NM_ITEM, "About...", "?",
        0, 0, NULL
    },
    {
        NM_ITEM, NM_BARLABEL, NULL,
        0, 0, NULL
    },
    {
        NM_ITEM, "Hide", "H",
        0, 0, NULL
    },
    {
        NM_ITEM, "Quit", "Q",
        0, 0, NULL
    },
    {
        NM_TITLE, "Edit", NULL,
        0, 0, NULL
    },
    {
        NM_ITEM, "Search...", "F",
        0, 0, NULL
    },
    {
        NM_ITEM, "Search Again", NULL,
        0, 0, NULL
    },
    {
        NM_TITLE, "Settings", NULL,
        0, 0, NULL
    },
    {
        NM_ITEM, "Print Headers?", NULL,
        CHECKIT | MENUTOGGLE, 0, NULL
    },
    {
        NM_ITEM, "Print All?", NULL,
        CHECKIT | MENUTOGGLE, 0, NULL
    },
    {
        NM_ITEM, NM_BARLABEL, NULL,
        0, 0, NULL
    },
    {
        NM_ITEM, "Save Settings", NULL,
        0, 0, NULL
    },
    {
        NM_END, NULL, NULL,
        0, 0, NULL
    }
};
