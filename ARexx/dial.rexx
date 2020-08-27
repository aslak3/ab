/* Program to dial the phone number of a given person. */

/* Make sure AB is running. */
if ~show(PORTS, 'AB_AREXX') then do
    say 'AB is not currently running!  Please start it and try again.'
    exit
    end

address 'AB_AREXX'

options results

/* Ask for and get the name. */
say 'Enter name ?'
parse pull NAME$
FOUND = 0;

/* Go to the first entry. */
'FIRST_ENTRY'

/* Start the search going. */
'SEARCH 1 'NAME$
FINISHED$ = RESULT

/* Continue until we have reached the end of the search. */
do while RC = 0 & FINISHED$ ~= 'Not found'

    /* If the search matches, then ask if he wants the number dialed.  If he
     * does, then dial it. */ 
    'GET_ENTRY_DETAILS 1'
    say 'Dial 'RESULT' [Y/N] ?'
    pull Y_OR_N$
    if Y_OR_N$ = 'Y' then do
        'GET_ENTRY_DETAILS 7'
        say 'Dialing 'RESULT
        'DIAL_ENTRY'
        if RC ~= 0 then do
            say 'Unable to dial.'
            end
        end
        FOUND = 1

        /* Search for more matches. */
        'SEARCH_AGAIN'
        FINISHED$ = RESULT
    end

/* Print up a message if no matches were found. */
if FOUND ~= 1 then do
    say NAME$' was not found!'
    end

exit
