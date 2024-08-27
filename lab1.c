/* lab1.c
 * Aarav Rekhi
 * aaravr
 * ECE 2220, Fall 2023
 * PP1
 *
 * NOTE:  You must update all of the following comments!
 * 
 * Purpose:  Review of C Programming
 *
 * Assumptions: The program is meant to track the number of times a value greater than or 
 * equal to the minimum threat value appears and inform the user. It is also supposed to
 * make any threat value greater than the minimum threat value into the new minimum threat
 * value. 
 *
 * Bugs:  Program does not have an accurate number of threats detected. Instead of making 
 * the highest value into the minimum threat value and detecting how many times it appears, 
 * it designates all values above the original minimum threat as threats.
 *
 * See the Programming Guide for requirements
 *
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux to see different options.  Or, set up your
 * own style.
 *
 * To create a nicely formated PDF file for printing install the enscript 
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 100
#define MAXSAMPLES 500
#define MINTHRESH 3

int main() 
{
    char line[MAXLINE];
    int min_threat_level;
    int false_alarm_count;
    int samples[MAXSAMPLES];
    int sampleCount = 0;

    // Prompt for min threat level
    while (1) 
    {
        printf("What is the minimum threat level (>= %d, -1 to exit)? ", MINTHRESH);
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &min_threat_level);

        if (min_threat_level == -1) 
        {
            printf("Exiting program.\n");
            return 0;
        }

        if (min_threat_level <= MINTHRESH) 
        {
            printf("\nThe input is invalid, you entered: %d\n", min_threat_level);
            // Ask for a valid value
            continue;
        } 
        
        else 
        {
            break;
        }
    }

    // Prompt for false alarm count
    while (1) 
    {
        printf("How many times must a threat level occur so not a false alarm? ");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &false_alarm_count);

        if (false_alarm_count == -1) 
        {
            printf("Exiting program.\n");
            return 0;
        }

        if (false_alarm_count <= 0) 
        {
            printf("A false alarm if the count is < %d\n", false_alarm_count);
            // Ask for a valid value
            continue;
        } 
        
        else 
        {
            break;
        }
    }

    // Sample collection and evaluation loop
    while (1) 
    {
        int threatScore;

        printf("Enter threat level score: ");
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &threatScore);

        if (threatScore == -1) 
        {
            // Evaluate the sample set for threat
            int threatCount = 0;
            for (int i = 0; i < sampleCount; i++) 
            {
                if (samples[i] = min_threat_level) 
                {
                    threatCount++;
                }
            }

            // Print threat detection result with the actual detected values
            if (threatCount = false_alarm_count) 
            {
                printf("Threat detected with level %d and appears %d times\n", min_threat_level, threatCount);
            } 
            
            else 
            {
                printf("No threat detected\n");
            }

            sampleCount = 0; // Reset sample count for the next set
        } 
        
        else if (threatScore < 0) 
        {
            printf("Goodbye\n");
            return 0;
        } 
        
        else if (sampleCount < MAXSAMPLES) 
        {
            // Add the threat level score to the sample set
            samples[sampleCount++] = threatScore;
        }
    }

    return 0;
}
