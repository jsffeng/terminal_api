#include <string.h>
#include "unity.h"
#include "Mockto_do_func.h"
#include "req_handler.h"

int call_by_test_svr_process_req3 = 0;
int call_by_test_svr_process_req4 = 0;
int call_by_test_svr_process_req5 = 0;
int call_by_test_query_term_info4 = 0;

const int TEST_USED_ID=14; 

void setUp(void)
{
  init_db();
 
  for (int i=0;i < TEST_USED_ID; i++)
    gl_terminal_info[i].flag = 1;
}

void tearDown(void)
{
}

/********************************************************************************
* Condition: When DB entry exist - used DB entry {15,"Visa","Credit",1} 	*
* Input: 15 									*
* Output: {"terminalID":15,"transactions":					*
*    	    {"cardType":"Visa","TransactionType":"Credit"}}			*
********************************************************************************/

void test_query_term_info1()
{

  char term_info_output[MAXJSON_INFOSIZE];
  char term_info_expect[]="{\"terminalID\":15,\"transactions\":{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}}";

  gl_terminal_info[14].id = 15;
  strcpy (gl_terminal_info[14].card_type, "\"Visa\"");
  strcpy (gl_terminal_info[14].transaction_type, "\"Credit\"");
  gl_terminal_info[14].flag = 1;

  TEST_ASSERT_EQUAL(0, query_term_info(15, term_info_output)); 

  TEST_ASSERT_EQUAL_STRING (term_info_expect, term_info_output);
}

/********************************************************************************
* Condition: When no DB entry exist						*
* Input: 15 									*
* Output: {)									*
********************************************************************************/

void test_query_term_info2()
{
  char term_json_entry_input[]="{}";
  char term_info_output[MAXJSON_INFOSIZE];
  char term_info_expect[]="{}";

  TEST_ASSERT_EQUAL(0, query_term_info(15, term_info_output)); 

  TEST_ASSERT_EQUAL_STRING (term_info_expect, term_info_output);
}


/********************************************************************************
* Condition: When query_db() fails						*
* Input: 150 									*
* Output: Function returns with non zero value	 				*
********************************************************************************/

void test_query_term_info3()
{
  char term_info_output[MAXJSON_INFOSIZE];

  /* query_db() should return 2 for out of range terminal ID */

  TEST_ASSERT_NOT_EQUAL(0, query_term_info(150, term_info_output)); 

}

/********************************************************************************
* Condition: When struct2json() fails						*
* Input: 15 									*
* Output: Function returns with non zero value 					*
********************************************************************************/

void test_query_term_info4()
{
  char term_info_output[MAXJSON_INFOSIZE];

  /* set data to let query_db() return 0 */
  gl_terminal_info[14].id = 15;
  strcpy (gl_terminal_info[14].card_type, "\"Visa\"");
  strcpy (gl_terminal_info[14].transaction_type, "\"Credit\"");
  gl_terminal_info[14].flag = 1;

  /* when struct2json() return 1 */
  call_by_test_query_term_info4 = 1;

  TEST_ASSERT_NOT_EQUAL(0, query_term_info(15, term_info_output)); 

  call_by_test_query_term_info4 = 0;

}

/********************************************************************************
* Input: {"cardType":"Visa","TransactionType":"Credit"} 			*
* Output: 								 	*
*       term_info_ptr->card_type: "Visa" 					*
*       term_info_ptr->transaction_type: "Credit"				*
********************************************************************************/

void test_parse_json()
{
  char term_json_entry_input[]="{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}";
  struct terminal_info_struct term_db_entry_input={0,"","",0};
  struct terminal_info_struct term_db_entry_expect={0,"\"Visa\"","\"Credit\"",0};

  TEST_ASSERT_EQUAL(0, parse_json(term_json_entry_input, &term_db_entry_input)); 

  TEST_ASSERT_EQUAL_STRING (term_db_entry_expect.card_type, term_db_entry_input.card_type);

  TEST_ASSERT_EQUAL_STRING (term_db_entry_expect.transaction_type, term_db_entry_input.transaction_type);
  
  TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_expect, &term_db_entry_input, sizeof(struct terminal_info_struct));
}

/********************************************************************************
* Input: N/A 									*
* Output: 								 	*
*       Any member of gl_terminal_info got initialized				*
********************************************************************************/

void test_init_db()
{
  struct terminal_info_struct term_db_entry_expect={0,"","",0};
  /* call init_db direclty */
  init_db(); 

  for (int i=0;i < MAXTERMID; i++)
  {
      TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_expect, &gl_terminal_info[i], sizeof(struct terminal_info_struct));
  }
}

/********************************************************************************
* Condition: When there is available terminal ID slot 				*
* Input: terminal_info_struct							*
* Output: 								 	*
*        Data inserted into DB						 	*
********************************************************************************/

void test_insert_db1()
{
  struct terminal_info_struct term_db_entry_input={15,"\"Visa\"","\"Credit\"",1};
  struct terminal_info_struct term_db_entry_default={0,"","",0};
  struct terminal_info_struct term_db_entry_update={0,"","",1};

  TEST_ASSERT_EQUAL(0, insert_db(&term_db_entry_input)); 
  
  for (int i=0;i < MAXTERMID; i++)
  {
    if (i < TEST_USED_ID )
      TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_update, &gl_terminal_info[i], sizeof(struct terminal_info_struct));
    else if (i == TEST_USED_ID )
      TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_input, &gl_terminal_info[i], sizeof(struct terminal_info_struct));
    else
      TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_default, &gl_terminal_info[i], sizeof(struct terminal_info_struct));
  }
}

/********************************************************************************
* Condition: When there is no available terminal ID slot (i.e. no available DB 	*
* slot in this design)								*
* Input: terminal_info_struct							*
* Output: 								 	*
*        nothing but return 1							*
********************************************************************************/

void test_insert_db2()
{
  struct terminal_info_struct term_db_entry_input={15,"\"Visa\"","\"Credit\"",1};
  struct terminal_info_struct term_db_entry_update={0,"","",1};

  for (int i=0;i < MAXTERMID; i++)
    gl_terminal_info[i].flag = 1;

  TEST_ASSERT_EQUAL(1, insert_db(&term_db_entry_input)); 

  for (int i=0;i < MAXTERMID; i++)
  {
      TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_update, &gl_terminal_info[i], sizeof(struct terminal_info_struct));
  }
}

/********************************************************************************
* Condition: When there is available terminal ID slot (i.e.  available DB 	*
* slot in this design)								*
* Input: Set the 1st available terminal ID slot is 15.				*
* Output: 								 	*
*        Set id with 15 							*
********************************************************************************/

void test_find_spare_id1()
{
  int id;
  const int id_expect=TEST_USED_ID+1;

  for (int i=0;i < (id_expect-1); i++)
    gl_terminal_info[i].flag = 1;

  TEST_ASSERT_EQUAL(0, find_spare_id(&id)); 
  TEST_ASSERT_EQUAL_INT(id_expect, id);
}

/********************************************************************************
* Condition: When there is no available terminal ID slot (i.e. no available DB 	*
* slot in this design)								*
* Input: mark all DB slot occupied.						*
* Output: 								 	*
*        set id with 0 								*
********************************************************************************/
void test_find_spare_id2()
{
  int id;
  const int id_expect = 0;

  for (int i=0;i < MAXTERMID; i++)
    gl_terminal_info[i].flag = 1;

  TEST_ASSERT_EQUAL(0, find_spare_id(&id)); 
  TEST_ASSERT_EQUAL_INT(id_expect, id);
}

/********************************************************************************
* Condition: When a terminal ID assigned successfully        			*
* Input: {"cardType":"Visa","TransactionType":"Credit"} 			*
* Output:									* 
* 	id {15}	was assigned & responded 					*
*	{15,"\"Visa\"","\"Credit\"",1} inserted into DB			 	* 
********************************************************************************/
void test_svr_process_req1()
{
  char term_json_entry_input[]="{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}";
  struct terminal_info_struct term_db_entry_expect={15,"\"Visa\"","\"Credit\"",1};
  char tm_id[TERMID_LENGTH];
 
  char tm_id_expect[] = "{15}";

  TEST_ASSERT_EQUAL(0, svr_process_req(term_json_entry_input,tm_id)); 

  TEST_ASSERT_EQUAL_STRING (tm_id_expect, tm_id);

  TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_expect, &gl_terminal_info[14], sizeof(struct terminal_info_struct));
}

/********************************************************************************
* Condition: when no available terminal ID slot to assign  		*
* Input: {"cardType":"Visa","TransactionType":"Credit"} 			*
* Output:									* 
* 	id {}	was responded		 					*
*	No update to DB							 	* 
********************************************************************************/
void test_svr_process_req2()
{
  char term_json_entry_input[]="{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}";
  struct terminal_info_struct term_db_entry_expect={0,"","",1};
  char tm_id[TERMID_LENGTH];
 
  char tm_id_expect[] = "{}";

  for (int i=0;i < MAXTERMID; i++)
    gl_terminal_info[i].flag = 1;

  TEST_ASSERT_EQUAL(0, svr_process_req(term_json_entry_input,tm_id)); 

  TEST_ASSERT_EQUAL_STRING (tm_id_expect, tm_id);

  for (int i=0;i < MAXTERMID; i++)
  TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_expect, &gl_terminal_info[i], sizeof(struct terminal_info_struct));
}

/********************************************************************************
* Condition: when parse_json() fails 						*
* Input: {"cardType":"Visa","TransactionType":"Credit"} 			*
* Output:									* 
*	return 1								*
********************************************************************************/
void test_svr_process_req3()
{
  char term_json_entry_input[]="{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}";
  char tm_id[TERMID_LENGTH];
 
  call_by_test_svr_process_req3 = 1;

  TEST_ASSERT_EQUAL(1, svr_process_req(term_json_entry_input,tm_id)); 

  call_by_test_svr_process_req3 = 0;

}


/********************************************************************************
* Condition: when find_spare_id() fails 					*
* Input: {"cardType":"Visa","TransactionType":"Credit"} 			*
* Output:									* 
*	return 1								*
********************************************************************************/
void test_svr_process_req4()
{
  char term_json_entry_input[]="{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}";
  char tm_id[TERMID_LENGTH];
 
  call_by_test_svr_process_req4 = 1;

  TEST_ASSERT_EQUAL(1, svr_process_req(term_json_entry_input,tm_id)); 

  call_by_test_svr_process_req4 = 0;
}


/********************************************************************************
* Condition: when insert_db() fails 						*
* Input: {"cardType":"Visa","TransactionType":"Credit"} 			*
* Output:									* 
*	return 1								*
********************************************************************************/
void test_svr_process_req5()
{
  char term_json_entry_input[]="{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}";
  char tm_id[TERMID_LENGTH];
 
  call_by_test_svr_process_req5 = 1;

  TEST_ASSERT_EQUAL(1, svr_process_req(term_json_entry_input,tm_id)); 

  call_by_test_svr_process_req5 = 0;
}

/********************************************************************************
* Condition: When terminal ID found in DB 					*
* Input: 15 									*
* Output: 								 	*
*        Data structure responded					 	*
********************************************************************************/
void test_query_db1()
{
  struct terminal_info_struct term_db_entry_input;
  
  /* set data for query_db() use */
  gl_terminal_info[14].id = 15;
  strcpy (gl_terminal_info[14].card_type, "\"Visa\"");
  strcpy (gl_terminal_info[14].transaction_type, "\"Credit\"");
  gl_terminal_info[14].flag = 1;

  TEST_ASSERT_EQUAL(0, query_db(15, &term_db_entry_input)); 

  TEST_ASSERT_EQUAL_MEMORY (&gl_terminal_info[14], &term_db_entry_input, sizeof(struct terminal_info_struct));
  
}

/********************************************************************************
* Condition: When terminal ID not found in DB 					*
* Input: 15 									*
* Output: 								 	*
*        return 1 							 	*
********************************************************************************/
void test_query_db2()
{
  struct terminal_info_struct term_db_entry_input;
  
  TEST_ASSERT_EQUAL(1, query_db(15, &term_db_entry_input)); 
}

/********************************************************************************
* Condition: When requested terminal ID out of range 				*
* Input: 0,150,-1 								*
* Output: 								 	*
*        return 2 							 	*
********************************************************************************/
void test_query_db3()
{
  struct terminal_info_struct term_db_entry_input;
  
  TEST_ASSERT_EQUAL(2, query_db(0, &term_db_entry_input)); 
  TEST_ASSERT_EQUAL(2, query_db(150, &term_db_entry_input)); 
  TEST_ASSERT_EQUAL(2, query_db(-1, &term_db_entry_input)); 
}

/********************************************************************************
* Condition: When terminal IDs found in DB 					*
* Input: N/A									*
* Output: 								 	*
*        The list of terminal IDs, e.g. "{81 84 87 90}"				* 
********************************************************************************/
void test_query_term_list1()
{
  char term_list_output[TERMLIST_LENGTH];
  char term_list_expect[TERMLIST_LENGTH] = "{81 84 87 90}";
  int i;
  
  /* set data for query_db() use */
  init_db();

  i = 80;
  while (i < 90)
  {
    gl_terminal_info[i].id = i + 1;
    strcpy (gl_terminal_info[14].card_type, "\"Visa\"");
    strcpy (gl_terminal_info[14].transaction_type, "\"Credit\"");
    gl_terminal_info[i].flag = 1;
    i = i + 3; 
  }

  TEST_ASSERT_EQUAL(0, query_term_list(term_list_output)); 

  TEST_ASSERT_EQUAL_STRING (term_list_expect, term_list_output);
  
}

/********************************************************************************
* Condition: When no terminal IDs assigned in DB				*
* Input: N/A									*
* Output: 								 	*
*        Empty terminal ID list as "{}"						*
********************************************************************************/
void test_query_term_list2()
{
  char term_list_output[TERMLIST_LENGTH];
  char term_list_expect[TERMLIST_LENGTH] = "{}";
  
  /* set data for query_db() use */
  init_db();

  TEST_ASSERT_EQUAL(0, query_term_list(term_list_output)); 

  TEST_ASSERT_EQUAL_STRING (term_list_expect, term_list_output);
  
}

/********************************************************************************
* Input: {15,"Visa","Credit",1} 						*	
* Output: {"terminalID":15,"transactions":					*
*    	    {"cardType":"Visa","TransactionType":"Credit"}}			*
********************************************************************************/

void test_struct2json()
{
  struct terminal_info_struct term_db_entry_input={15,"\"Visa\"","\"Credit\"",1};
  char term_info_output[MAXJSON_INFOSIZE];
  char term_info_expect[]="{\"terminalID\":15,\"transactions\":{\"cardType\":\"Visa\",\"TransactionType\":\"Credit\"}}";

  TEST_ASSERT_EQUAL(0, struct2json(&term_db_entry_input, term_info_output)); 

  TEST_ASSERT_EQUAL_STRING (term_info_expect,term_info_output );
}
