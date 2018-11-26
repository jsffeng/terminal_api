Initial Development environment (Sep 16,2018):
--------------------------
Linux Version: ubuntu 14.04.5  
libmicrohttpd version: 0.9.44  
cJSON Libarary - downloaded lastest version from https://github.com/DaveGamble/cJSON on Sep 16, 2018, see ../Lib/cJSON.install  

Current Development environment (Since Sep 21,2018):
--------------------------
Linux Version: ubuntu 16.04.01    
libmicrohttpd version: 0.9.44  
cJSON Libarary - downloaded lastest version from https://github.com/DaveGamble/cJSON on Sep 21, 2018, see ../Lib/cJSON.install  

Install package:  
----------------
sudo apt install libmicrohttpd-dev  
sudo apt install curl  
complied and install cJSON  
  
Source code:
-------------------------
tar -xzvf term_api.tar.gz  
term_api/  
term_api/test/  
term_api/test/test_func.c  
term_api/test/makefile  
term_api/test/curl.scr  
term_api/req_handler.c  
term_api/makefile  
term_api/term_svr.c  
term_api/defines.h  
term_api/req_handler.h  
term_api/term_svr.h  

 ubuntu->ls  
defines.h  makefile  req_handler.c  req_handler.h  term_svr.c  term_svr.h  test
 ubuntu->make build  
gcc -c  -I/usr/include  -I/usr/include/cjson ./req_handler.c ./term_svr.c
gcc  -I/usr/include  -I/usr/include/cjson -o term_api ./req_handler.o ./term_svr.o  -L/usr/lib  -lmicrohttpd  -lcjson  


end2end test on with term_api running on this terminal, run curl on another terminal.
-------------------------------------------------------------
 ubuntu01->cd test
 ubuntu01->ls
curl.scr  makefile  test_func.c
 ubuntu01->bash -x curl.scr  
+ curl -X POST -d 'json={"TransactionType":"Credit","cardType":"Visa"}' http://localhost:8888/terminals  
16+ echo  
  
+ sleep 1  
+ curl -X GET http://localhost:8888/terminals/15  
{"terminalID":15;"TransactionType":"Credit","cardType":"Visa"}+ sleep 1  
+ echo  
  
+ curl -X GET http://localhost:8888/terminals/35  
{"terminalID":35;"TransactionType":"Credit","cardType":"Visa"}+ sleep 1  
+ echo  
  
+ curl -X GET http://localhost:8888/terminals   
{4 8 19 20 40}+ sleep 1  
+ echo  

unit/fucntion test(take parse_json() as example): 
----------------------------------------------------------------
 ubuntu->ls  
defines.h  req_handler.c  req_handler.o  term_svr.c  term_svr.o
makefile   req_handler.h  term_api       term_svr.h  test  
 ubuntu->cd test  
 ubuntu->ls  
curl.scr  makefile  test_func.c  
 ubuntu->make test  
gcc -c  -I/usr/include  -I/usr/include/cjson ../req_handler.c  
ar rcs -o ../libfunc.a req_handler.o  
gcc  -I/usr/include  -I/usr/include/cjson -I/usr/include/cjson -o test_func\  
	 ./test_func.c ../libfunc.a -lcjson  
 ubuntu->test_func  
Test of function parse_json()  
   
cardType is "Visa"  
TransactionType is "Credit"  
  
  
Clean up binaries  
-------------------------------------------------
 ubuntu->ls  
defines.h  makefile  req_handler.c  req_handler.h  term_svr.c  term_svr.h  test  
 ubuntu->  
 ubuntu->make clean  
rm -rf ./req_handler.o ./term_svr.o term_api ./test/test_func.o ./test/req_handler.o ./test/test_func libfunc.a  
 ubuntu->wc -l *
     37 defines.h  
     51 makefile  
    123 req_handler.c  
     11 req_handler.h  
    233 term_svr.c  
     18 term_svr.h  



