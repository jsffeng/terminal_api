#if defined(_MSC_VER) && _MSC_VER+0 <= 1800
/* Substitution is OK while return value is not used */
#define snprintf _snprintf
#endif

#define PORT 8888

/* Following referenced from Manual of libmicrohttpd 
maximum number of bytes to use for internal buffering 
(used only for the parsing, specifically the parsing of 
the keys). A tiny value (256-1024) should be sufficient; 
do NOT use a value smaller than 256; for good performance, 
use 32k or 64k (i.e. 65536). */

#define POSTBUFFERSIZE 512 

/* Regular json message less than 64 bytes, except when 
returning a list of terminal IDs  */ 
#define MAXJSON_INFOSIZE 128

/* valid term id 1-100, 0 not used */
#define MAXTERMID 100

/* 99%  term id have or less than 2 digits, also 
divided by a blank in the list */ 
#define TERMLIST_LENGTH 3*MAXTERMID 

#define TERMID_LENGTH 10
#define REST_API_TERM "/terminals"
#define REST_API_TERM_LENGTH  10

#define CARD_NAME_LENGTH 20
#define TRANS_NAME_LENGTH 10

#define GET 0
#define POST 1

