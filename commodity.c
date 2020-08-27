/* Address Book
 * by Lawrence Manning 
 *
 * filename: commodity.c
 */

#include "ab.h"
#include "externals.h"

/* This function attempts to open the programs broker and also sets up a hotkey
 * for it.  Calls clean_up() if anything goes wrong just like the rest.  Hotkey
 * failure, however, is not a terminal error. */
void open_commodity(void)
{
    struct NewBroker new_broker =
    {
        NB_VERSION,                 /* Structure version number. */
        commodity_name,             /* Name in Exchange list. */
        version_string,             /* top line in Exchange description text. */
        commodity_description,      /* Bottom line in description text. */
        NBU_UNIQUE | NBU_NOTIFY,    /* Only one AB, and notify on second run. */
        COF_SHOW_HIDE,              /* Make Show and Hide active in Exchange. */
        settings.cx_priority,       /* Commodity priority. */
        NULL                        /* This is the msg port.  Fill in later. */
    };
    CxObj *new_object = NULL;
    BOOL hotkey_error = FALSE;
    LONG broker_create_error;

    /* First make it so rest of program sees hotkey as being inactive. */
    hotkey_enabled = FALSE;

    /* Make message port. */
    if (!(commodity_port = CreatePort(NULL, 0)))
        clean_up(RETURN_FAIL, "Unable to create commodity message port.");

    /* Make the broker and activate it. */
    new_broker.nb_Port = commodity_port;
    if (!(commodity_broker = CxBroker(&new_broker, &broker_create_error)))
    {
        /* Something went wrong.  Now see what sort of error it was.  If the
         * user tried to run two copies of AB, then we don't generate an
         * error, we just call clean_up.  Otherwise (ie. we run out of memory
         * or something else went wrong) we call clean_up with a useful
         * error message, and a fatal error code. */
        if (broker_create_error == CBERR_DUP)
            clean_up(RETURN_OK, "");
        else
            clean_up(RETURN_FAIL, "Unable to create broker.");
    }
    ActivateCxObj(commodity_broker, 1);

    /* Now we need to create the filter for the commodity's hotkey.  If this
     * creation fails then it dosn't mean the hotkey was invalid, just that
     * we have ran out of memory. */
    if (!(commodity_filter = CxFilter(settings.cx_popkey)))
        hotkey_error = TRUE;
    else
    {
        /* Now we add in the bits and pieces to the filter object.  First up is
         * the signal bit that will occure when the user presses the hotkey. */
        if (!(new_object = CxSignal(FindTask(NULL), SIGBREAKB_CTRL_F)))
            hotkey_error = TRUE;
        else
        {
            AttachCxObj(commodity_filter, new_object);

            /* Went ok, now add in the translation part. */
            if (!(new_object = CxTranslate(NULL)))
                hotkey_error = TRUE;
            else
            {
                AttachCxObj(commodity_filter, new_object);

                /* Now check for a general error. */
                if (CxObjError(commodity_filter))
                    hotkey_error = TRUE;
            }
        }
    }

    /* Act on the error flag.  If there was no errror then link the filter
     * to the broker. */
    if (hotkey_error)
    {
        DeleteCxObjAll(commodity_filter);
        commodity_filter = NULL;
    }
    else
    {
        AttachCxObj(commodity_broker, commodity_filter);
        hotkey_enabled = TRUE;
    }
}

/* This function deletes all the commodity features. */
void close_commodity(void)
{
    CxMsg *commodity_message;

    /* First close down the broker.  This kills the filter object, as well. */
    if (commodity_broker)
    {
        DeleteCxObjAll(commodity_broker);
        commodity_broker = NULL;
        commodity_filter = NULL;
    }

    /* Now the message port needs to go. */
    if (commodity_port)
    {
        /* First remove messags.  This requires that task switching be disabled
         * to prevent additional messages ariving while we are deleting. */
        Forbid();
        while(commodity_message = (CxMsg *) GetMsg(commodity_port))
            ReplyMsg((struct Message *) commodity_message);
        DeleteMsgPort(commodity_port);
        Permit();

        commodity_port = NULL;
    }

    hotkey_enabled = FALSE;
}
