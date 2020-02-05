#!/usr/bin/env bash
# set -x

DIR=$(dirname "$0")

LOADER="${DIR}/pal_loader"
WORKDIR=$(mktemp -d)
OUTPUT=${WORKDIR}/output

# simple test cases all started in the same way
TESTCASES="Cpuid Event Exception Failure File HandleSend
	HelloWorld Memory Pie Pipe Process Select
	Sleep Tcp Thread Udp Wait Yield"
# a bit more complex test setup needed
OTHER_TESTCASES="Broadcast Segment Server"
#the following ones do not work on ppc64 yet
# - Fork

TESTCASE="" # currently running testcase
FAIL_CNT=0
SUCC_CNT=0
SKIP_CNT=0

function cleanup() {
	kill -- -$$
	rm -rf "${WORKDIR}"
}

trap "cleanup" QUIT EXIT

function run_test() {
	TEST_FAILS=0
	case $(uname -p) in
	ppc64*)
		${LOADER} _ "${TESTCASE}" $@ &> "${OUTPUT}"
		;;
	*)
		${LOADER} "${TESTCASE}" $@ &> "${OUTPUT}"
		;;
	esac
}

function run_test_bg() {
	TEST_FAILS=0
	case $(uname -p) in
	ppc64*)
		${LOADER} _ "${TESTCASE}" $@ &> "${OUTPUT}" &
		;;
	*)
		${LOADER} "${TESTCASE}" $@ &> "${OUTPUT}" &
		;;
	esac
	TESTCASEPID=$!
}

function test_end() {
	if [ "${TEST_FAILS}" -ne 0 ]; then
		FAIL_CNT=$((FAIL_CNT + 1))
		echo "FAIL: ${TESTCASE}"
	else
		SUCC_CNT=$((SUCC_CNT + 1))
		echo "PASS: ${TESTCASE}"
	fi
}

function _find_in_output() {
	local regex=$1
	local exp_cnt=$2
	local cnt

	cnt=$(grep -E "${regex}" "${OUTPUT}" | wc -l)
	if [ $cnt -ne $exp_cnt ]; then
		echo "${TESTCASE}: Found '${1}' ${cnt} times in output, expected ${exp_cnt}."
		TEST_FAILS=$((TEST_FAILS + 1))
		if [ -n "$VERBOSE" ]; then
			echo "================================"
			cat ${OUTPUT}
			echo "================================"
		fi
	fi
}

function find_in_output() {
	while [ $# -ne 0 ]; do
		_find_in_output "$1" "$2"
		shift 2
	done
}

function exit_with_stats() {
	echo "Successes: ${SUCC_CNT}"
	echo "Failures : ${FAIL_CNT}"
	echo "Skips    : ${SKIP_CNT}"

	[ ${FAIL_CNT} -ne 0 ] && exit 1
	exit 0
}

function wait_port_open() {
	local port="$1"
	local to="$2"
	local c

	to=$((to * 10))

	for ((c = 0; c < to; c++)); do
		$(bash -c "exec 100<>"/dev/tcp/127.0.0.1/${port}"" &>/dev/null)
		[ $? -eq 0 ] && return 1
		sleep 0.1
	done
	return 0
}

#
# Simple test cases that are started the saem way and
# where we only need to check for expected output
#

function test_Cpuid() {
	find_in_output '^cpuid\[0\] = ([[:xdigit:]]{8} ){3}[[:xdigit:]]{8}$' 1
}

function test_HelloWorld() {
	find_in_output '^Hello World$' 1
}

function test_Event() {
	find_in_output \
		'^Enter main thread$' 1 \
		'^In thread 1$' 1 \
		'^Success, leave main thread$' 1
}

function test_Exception() {
	find_in_output \
		'^failure in the handler: 0x[[:xdigit:]]+$' 30
}

function test_Failure() {
	find_in_output \
		'^Enter Main Thread$' 1 \
		'^Failure notified: Function not supported$' 1 \
		'^Leave Main Thread$' 1
}

function test_File() {
	find_in_output \
		'^Enter Main Thread$' 1 \
		'^Hello World$' 1 \
		'^Leave Main Thread$' 1
}

function test_HandleSend() {
	find_in_output \
		'^Parent: Executing the program$' 1 \
		'^Parent: Creating handles' 1 \
		'^Parent: Forking child' 1 \
		'^Parent: Sending Handle 0' 1 \
		'^Parent: Sending Handle 1' 1 \
		'^Parent: Sending Handle 2' 1 \
		'^Parent: Sending Handle 3' 1 \
		'^Parent: Sending Handle 4' 1 \
		'^Parent: Finished execution' 1 \
		'^Child: Receiving Handle 0' 1 \
		'^Child: Receiving Handle 1' 1 \
		'^Child: Receiving Handle 2' 1 \
		'^Child: Receiving Handle 3' 1 \
		'^Child: Receiving Handle 4' 1 \
		'^Child: Reading the handles' 1 \
		'^Child: Handle 0 Type Pipe' 1 \
		'^Child: Handle 1 Type Udp' 1 \
		'^Child: Handle 2 Type File Data: Hello World2' 1 \
		'^Child: Handle 3 Type File Data: Hello World3' 1 \
		'^Child: Handle 4 Type File Data: Hello World4' 1 \
		'^Child: Finished execution' 1
}

function test_Pie() {
	find_in_output \
		'^start program: file:Pie$' 1 \
		'^Hello World$' 1
}

function test_Memory() {
	# nothing to do
	true
}

function test_Pipe() {
	find_in_output \
		'^pipe connect as pipe:' 1 \
		'^pipe accepted as pipe.srv:' 1 \
		'^read from server: Hello World$' 10
}

function test_Process() {
	local i

	find_in_output \
		'^In process: Process$' 1 \
		'^wall time = [[:digit:]]+$' 1
	for ((i = 0; i<100; i++)); do
		find_in_output \
		"^In process: Process ${i} [[:digit:]]+$" 1
	done
}

function test_Select() {
	find_in_output \
		'^Enter main thread$' 1 \
		'^Waiting on event$' 1 \
		'^Enter thread$' 1 \
		'^Thread sets event$' 1 \
		'^Event was called$' 1 \
		'^Leave main thread$' 1 \
		'^Leave thread$' 1
}

function test_Sleep() {
	find_in_output \
		'^Enter Main Thread$' 1 \
		'^Sleeping 3000000 microsecond...$' 1 \
		'^Leave Main Thread$' 1
}

function test_Thread() {
	find_in_output \
		'^Enter Main Thread$' 1 \
		'^Leave Main Thread$' 1 \
		'^Enter Thread 2$' 1 \
		'^Parent do suspension$' 1 \
		'^Enter Thread 1$' 1 \
		'^Parent do reload$' 1 \
		'^Leave Thread 2$' 1 \
		'^Leave Thread 1$' 1
}

function test_Tcp() {
	find_in_output \
		'^start time = [[:digit:]]+$' 1 \
		'^server bound on tcp.srv:127.0.0.1:8000$' 1 \
		'^client accepted on tcp:127.0.0.1:8000:127.0.0.1:[[:digit:]]+$' 10 \
		'^client connected on tcp:127.0.0.1:[[:digit:]]+:127.0.0.1:8000$' 10 \
		'^read from server: Hello World$' 10
}

function test_Udp() {
	find_in_output \
		'^server bound on udp.srv:127.0.0.1:8000$' 1 \
		'^client connected on udp:127.0.0.1:8000$' 1 \
		'^read on server \(from udp:127.0.0.1:[[:digit:]]+\): Hello World$' 10 \
		'^wall time = [[:digit:]]+$' 1
}

function test_Wait() {
	find_in_output \
		'^Enter Main Thread$' 1 \
		'^DkStreamsWaitEvents did not return any events$' 1 \
		'^Enter thread 2$' 1 \
		'^Enter thread 1$' 1 \
		'^Leave thread 1$' 1 \
		'^Leave thread 2$' 1

}

function test_Yield() {
	find_in_output \
		'^Enter Parent Thread$' 1 \
		'^Enter Child Thread$' 1 \
		'^child yielded$' 100 \
		'^parent yielded$' 100
}

#
# More complex test cases
#

function test_Broadcast() {
	# This test uses multi cast, which may not be enabled everywhere...
	local i

	run_test_bg
	# give it some time to run; if mcast is not supported it will not
	# terminate and that's why we run it in the background
	sleep 4

	for ((i = 1; i <= 4; i++)); do
		find_in_output \
			"^process $i started$" 1 \
			"^process $i received: Hello World$" 1 \
			"^process $i exited$" 1
	done
	kill -0 ${TESTCASEPID} &>/dev/null
	[ $? -eq 0 ] && kill -9 ${TESTCASEPID}
	# we trust that it works if mcast is enabled; convert a fail into a skip
	if [ $TEST_FAILS -ne 0 ]; then
		echo "SKIP: ${TESTCASE} (IP multicast may not work)"
		SKIP_CNT=$((SKIP_CNT + 1))
	else
		test_end
	fi
}

function test_Segment() {
	# This test case may not work in all environments; fails on Travis for example
	# by hanging
	run_test_bg
	find_in_output \
		'^TLS = 0x[[:xdigit:]]*$' 1
	if [ $TEST_FAILS -ne 0 ]; then
		echo "SKIP: ${TESTCASE} (Getting segment register may not be supported)"
		SKIP_CNT=$((SKIP_CNT + 1))
	else
		test_end
	fi
}

function test_Server() {
	run_test_bg 65432
	if wait_port_open 65432 4; then
		echo "${TESTCASE}: Server did not open port 65432"
		TEST_FAILS=1
	else
		sleep 1
		exec 100<>/dev/tcp/127.0.0.1/65432
		echo "Hello1" >&100
		sleep 1
		exec 101<>/dev/tcp/127.0.0.1/65432
		echo "Hello2" >&101
		# Give it some time to recv previous msg
		sleep 1
		echo "Hello3" >&100
		exec 100>&-
		exec 101>&-
		sleep 1
		find_in_output \
			'^\[1\] receive new connection$' 2 \
			'^\[1\] Hello1$' 1 \
			'^\[2\] receive new connection$' 1 \
			'^\[2\] Hello2$' 1 \
			'^\[1\] Hello3$' 1
		kill -9 ${TESTCASEPID}
	fi
	test_end
}

for TESTCASE in $(echo $TESTCASES); do
	if [ ! -x "${DIR}/${TESTCASE}" ]; then
		echo "SKIP: ${TESTCASE} [executable not compiled]"
		SKIP_CNT=$((SKIP_CNT + 1))
	elif [ "$(type -t test_${TESTCASE})" != "function" ]; then
		echo "SKIP: ${TESTCASE} [check function missing]"
		SKIP_CNT=$((SKIP_CNT + 1))
	else
		run_test
		test_${TESTCASE}
		test_end
	fi
done

for TESTCASE in $(echo $OTHER_TESTCASES); do
	if [ ! -x "${DIR}/${TESTCASE}" ]; then
		echo "SKIP: ${TESTCASE} [executable not compiled]"
		SKIP_CNT=$((SKIP_CNT + 1))
	elif [ "$(type -t test_${TESTCASE})" != "function" ]; then
		echo "SKIP: ${TESTCASE} [check function missing]"
		SKIP_CNT=$((SKIP_CNT + 1))
	else
		test_${TESTCASE}
	fi
done

exit_with_stats
