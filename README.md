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

Source code:  
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
test/curl.scr

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
curl.scr  UTfuncs           UTfuncs_runner_test.o  UTfuncs_ut_test.o
ERR.log   UTfuncs_runner.c  UTfuncs_ut.c  
ubuntu->UTfuncs  
./test/UTfuncs_ut.c:33:test_query_term_info1:PASS  
./test/UTfuncs_ut.c:59:test_query_term_info2:PASS  
./test/UTfuncs_ut.c:83:test_query_term_info3:PASS  
./test/UTfuncs_ut.c:104:test_query_term_info4:PASS  
./test/UTfuncs_ut.c:130:test_parse_json:PASS  
./test/UTfuncs_ut.c:151:test_init_db:PASS  
./test/UTfuncs_ut.c:170:test_insert_db1:PASS  
./test/UTfuncs_ut.c:197:test_insert_db2:PASS  
./test/UTfuncs_ut.c:221:test_find_spare_id1:PASS  
./test/UTfuncs_ut.c:240:test_find_spare_id2:PASS  
./test/UTfuncs_ut.c:259:test_svr_process_req1:PASS  
./test/UTfuncs_ut.c:281:test_svr_process_req2:PASS  
./test/UTfuncs_ut.c:306:test_svr_process_req3:PASS  
./test/UTfuncs_ut.c:326:test_svr_process_req4:PASS  
./test/UTfuncs_ut.c:345:test_svr_process_req5:PASS  

-----------------------  
15 Tests 0 Failures 0 Ignored  
OK  

