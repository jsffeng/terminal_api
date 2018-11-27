#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

#include "req_handler.h"

#ifdef UTFLAG 
extern int call_by_test_svr_process_req3;
extern int call_by_test_query_term_info4;
#endif

/************************************************************************
* Called by POST, will assign a terminal ID, store an entry 		*
* in DB (in-memory data array), then return ID with in json format.	*
*									*
* Parameter examples:							*
* 	input_data: {"TransactionType":"Credit","cardType":"Visa"}	*
* 	resp_data: {15}, if a terminal ID assigned successfully		*
* 	resp_data: {}, if no available terminal ID			*
*									*
* Return values:							*
* 	if succeeds, return 0						*
* 	if fails, return 1						*
************************************************************************/

int
svr_process_req (char * input_data, char * resp_data)
{
  int id=0; 
  int ret_val;
  char id_char[TERMID_LENGTH] = "";
  struct terminal_info_struct l_terminal_info;

  /* For unit testing, intialize l_terminal_info so as to quickly compare with expected
  Data, which has been initialized to 0 by compiler for unused char bytes. 
  */

  #ifdef UTFLAG 
   memset (&l_terminal_info,0,sizeof(l_terminal_info)); 
  #endif

  ret_val=parse_json(input_data, &l_terminal_info);
  
  if (ret_val) 
  {
    ERRLOG ("unexpected return value.");
    return 1;
  }
  /* set id 0 if no spare */
  ret_val=find_spare_id(&id);
  
  if (ret_val) 
  {  
    ERRLOG ("unexpected return value.");
    return 1;
  }
  if ( id != 0)
  {
    l_terminal_info.id = id;
    l_terminal_info.flag = 1;
   	
    snprintf(id_char,TERMID_LENGTH,"{%d}",id);
    if (insert_db(&l_terminal_info) != 0)
    {  
      ERRLOG ("unexpected return value.");
      return 1;
    }
  }
  else
  {
    strncpy(id_char,"{}",3);
  }
 
  snprintf (resp_data, TERMID_LENGTH, "%s", id_char);
  return 0; 
}

/************************************************************************
* Query DB for the terminal id, return term_info in json format 	*
*									*
* Parameter examples:							*	
*	id: 15								*
* 	term_info: {"terminalID":15,"TransactionType":"Credit",		*
*	"cardType":"Visa"} 						*
*									*
* Return values:							*
* 	if succeeds, return 0						*
* 	if fails, return 1						*
************************************************************************/

int
query_term_info (int id, char *term_info)
{
  struct terminal_info_struct term_db_entry; 
  struct terminal_info_struct *term_db_ptr; 
  char term_json_entry[MAXJSON_INFOSIZE] = "";
  char *term_json_ptr;
  int ret_val;

  term_db_ptr = &term_db_entry;	 
  term_json_ptr = term_json_entry;	

  ret_val=query_db(id, term_db_ptr);

  if (ret_val == 0)
  {
    ret_val=struct2json((struct terminal_info_struct *)term_db_ptr, term_json_ptr);
  
    if (ret_val) 
    {
      ERRLOG ("unexpected return value.");
      return 1;
    }
  }
  else if (ret_val == 1) 
  {
  /* if id no found in DB, let term_json_ptr pointed to "{}" */ 
    strncpy(term_json_ptr, "{}",3); 
  }
  else 
  {
    ERRLOG ("unexpected return value.");
    return 1;
  }
 
  snprintf(term_info, MAXJSON_INFOSIZE,"%s",term_json_ptr); 
        
  return 0; 
}

/************************************************************************
* Respond all assigned terminal IDs in DB.		 		*
*									*
* Parameter examples:							*
* 	if found, term_list popluated, e.g. "{2 4 5 10}"		*
* 	if not found, term_list popluated as "{}"			*
*									*
* Return values:							*
* 	if succeeds, return 0						*
* 	if abormal error, return 1 (not used so far)			*
************************************************************************/

int
query_term_list (char *term_list)
{
  int id;
  int first_flag = 0;
  char id_str[ID_CHAR_LENGTH] = "";
  strncpy (term_list,"{",2);

  for (int i=0;i < MAXTERMID; i++)
  {
    if (gl_terminal_info[i].flag == 1)
    {
      if (first_flag == 0)
      {
        snprintf(id_str,ID_CHAR_LENGTH, "%d", gl_terminal_info[i].id);
        strncat(term_list, id_str, strlen(id_str));
        first_flag = 1;
      }
      else
      {
        snprintf(id_str,ID_CHAR_LENGTH, "%d", gl_terminal_info[i].id);
        strncat(term_list, " ", 1);
        strncat(term_list, id_str, strlen(id_str));
      }
    }
  }
  
  strncat(term_list, "}", 1);
 
  return 0; 
}

/************************************************************************
* Store data in json format to data structure provided.	 		*
*									*
* Parameter examples:							*
* 	p_msg: {"TransactionType":"Credit","cardType":"Visa"}		*
* 	term_info_ptr->card_type: "Visa"	 			*
* 	term_info_ptr->transaction_type: "Credit"			*
*									*
* Return values:							*
* 	if succeeds, return 0						*
* 	if fails, return 1						*
************************************************************************/

int parse_json(char * p_msg, struct terminal_info_struct *term_info_ptr)
{
  cJSON *p_json;
  cJSON *p_sub;

  if(NULL == p_msg)
  {
    ERRLOG ("empty json message.");
    return 1;
  }

  p_json = cJSON_Parse(p_msg);
  if(NULL == p_json)                                                            
  {
    ERRLOG ("failure in cJSON_Parse().");
    return 1;
  }

  /* Extract cardType */ 
  p_sub = cJSON_GetObjectItem(p_json, "cardType");
  snprintf (term_info_ptr->card_type, CARD_NAME_LENGTH , "%s", cJSON_Print(p_sub));
 
  /* Extract TransactionType */ 
  p_sub = cJSON_GetObjectItem(p_json, "TransactionType");
  snprintf (term_info_ptr->transaction_type, TRANS_NAME_LENGTH , "%s", cJSON_Print(p_sub));

  cJSON_Delete(p_json);

  #ifdef UTFLAG 
  if ( call_by_test_svr_process_req3 )
  /* This is for unit testing */
    return 1;
  else
    return 0;
  #else
    return 0;
  #endif
}

/************************************************************************
* Convert data structure in DB to string in json format. 		*
*									*
* Parameter examples:							*
* 	tm_db_ptr point to data like {15,"Visa","Credit",1}		*
*	tm_json[] contains string like:					*
*	{"terminalID":15,"TransactionType":"Credit","cardType":"Visa"}	*
* Return values:							*
* 	if succeeds, return 0						*
* 	if abormal error, return 1 (not used so far)			*
************************************************************************/
int
struct2json(struct terminal_info_struct *tm_db_ptr, char tm_json[])
{

  char id_str[ID_CHAR_LENGTH] = "";
  
  strncpy (tm_json,"{\"terminalID\":",15);
  snprintf(id_str,ID_CHAR_LENGTH, "%d",tm_db_ptr->id);
  strncat(tm_json, id_str, strlen(id_str));
  
  strncat(tm_json, ",\"TransactionType\":",19);
  strncat(tm_json, tm_db_ptr->transaction_type, strlen(tm_db_ptr->transaction_type));

  strncat(tm_json, ",\"cardType\":",12);
  strncat(tm_json, tm_db_ptr->card_type, strlen(tm_db_ptr->card_type));

  strncat(tm_json, "}",1);

  #ifdef UTFLAG
  if ( call_by_test_query_term_info4 )
  /* This is for unit testing */
    return 1;
  else
    return 0;
  #else
    return 0;
  #endif

}

