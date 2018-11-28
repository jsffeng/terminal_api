A RESTful API server to handle client/terminal request as belows: 
-------------------------------------------------------------------  
1. When client sends POST request as the following command does:  
curl -X POST -d ‘json={"cardType":"Visa","TransactionType":"Credit"}’ http://localhost:8888/terminals  
Server service should take the following actions:   
  - Assign a terminal ID, and store the input data entry into the in-memory array.  
  - If above succeeds, return the terminal ID, e.g. {16}, to client.  
  - If there is no available terminal ID to assign, return {}, which means the system is running on the maximum capacity designed.  
  - Returning empty to client means abnormal error happens on server side.  
Input data in JSON format:   
{   
        “cardType”: [  
        “Visa”,  
        “MasterCard”,  
        "EFTPOS”  
    ],  
        “TransactionType”: [  
        “Cheque”,  
        “Savings”,  
        “Credit”  
    ]   
  
  
2. When the client sends GET request as the following command does:  
curl -X GET http://localhost:8888/terminals/<id>  
  
Server service should take the following actions:   
  - If succeed, return the output data entry to Client in JSON format, e.g.
	{"terminalID":15,"transactions":{"cardType":"Visa","TransactionType":"Credit"}}
  - If no Data Entry exists, just return {} to client.  
  - Returning empty to client means abnormal error happens on server side.  
  
Output data in JSON format:   
{  
     "terminalID": 1,  
     "transactions": [  
     {  
          "cardType": "Visa",  
          "TransactionType": "Credit"  
     },  
     {  
          "cardType": "EFTPOS",  
          "TransactionType": "Savings"  
     }  
     ]  
}  
  
3. When client sends GET request as the following command does:  
curl -X GET http://localhost:8888/terminals/  
  
Server service should take the following actions:   
  - If succeed, return the terminal ID list to client, e.g. {2 24 35 48 80}.  
  - If there is no terminal ID in use, return {} to client.  
  - Returning empty to client means abnormal error happens on server side. 
   
Current Development Environment (Nov 20,2018): 
--------------------------
Linux Version: ubuntu 16.04.01  
libmicrohttpd version: 0.9.44 (re-run "sudo apt install libmicrohttpd-dev" can see it)  
cJSON Libarary - clone & complied from https://github.com/DaveGamble/cJSON on Nov 20, 2018.

Install packages:
----------------
sudo apt install libmicrohttpd-dev  
sudo apt install curl  
complied and install cJSON  
download cmock from  https://github.com/ThrowTheSwitch/CMock   
sudo apt install expect

Source list:  
------------------------- 
db_api.c  
global.h  
Makefile  
README.md  
req_handler.c  
req_handler.h  
term_svr.c  
term_svr.h  
test  
to_do_func.c  
to_do_func.h  
test/UTfuncs_ut.c  
test/system_test.scr

make commands:
-------------------------
make BLD : build final executable term_api.  
make UT: build executable test/UTfuncs for unit testing.  
make clean_all: clean up generated sources(by cmock) and all binaries.  

Unit testing (function level):  
----------------------------------------------------------------
Unit testing was done by cmock/unity, with mock following two functions:  
ubuntu->cat to_do_func.h  
#include "req_handler.h"  
int struct2json(struct terminal_info_struct *tm_db_ptr, char tm_json[]);  
int query_db(int id, struct terminal_info_struct *tm_db_ptr);  

ubuntu->cd test  
ubuntu->ls  
curl.scr  system_test.scr  UTfuncs_runner.c       UTfuncs_ut.c  
ERR.log   UTfuncs          UTfuncs_runner_test.o  UTfuncs_ut_test.o  
ubuntu->UTfuncs  
./test/UTfuncs_ut.c:32:test_query_term_info1:PASS  
./test/UTfuncs_ut.c:54:test_query_term_info2:PASS  
./test/UTfuncs_ut.c:72:test_query_term_info3:PASS  
./test/UTfuncs_ut.c:88:test_query_term_info4:PASS  
./test/UTfuncs_ut.c:114:test_parse_json:PASS  
./test/UTfuncs_ut.c:135:test_init_db:PASS  
./test/UTfuncs_ut.c:154:test_insert_db1:PASS  
./test/UTfuncs_ut.c:181:test_insert_db2:PASS  
./test/UTfuncs_ut.c:205:test_find_spare_id1:PASS  
./test/UTfuncs_ut.c:224:test_find_spare_id2:PASS  
./test/UTfuncs_ut.c:243:test_svr_process_req1:PASS  
./test/UTfuncs_ut.c:265:test_svr_process_req2:PASS  
./test/UTfuncs_ut.c:290:test_svr_process_req3:PASS  
./test/UTfuncs_ut.c:310:test_svr_process_req4:PASS  
./test/UTfuncs_ut.c:329:test_svr_process_req5:PASS  
./test/UTfuncs_ut.c:347:test_query_db1:PASS  
./test/UTfuncs_ut.c:369:test_query_db2:PASS  
./test/UTfuncs_ut.c:382:test_query_db3:PASS  
./test/UTfuncs_ut.c:397:test_query_term_list1:PASS  
./test/UTfuncs_ut.c:428:test_query_term_list2:PASS  
./test/UTfuncs_ut.c:448:test_struct2json:PASS  
   
-----------------------  
21 Tests 0 Failures 0 Ignored  
OK  
