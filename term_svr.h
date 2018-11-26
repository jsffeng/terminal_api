/* Global variable used to store string sent back to clients */
char gl_term_list[TERMLIST_LENGTH];
char gl_term_info[MAXJSON_INFOSIZE];
char gl_tm_id[TERMID_LENGTH];

struct connection_info_struct
{
  int connectiontype;
  char *post_data;
  struct MHD_PostProcessor *postprocessor;
};

/* const char *error_info = "{\"failure\": \"errcode\"}"; */

extern int svr_process_req (char * input_data, char * resp_data);
extern int query_term_info (int id, char *term_info);
extern int query_term_list (char *term_list);
extern int init_array ();
