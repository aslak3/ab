/* Program to list the names and addresses of everyone on the current
 * project. */

/* Make sure AB is running. */
if ~show(PORTS, 'AB_AREXX') then do
    say 'AB is not currently running!  Please start it and try again.'
    exit
    end

address 'AB_AREXX'

options results

END = 0

/* Go to the first entry. */
'FIRST_ENTRY'

n = 0

/* Continue until we have reached the end of the project. */
do while END = 0

    n = n + 1

    /* Print the name of this entry. */
    'GET_ENTRY_DETAILS 1'
    say RESULT

    field = 2
    do while field <= 6
        'GET_ENTRY_DETAILS 'field
        if RESULT ~= 'Field empty' then
            say RESULT

        field = field + 1
        end

    'GET_ENTRY_DETAILS 8'
    if RESULT ~= 'Field empty' then
        say RESULT

    /* Go to the next entry. */
    'NEXT_ENTRY'
    END = RC

    say

    end

exit
