# AB

This is the first large C program I ever wrote. It is an address book
program for AmigaOS 3. The below is a conversion of the AB.guide help
file to HTML, made with the script I found at
https://github.com/niklasekstrom/amigaguide_to_html

I made this repo just to preserve the code, nothing more.

<div id="main">
<hr/>
<p><b>Contents page</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>

                        <b>AB v1.2b by Lawrence Manning</b>

                   Please do not distribute this version! *

                         This program is <i>FREEWARE!</i>


                                 <b>CONTENTS</b>


    <a href="#distribution">     Distribution    </a>    FREEWARE notice.

    <a href="#introduction">     Introduction    </a>    What this program does.

    <a href="#installation">     Installation    </a>    The system requirements are listed here too.

    <a href="#starting">    Starting up AB   </a>    Loading up through Workbench or CLI.

    <a href="#operation">      Operation      </a>    The main section.

    <a href="#menus">        Menus        </a>    Description of the menus.

    <a href="#configure">     Configuring     </a>    Setting up AB to suit you.

    <a href="#arexx_support">    AREXX support    </a>    All about AB'S ARexx port.

    <a href="#history">   Version History   </a>    Fascinating...

    <a href="#author">       Author        </a>    My beta-testers are listed here too.


         <b>AB and its documentation are (c) 1995-97 Lawrence Manning</b>

* This version might (well almost certainly does) have some bugs and also
the docs haven't been checked for spelling mistakes.  So don't give it to
anyone else please. :)
</pre>
</div>
<div id="distribution">
<hr/>
<p><b>AB is FREEWARE!</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Distribution and Warranty</b>

AB is FREEWARE.  There are no limitations to how it may be distributed,
provided that the original archive remains intact, and none of the files are
removed or modified.  It can be put in public domain libraries, on Fred Fish
CDS and the like, provided I am notified of this.  If it is included on any
magazine cover-disks, then I would also like to be informed.  The archive
contains the following files:

    AB/AB                    Executable.
    AB/AB.info               The executable's icon.
    AB/AB.guide              Documentation.
    AB/AB.guide.info         Documentation's icon.
    AB/ARexx                 Directory containing some example scripts.
    AB/ARexx.info            Above directory's icon.
    AB/ARexx/                Some example scripts are in this directory.

Please do not powerpack (or similar) any of the files, as this can decrease
compatible with any future systems.

No warranty is given for use of this program.  Use it at your own risk!  I
can not be held responsible for any loss of data, or damage to equipment. So
if your hard disk crashes when you use the program, don't go moaning to me.
It wasn't my fault, honest!
</pre>
</div>
<div id="introduction">
<hr/>
<p><b>Introduction to AB</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Introduction</b>

AB (short for Address Book) is a little utility for storing and accessing
the names of addresses of people you know.  It was primarily designed to be
small, and easy to use.  There is also no limit to the length of the
individual fields.  Actually, the limit is 200 characters, but this should
be acceptable to everyone.

You can store the following pieces of information for each person: the name,
address (five lines), phone number, E-mail address, and a comment about the
individual.  All fields are optional, except for the person's name.

After entering the details, it is possible to search for specific people,
and also produce printouts of people in the database.  Another handy feature
is the ability to use the Amiga's audio output to tone-dail someone's phone
number.  The program can also be controlled via ARexx scripts, examples
included.

<b>Features</b>

    *   Unlimited number of entries.  Memory permitting of course.

    *   Field text length limited to 200 characters.  Should be enough
        for everyones' needs.

    *   Configurable printing support.

    *   Font-sensitive GUI.

    *   Tone-dial using Amiga's audio output.

    *   Almost entirely controllable using ARexx scripts.  Example scripts
        included in distribution.

    *   Installs as a commodity.


    See: <a href="#installation">Installation</a>, <a href="#starting">Starting up AB</a> and <a href="#operation">Operation</a>
</pre>
</div>
<div id="installation">
<hr/>
<p><b>Installing AB</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>System Requirements</b>

The only system requirement is that your Amiga has OS 3.0 installed.  It
will run fine from a floppy based system, and has very small memory
requirements.


<b>Installation</b>

Simple.  Make a directory on your hard disk, and drag the icons across.  AB
does not need any external libraries, besides those present on the standard
Workbench disk.
</pre>
</div>
<div id="starting">
<hr/>
<p><b>Loading AB from the Workbench and CLI</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Starting AB</b>

Workbench usage

    AB can be started from the Workbench, either through the program icon,
or through a suitably configured project icon.  AB can also be <a href="#configure">configured</a> by
setting tooltypes in the program icon.  If you are going to run AB via the
WBStartup drawer, it is advisable to set the DONOTWAIT tooltype.

CLI usage

    If the program is launched through a CLI, you can also automatically
load an AB project by supplying its filename as an argument.  eg:

    AB s:addresses

would attempt to load s:addresses into AB on startup.
</pre>
</div>
<div id="operation">
<hr/>
<p><b>Program operation</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Operation</b>

Upon loading, you will be presented with the main window.  This is where you
can access the menus, add new people to the database, and obtain print-outs.
Most of this window is used to display record details.  At the bottom of the
window is a scroll bar for moving through the list of people in the project,
as well as buttons for adding new people, editing the currently viewed
record, and removing people from the database.  It is also possible to move
through the project using the cursor keys.

To add a new person, click on the button marked New.  You will be presented
with another window, where you should type in the details for this new
person.  Any boxes can be left blank, except the name one.  When you have
finished, click on the Ok button at the bottom.  If you decide not to add
this person, click on Cancel, or just close the window.

If you want to edit the currently displayed person's details, click on Edit.
You will be presented with the same window as described above.  Change the
fields which need changing, and click on Ok.  Cancel aborts any changes you
have made.

Once you're done adding everyones' details, you can get a printout by
selecting Print from the Project menu.  It is possible to configure the
output that is sent to the print using the Settings menu.

It is also possible to search the database, and to have your Amiga dial a
persons phone number for you.  AB also installs as a commodity, so it is
possible to control it via the Exchange program.


    See: <a href="#menus">Menus</a>, <a href="#tonedial">Tone-dial usage</a> and <a href="#configure">Configuring</a>
</pre>
</div>
<div id="menus">
<hr/>
<p><b>Description of the menus</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Menus</b>

The menus are divided into three sections:

   <a href="#project_menu">   Project    </a>

   <a href="#edit_menu">   Edit       </a>

   <a href="#settings_menu">   Settings   </a>
</pre>
</div>
<div id="project_menu">
<hr/>
<p><b>Description of the Project menu</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Project Menu</b>

New  (shortcut Amiga-N)

    This clears the current address book from memory.

Open...  (shortcut Amiga-O)

    This loads a new address book database into memory.  If the database
could not be loaded for some reason, then you will loose the currently
viewed database.

Save  (shortcut Amiga-S)

    This saves the currently named project.  If the project has no filename,
then you will be asked to enter one using the standard file requester.

Save As...  (shortcut Amiga-A)

    This saves the current project under a new name.

Print  (shortcut Amiga-P)

    This prints entries from the current project.  The actual format of the
entries printed depends on choices made in the <a href="#settings_menu">settings</a> menu.

About...  (shortcut Amiga-?)

    Displays some information about the program.

Hide  (shortcut Amiga-H)

    Removes AB's GUI, but does not quit the program.  You can open the GUI
again either by pressing the user-definable hotkey (by default it is ctrl
alt a) or by using the Commodities Exchange program.

Quit  (shortcut Amiga-Q)

    I wonder what this does?
</pre>
</div>
<div id="edit_menu">
<hr/>
<p><b>Description of the Edit menu</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Edit Menu</b>

Search  (shortcut Amiga-F)

    This option searches for a specific record.  It will open up a new
window where you can type in the search details.  A cycle button selects the
type of search to perform.  Name, phone number and E-mail address searches
are possible.  The search string entered can be a partial string so you
could, for example, search for people with a London phone number. Another
use for this is if you can only remember the first name of a person you are
searching for. If the person is found, then you will be taken to that
record, else you'll hear a beep.

Search Again  (no shortcut)

    This option repeats a previously defined record search.
</pre>
</div>
<div id="settings_menu">
<hr/>
<p><b>Description of the Settings menu</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Settings Menu</b>

Print Headers?  (no shortcut)

    Check this menu to print the "--- Entry x of y ---" text that appears at
the top of every printed entry.

Print All?  (no shortcut)

    Check this menu item to make AB print all entries in the current
project. If this is not checked, then only the current entry is printed.

Save Settings  (no shortcut)

    This option saves current window positions into the program's tooltypes.
See <a href="#configure">Configuring</a> for more information.
</pre>
</div>
<div id="tonedial">
<hr/>
<p><b>Tone-dial usage</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Using AB's tone-dial feature</b>

AB has a built in feature that allows the user to dial the phone number of
the currently viewed person.  To use it, hold the handset near the left
speaker of your Amiga setup. Then press the Dial button at the bottom of
AB's main window. You'll then hear the dial-tones.  When the last number has
been 'dialled' the call should have been made.

If you have problems getting audio output to work, you may have an
application running which is using the audio channels.  Close that program
down and try clicking on Dial again.  AB does not 'hog' the audio channels
like some programs; it allocates the channels only when it actually needs to
dial the number.

It is possible to configure AB's tone-dial feature in order to change the
duration of the dial-tones, and the delay between successive tones. However,
it is not likely that you will need to do this as the default settings
should work.


    See: <a href="#configure">Configuring</a>
</pre>
</div>
<div id="configure">
<hr/>
<p><b>Configuring AB</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Configuring AB</b>

The following tooltypes are supported by AB:

    MAIN_WINDOW_LEFT    :   Horizontal position of the main window.

    MAIN_WINDOW_TOP     :   Vertical position of the main window.

    MAIN_WINDOW_WIDTH   :   Width of the main window. *

    EDIT_WINDOW_LEFT    :   Horizontal position of the edit window.

    EDIT_WINDOW_TOP     :   Vertical position of the edit window.

    EDIT_WINDOW_WIDTH   :   Width of the edit window. *

    DIAL_DURATION       :   The length of an individual tone-dail tone, in
                            50ths of a second.

    DIAL_DELAY          :   The time-gap between each tone-dial tone, in
                            50ths of a second.

    PUBLIC_SCREEN       :   Name of the public screen that AB should try to
                            open on.  If it can't open on this screen then
                            it will try the default public screen.

    PRINT_HEADERS       :   YES prints entry headers, NO doesn't.

    PRINT_ALL           :   YES prints every entry, NO only prints the
                            currently displayed one.

    CX_PRIORITY         :   Commodity priority.

    CX_POPKEY           :   The hot key which when pressed will open AB's
                            main window.  Default is ctrl alt a.

    CX_POPUP            :   If this is YES then the main window will open
                            automatically when AB is started.  NO means it
                            will not.

    OLD_SORTING         :   If NO then sorting is determined by the last
                            name in the Name field, YES means the whole
                            field is considered.  Default is NO.

All settings have sensible defaults.

* These settings will be ignored if the window is smaller then calculated
  minimum size.

    See: <a href="#tonedial">Tone-dail usage</a> and <a href="#settings_menu">Settings menu</a>
</pre>
</div>
<div id="arexx_support">
<hr/>
<p><b>AB supports ARexx</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>AB supports ARexx</b>

The ARexx port AB uses is called "AB_AREXX".  This port allows you to
control AB using ARexx scripts.  You can do nearly all the things you would
do with AB's ARexx port as you would with the GUI.

The following code shows how to set up a script to give commands to AB:

    /* Fragment of code showing how to access AB's ARexx port. */

    /* First check that AB is running. */
    if ~(show(PORTS, 'AB_AREXX')) then do
        say 'AB is not currently running!  Please start it and try again.'
        exit
        end

    /* Now direct commands to AB. */
    address 'AB_AREXX'

The code above directs ARexx to send commands to AB.  For instance, the
line:

    'DIAL_ENTRY'

would make AB tone-dial the currently displayed person.


<b>About AB's return-codes</b>

After you call one one AB's functions you can see if it succeeded or not by
looking at the RC variable.  It can take one of four values, depending on
the severity of the error:

    RC = 0      Command was executed successfully.
    RC = 5      Command failed to execute, but was syntactically correct.
    RC = 10     Command's parameter(s) was not specified, or was invalid.
    RC = 20     Command was not recognised by AB.

Also, several commands return the ARexx variable RESULT, which contains
additional information.  For instance, the <a href="#arexx_commands">GET_ENTRY_DETAILS</a> command uses
the RESULT variable to tell the script the contents of a particular field in
the current record.

As is usual for ARexx scripts, the command 'options results' must be issued
in order to receive the commands RESULT value.


    See: <a href="#arexx_commands">ARexx commands</a> and <a href="#example_scripts">Example scripts</a>
</pre>
</div>
<div id="arexx_commands">
<hr/>
<p><b>AB supports loads of commands!</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>ARexx command list</b>

Command:
    QUIT [NO_CONFIRM]

Function:
    QUIT quits AB.  If NO_CONFIRM is specified, then there will be no
confirmation requester if the project has changed.

Results:
    RC is always 0.


Command:
    HIDE

Function:
    Removes AB's GUI from the screen.

Results:
    RC is always 0.


Command:
    SHOW

Function:
    Opens AB's GUI.  If the selected public screen is no longer open, then
AB will appear on the default screen, which is usually Workbench.

Results:
    RC is always 0.


Command:
    FIRST_ENTRY, PREV_ENTRY, NEXT_ENTRY and LAST_ENTRY

Function:
    These commands navigate about the project.

Results:
    See <a href="#arexx_support">ARexx support</a>.


Command:
    DELETE_ENTRY

Function:
    This command deletes the current entry.

Results:
    See <a href="#arexx_support">ARexx support</a>.


Command:
    DIAL_ENTRY

Function:
    This command dials the currently viewed entry, using the Amiga's audio
output.

Results:
    Command fails (RC of 5) if any of the following occur: Project is empty,
current entry does not contain a phone number, audio.device fails to open,
or the audio channels are stolen by another application during playing of
the dial-tones.


Command:
    DIAL_NUMBER &lt;number>

Function:
    This command can be used to dial the phone number specified in
&lt;number>.

Results:
    RC will be 5 if AB was not able to dial the phone number.


Command:
    NEW_PROJECT, OPEN_PROJECT &lt;filename>, SAVE_PROJECT, SAVE_AS_PROJECT
&lt;filename>

Function:
    These four commands load, save and remove projects in the same way as if
the command was selected from the project menu.  &lt;filename> is the name of
the project file to use.

Results:
    See <a href="#arexx_support">ARexx support</a>.


Command:
     SEARCH &lt;type> &lt;string>

Function:
    Starts a search for &lt;string>.  The type of search depends on the &lt;type>
specified:

    1       Name searches.
    2       Phone number searches.
    3       E-mail address searches.

If the search is successful, then the matching entry will be displayed. Note
that the presently displayed entry will also be searched, so if that entry
matches the search parameters then nothing will appear to have changed.

Results:
    RC will be 0 regardless of weather a match is found or not.  If a match
is not found then RESULT will be 'Not found'.  See the <a href="#example_scripts">example scripts</a> for a
demonstration of searching.


Command:
    SEARCH_AGAIN

Function:
    Repeats a previously defined search.  Note that the search starts at the
next entry after the current one.

Results:
    See the results section of the command <a href="#arexx_commands">SEARCH</a>.


Command:
    SAVE_SETTINGS

Function:
    This command saves the current settings the the program icon's
tooltypes.

Results:
    See <a href="#arexx_support">ARexx support</a>.


Command:
    GET_ENTRY_DETAILS &lt;field number>

Function:
    This command extracts information from the currently viewed entry.
&lt;field number> is the number of the field that you wish to look at:

    1       Name field.
    2 to 6  Address fields one through five.
    7       Phone number field.
    8       E-mail address field.
    9       Comment field.

Results:
    If the field is empty, then RESULT will contain the string 'Field
empty'.  Otherwise, RESULT will be the string contained in the relevant
field of the current entry.


Command:
    NEW_ENTRY &lt;string>

Function:
    This command creates a new entry with the name set to &lt;string>.  It is
made the currently viewed entry.

Results:
    See <a href="#arexx_support">ARexx support</a>.


Command:
    PUT_ENTRY_DETAILS &lt;field number> &lt;string>

Function:
    This command changes the text in the specified field to that of
&lt;string>. The &lt;field number> used is the same as for the <a href="#arexx_commands">GET_ENTRY_DETAILS</a>
command.  Note that if the name field is changed, then the ordering of the
entries within the project will change also.  This is because the entries
are sorted alphabetically with respect to the names.  Passing a &lt;string> of
'Field empty' (case is important) will clear the selected field.  It is not
possible to clear the Name field of an entry in this way though.

Result:
    See <a href="#arexx_support">ARexx support</a>.
</pre>
</div>
<div id="example_scripts">
<hr/>
<p><b>AB comes with some example scripts</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Example scripts</b>

AB's archive contains an additional directory (called ARexx) which contains
some example scripts for you to look at, and see how the various commands
function.

To use these scripts, run AB and then open a Shell window.  CD to the ARexx
directory and then type RX &lt;script name>, where &lt;script name> is the name of
the script that you wish to run.


<b>List of the example scripts</b>

Script:
    <a href="ARexx.html#dial.rexx%2Fmain">dial.rexx</a>

What it does:
    This script asks for a persons name.  Then it searches through the entry
list and tone-dials any entries that match the name specified.


Script:
    <a href="ARexx.html#import.rexx%2Fmain">import.rexx</a>

What it does:
    A simple importing script.  Supply this script with the name of a file
containing entry details and this script will add these entries into the
current project.  Each entry should consist of nine lines.  The first one
should be the name field, the second should be address line one, and so on.


Script:
    <a href="ARexx.html#names_and_addresses.rexx%2Fmain">names_and_addresses.rexx</a>

What it does:
    The script outputs the names and address (including e-mail) of all
entries in the current project.


Script:
    <a href="ARexx.html#search.rexx%2Fmain">search.rexx</a>

What it does:
    Asks user for a name, and then opens the AB window with the person
specified displayed.  Demonstrates searching commands, as well as hideing
and showing from within an ARexx script.


Script:
    <a href="Arexx.html#simple_list.rexx%2Fmain">simple_list.rexx</a>

What it does:
    This script simply prints out a list of all the names of people in the
project.
</pre>
</div>
<div id="history">
<hr/>
<p><b>Version history for AB</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Version History</b>

v0.1 Unknown date

      * Test version.

v1.0 7-11-95

      * First release (on my web page)

v1.0a 21-11-95, 28-11-95, 3-12-95, 18-12-95

      * Major bug in search function fixed.  It resulted in an accidental
        change of search type.  Picard reported strange things when
        searching.
      * Added command line support, you can now load an AB file on 
        startup.
      * Added default tool and shift-clicking to wbmain().  Only works on
        AB files which are in the same dir as program.  Need to fix this!

v1.0b 12-1-96, 13-1-96, 20-1-96, 22-1-96

      * Added printer support at last!  Pretty basic still, but useful.
      * Rearranged GUI layout slightly.  Made the address details in the
        entry_node into an array of 5 string pointers.
      * Added tone dialling.  Tested as a stand alone program, but is un-
        tested in AB.  Rearranged gadget layout again to incorporate the
        dial gadget.
      * Added tooltypes to make tone dialling times configurable.  Fixed
        Workbench startup.  Now accepts project icons in any directory.

v1.1 22-1-96

      * New release on web page.

v1.1a 25-1-96, 31-1-96

      * General tweaking.
      * Added the quit message requester.  If changes have been made 
        since the last save, then a warning requester will appear.

v1.1b 4-2-96, 6-2-96, 10-2-96

      * GUI is now font sensitive.  Doesn't check that window will fit on
        screen though.
      * Added an ARexx port!  Command parser is a bit primitive, but it
        works.  15 commands are supported.
      * Added ARexx command list and an example script to the
        documentation.

v1.1c 24-2-95, 28-2-96

      * At the request of Philip Reyntens, printing has been improved so
        that blank fields are no longer printed.  Also added two more
        commands: NEW_ENTRY and PUT_ENTRY_DETAILS.  These two commands
        make it possible to add new entries from with an ARexx script.
      * Added the two new commands to the documentation.

v1.1d 5-3-96, 6-3-96, 7-3-96, 9-3-96

      * Added Public Screen support.  It appears that some programs
        which open public screens (namely MUI and Memacs) do not close
        their public screens when the last visitor window screen has left.
        Not sure if this is correct behavior yet.
      * Added a Zoom gadget to AB's main window.
      * Added Print Headings? and Print All? menu items, as well as their
        respective tooltypes.  Also reorganised the way that default
        settings are stored.
      * Made Tooltype saving better, and also made it so you can clear a
        field with the PUT_ENTRY_DETAILS command.  Updated documentation.

v1.1e 12-3-96, 13-3-96, 16-3-96, 23-3-96

      * Added HIDE and SHOW ARexx commands.
      * Added commodity support.
      * Added Amiga-F shortcut for Search function.  Wow.
      * Improved commodity support.  Fixed potentioal bug in
        close_commodity, and added code to properly detect for duplicate
        copies of AB being run together.

v1.2 26-3-96, 28-3-96, 29-3-96

      * New release on web page.
      * Problems in documentation.  Fixed them.
      * Changed the main window updating code so that the Dial button is
        only active when the current entry contains a phone number.  Also
        added in new command, DIAL_NUMBER which dials the number specified
        in its argument.  Added command description to documentation.  Now
        detects an illegal hotkey, but does nothing about it.

1.2a 14-5-96, 13-8-96, 10-9-96, 13-9-96, 17-9-96, 22-11-96

      * Chris Davis requested two things: AB should run on OS2.0, and AB
        should have a tooltype to set the window width.  This version has
        these two things added.  Compatability with OS2.0 is untested
        however.  He requested other things, but they will have to wait
        till later.
      * v2.0 compatibility still not tested.  Removed need for
        NO_OF_AREXX_COMMANDS.
      * The window title now shows the number of the currently viewed
        entry and the number of entries in the project.  The four
        movement buttons have been replaced with a single scroll bar.
      * Added keyboard shortcuts for gadgets.
      * Internal changes and made it so entries are sorted by surname.
        Fixed two bugs which have been in the program since the start.
        Both bugs were to do with access to memory not owned by the
        program.
      * BenB requested that cursor keys be used to move through records
        instead of . and ,.  Did this

1.2b 6-4-97

      * Chris Davis reported that AB didn't run under 2.0, so I have
        given up in getting it to run on the older OS for now.  Also he
        asked me to add in the OLD_SORTING tooltype which I have done.
</pre>
</div>
<div id="author">
<hr/>
<p><b>How to contact me</b></p>
<p><a>Contents</a> <a>Index</a> <a>Help</a> <a>Prev</a> <a>Next</a> </p>
<pre>


<b>Author Information and Contact Details</b>

Send bug-reports, and any other stuff to:

Snail mail:
    Lawrence Manning
    9 Abbey Hill
    Netley Abbey
    Southampton
    Hants.
    SO31 5FB
    ENGLAND

Internet E-mail:
    lawrence@aslak.demon.co.uk
    manningl@ee.port.ac.uk valid until July 1997

World Wide Web:
    http://www.ee.port.ac.uk/~students/Aslak/index.html

IRC:
    You can sometimes find me on #10forward, on the DALNet server network.
The nickname I use is Aslak.

A plea for money, dosh, PD programs etc:
    Because AB is FREEWARE, I expect no payment for use of the program.
However any payment (money, PD programs, etc.) would be very gratefully
received!


<b>Thanks too...</b>

First of all I would like to thank Eddy Carroll for releasing the source
code for his amazing SnoopDOS program into the public domain.  This helped
greatly with the writing of AB.  I would also like to thank the following
people for helping me bug-test AB, and also for suggesting improvements to
make:

    Philip Reyntens
    Grant Fribbens
    Imran Chaudhry
    Chris Davis

..and any other people I've missed out!
</pre>
</div>
<hr/>
