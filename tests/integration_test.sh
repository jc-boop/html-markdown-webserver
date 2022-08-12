#!/bin/bash

# this variable will track the returned result number (eg 0 for success, 1 for failure) of the various commands executed
# it will be returned at the end so that, if any command fails for whatever reason, this test will say it failed
res=0

cd ../build
res=$( expr $? + $res )

./bin/server ../tests/myconfig &>/dev/null & # start the server silently and run it in the background
server_pid=$!				     # store the pid of the last run process (the server) so we can kill it at the end
res=$( expr $? + $res )
if [ $res -eq 1 ]
then
    echo "server could not start" 1>&2
fi

sleep 1s			# so that the server is up and ready for us to start querying (ran into errors without this)

# these strings will be used to construct the "answer" we wil compare the response against
str1=" /echo/ HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/"
curlv=$(curl --version | egrep -o "^curl[ ][0-9\.]+" | egrep -o "[0-9\.]+") # get the curl version number
res=$( expr $? + $res )
str2="\r\nAccept: */*\r\n"

newline="\r\n"
contentLength="Content-Length: "
contentType="\r\nContent-Type: application/x-www-form-urlencoded\r\n"

header=$str1$curlv$str2

msg="123456789"

# get request with no message, expect response with no body (only header)
method="GET"
curl -sS -o res1.txt http://localhost:8080/echo/
res=$( expr $? + $res )
echo -e -n $method$header$newline >ans1.txt # store the answer string in a file

# post request with message, expect response with header and content (body - message)
method="POST"
len=9
curl -sS -o res2.txt -d $msg http://localhost:8080/echo/
res=$( expr $? + $res )
echo -e -n $method$header$contentLength$len$contentType$newline$msg >ans2.txt # store the answer string in a file

# compare the responses against what we expected
diff -u res1.txt ans1.txt 1>&2
res=$( expr $? + $res )
diff -u res2.txt ans2.txt 1>&2
res=$( expr $? + $res )

# clean up
rm res1.txt res2.txt ans1.txt ans2.txt
res=$( expr $? + $res )

kill $server_pid
res=$( expr $? + $res )

if [ $res -eq 1 ]
then
    echo "test failed" 1>&2
else
    echo "test succeeded" 1>&2
fi

exit $res
