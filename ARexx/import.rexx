/* Program to import person details from a file and put them in an AB
 * project. */

/* Make sure AB is running. */
if ~show(PORTS, 'AB_AREXX') then do
    say 'AB is not currently running!  Please start it and try again.'
    exit
    end

address 'AB_AREXX'

options results

/* Ask for and get the filename. */
say 'Enter filename ?'
parse pull FILENAME$

/* Open the file. */
call Open(FILEHANDLE, FILENAME$)

/* Initalize some variables. */
NUMBER_ADDED=0
ERROR=0

do while ERROR=0
    /* Get the name field from the file. */
    TEXT_LINE$=Readln(FILEHANDLE)
    /* Check it for a blank line.  If this happens then the field line is
     * either empty, or we have reached the end of the file. */
    if TEXT_LINE$='' then
        ERROR=1
    else do
        /* Otherwise make a new entry with this name. */
        NUMBER_ADDED=NUMBER_ADDED+1
        'NEW_ENTRY 'TEXT_LINE$

        /* For each of the other fields, read in the line from the file.  If it
         * empty then make the field in the AB project empty, or simply copy
         * the text across to the desired field. */
        do FIELD_NO=2 to 9 by 1 & ERROR=0
            TEXT_LINE$=Readln(FILEHANDLE)
            if TEXT_LINE$='' then
                'PUT_ENTRY_DETAILS 'FIELD_NO' Field empty'
            else do
                'PUT_ENTRY_DETAILS 'FIELD_NO TEXT_LINE$
                end
            end
        end
    end

say 'Added 'NUMBER_ADDED' entries.'
exit
