#include "stdio.h"
#include "req_handler.h"

int
struct2json(struct terminal_info_struct *tm_db_ptr, char tm_json[])
{
  return 0;
}

/* return 0, id found in DB */
/* return 1, id no found in DB , this could happen when client send a wrong id */
/* return 2, abormal errors when query DB */
int
query_db(int id, struct terminal_info_struct *tm_db_ptr)
{
  return 0;
}

