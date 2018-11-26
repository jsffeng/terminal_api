#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

#include "req_handler.h"

#ifdef UTFLAG 
extern char TEST_BUF0[MAXJSON_INFOSIZE];
extern char TEST_BUF1[MAXJSON_INFOSIZE];
extern int call_by_test_svr_process_req3;
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
  char id_char[TERMID_LENGTH];
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
   	
    sprintf(id_char,"{%d}",id);
    if (insert_db(&l_terminal_info) != 0)
    {  
      ERRLOG ("unexpected return value.");
      return 1;
    }
  }
  else
  {
    sprintf(id_char,"{}");
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
  char term_json_entry[MAXJSON_INFOSIZE];
  char *term_json_ptr;
  int ret_val;

  #ifdef UTFLAG 
  /*
    printf("TEST_BUF0->card_type is %s\n", ((struct terminal_info_struct *) TEST_BUF0)->card_type);
  */
  term_db_ptr = (struct terminal_info_struct *) TEST_BUF0;	 
  #else
  term_db_ptr = &term_db_entry;	 
  #endif

  ret_val=query_db(id, term_db_ptr);

  if (ret_val == 1) 
  {
  /* if id no found in DB */ 
  /* term_db_ptr is NULL */
    term_db_ptr = NULL;
  }
  else if (ret_val)
  {
    ERRLOG ("unexpected return value.");
    return 1;
  }
   
  #ifdef UTFLAG 
  term_json_ptr = TEST_BUF1;
  #else
  term_json_ptr = term_json_entry;	
  #endif

  /* if term_db_ptr is NULL, term_json_ptr pointed to "{}"; */
  ret_val=struct2json((struct terminal_info_struct *)term_db_ptr, term_json_ptr);
  
  if (ret_val) 
  {
    ERRLOG ("unexpected return value.");
    return 1;
  }

  if ( term_db_ptr == NULL ) 
  {
    strcpy(term_info, term_json_ptr); 
  }
  else
    snprintf(term_info, MAXJSON_INFOSIZE,term_json_ptr, id); 
        
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
