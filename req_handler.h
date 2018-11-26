#ifndef  __REQ_HANDLER__
#define  __REQ_HANDLER__

#include "global.h"
struct terminal_info_struct
{
  int id;
  char card_type[CARD_NAME_LENGTH];
  char transaction_type[TRANS_NAME_LENGTH];
  int flag; /* 0 = spare, 1 = used */
};

/* In-memory array, maximum number of DB slots = maximum number of terminal slots */
/* Maximum number of terminal slots depend on system capacity */
struct terminal_info_struct gl_terminal_info[MAXTERMID];

extern void init_db();
extern int svr_process_req (char * input_data, char * resp_data);
extern int query_term_info(int id, char *term_info);
extern int parse_json(char * p_msg, struct terminal_info_struct *term_info_ptr);
extern int find_spare_id(int *id_ptr);
extern int insert_db(struct terminal_info_struct *terminal_info_ptr);
extern int query_db(int id, struct terminal_info_struct * tm_db_ptr);
extern int struct2json(struct terminal_info_struct * tm_db_ptr, char tm_json[]);


#endif
