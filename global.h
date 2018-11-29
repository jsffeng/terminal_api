#ifndef __GLOBAL_DEFINE__ 
#define __GLOBAL_DEFINE__

#define PORT 8888

/* Following referenced from Manual of libmicrohttpd 
maximum number of bytes to use for internal buffering 
(used only for the parsing, specifically the parsing of 
the keys). A tiny value (256-1024) should be sufficient; 
do NOT use a value smaller than 256; for good performance, 
use 32k or 64k (i.e. 65536). */

#define POSTBUFFERSIZE 512 

/* Output JSON messages should be less than 90 bytes if no blank */
#define MAXJSON_INFOSIZE 128

/* Valid term id 1-100, 0 not used */
#define MAXTERMID 100

/* The string length to contain 1 terminal id, including '\0' */
/* Need to re-set this value if  MAXTERMID changed */
#define ID_CHAR_LENGTH 4

/* Maximum size to store {1 2 3 ...100} is 9x2+90x3+1x4+1 */
/* Need to recalculate if MAXTERMID changed */
#define TERMLIST_LENGTH 293

/* TERMID stored in string with format {}, so 7 characters for ID */
/* If MAXTERMID increased, need to review this length */
#define TERMID_LENGTH 10
#define REST_API_TERM "/terminals"
#define REST_API_TERM_LENGTH  10

#define CARD_NAME_LENGTH 20

/* To make no holes in structure terminal_info_struct , otherwise, 
 may cause random value in the holes, not good for binary comparision 
in UT test*/
#define TRANS_NAME_LENGTH 12

#define GET 0
#define POST 1

/* Need to change it to official path for real use */
#define LOGFILE "./ERR.log"

FILE *errlog;

#define ERRLOG(msg) do { \
  errlog = fopen (LOGFILE,"a+");\
  if (errlog == NULL) return 1;\
  fprintf(errlog, "ERROR: %s line %d, function %s(), %s\n", __FILE__, __LINE__,__FUNCTION__, msg );\
  fclose(errlog);\
  } while(0)

#endif
