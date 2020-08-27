/* Address Book
 * by Lawrence Manning 
 *
 * filename: printer.c
 */

#include "ab.h"
#include "externals.h"

/* This function attempts to open the printer device as a file.  Returns TRUE
 * for success, FALSE for failure. */
BOOL open_printer_file(void)
{
    if (!(printer_file = Open("PRT:", MODE_NEWFILE)))
        return (FALSE);
    else
        return (TRUE);
}

/* This function closes the printer file.  Checks to see if the printer file
 * is actually open before closing it. */
void close_printer_file(void)
{
    if (printer_file)
    {
        Close(printer_file);
        printer_file = NULL;
    }
}

/* This function prints out a entry to the printer.  Returns TRUE for success,
 * FALSE for failure.  Failure caused by the printer file not being open at 
 * call time, or if there were problems FPuts()ing the data to the printer. */
BOOL write_entry_to_printer_file(struct entry_node_type *entry_node,
    UBYTE *header_string)
{
    UBYTE *large_entry_string;
    LONG write_success;
    UBYTE entry_string[GENERAL_STRING_SIZE + 100]; /* Holds 1 print line. */
    LONG address_count;

    /* Must check to see if printer file is open before proceding. */
    if (printer_file)
    {
        if (large_entry_string = AllocMem(2000, MEMF_CLEAR))
        {
            /* Clear large_entry_string. */
            strcpy(large_entry_string, "");

            /* Write the record out to a string. */
            /* sprintf() the lines to local strings, and add these to the
             * large_entry_string a line at a time. */
            if (settings.print_headers)
            {
                sprintf(entry_string, "%s", header_string);
                strcat(large_entry_string, entry_string);
            }

            if (entry_node->name)
            {
                sprintf(entry_string, "Name:    %s\n", entry_node->name);
                strcat(large_entry_string, entry_string);
            }

            if (entry_node->address[0])
            {
                sprintf(entry_string, "Address: %s\n", entry_node->address[0]);
                strcat(large_entry_string, entry_string);
            }

            for (address_count = 1; address_count < 5; address_count++)
            {
                if (entry_node->address[address_count])
                {
                    sprintf(entry_string, "         %s\n",
                        entry_node->address[address_count]);
                    strcat(large_entry_string, entry_string);
                }
            }

            if (entry_node->phone)
            {
                sprintf(entry_string, "Phone:   %s\n", entry_node->phone);
                strcat(large_entry_string, entry_string);
            }

            if (entry_node->email)
            {
                sprintf(entry_string, "E-mail:  %s\n", entry_node->email);
                strcat(large_entry_string, entry_string);
            }

            if (entry_node->comment)
            {
                sprintf(entry_string, "Comment: %s\n", entry_node->comment);
                strcat(large_entry_string, entry_string);
            }

            /* Seperate each entry with a blank line. */
            strcat(large_entry_string, "\n");

            /* Write the string to the printer file, and check the result. */
            write_success = FPuts(printer_file, large_entry_string);

            /* Regardless of success or failure, free buffer memory. */
            FreeMem(large_entry_string, 2000);

            /* See if put was executed sucessfuly. */
            if (write_success == EOF)
                return (FALSE);
            else
                return (TRUE);
        }
        /* This is if AllocMem fails. (no memory for buffer.) */
        else
            return (FALSE);
    }
    /* And this is if printer failures to open. */
    else
        return (FALSE);
}
