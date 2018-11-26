#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

#include "../defines.h"
#include "../term_svr.h"
#include "../req_handler.h"

void test_parse_json()
{
  char *p_msg = "{\"cardType\":\"Visa\",\"Transaction\":\"Credit\"}";
  struct terminal_info_struct test_term_info;
  parse_json(p_msg, &test_term_info);
  printf("cardType is %s\n",test_term_info.card_type);
  printf("TransactionType is %s\n",test_term_info.transaction_type);
  
}

int
main ()
{
  printf ("Test of function parse_json()\n\n");

  test_parse_json();
  return 0;
}
