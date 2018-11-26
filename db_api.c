#include <stdio.h>
#include <string.h>
#include "req_handler.h"

#ifdef UTFLAG
extern int call_by_test_svr_process_req4;
extern int call_by_test_svr_process_req5;
#endif

/* Find a spare terminal ID in range of 1-100 */
/* If no available terminal ID, set id = 0 */
/* Succeed, return 0; abormal error return 1 */
int
find_spare_id(int *id_ptr)
{
  int id = 0;

  for (int i=0;i < MAXTERMID; i++)
  {
    if (gl_terminal_info[i].flag  == 0)
    {
      id = i + 1;
      break;
    }
  }
  
  *id_ptr = id;

  #ifdef UTFLAG
  if ( call_by_test_svr_process_req4 )
  /* This is for unit testing */
    return 1;
  else
    return 0;
  #else
    return 0;
  #endif

}  

/************************************************************************
* Insert an entry to global arrary gl_terminal_info[MAXTERMID]		*
* return 								*
* Return values:							*
* 	if inserted, return 0						*
* 	if no available terminal ID slot (equal to DB slot in this 	*
*	design), 							*
*	return 1							*
*	abnormal error, return 2 (Not being used currenlty )		*
************************************************************************/
int
insert_db(struct terminal_info_struct *terminal_info_ptr)
{
  int i;

  for (i=0;i < MAXTERMID; i++)
  {
     if (gl_terminal_info[i].flag != 0 ) 
       continue;
     else
       break;
  }

  if (i < MAXTERMID)
     memcpy(&gl_terminal_info[i],terminal_info_ptr, sizeof(struct terminal_info_struct));
  else if (i == MAXTERMID)
  {
    /* No available DB slot, which means no available terminal ID slot */ 
    ERRLOG ("no available terminal ID slot.");
    return 1;
  }
  else
  {
    /* should not reach here */
    ERRLOG ("abnormal error.");
    return 2;
  }

  #ifdef UTFLAG
  if ( call_by_test_svr_process_req5 )
  /* This is for unit testing */
    return 2;
  else
    return 0;
  #else
    return 0;
  #endif
}

/* Init all members with 0 or NULL for global array gl_terminal_info[MAXTERMID] */

/* 
void
init_db()
{
  for (int i=0;i < MAXTERMID; i++)
  {
    gl_terminal_info[i].id = 0;
    gl_terminal_info[i].flag = 0;
    for (int j=0; j<CARD_NAME_LENGTH; j++)	
      gl_terminal_info[i].card_type[j]='\0';
    for (int j=0; j<TRANS_NAME_LENGTH; j++)
      gl_terminal_info[i].transaction_type[j]='\0';
  }  
}
*/

void
init_db()
{
   memset(&gl_terminal_info[0],0,sizeof(gl_terminal_info));
}
