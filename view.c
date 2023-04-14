/* view.c -- view module for clock project (the V in MVC)
 *
 * Liam Tyler, Cory Lillis, Andy Richmond, 13 April 2023
 *
 * Copyright (C) Liam Tyler, Cory Lillis, Andy Richmond, 2009-2019, All Rights Reserved
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
    char *timeformat = "(no format)"; // see strftime(3)

    /* If not in LED mode, return the string as it should be printed.
     *   '10/31/2023 dt' (date of Hallowe'en) or '12:34:56 am'.
     *
     * If in LED mode, don't add slashes or colons; the LEDs can't
     *    do slashes, and the colons are controlled separately.
     *    Return something like '103123d' (date of Halloween) or
     *    '123456a' (12:34:56 am).
     */
    if ( view_props & DATE_MODE ) {
        if(view_props & LED_MODE){
                timeformat = "%_m%d%y dt";
            }else{
                timeformat = "%_m/%d/%Y dt";
            }
    } else {    
        if ( view_props & AMPM_MODE ) {
            if(view_props & LED_MODE){
                timeformat = "%l%M%S %P";
            }else{
                timeformat = "%l:%M:%S %P";
            }
        } else {
            if ( view_props & LED_MODE ) {
                timeformat = "%H%M%S 24";
            }else {
                    timeformat = "%H:%M:%S 24";
            }
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
	//timeformat = "%_m%d%y dt";
        return;
    }
    
    for (i = 0; i < 6; i++) {
        switch ( make_timestring(dateinfo)[i] ) {
            case ' ': bitvalues = 0x00; break;
            case '1': bitvalues = 0x03; break;
            case '2': bitvalues = 0x76; break; // 12456 = 0111 0110
            case '3': bitvalues = 0x57; break; // 21406 = 0101 0111
            case '4': bitvalues = 0x1b; break; // 1340  = 0001 1011
            case '5': bitvalues = 0x5d; break;
            case '6': bitvalues = 0x7d; break;
            case '7': bitvalues = 0x07; break;
            case '8': bitvalues = 0x7f; break;
            case '9': bitvalues = 0x1f; break;
            case '0': bitvalues = 0x6f; break;
        }
        where[i] = bitvalues;
    }
    where[7] = 0x02;
    if ( dateinfo->tm_sec % 2 == 0 ) {
        where[7] |= 0xf0;
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
