struct terminal_info_struct
{
  int id;
  char card_type[CARD_NAME_LENGTH];
  char transaction_type[TRANS_NAME_LENGTH];
  int flag; /* 0 = spare, 1 = used */
};

/* In-memory array */
struct terminal_info_struct gl_terminal_info[MAXTERMID];

extern int parse_json(char * p_msg, struct terminal_info_struct *term_info_ptr);
extern int find_spare_id(int *id_ptr);
extern int insert_array(struct terminal_info_struct *terminal_info_ptr);
extern int init_array();

