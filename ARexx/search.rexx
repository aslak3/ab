/* Program to search for a specific entry and display it. */

/* Make sure AB is running. */
if ~show(PORTS, 'AB_AREXX') then do
    say 'AB is not currently running!  Please start it and try again.'
    exit
    end

address 'AB_AREXX'

options results

'HIDE'

/* Ask for and get the name. */
say 'Enter name ?'
parse pull NAME$
FOUND = 0;

/* Go to the first entry. */
'FIRST_ENTRY'

/* Start the search going. */
'SEARCH 1 'NAME$
FINISHED$ = RESULT

/* See if we have found it. */
if RC = 0 & FINISHED$ ~= 'Not found' then do
    'SHOW'
    end
if RC = 0 & FINISHED$ = 'Not found' then do
    say NAME$' was not found!'
    end
if RC ~= 0 then do
    say 'Unable to search'
    end

exit

