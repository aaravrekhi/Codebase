/* lab4.c template
 * Aarav Rekhi  << replace with your name!
 * aaravr        << and user name
 * ECE 2220, Summer 2023
 * MP4
 * Subject: ECE2220-1, #4
 *
 * Purpose: A simple packet sniffer that can handle packets with non-printable
 *          characters. The sniffer can find any byte, string, or web address 
 *          in any packet.
 *
 * Assumptions: 
 *     Input file is read in as bytes.  The input file must have the correct
 *     format.  Minimal error checking is included to validate the input data.
 *
 *
 * Command line argument
 *     Name of file to read.  The file must contain one packet per line with
 *     the format
 *        size packet_contents
 *     The size is the number of bytes in the packet, and the packet_contents
 *     is the binary data, read as bytes.  The size of the packet must be
 *     correct, and there must be exactly one space character between the size
 *     field and the start of the packet_contents.  Note the bytes in a packet
 *     can have any value, so the packet may not be printable.  
 *
 * Bugs:
 *
 * Notes:
 *     The number of packets that can be stored is limited to MAXPACKETS - 1
 *     because one position is needed to mark where the last packet ends.
 *
 * See the ECE 2220 programming guide
 *
 * Format with
 *     astyle --style=kr lab4.c
 *
 * Replace "kr" with: bsd, ansi, java, gnu, linux, vtk, or google. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 128

#define MAXPACKETLENGTH 128
#define MAXPACKETS      8192
#define MAXFILESIZE     1048576
#define FALSE           0
#define TRUE            1

/*----------------------------------------------------------*/
/* A stub function for printing packets as 16-byte aligned lines of text.
 *
 * input:
 *     position: the location of the point of interest. For example, the found
 *               byte or the first character in a found string. If position is
 *               -1, then there is no point of interest.              
 *     slen:  the length of the found string (or 1 for a found byte)
 *     packets: an array of pointers to each packet's starting address in memory
 *     packetnum: the index of the packet of interest    
 *
 * output:
 *     prints a packet as formatted 16-byte lines of text.
 *     This function continues to print 16-byte lines so that the entire packet
 *     is displayed.  A non-printable character is replaced with a space (' ').
 *     In addition, the point of interest (e.g. the found byte or found string)
 *     is marked, as discussed in the pdf.
 */

void print_packet(int position, int slen, char *packets[MAXPACKETS], int packetnum)
{
    int pktLen = (packets[packetnum+1] - packets[packetnum]) / sizeof(char); // find length of packet
    char *pktPtr = packets[packetnum];

    for (int pceIdx = 0; pceIdx < (pktLen % 16 == 0 ? pktLen / 16 : (pktLen + (16 - pktLen % 16)) / 16); ++pceIdx) { // loop and print each individual piece
        printf("          ");

        for (int i = 0; i < 16; ++i) { // first line
            printf("%d ", i % 10);

            if (i < 15) {
                printf(" "); // gap in printing
            }
        }

        printf("\n[%4d-%d] ", packetnum, pceIdx); // second line

        for (int strIdx = pceIdx * 16; strIdx < (pceIdx + 1) * 16; ++strIdx) { // to verify that packet has it
            if (strIdx < (pceIdx + 1) * 16) {// gap in printing
                printf(" ");
            }

            if (strIdx < pktLen) { // verify that it can be printed
                if (pktPtr[strIdx] >= 33 && pktPtr[strIdx] <= 127) {
                    printf("%c ", pktPtr[strIdx]);
                }
    
                else {
                    printf("  ");
                }
            } 
            
            else {
                printf("  ");
            }
        }

        printf("\n         "); // third line

        for (int strIdx = pceIdx * 16; strIdx < (pceIdx + 1) * 16; ++strIdx) {
            if (strIdx < (pceIdx + 1) * 16) {
                printf(" ");            
            }
    
            if (strIdx < pktLen) {
                if (pktPtr[strIdx] >= 33 && pktPtr[strIdx] <= 127) { // check if it is an ASCII character
                    printf("%02X", pktPtr[strIdx]);
                } 
                
                else {
                    printf("%02X", (char) pktPtr[strIdx] & 0xFF); // prints nothing
                }
            } 
            
            else {
                printf("XX");
            }
        }

        printf("\n         "); // fourth line

        for (int strIdx = pceIdx * 16; strIdx < (pceIdx + 1) * 16; ++strIdx) { // gap in printing
            if (strIdx > position && strIdx <= (position + slen - 1)) {
                printf("-");
            }
            
            else if (strIdx < pktLen) {
                printf(" "); // prints mark character
            }
                
            if (strIdx == position) {
                printf("^ ");
            } 
            
            else if (strIdx > position && strIdx < (position + slen - 1)) {
                printf("--");
            } 
            
            else if (strIdx == (position + slen - 1)) {
                printf("-|");
            } 
            
            else if (strIdx < pktLen) {
                printf("  ");
            }
        }

        printf("\n\n");
    }
}

/*----------------------------------------------------------*/
/* This function is a stub for searching a particular packet for a byte.
 *
 * Use packets[packetnum] as the starting address for the particular packet.
 * Search within this packet for the first matching byte. Use the starting 
 * address of the next packet (packets[packetnum+1]) to know when to end.
 *
 * input:
 *     packets: an array of pointers to each packet's starting address in memory
 *     packetnum: is the index of the packet of interest
 *     byte: is a string with the hex characters of the byte to find
 *
 * return value:
 *     1 if byte found within the packet
 *     0 if byte not found
 * side effects:
 *     If the byte is found, your function should call print_packet() and 
 *     display the matched byte as discussed in the pdf
 */

int find_byte_packet(char *packets[MAXPACKETS], int packetNum, char *byte)
{
    int found = FALSE;
    int pktLen = (packets[packetNum+1] - packets[packetNum]) / sizeof(char); // find length of packet
    char *pktPtr = packets[packetNum];
    unsigned char hexVal = 0;

    for (int i = 1; i >= 0; i--) {
        if (byte[1-i] >= 65 && byte[1-i] <= 70) { // A --> F
            hexVal |= (byte[1-i]-55) << (i * 4);
        }

        else if (byte[1-i] >= 97 && byte[1-i] <= 102) { // a --> f
            hexVal |= (byte[1-i]-87) << (i * 4);
        }

        else {
            hexVal |= (byte[1-i]-48) << (i * 4);
        }
    }

    for (int j = 0; j < pktLen; ++j) {
        if (hexVal == (pktPtr[j] & 0xFF)) { // compare bytes without the sign bits
            print_packet(j, 1, packets, packetNum);
            found = TRUE;
            break;
        }
    }

    return found;
}

/*----------------------------------------------------------*/
/* This function is a stub for searching a particular packet for a string.
 *
 * Use packets[packetnum] as the starting address for the particular packet.
 * Search within this packet for the first matching byte. Use the starting 
 * address of the next packet (packets[packetnum+1]) to know when to end.
 *
 * input:
 *     packets: an array of pointers to each packet's starting address in memory
 *     packetnum: is the index of the packet of interest
 *     str: is the string to find
 *
 * return value:
 *     1 if string found within the packet
 *     0 if string not found
 * side effects:
 *     If the string is found, your function should call print_packet() 
 *     and display the matched string as discussed in the pdf
 */

int find_string_packet(char *packets[MAXPACKETS], int packetNum, const char *str)
{
    int pktLen = (packets[packetNum+1] - packets[packetNum]) / sizeof(char); // find length of packet
    int found = 0, strIndex, cmpCnt = 0;
    char *strPtr, *pktPtr = packets[packetNum];

    for (strIndex = 0; strIndex < pktLen; ++strIndex) {
        strPtr = (char*) str;

        for (int i = 0; i < strlen(str); ++i) { // compare string in array to string provided by header
            if (*strPtr == pktPtr[strIndex+i]) {
                cmpCnt++;
                strPtr++;
            } 
            
            else {
                cmpCnt = 0; // reset count
                break;
            }
        }

        if (cmpCnt == strlen(str)) { // set found to TRUE
            found = TRUE;
            break;
        }
    }

    if (found == TRUE) { // prints
        print_packet(strIndex, strlen(str), packets, packetNum);
    }

    return found;
}

/*----------------------------------------------------------*/
/* This function is a stub for searching all packets for a string.
 *
 * You may find that the implementation of this function is straightforward
 * provided that you have already implemented the find_string_packet() function.
 *
 * input:
 *     packets: an array of pointers to each packet's starting address in memory
 *     numpackets: the total number of packets
 *     str: the string to find
 *
 * return value:
 *     the number of packets found to contain the string. Note that a return 
 *     value of 0 indicates the string was never found.
 *
 * side effects:
 *     This function should also formatted print each packet that contains the string.
 */

int find_string_all(char *packets[MAXPACKETS], int numPackets, const char *str)
{
    int found = 0;

    for (int i = 0; i < numPackets; i++) {
        found += find_string_packet(packets, i, str); // find number of packets
    }

    return found;
}

/*----------------------------------------------------------*/
/* This function is a stub for searching all packets for a byte.
 *
 * You may find that the implementation of this function is straightforward
 * provided that you have already implemented the find_byte_packet() function.
 *
 * input:
 *     packets: an array of pointers to each packet's starting address in memory
 *     numpackets: the total number of packets
 *     byte: a string with the hex characters of the byte to find
 *
 * return value:
 *     the number of packets found to contain the byte. Note that a return 
 *     value of 0 indicates the byte was never found.
 *
 * side effects:
 *     This function should also formatted print each packet that contains the byte.
 */

int find_byte_all(char *packets[MAXPACKETS], int numPackets, char *byte)
{
    int found = 0;

    for (int i = 0; i < numPackets; i++) {
        found += find_byte_packet(packets, i, byte); // find number of packets
    }
    return found;
}

/*----------------------------------------------------------*/
/* This function is a stub for searching all packets for website domain names.
 *
 * Although not strictly required, you may find that the implementation of this 
 * function is straightforward if you first implement the find_website_packet() 
 * function, similar to find_string_packet() and find_byte_packet().
 *
 * A string is a valid website only if it has the following form:
 *
 *      www.*.%%%
 *
 * That is, a website always
 *      1) begins with the literal string "www.",
 *      2) "*" means one or more alphabetic characters, and
 *      3) ends with ".%%%" where each % is exactly one alphabetic character.
 *
 * A string that violates this pattern for any reason is not a valid website.
 *
 * input:
 *     packets: an array of pointers to each packet's starting address in memory
 *     numpackets: the total number of packets
 *
 * return value:
 *     the number of packets found to contain a website.  Note that a return 
 *     value of 0 indicates that no websites were found.
 *
 * side effects:
 *     This function should NOT print the packets that contain a website.
 *     Instead, this function prints the name of each website that is found, 
 *     including duplicates.  (e.g. www.google.com)
 */

int find_websites(char *packets[MAXPACKETS], int numPackets)
{
    int found = FALSE;
    int pktLen = 0;
    char *pktPtr;
    char *sitePtr;
    char *headPtr;
    char *buttPtr;

    for (int i = 0; i < numPackets; i++) { // check in every packet
        pktPtr = packets[i];
        pktLen = (packets[i + 1] - packets[i]) / sizeof(char); // find length of packet

        for (int strIndex = 0; strIndex < pktLen; strIndex++) {
            int result = TRUE;

            for (int j = 0; j < 4; j++) {
                if (pktPtr[strIndex + j] != "www."[j]) {
                    result = FALSE;
                    break;
                }
            }

            if (result && (strIndex + 3 < pktLen)) {
                sitePtr = &pktPtr[strIndex + 4]; // if website has www., set sitePTR to website name
                headPtr = &pktPtr[strIndex]; // place website address to headPtr
                
                if ((isupper((int)*sitePtr) || islower((int)*sitePtr)) && (strIndex + 3 < pktLen)) {
                    while ((isupper((int)*sitePtr) || islower((int)*sitePtr)) && sitePtr < packets[i + 1]) {
                        sitePtr++;
                    }
                    
                    if (*sitePtr == 46 && (sitePtr + 3) < packets[i + 1]) {
                        if ((isupper((int)sitePtr[1]) || islower((int)sitePtr[1])) && (isupper((int)sitePtr[2]) || islower((int)sitePtr[2])) && (isupper((int)sitePtr[3]) || islower((int)sitePtr[3]))) {
                            buttPtr = sitePtr;
                            sitePtr = headPtr;
                            sitePtr = &sitePtr[4];
                            print_packet(sitePtr - pktPtr, buttPtr - sitePtr, packets, i);
                            found = TRUE;
                            strIndex = sitePtr - pktPtr;
                        }
                    }
                }
            }
        }
    }

    return found;
}

/*----------------------------------------------------------*/
/* Stores a packet within the dynamically-allocated memory space.
 *
 * Note that the malloc() is done in main, and the file processing is done 
 * within process_input().  The function process_input() will call this 
 * function to store an individual packet within the space and to update the 
 * packets[] accordingly.
 *
 * input:
 *     packetspace: the dynamically-allocated memory to space to store packets
 *     packets: an array of pointers to each packet's starting address in memory
 *     packet: a string that contains the characters of the packet. Although 
 *             packet is appended with a '\0', you should not use string 
 *             functions in this function because packet may also contain a '\0'.
 *     packetlength: the number of characters in the packet
 *     packetnum: this packet's number - its starting address should be saved
 *                in packets[packetnum].
 *
 * return value: none
 *
 * other considerations:
 *     The first packet that is stored will start at the memory address denoted
 *     by packetspace. Each additional packet will be stored right after where 
 *     the last one left off. As such, after a packet is stored and a pointer 
 *     to its location within packetspace is added at packets[packetnum], the 
 *     pointer in packets[packetnum+1] should also be updated
 *     to contain the starting address for the next incoming packet.
 */

void store_packet(char * packetspace, char * packets[MAXPACKETS], char * packet, int packetlength, int packetnum) 
{
    char *packet_ptr;

    if(packetnum==0) { // check whether this is the first packet
        packets[0] = packetspace; // starts off at the first address in packet
    }

    packet_ptr = packets[packetnum]; // designate packet of interest

    for (int i = 0; i < packetlength; ++i) { // copy string into array
        *packet_ptr = packet[i];
        packet_ptr++;
    }

    packets[packetnum+1] = packet_ptr; // find where the next packet begins
}

// 
// Functions from here to end of this file should NOT be changed
//

/* print the menu of choices to the user 
 *
 */

void print_menu(void)
{
    printf("Simple sniffer commands\n\n");
    printf("f FF num : Search for first instance of byte (in hex) in packet number num\n");
    printf("g FF     : Search for first instance of byte in each packet\n");
    printf("s string num  : Search for first instance of string in packet number num\n");
    printf("t string      : Search for first instance of string in each packet\n");
    printf("w     : Search all packets for website domain names\n");
    printf("p num : Print packet number num (formatted print)\n");
    printf("q     : Quit\n");
    printf("?     : Print this menu\n");
}

/*----------------------------------------------------------*/
/*
 *  Read in the input file, extract a packet, and send it to store_packet()
 */

int process_input(char * filename, char * packetspace, char * packets[MAXPACKETS])
{
    FILE * packetsfile;    
    int i;
    int j;
    char c;
    int packetlength = -1;;
    // Note: packet is a temporary buffer used to hold contents while
    //       collecting from the input file.  The packet contents are
    //       copied from this temporary buffer into the final
    //       packetspace memory location using the store_packet() function.
    // one extra byte for null 
    char *packet = (char *) calloc(MAXPACKETLENGTH+1, sizeof(char));

    // begin reading in packets from file
    packetsfile = fopen(filename, "r");
    
    if(packetsfile == NULL) {
        printf("Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (fscanf(packetsfile, "%d", &packetlength) != EOF) {
        // note need to leave one space for end of last packet location
        if (i >= MAXPACKETS - 1) {
            printf("invalid input file: too many packets %d\n", i);
            exit(EXIT_FAILURE);
        }

        if (packetlength < 1 || packetlength >= MAXPACKETLENGTH) {
            printf("invalid input file.  packet %d has length %d\n",
                    i, packetlength);
            exit(EXIT_FAILURE);
        }
        // consume initial ' ' in file
        fscanf(packetsfile, "%c", &c);
        if (c != ' ') {
            printf("invalid input file: packet %d missing space '%c'\n", i, c);
            exit(EXIT_FAILURE);
        }
        // read packet
        for (j = 0; j < packetlength; j++) {
            fscanf(packetsfile, "%c", &packet[j]);
        }
        // don't forget the null character for a string (but not packet)
        packet[j] = '\0';
        // consume trailing '\n' in file
        fscanf(packetsfile, "%c", &c);
        if (c != '\n') {
            printf("invalid input file: packet %d missing end of line '%c'\n", i, c);
            exit(EXIT_FAILURE);
        }

        // send packet to be stored
        store_packet(packetspace, packets, packet, packetlength, i);
        i++;

        // null packet for next round - for safety
        memset(packet, 0, MAXPACKETLENGTH);
        packetlength = -1;
    }
    if (i == 0) {
        printf("invalid input file: no packets found %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fclose(packetsfile);
    free(packet);
  
    return i;       // the number of packets 
}

/*----------------------------------------------------------*/

int main(int argc, char *argv[])
{
    char *filename;        // the input file name      
    char *packetspace;     // starting address of memory block to store file    
    int fn_len;            // length of the input file name          
    int found = 0;         // if search was successful       
    int items;

    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    int  packetnum;

    char *packets[MAXPACKETS];
    int numpackets;

    if (argc != 2) {
        printf("Usage: lab4 filename\n");
        exit(1);
    }

    // prepare filename
    fn_len = strlen(argv[1]);
    // remember the null
    filename = (char *) malloc((fn_len + 1) * sizeof(char));
    strcpy(filename, argv[1]);

    // create large space for packing packets
    packetspace = (char *) malloc(MAXFILESIZE * sizeof(char));
    
    // process the input file and store the packets in packetspace   
    numpackets = process_input(filename, packetspace, packets);

    printf("Processed %d packets.\n\n", numpackets);

    // menu and selection
    print_menu();

    printf("> ");
    while (fgets(line, MAXLINE, stdin) != NULL) {
        printf("\n");
        items = sscanf(line, "%s%s%d", command, inputcs, &packetnum);
        if (items == 3 && strcmp(command, "f") == 0) { 
            if (strlen(inputcs) != 2 || !isxdigit(inputcs[0]) || !isxdigit(inputcs[1])) {
                printf("f Invalid byte: %s\n", inputcs);
            } else if (packetnum < 0 || packetnum >= numpackets) {
                printf("f Invalid packet: %d\n", packetnum);
            } else {
                found = find_byte_packet(packets, packetnum, inputcs);
                if (!found) {
                    printf("Did not find byte: %s in packet %d\n", inputcs, packetnum);
                } else {
                    printf("Found byte: %s in packet %d\n", inputcs, packetnum);
                }
            }
        } else if (items == 2 && strcmp(command, "g") == 0) {  
            if (strlen(inputcs) != 2 || !isxdigit(inputcs[0]) || !isxdigit(inputcs[1])) {
                printf("g Invalid byte: %s\n", inputcs);
            } else {
                found = find_byte_all(packets, numpackets, inputcs);
                if (!found) {
                    printf("Did not find byte: %s in any packet\n", inputcs);
                } else {
                    printf("Found byte: %s in %d unique packets\n", inputcs, found);
                }
            }
        } else if (items == 3 && strcmp(command, "s") == 0) {  
            if (packetnum < 0 || packetnum >= numpackets) {
                printf("s Invalid packet number: %d\n", packetnum);
            }
            found = find_string_packet(packets, packetnum, inputcs);
            if (!found) {
                printf("String not found: '%s' in packet %d\n", inputcs, packetnum);    
            } else {
                printf("Found string: '%s' in packet %d\n", inputcs, packetnum);
            }
        } else if (items == 2 && strcmp(command, "t") == 0) {  
            found = find_string_all(packets, numpackets, inputcs);
            if (!found) {
                printf("String not found: '%s' in any packet\n", inputcs);    
            } else {
                printf("Found string: '%s' in %d unique packets\n", inputcs, found);
            }
        } else if (items == 1 && strcmp(command, "w") == 0) {  
            found = find_websites(packets, numpackets);
            if (!found) {
                printf("No websites found in any packet\n");    
            } else {
                printf("Found websites in %d unique packets\n", found);
            }
        }  else if (items == 2 && strcmp(command, "p") == 0) { 
            packetnum = -1;
            packetnum = atoi(inputcs);
            if (packetnum < 0 || packetnum >= numpackets) {
                printf("p Invalid packet number: %d\n", packetnum);
            } else {
                print_packet(-1, 1, packets, packetnum);
            }
        } else if (items == 1 && strcmp(command, "q") == 0) { 
            break;
        } else if (items == 1 && strcmp(command, "?") == 0) { 
            print_menu();
        } else {
            printf("# :%s", line);
        }
        printf("> ");
    }

    // for every malloc there must be a free
    free(packetspace);
    free(filename);

    printf("Goodbye\n");
    return EXIT_SUCCESS;
}