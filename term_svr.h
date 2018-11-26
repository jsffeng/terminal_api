#ifndef  __TERM_SRV__
#define  __TERM_SRV__

#include "global.h"

/* Global variable used to store string sent back to clients */
char gl_term_list[TERMLIST_LENGTH];
char gl_term_info[MAXJSON_INFOSIZE];

/* TERMID stored in string with format {}, so 7 characters for ID */
/* If MAXTERMID increased, need to review this length */
char gl_tm_id[TERMID_LENGTH];

struct connection_info_struct
{
  int connectiontype;
  char *post_data;
  struct MHD_PostProcessor *postprocessor;
};

extern void init_db ();
extern int svr_process_req (char * input_data, char * resp_data);
extern int query_term_info (int id, char *term_info);
extern int query_term_list (char *term_list);

#endif
