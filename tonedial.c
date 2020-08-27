/* Address Book
 * by Lawrence Manning 
 *
 * filename: tonedial.c
 */

#include "ab.h"
#include "externals.h"

/* This function prepers the data structures needed by the audio.device. 
 * It is called by mymain() during startup. */
void open_audio_data(void)
{
    LONG counter;

    /* First setup clock_rate.  This depends of whether this computer is PAL
     * or NTSC. */
    if (GfxBase->DisplayFlags & PAL)
        clock_rate = CLOCK_RATE_PAL;
    else
        clock_rate = CLOCK_RATE_NTSC;

    /* Allocate memory for sine wave data.  Must be in chip ram. */
    if (!(sine_wave_data = AllocMem(SINE_WAVE_SAMPLES, MEMF_CHIP)))
        clean_up(RETURN_FAIL, allocmem_failure_string);

    /* Now fill in the sample data to contain a sine wave. */
    sine_wave_data[0] = 0; sine_wave_data[1] = 43;
    sine_wave_data[2] = 82; sine_wave_data[3] = 110;
    sine_wave_data[4] = 125; sine_wave_data[5] = 125;
    sine_wave_data[6] = 110; sine_wave_data[7] = 82;
    sine_wave_data[8] = 43; sine_wave_data[9] = 0;
    sine_wave_data[10] = -43; sine_wave_data[11] = -82;
    sine_wave_data[12] = -110; sine_wave_data[13] = -125;
    sine_wave_data[14] = -125; sine_wave_data[15] = -110;
    sine_wave_data[16] = -82; sine_wave_data[17] = -43;

    /* Now we need two message ports and two audio io structures. */
    for (counter = 0; counter < 2; counter++)
    {
        if (!(audio_port[counter] = CreatePort(NULL, 0)))
            clean_up(RETURN_FAIL, "Unable to create audio ports.");

        if (!(audio_io[counter] = (struct IOAudio *) AllocMem(sizeof(struct IOAudio),
            MEMF_CLEAR)))
        {
            clean_up(RETURN_FAIL, "Unable to create audio io block.");
        }
    }
}

/* The function deallocates the memory used by the previous function.  It
 * is called by clean_up(). */
void close_audio_data(void)
{
    LONG counter;

    /* Free sine_wave_data from memory. */
    if (sine_wave_data)
    {
        FreeMem(sine_wave_data, SINE_WAVE_SAMPLES);
        sine_wave_data = NULL;
    }

    /* Free the audio port and audio io block from memory. */
    for (counter = 0; counter < 2; counter++)
    {
        if (audio_port[counter])
        {
            DeletePort(audio_port[counter]);
            audio_port[counter] = NULL;
        }

        if (audio_io[counter])
        {
            FreeMem(audio_io[counter], sizeof(struct IOAudio));
            audio_io[counter] = NULL;
        }
    }
}    

/* This function opens the audio.device.  It allocates two channels.
 * Returns TRUE for success, or FALSE for failure. */
BOOL open_audio_device(void)
{
    LONG counter;
    BOOL return_value = FALSE;

    for (counter = 0; counter < 2; counter++)
    {
        /* Set up the audio block to allocate the required channels. */
        audio_io[counter]->ioa_Request.io_Message.mn_ReplyPort = audio_port[counter];
        audio_io[counter]->ioa_Request.io_Message.mn_Node.ln_Pri = 0;
        audio_io[counter]->ioa_Request.io_Command = ADCMD_ALLOCATE;
        audio_io[counter]->ioa_Request.io_Flags = ADIOF_NOWAIT;
        audio_io[counter]->ioa_AllocKey = 0;
        audio_io[counter]->ioa_Data = &allocate_which_channels[counter][0];
        audio_io[counter]->ioa_Length = 1;

        /* Open the audio device! */
        if (!(audio_device_open[counter] = (BOOL) !OpenDevice("audio.device", 0,
            (struct IORequest *) audio_io[counter], 0)))
        {
            close_audio_device();
        }
    }

    if (audio_device_open[0] && audio_device_open[1])
        return_value = TRUE;

    return (return_value);
}

/* This function closes the audio device. */
void close_audio_device(void)
{
    LONG counter;

    for (counter = 0; counter < 2; counter++)
    {
        if (audio_device_open[counter])
        {
            CloseDevice((struct IORequest *) audio_io[counter]);
            audio_device_open[counter] = FALSE;
        }
    }
}

/* Now we have the function that does the work.  It plays the tones required
 * to dial a number through the left speaker.  Returns TRUE for success,
 * FALSE for failure. */
BOOL tone_dial_number(UBYTE *phone_number_string)
{
    LONG phone_number_counter; /* Incremented as each tone is played. */
    LONG phone_number_length; /* Number of charecters in phone number. */
    LONG phone_number_digit; /* The UBYTE in the phone number as a LONG. */
    LONG tone_frequencies[10][2] =
    {
        {941, 1336}, {697, 1209}, {697, 1336}, {697, 1447}, {770, 1209},
        {770, 1336}, {770, 1447}, {852, 1209}, {852, 1336}, {852, 1447}
    };
    LONG counter;
    BOOL channel_finished[2];
    LONG channel_frequency[2];
    ULONG signals_waiting, signals_received;
    struct Message *audio_message;
    BOOL audio_error = FALSE;

    phone_number_length = strlen(phone_number_string);

    /* Go through each charecter in the phone number string... */
    phone_number_counter = 0;
    while (phone_number_counter < phone_number_length && !audio_error)
    {
        /* Make sure that this charecter is numeric. */
        if (isdigit(phone_number_string[phone_number_counter]))
        {
            /* Reset the finished flags, and work out what frequency the note
             * should be played at. */
            channel_finished[0] = FALSE; channel_finished[1] = FALSE;
            phone_number_digit = (LONG)  phone_number_string[phone_number_counter] - '0';

            for (counter = 0; counter < 2; counter++)
            {
                /* See what frequency the tone should be played at. */
                channel_frequency[counter] = tone_frequencies[phone_number_digit][counter];

                /* Build audio io block. */
                audio_io[counter]->ioa_Request.io_Message.mn_ReplyPort =
                    audio_port[counter];
                audio_io[counter]->ioa_Request.io_Command = CMD_WRITE;
                audio_io[counter]->ioa_Request.io_Flags = ADIOF_PERVOL;
                audio_io[counter]->ioa_Data = (BYTE *) sine_wave_data;
                audio_io[counter]->ioa_Length = SINE_WAVE_SAMPLES;
                audio_io[counter]->ioa_Period = clock_rate /
                    (SINE_WAVE_SAMPLES * channel_frequency[counter]);
                audio_io[counter]->ioa_Volume = 48;
                audio_io[counter]->ioa_Cycles =
                    (settings.dial_duration * channel_frequency[counter]) / 50;
            }

            /* Play the two notes at the same time. */
            BeginIO((struct IORequest *) audio_io[0]);
            BeginIO((struct IORequest *) audio_io[1]);

            /* Keep going until BOTH channels have stopped playing. */
            while (!channel_finished[0] || !channel_finished[1])
            {
                /* Construct signal masks and wait for either channel to
                 * finish. */
                signals_waiting = (1L << audio_port[0]->mp_SigBit) |
                    (1L << audio_port[1]->mp_SigBit);
                signals_received = Wait(signals_waiting);

                /* First channel finished? */
                if (signals_received & (1L << audio_port[0]->mp_SigBit))
                {
                    audio_message = GetMsg(audio_port[0]);
                    if (audio_io[0]->ioa_Request.io_Error != 0)
                        audio_error = TRUE;
                    channel_finished[0] = TRUE;
                }

                /* Second channel finished? */
                if (signals_received & (1L << audio_port[1]->mp_SigBit))
                {
                    audio_message = GetMsg(audio_port[1]);
                    if (audio_io[1]->ioa_Request.io_Error != 0)
                        audio_error = TRUE;
                    channel_finished[1] = TRUE;
                }
            }

            /* Pause between each tone, if we are not at the last digit.  */
            if (phone_number_counter < (phone_number_length - 1))
                Delay(settings.dial_delay);

        } /* End if. */
        phone_number_counter++;
    } /* Next character. */

    return(!audio_error);
}
