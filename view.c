/* view.c -- view module for clock project (the V in MVC)
 *
 * Darren Provine, 17 July 2009
 *
 * Copyright (C) Darren Provine, 2009-2019, All Rights Reserved
 */

#include "clock.h"
#include "view.h"

/* see "view.h" for list of bits that set properties */

int view_props = 0x00; // default is 24-hour mode, plain text

// returns old properties so you can save them if needed
void set_view_properties(int viewbits)
{
    view_props = viewbits;
}

int get_view_properties()
{
    return view_props;
}


void do_test(struct tm *dateinfo)
{
    // turn display bits on and off
    
    display();
    fflush(stdout);
}

#define MAX_TIMESTR 40 // big enough for any valid data
// make_timestring
// returns a string formatted from the "dateinfo" object.
char * make_timestring (struct tm *dateinfo)
{
    // Declaring the pointer like this can lead to crashes!
    // We'll see how to fix it in class.  NEVER DO THIS!
    char *timeformat; // see strftime(3)

    /* If not in LED mode, return the string as it should be printed.
     *   '10/31/2023 dt' (date of Hallowe'en) or '12:34:56 am'.
     *
     * If in LED mode, don't add slashes or colons; the LEDs can't
     *    do slashes, and the colons are controlled separately.
     *    Return something like '103123d' (date of Halloween) or
     *    '123456a' (12:34:56 am).
     */
    if ( view_props & DATE_MODE ) {

    } else {
        if ( view_props & AMPM_MODE ) {
            // do something
        } else {
            timeformat = "%H:%M:%S 24";
        }
    }

    // make the timestring and return it
    static char timestring[MAX_TIMESTR];
    strftime(timestring, MAX_TIMESTR, timeformat, dateinfo);
    return timestring;
}

/* We get a pointer to a "struct tm" object, put it in a string, and
 * then send it to the screen.
 */
void show_led(struct tm *dateinfo)
{

    digit *where = get_display_location();
    int i;
    digit  bitvalues = 0;

    if ( view_props & TEST_MODE ) {
        do_test(dateinfo);
        return;
    }
    
    for (i = 0; i < 6; i++) {
        switch ( make_timestring(dateinfo)[i] ) {
            case ' ': bitvalues = 0x00; break;
            case '1': bitvalues = 0x01; break;
            case '2': bitvalues = 0x02; break;
            case '3': bitvalues = 0x03; break;
            case '4': bitvalues = 0x04; break;
            case '5': bitvalues = 0x05; break;
            case '6': bitvalues = 0x06; break;
            case '7': bitvalues = 0x07; break;
            case '8': bitvalues = 0x08; break;
            case '9': bitvalues = 0x09; break;
            case '0': bitvalues = 0xff; break;
        }
        where[i] = bitvalues;
    }
    display();
    fflush(stdout);
}

void show_text(struct tm *dateinfo)
{
    printf("\r%s ", make_timestring(dateinfo));
    fflush(stdout);
}


void show(struct tm *dateinfo)
{
    if ( view_props & LED_MODE )
        show_led(dateinfo);
    else
        show_text(dateinfo);        
}
