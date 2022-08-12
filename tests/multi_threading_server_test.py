import requests
import json
import sys, os
import subprocess
import atexit
import time
import signal

def test_multithreading_w_sleep_echo():

    if os.path.exists("../tests/log/SERVERLOG_0.log"):
        os.remove("../tests/log/SERVERLOG_0.log")

    process_run_server = subprocess.Popen(["../build/bin/server", "../config"])
    
    process_run_sleep_req = subprocess.Popen(["printf", "\'GET /sleep HTTP/1.1\r\n\r\n\'"], \
                                             stdout=subprocess.PIPE)
    outs = None
    try:
        outs = subprocess.check_output(["nc", "localhost", "8080"], 
                                        stdin=process_run_sleep_req.stdout, timeout=5)
        if outs != None:
            return False;
    except:
        process_run_sleep_req.kill()

    process_run_echo_req = subprocess.Popen(["printf", "\'GET /echo HTTP/1.1\r\n\r\nHello\'"], \
                                             stdout=subprocess.PIPE, shell=True)

    outs_1 = None
    try:
        outs_1 = subprocess.check_output(["nc", "localhost", "8080"], 
                                          stdin=process_run_echo_req.stdout, timeout=15)
        print(outs_1)
        if outs_1 == None:
            return False;
    except:
        process_run_echo_req.kill()
  
    process_run_echo_req.kill()
    process_run_sleep_req.kill()
    process_run_server.kill()


    return True;

succ = test_multithreading_w_sleep_echo()

if os.path.exists("../tests/log/SERVERLOG_0.log"):
    os.remove("../tests/log/SERVERLOG_0.log")

if succ:
    os._exit(0)
else:
    os._exit(1)

