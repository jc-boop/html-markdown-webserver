#!/bin/bash

# updates the res variable with the success/fail value of the previous command (0 is a success)
function update_res {
    res=$( expr $? + $res )
}

# checks the value of res, assumes the command to run the server was executed before calling
function server_check {
    if [ $res -eq 1 ]
    then
	echo "server could not start" 1>&2
    fi
}

# tests the server's response to different kill signals
# takes the kill signal number as an optional argument (default is SIGTERM)
# the function starts the server and then kills it with the specified signal
function signal_test {
    ../bin/server ../../tests/myconfig &>$file &
    server_pid=$!
    update_res
    server_check

    sleep 1s
    
    if [ $# -eq 1 ]
    then
	kill -$1 $server_pid
	update_res
    else
	kill $server_pid
	update_res
    fi
}

# checks if the kill signal event was logged
# takes the kill signal name as argument 1 and the log file name as argument 2
# searches the log for the kill signal name (since the code logs the name)
function signal_check {
    count=$( cat $2 | grep "$1" | wc -l )
    update_res

    if [ $count -ne 1 ]
    then
	echo "$1 test, reading from $2 failed"
	exit 1
    fi
}


res=0
file="console_log_test.txt"

cd ../build
update_res
mkdir test_dir			# make test directory so as not to stomp the other logs
update_res
cd test_dir
update_res

# test the SIGTERM signal
signal_test
signal_check "SIGTERM" $file

# test the SIGINT signal
SIGINT=2
signal_test $SIGINT
signal_check "SIGINT" $file


# clean up
cd ..
update_res
rm -r test_dir
update_res


if [ $res -eq 1 ]
then
    echo "test failed" 1>&2
else
    echo "test succeeded" 1>&2
fi

exit $res
