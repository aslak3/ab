/* Address Book
 * by Lawrence Manning 
 *
 * filename: entries.c
 */

#include "ab.h"
#include "externals.h"

/* This function allocates memory for, and initalizes, the address book list. */
void open_entry_list(void)
{
    if (!(entry_list = AllocMem(sizeof(struct MinList), MEMF_CLEAR)))
        clean_up(RETURN_FAIL, allocmem_failure_string);

    NewList((struct List *) entry_list);

    number_of_entry_nodes = 0;
}

/* This function frees all the memory used by the address book list.  Assmumes
 * nothing. */
void close_entry_list(void)
{
    struct entry_node_type *entry_node;
    struct entry_node_type *next_entry_node;

    /* First see if there is a list to free. */
    if (entry_list)
    {
        /* Find first node. */
        entry_node = (struct entry_node_type *) entry_list->mlh_Head;

        /* Free all subsequent nodes, up until the last one. */
        while (entry_node->node.mln_Succ)
        {
            next_entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
            close_entry_node(entry_node);
            entry_node = next_entry_node;
            /* Note that the list header isn't updated as nodes are freed. */
        }

        /* Finally, free the list header itself. */
        FreeMem(entry_list, sizeof(struct MinList));

        entry_list = NULL;
    }
}

/* This function allocates memory for a new node. */
struct entry_node_type *open_entry_node(void)
{
    struct entry_node_type *entry_node = NULL;

    if (!(entry_node = AllocMem(sizeof(struct entry_node_type), MEMF_CLEAR)))
        clean_up(RETURN_FAIL, allocmem_failure_string);

    return (entry_node);
}

/* This function frees up a node.  The information contained in it, and the node
 * itself are freed. */
void close_entry_node(struct entry_node_type *entry_node)
{
    LONG address_count;

    /* See if the string pointer is used.  If it is, free the memory used
     * to store the string.  There is no need to NULL the string pointers
     * because the entire structure will be freed from memory. */
    if (entry_node->name)
        FreeMem(entry_node->name, strlen(entry_node->name) + 1);

    /* Do the same for the address text. */
    for (address_count = 0; address_count < 5; address_count++)
    {
        if (entry_node->address[address_count])
        {
            FreeMem(entry_node->address[address_count],
                strlen(entry_node->address[address_count]) + 1);
        }
    }

    if (entry_node->phone)
        FreeMem(entry_node->phone, strlen(entry_node->phone) + 1);

    if (entry_node->email)
        FreeMem(entry_node->email, strlen(entry_node->email) + 1);

    if (entry_node->comment)
        FreeMem(entry_node->comment, strlen(entry_node->comment) + 1);

    /* Remember to free the node itself too! */
    FreeMem(entry_node, sizeof(struct entry_node_type));
}

/* Add a contrusted node to the list.  It could be using details from the edit
 * window, or from a loaded address book project.  Assumes that the name field
 * contains a valid name. */
void link_entry_node_to_list(struct entry_node_type *link_entry_node)
{
    struct entry_node_type *entry_node;         /* Search pointer. */
    struct entry_node_type *pred_entry_node;

    /* If the list is empty, we can simply add the entry in.  Simple. */
    if (IsListEmpty((struct List *) entry_list))
    {
        link_entry_node->entry_node_number = 0;
        AddHead((struct List *) entry_list, (struct Node *) link_entry_node);
    }
    else
    {
        /* Now we see if the new node should become the new head of the list.
         * This happens if the new node is alphabetically before the head
         * node. */
        if (compare_names(link_entry_node->name, ((struct entry_node_type *)
            entry_list->mlh_Head)->name) < 0)
        {
            link_entry_node->entry_node_number = 0;
            AddHead((struct List *) entry_list, (struct Node *) link_entry_node);
        }
        else
        {
            /* The new node should now be placed somewhere within the list.
             * Set up the search pointer to point to the second node in the
             * list.  We know that the new node will go after the head node,
             * so we don't bother checking it twice. */
            entry_node = (struct entry_node_type *)
                entry_list->mlh_Head->mln_Succ;
            
            /* Keep searching while the new node is alphabetically after the
             * search node (or the same).  NOTE that this while loop relies
             * on C being 'keen' (not 'lazy') so that the expression stops
             * being evaluated if the first section (entry_node->node.mln_Succ)
             * is FALSE.  Otherwise an invalid string might be passed to
             * compare_names(), cos we are looking at the list header. */
            while (entry_node->node.mln_Succ &&
                (compare_names(link_entry_node->name, entry_node->name) >= 0))
            {
                entry_node = (struct entry_node_type *)  entry_node->node.mln_Succ;
            }

            /* The new node should be inserted AFTER the node before the one
             * which made us leave the above while loop. */
            pred_entry_node = (struct entry_node_type *) entry_node->node.mln_Pred;

            /* Set this new entry's number to the number of the entry before.
             * It will be incrememted at bottom of this function. */
            link_entry_node->entry_node_number = pred_entry_node->entry_node_number;

            /* Insert the node at the position obtained.  If this node is going
             * to be the last in the list, then entry_node will point
             * to the tail. */ 
            Insert((struct List *) entry_list, (struct Node *) link_entry_node,
                (struct Node *) pred_entry_node);
        }
    }

    /* Now we adjust all nodes following (and including) the one we just linked
     * into the list.  The current entry's number will prior to this point be
     * one less then the number required, so this loop makes the new entry's
     * number correct. */
    entry_node = link_entry_node;

    while (entry_node->node.mln_Succ)
    {
        entry_node->entry_node_number++;
        entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
    }

    /* Increment entries counter. */
    number_of_entry_nodes++;

}

/* This little function removes an entry node from the list. */
void remove_entry_node_from_list(struct entry_node_type *remove_entry_node)
{
    struct entry_node_type *entry_node;

    /* We need to adjust all the entry number for entries after the one
     * we are removing.  All entries following this one have their numbers
     * decrememted. */
    entry_node = remove_entry_node;

    while (entry_node->node.mln_Succ)
    {
        entry_node->entry_node_number--;
        entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
    }

    /* Do the actual node removal. */
    Remove((struct Node *) remove_entry_node);

    /* Decrement entries counter. */
    number_of_entry_nodes--;
}

/* This function copies the details held in the edit window's string gadgets
 * into the node. */
void copy_edit_window_to_entry_node(struct entry_node_type *entry_node)
{
    LONG length;

    /* See if there is a string in the name gadget. */
    if (length = strlen_gadget(edit_window_gadget_list[GADID_NAME_STRING]))
    {
        /* If there is copy it to the node. */
        if (!(entry_node->name = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);
        
        strcpy_gadget(edit_window_gadget_list[GADID_NAME_STRING], entry_node->name);
        
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_ADDRESS_ONE_STRING]))
    {
        if (!(entry_node->address[0] = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);
    
        strcpy_gadget(edit_window_gadget_list[GADID_ADDRESS_ONE_STRING],
            entry_node->address[0]);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_ADDRESS_TWO_STRING]))
    {
        if (!(entry_node->address[1] = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);
    
        strcpy_gadget(edit_window_gadget_list[GADID_ADDRESS_TWO_STRING],
            entry_node->address[1]);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_ADDRESS_THREE_STRING]))
    {
        if (!(entry_node->address[2] = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);
    
        strcpy_gadget(edit_window_gadget_list[GADID_ADDRESS_THREE_STRING],
            entry_node->address[2]);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_ADDRESS_FOUR_STRING]))
    {
        if (!(entry_node->address[3] = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);
    
        strcpy_gadget(edit_window_gadget_list[GADID_ADDRESS_FOUR_STRING],
            entry_node->address[3]);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_ADDRESS_FIVE_STRING]))
    {
        if (!(entry_node->address[4] = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);
    
        strcpy_gadget(edit_window_gadget_list[GADID_ADDRESS_FIVE_STRING],
            entry_node->address[4]);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_PHONE_STRING]))
    {
        if (!(entry_node->phone = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);

        strcpy_gadget(edit_window_gadget_list[GADID_PHONE_STRING], entry_node->phone);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_EMAIL_STRING]))
    {
        if (!(entry_node->email = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);

        strcpy_gadget(edit_window_gadget_list[GADID_EMAIL_STRING], entry_node->email);
    }

    if (length = strlen_gadget(edit_window_gadget_list[GADID_COMMENT_STRING]))
    {
        if (!(entry_node->comment = AllocMem(length + 1, MEMF_CLEAR)))
            clean_up(RETURN_FAIL, allocmem_failure_string);

        strcpy_gadget(edit_window_gadget_list[GADID_COMMENT_STRING], entry_node->comment);
    }
}

/* This is the main save routine.  Pass it a filename and it will attempt to
 * save the current entry list to disk.  Returns TRUE for success or FALSE for
 * failure.  Failure is caused by being unable to open the file for writing,
 * or if there are problems writing the data to disk. */
BOOL save_entry_list(UBYTE *filename)
{
    struct entry_node_type *entry_node;
    BPTR file_handle = NULL;
    LONG address_count;

    /* Open the file for writing. */
    if (!(file_handle = Open(filename, MODE_NEWFILE)))
        return (FALSE);

    if (!(write_string(FILE_HEADER_STRING, file_handle)))
    {
        Close(file_handle);
        return (FALSE);
    }

    entry_node = (struct entry_node_type *) entry_list->mlh_Head;

    while (entry_node->node.mln_Succ)
    {
        /* Name. */
        if (!(write_string(entry_node->name, file_handle)))
        {
            Close(file_handle);
            return (FALSE);
        }

        /* Address. */
        for (address_count = 0; address_count < 5; address_count++)
        {
            if (!(write_string(entry_node->address[address_count], file_handle)))
            {
                Close(file_handle);
                return (FALSE);
            }
        }

        /* Phone. */
        if (!(write_string(entry_node->phone, file_handle)))
        {
            Close(file_handle);
            return (FALSE);
        }

        /* E-mail. */
        if (!(write_string(entry_node->email, file_handle)))
        {
            Close(file_handle);
            return (FALSE);
        }

        /* Comment. */
        if (!(write_string(entry_node->comment, file_handle)))
        {
            Close(file_handle);
            return (FALSE);
        }

        entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;

        /* Now see if there are any more entries.  If there are, then write
         * FALSE, else write TRUE. */
        if (entry_node->node.mln_Succ)
        {
            if (FPutC(file_handle, FALSE) == DOSTRUE)
            {
                Close(file_handle);
                return (FALSE);
            }
        }
        else
        {
            if (FPutC(file_handle, TRUE) == DOSTRUE)
            {
                Close(file_handle);
                return (FALSE);
            }
        }
    }

    Close(file_handle);
    return (TRUE);
}

/* This function loads in the address file.  It adds new entries from the disk
 * file into the current list.  If a new list is required, then the current
 * one should be removed before loading. */
BOOL load_entry_list(UBYTE *file_name)
{
    BPTR file_handle = NULL;
    struct entry_node_type *entry_node;
    BOOL last_entry_node = FALSE;
    UBYTE *header_text;
    LONG address_count;

    if (!(file_handle = Open(file_name, MODE_OLDFILE)))
        return (FALSE);

    /* Read in the header text. */
    if (!(read_string(&header_text, file_handle)))
    {
        Close(file_handle);
        return (FALSE);
    }

    /* Check the header text is correct. */
    if (strcmp(header_text, FILE_HEADER_STRING) != 0)
    {
        Close(file_handle);
        return (FALSE);
    }

    /* Remember to free the header string text.  Didn't do this before v1.0b. */
    FreeMem(header_text, strlen(header_text) + 1);

    while (!last_entry_node)
    {
        if (!(entry_node = open_entry_node()))
        {
            Close(file_handle);
            return (FALSE);
        }

        /* Name. */
        if (!(read_string(&entry_node->name, file_handle)))
        {
            close_entry_node(entry_node);
            Close(file_handle);
            return (FALSE);
        }

        /* Address. */
        for (address_count = 0; address_count < 5; address_count++)
        {
            if (!(read_string(&entry_node->address[address_count],
                file_handle)))
            {
                close_entry_node(entry_node);
                Close(file_handle);
                return (FALSE);
            }
        }

        /* Phone. */
        if (!(read_string(&entry_node->phone, file_handle)))
        {
            close_entry_node(entry_node);
            Close(file_handle);
            return (FALSE);
        }

        /* Email. */
        if (!(read_string(&entry_node->email, file_handle)))
        {
            close_entry_node(entry_node);
            Close(file_handle);
            return (FALSE);
        }

        /* Comment. */
        if (!(read_string(&entry_node->comment, file_handle)))
        {
            close_entry_node(entry_node);
            Close(file_handle);
            return (FALSE);
        }

        /* Now see if this is the last node. */
        if ((last_entry_node = FGetC(file_handle)) == DOSTRUE)
        {
            close_entry_node(entry_node);
            Close(file_handle);
            return (FALSE);
        }

        /* Now add this entry node to the list. */
        link_entry_node_to_list(entry_node);

   } /* Go back for more. */

    Close(file_handle);
    return (TRUE);
}

/* This function searches for a specific entry node, starting from a specific
 * node.  The start node is a parameter, whist search string and type are 
 * globals.  Returns the address of the found node, or NULL if it couldn't
 * find the node.  NEW: If skip_current_entry is TRUE then the search starts
 * from the node following start_entry_node. */
struct entry_node_type *entry_search(struct entry_node_type *start_entry_node,
    BOOL skip_current_entry)
{
    struct entry_node_type *entry_node;
    struct entry_node_type *found_entry_node = NULL;
    BOOL match_found = FALSE;
    UBYTE *test_string;

    if (skip_current_entry)
        entry_node = (struct entry_node_type *) start_entry_node->node.mln_Succ;
    else
        entry_node = start_entry_node;

    /* Start from the inital node, and continue searching until we reach the
     * end of the list, or a match is found. */
    while (entry_node->node.mln_Succ && !match_found)
    {
        /* See what kind of search we are doing, and set the test string up. */
        switch (search_type)
        {
            case SEARCH_FOR_NAME:
                test_string = entry_node->name;
                break;

            case SEARCH_FOR_PHONE:
                test_string = entry_node->phone;
                break;

            case SEARCH_FOR_EMAIL:
                test_string = entry_node->email;
                break;

            /* In case we are passed an invalid type, return NULL. */
            default:
                return (NULL);
                break;
        }

        /* The field we are searching must be valid.  We check this by ensuring
         * that test_string is non NULL. */
        if (test_string)
        {
            /* See if there is a match. */
            if (stristr(test_string, search_string))
            {
                match_found = TRUE;
                found_entry_node = entry_node;
            }
        }

        /* Go to next entry node. */
        entry_node = (struct entry_node_type *) entry_node->node.mln_Succ;
    }

    return (found_entry_node);
}

/* Function to order names.  Typically called when an entry is added to the
 * entry list.  name_one would be the name for the new entry, and name_two
 * the name for an entry already in the list.  Returns < 0 if name_one is
 * before name_two, 0 if they are the same, or > 0 if name_one is after
 * name_two.  Actual function depends on the OLD_SORTING tooltype.  If this
 * is FALSE then the last name from the complete string is compared, other-
 * wise the whole string is considered.  This is a HACK. */
LONG compare_names(UBYTE *name_one, UBYTE *name_two)
{
    UBYTE *start_last_name_one;
    UBYTE *start_last_name_two;

    /* Determine type of comparison to do.  Old type? */
    if (settings.old_sorting)
        return (stricmp(name_one, name_two));
    else
    {
        /* Firt find the end of each string.  This is the start address of each
         * string, plus its length.  start_last_name_one will point to the
         * trailing NULL in name_one. */
        start_last_name_one = name_one + strlen(name_one);
        start_last_name_two = name_two + strlen(name_two);

        /* Now, starting at the end of this string, move towards the start and
         * stop when we get the beginning or we find a space.  Need to adjust
         * start_last_name_one so it points to the first character AFTER the
         * space. */
        while (start_last_name_one >= name_one && *start_last_name_one != ' ')
            start_last_name_one--;
        start_last_name_one++;

        while (start_last_name_two >= name_two && *start_last_name_two != ' ')
            start_last_name_two--;
        start_last_name_two++;

       /* Call stricmp with these strings. */
       return (stricmp(start_last_name_one, start_last_name_two));
    }
}
