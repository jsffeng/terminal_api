A RESTful API server to handle client/terminal request as belows: 
-----------------------------------------------------------------  
1. When client sends POST request as the following command does:  
curl -X POST -d ‘json={"cardType":"Visa","TransactionType":"Credit"}’ http://localhost:8888/terminals  
Server service should take the following actions:   
  - Assign a terminal ID, and store the input data entry into the in-memory array.  
  - If above succeeds, return the terminal ID, e.g. {16}, to client.  
  - If there is no available terminal ID to assign, return {}, which means the system is running on the maximum capacity designed.  
  - Returning empty to client means abnormal error happens on server side.  
NOTE:Input data in JSON format:   
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
NOTE:Output data in JSON format:   
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
---------------------------------------------  
Linux Version: ubuntu 16.04.01  
libmicrohttpd version: 0.9.44 (re-run "sudo apt install libmicrohttpd-dev" can see it)  
cJSON Libarary - clone & complied from https://github.com/DaveGamble/cJSON on Nov 20, 2018.

Install packages:
----------------  
sudo apt install libmicrohttpd-dev  
sudo apt install curl  
complied and install cJSON  
download cmock from  https://github.com/ThrowTheSwitch/CMock   

Source list:  
------------  
db_api.c  
global.h  
Makefile  
README.md  
req_handler.c  
req_handler.h  
term_svr.c  
term_svr.h  
to_do_func.c  
to_do_func.h  
test/UTfuncs_ut.c  
test/system_test.scr  

make commands:  
--------------  
Run make commmand in current directory.  
make BLD : create final executable term_api.  
make UT: create executable UTfuncs under test directory for unit testing.  
make clean_all: clean up generated sources(by cmock) and all binaries in current directory and test directory.  

Logging:  
------------  
ERR.log (in directory term_api being invoked) - Any code failure (non-zero return value) will put a message with recording the function name and code line.   

Unit testing:
------------------------------   
Unit testing was done with cmock/unity and run under test directory.  

ubuntu->cd test  
ubuntu->ls  
ERR.log          system_test.scr  UTfuncs_runner.c       UTfuncs_ut.c  
System_test.log  UTfuncs          UTfuncs_runner_test.o  UTfuncs_ut_test.o  
ubuntu->UTfuncs  
./test/UTfuncs_ut.c:32:test_query_term_info1:PASS  
./test/UTfuncs_ut.c:54:test_query_term_info2:PASS  
./test/UTfuncs_ut.c:72:test_query_term_info3:PASS  
./test/UTfuncs_ut.c:88:test_query_term_info4:PASS  
./test/UTfuncs_ut.c:114:test_parse_json1:PASS  
./test/UTfuncs_ut.c:136:test_parse_json2:PASS  
./test/UTfuncs_ut.c:157:test_parse_json3:PASS  
./test/UTfuncs_ut.c:170:test_parse_json4:PASS  
./test/UTfuncs_ut.c:184:test_parse_json5:PASS  
./test/UTfuncs_ut.c:198:test_parse_json6:PASS  
./test/UTfuncs_ut.c:212:test_init_db:PASS  
./test/UTfuncs_ut.c:231:test_insert_db1:PASS  
./test/UTfuncs_ut.c:258:test_insert_db2:PASS  
./test/UTfuncs_ut.c:282:test_find_spare_id1:PASS  
./test/UTfuncs_ut.c:301:test_find_spare_id2:PASS  
./test/UTfuncs_ut.c:320:test_svr_process_req1:PASS  
./test/UTfuncs_ut.c:342:test_svr_process_req2:PASS  
./test/UTfuncs_ut.c:367:test_svr_process_req3:PASS  
./test/UTfuncs_ut.c:387:test_svr_process_req4:PASS  
./test/UTfuncs_ut.c:406:test_svr_process_req5:PASS  
./test/UTfuncs_ut.c:424:test_query_db1:PASS  
./test/UTfuncs_ut.c:446:test_query_db2:PASS  
./test/UTfuncs_ut.c:459:test_query_db3:PASS  
./test/UTfuncs_ut.c:474:test_query_term_list1:PASS  
./test/UTfuncs_ut.c:505:test_query_term_list2:PASS  
./test/UTfuncs_ut.c:525:test_struct2json:PASS  
  
-----------------------    
26 Tests 0 Failures 0 Ignored  
OK  
   
   
System testing (Acceptance testing):    
------------------------------------    
In a terminal window, invoke server product.  
ubuntu->cd test  
ubuntu->../term_api  

In another terminal window, run automate system testing script under test directory and verify the result.  
ubuntu->cd test  
ubuntu->system_test.scr  
Testcase1:[In inital empty DB, query assigned terminal IDs, receive {}]:PASS  
Testcase2:[Send Visa input data to server, get expected ID]:PASS  
Testcase3:[Query an assigned Visa terminal ID, get expected output data]:PASS  
Testcase4:[Send EFTPOS input data to server, get expected ID]:PASS  
Testcase5:[Query an assigned EFTPOS terminal ID, get expected output data]:PASS  
Testcase6:[Send MasterCard input data to server, get expected ID]:PASS  
Testcase7:[Query an assigned MasterCard terminal ID, get expected output data]:PASS  
Testcase8:[Send typo json data "BADTransactionType" to server, receive nothing]:PASS  
Testcase9:[Send typo json data "cardBADType" to server, receive nothing]:PASS  
Testcase10:[Query all assigned terminal IDs, get expected ID list]:PASS  
Testcase11:[Query a non-assigned terminal ID, receive {}]:PASS  
Testcase12:[Query an out-of-range terminal ID, receive nothing]:PASS  
Testcase13:[When no available terminal ID, send json input data to server and receive {}]:PASS  

--------------------------------   
13 Tests: 13 Successes 0 Failures  
OK  
NOTE: Please see ./System_test.log for test details.  


