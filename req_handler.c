/*
#include <sys/types.h>
#include <sys/select.h>
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

#include "defines.h"
#include "req_handler.h"

/* Called by POST, will assign a terminal ID, store an entry
in in-memory data array, then return ID with in json format 
if succeeds. */

int
svr_process_req (char * input_data, char * resp_data)
{
  int id=0; 
  int ret_val;
  char id_char[TERMID_LENGTH];
  struct terminal_info_struct l_terminal_info;

  ret_val=parse_json(input_data, &l_terminal_info);
  
  if (ret_val) 
    return 1;

  /* set id 0 if no spare */
  ret_val=find_spare_id(&id);
  
  if (ret_val) 
    return 1;

  if ( id != 0)
  {
    l_terminal_info.id = id;
    l_terminal_info.flag = 1;
   	
    sprintf(id_char,"{%d}",id);
    insert_array(&l_terminal_info);
  }
  else
  {
    sprintf(id_char,"{}");
  }
 
  snprintf (resp_data, TERMID_LENGTH, "%s", id_char);
  return 0; 
}


int
query_term_info (int id, char *term_info)
{
  struct terminal_info_struct term_db_etry; 
  char term_json_entry[MAXJSON_INFOSIZE];
  

  /*Need query DB, and then call cJSON to convert, here just
  hard-code for prototype */
 
  printf("prototype query_term_info\n");
  
  char json_format[]="{\"terminalID\":%d;\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}"; 
  /* if id no found in DB */ 
  /* char json_format[]="{}"; */
  snprintf (term_info, MAXJSON_INFOSIZE, json_format, id); 
        
  return 0; 
}


int
query_term_list (char *term_list)
{
  char list_t[] = "{4 8 19 20 40}"; /* hard-code for prototype */
  /* if  no term ID in use */
  /* char list_[] = "{}" */
  printf("prototype query_term_list\n");
  
  snprintf (term_list, TERMLIST_LENGTH, "%s", list_t); 
  return 0; 
}

int parse_json(char * p_msg, struct terminal_info_struct *term_info_ptr)
{
  cJSON *p_json;
  cJSON *p_sub;

  if(NULL == p_msg)
  {
    return 1;
  }

  p_json = cJSON_Parse(p_msg);
  if(NULL == p_json)                                                            
  {
      return 1;
  }

  p_sub = cJSON_GetObjectItem(p_json, "cardType");
  snprintf (term_info_ptr->card_type, CARD_NAME_LENGTH , "%s", cJSON_Print(p_sub));
  
  p_sub = cJSON_GetObjectItem(p_json, "Transaction");
  snprintf (term_info_ptr->transaction_type, TRANS_NAME_LENGTH , "%s", cJSON_Print(p_sub));

  cJSON_Delete(p_json);

  return 0;
}

int 
find_spare_id(int *id_ptr)
{
 /* prototype code */
 
  int array_id;

  array_id = 15; /* hard-code */

 /* spare array id + 1, id starts from 1 */
 *id_ptr = array_id + 1;
  return 0;
}

int
insert_array(struct terminal_info_struct *terminal_info_ptr)
{
 /* prototype code, insert an entry to global arrary gl_terminal_info[MAXTERMID]*/
 return 0;
}

int
init_array()
{
 /* prototyme code, init all members with 0 or NULL for global array gl_terminal_info[MAXTERMID] */

 return 0;
}
