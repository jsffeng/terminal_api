#include <string.h>
#include "unity.h"
#include "Mockto_do_func.h"
#include "req_handler.h"

char TEST_BUF0[MAXJSON_INFOSIZE];
char TEST_BUF1[MAXJSON_INFOSIZE];
int call_by_test_svr_process_req3 = 0;
int call_by_test_svr_process_req4 = 0;
int call_by_test_svr_process_req5 = 0;

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
* Condition: When DB entry exist - used DB entry {15,"Visa","Credit",1} for 	*
mock function query_db()							*
* Input: 15 									*
* Output: {"terminalID":15,"TransactionType":"Credit","cardType":"Visa"} 	*
********************************************************************************/

void test_query_term_info1()
{
  struct terminal_info_struct term_db_entry_input={15,"Visa","Credit",1};
  char term_json_entry_input[]="{\"terminalID\":%d,\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  char term_info_output[MAXJSON_INFOSIZE];
  char term_info_expect[]="{\"terminalID\":15,\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";

  /* set query_db(), to return 0 */
  memcpy(TEST_BUF0,&term_db_entry_input, sizeof(term_db_entry_input));
  query_db_ExpectAndReturn(15, (struct terminal_info_struct *) TEST_BUF0, 0);

  /* set struct2json(), to return 0 */
  strcpy(TEST_BUF1, term_json_entry_input);
  struct2json_ExpectAndReturn((struct terminal_info_struct *) TEST_BUF0, TEST_BUF1,0);

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

  /* set query_db(), to return 1 */
  query_db_ExpectAndReturn(15, (struct terminal_info_struct *) TEST_BUF0, 1);

  /* set struct2json(), to return 0 */
  strcpy(TEST_BUF1, term_json_entry_input);
  struct2json_ExpectAndReturn((struct terminal_info_struct *) NULL, TEST_BUF1,0);

  TEST_ASSERT_EQUAL(0, query_term_info(15, term_info_output)); 

  TEST_ASSERT_EQUAL_STRING (term_info_expect, term_info_output);
}

/********************************************************************************
* Condition: When query_db() fails						*
* Input: 15 									*
* Output: Function returns with non zero value	 				*
********************************************************************************/

void test_query_term_info3()
{
  struct terminal_info_struct term_db_entry_input={15,"Visa","Credit",1};
  char term_json_entry_input[]="{\"terminalID\":%d;\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  char term_info_output[MAXJSON_INFOSIZE];
  char term_info_expect[]="{\"terminalID\":15;\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";

  /* set query_db(), to return 2 */
  memcpy(TEST_BUF0,&term_db_entry_input, sizeof(term_db_entry_input));
  query_db_ExpectAndReturn(15, (struct terminal_info_struct *) TEST_BUF0, 2);

  TEST_ASSERT_NOT_EQUAL(0, query_term_info(15, term_info_output)); 

}

/********************************************************************************
* Condition: When struct2json() fails						*
* Input: 15 									*
* Output: Function returns with non zero value 					*
********************************************************************************/

void test_query_term_info4()
{
  struct terminal_info_struct term_db_entry_input={15,"Visa","Credit",1};
  char term_json_entry_input[]="{\"terminalID\":%d;\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  char term_info_output[MAXJSON_INFOSIZE];
  char term_info_expect[]="{\"terminalID\":15;\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";

  /* set query_db(), to return 0 */
  memcpy(TEST_BUF0,&term_db_entry_input, sizeof(term_db_entry_input));
  query_db_ExpectAndReturn(15, (struct terminal_info_struct *) TEST_BUF0, 0);

  /* set struct2json(), to return 1 */
  strcpy(TEST_BUF1, term_json_entry_input);
  struct2json_ExpectAndReturn((struct terminal_info_struct *) TEST_BUF0, TEST_BUF1,1);

  TEST_ASSERT_NOT_EQUAL(0, query_term_info(15, term_info_output)); 

}

/********************************************************************************
* Input: {"TransactionType":"Credit","cardType":"Visa"} 			*	
* Output: 								 	*
*       term_info_ptr->card_type: "Visa" 					*
*       term_info_ptr->transaction_type: "Credit"				*
********************************************************************************/

void test_parse_json()
{
  char term_json_entry_input[]="{\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
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
* Input: {"TransactionType":"Credit","cardType":"Visa"}				*
* Output:									* 
* 	id {15}	was assigned & responded 					*
*	{15,"\"Visa\"","\"Credit\"",1} inserted into DB			 	* 
********************************************************************************/
void test_svr_process_req1()
{
  char term_json_entry_input[]="{\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  struct terminal_info_struct term_db_entry_expect={15,"\"Visa\"","\"Credit\"",1};
  char tm_id[TERMID_LENGTH];
 
  char tm_id_expect[] = "{15}";

  TEST_ASSERT_EQUAL(0, svr_process_req(term_json_entry_input,tm_id)); 

  TEST_ASSERT_EQUAL_STRING (tm_id_expect, tm_id);

  TEST_ASSERT_EQUAL_MEMORY (&term_db_entry_expect, &gl_terminal_info[14], sizeof(struct terminal_info_struct));
}

/********************************************************************************
* Condition: when no available terminal ID slot to assign  		*
* Input: {"TransactionType":"Credit","cardType":"Visa"}				*
* Output:									* 
* 	id {}	was responded		 					*
*	No update to DB							 	* 
********************************************************************************/
void test_svr_process_req2()
{
  char term_json_entry_input[]="{\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
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
* Input: {"TransactionType":"Credit","cardType":"Visa"}				*
* Output:									* 
*	return 1								*
********************************************************************************/
void test_svr_process_req3()
{
  char term_json_entry_input[]="{\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  char tm_id[TERMID_LENGTH];
 
  call_by_test_svr_process_req3 = 1;

  TEST_ASSERT_EQUAL(1, svr_process_req(term_json_entry_input,tm_id)); 

  call_by_test_svr_process_req3 = 0;

}


/********************************************************************************
* Condition: when find_spare_id() fails 						*
* Input: {"TransactionType":"Credit","cardType":"Visa"}				*
* Output:									* 
*	return 1								*
********************************************************************************/
void test_svr_process_req4()
{
  char term_json_entry_input[]="{\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  char tm_id[TERMID_LENGTH];
 
  call_by_test_svr_process_req4 = 1;

  TEST_ASSERT_EQUAL(1, svr_process_req(term_json_entry_input,tm_id)); 

  call_by_test_svr_process_req4 = 0;
}


/********************************************************************************
* Condition: when insert_db() fails 						*
* Input: {"TransactionType":"Credit","cardType":"Visa"}				*
* Output:									* 
*	return 1								*
********************************************************************************/
void test_svr_process_req5()
{
  char term_json_entry_input[]="{\"TransactionType\":\"Credit\",\"cardType\":\"Visa\"}";
  char tm_id[TERMID_LENGTH];
 
  call_by_test_svr_process_req5 = 1;

  TEST_ASSERT_EQUAL(1, svr_process_req(term_json_entry_input,tm_id)); 

  call_by_test_svr_process_req5 = 0;
}
