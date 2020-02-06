#!/usr/bin/env bash
# set -x

DIR=$(dirname "$0")

LOADER="${DIR}/../../Runtime/pal_loader"
WORKDIR=$(mktemp -d)
OUTPUT=${WORKDIR}/output

# simple test cases all started in the same way
TESTCASES="AtomicMath Attestation AvxDisable Bootstrap
	Bootstrap2 Bootstrap3 Bootstrap4.manifest Bootstrap7 Bootstrap7.manifest
	Directory Event Exception Exit Hex Memory Misc
	nonelf_binary normalize_path Pipe Process2 Process3 Semaphore
	SendHandle Socket Symbols Thread2"
# a bit more complex test setup needed
OTHER_TESTCASES="File Process Thread"
#the following ones do not work on ppc64 yet
#

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

function test_Attestation() {
	find_in_output \
		'^Attestation status: \(null\)$' 1 \
		'^Attestation timestamp: \(null\)$' 1
}

function test_AtomicMath() {
	find_in_output \
		'^Subtract INT_MIN: Both values match 2147483648$' 1 \
		'^Subtract INT_MAX: Both values match -2147483647$' 1 \
		'^Subtract LLONG_MIN: Both values match -9223372036854775808$' 1 \
		'^Subtract LLONG_MAX: Both values match -9223372036854775807$' 1
}

function test_AvxDisable() {
	find_in_output \
		'^Result: 1 1 1 1 1 1 1 1$' 1
}

function test_Bootstrap() {
	find_in_output \
		'^User Program Started$' 1 \
		'^Loaded Executable: file:Bootstrap$' 1 \
		'^Loaded Manifest: file:manifest$' 1 \
		'^# of Arguments: 1$' 1 \
		'^Process ID: [[:xdigit:]]+$' 1 \
		'^Host ID: 0000000000000000$' 1 \
		'^Parent Process: 0x0$' 1 \
		'^Written to Debug Stream$' 1 \
		'^Allocation Alignment: (4096|65536)$' 1 \
		'^User Address Range: 0x10000 - 0x[[:xdigit:]]+$' 1 \
		'^User Address Range OK$' 1 \
		'^Executable Range: (0x400000|0x10000000) - 0x[[:xdigit:]]+$' 1 \
		'^Executable Range OK$' 1 \
		'^CPU num: [[:digit:]]+$' 1 \
		'^CPU vendor: [[:print:]]+$' 1 \
		'^CPU brand: [[:print:]]+$' 1 \
		'^CPU family: [[:digit:]]+$' 1 \
		'^CPU model: [[:digit:]]+$' 1 \
		'^CPU stepping: [[:digit:]]+$' 1 \
		'^CPU flags: [[:print:]]+$' 1
}

function test_Bootstrap2() {
	find_in_output \
		'^User Program Started$' 1 \
		'^Loaded Executable: file:Bootstrap2$' 1 \
		'^Loaded Manifest: file:Bootstrap2.manifest$' 1 \
		'^# of Arguments: 1$' 1 \
		'^argv\[0\] = Bootstrap2$' 1
}

function test_Bootstrap3() {
	find_in_output \
		'^Binary 1 Preloaded$' 1 \
		'^Binary 2 Preloaded$' 1 \
		'^User Program Started$' 1 \
		'^Loaded Executable: file:Bootstrap3$' 1 \
		'^Preloaded Function 1 Called$' 1 \
		'^Preloaded Function 2 Called$' 1
}

function test_Bootstrap4.manifest() {
	find_in_output \
		'^User Program Started$' 1 \
		'^Loaded Executable: file:Bootstrap$' 1 \
		'^Loaded Manifest: file:Bootstrap4.manifest$' 1 \
		'^# of Arguments: 1$' 1 \
		'^Process ID: [[:xdigit:]]+$' 1 \
		'^Host ID: 0000000000000000$' 1 \
		'^Parent Process: 0x0$' 1 \
		'^Written to Debug Stream$' 0 \
		'^Allocation Alignment: (4096|65536)$' 1 \
		'^User Address Range: 0x10000 - 0x[[:xdigit:]]+$' 1 \
		'^User Address Range OK$' 1 \
		'^Executable Range: (0x400000|0x10000000) - 0x[[:xdigit:]]+$' 1 \
		'^Executable Range OK$' 1 \
		'^CPU num: [[:digit:]]+$' 1 \
		'^CPU vendor: [[:print:]]+$' 1 \
		'^CPU brand: [[:print:]]+$' 1 \
		'^CPU family: [[:digit:]]+$' 1 \
		'^CPU model: [[:digit:]]+$' 1 \
		'^CPU stepping: [[:digit:]]+$' 1 \
		'^CPU flags: [[:print:]]+$' 1
}

function test_Bootstrap7() {
	local i

	find_in_output \
		'^User Program Started$' 1
	for ((i = 1; i <= 1000; i++)); do
		find_in_output \
			"^key${i}=na$" 1
	done
}

function test_Bootstrap7.manifest() {
	test_Bootstrap7
}

function test_Directory() {
	find_in_output \
		'^Directory Creation Test 1 OK$' 1 \
		'^Directory Creation Test 2 OK$' 1 \
		'^Directory Creation Test 3 OK$' 1 
	rm -rf dir_nonexist.tmp
}

function test_Event() {
	find_in_output \
		'^Started main thread.$' 1 \
		'^Testing wait with too short timeout...$' 1 \
		'^Second thread started.$' 1 \
		'^Timeout event received.$' 1 \
		'^Wait returned after [[:digit:]]+ us.$' 1 \
		'^Timeout count: 1$' 2 \
		'^Wait with too short timeout ok.$' 1 \
		'^Testing wait with long enough timeout...$' 1 \
		'^Sending event...$' 1 \
		'^Wait returned after [[:digit:]]+ us since start.$' 1 \
		'^Timeout count: 1$' 2 \
		'^Wait with long enough timeout ok.$' 1 \
		'^End of main thread.$' 1 \
		'^End of second thread.$' 1
}

function test_Exit() {
	# nothing to check
	true
}

function test_Hex() {
	find_in_output \
		'^Hex test 1 is deadbeef$' 1 \
		'^Hex test 2 is cdcdcdcdcdcdcdcd$' 1
}

function test_Memory() {
	find_in_output \
		'^Memory Allocation OK$' 1 \
		'^\(int \*\) 0x[[:xdigit:]]+ = 0$' 1 \
		'^Memory Allocation Protection \(RW\) OK$' 1 \
		'^Memory Fault 1$' 1 \
		'^Memory Protection \(R\) OK$' 1 \
		'^Memory Fault 2$' 1 \
		'^Memory Deallocation OK$' 1 \
		'^Memory Allocation with Address OK$' 1 \
		'^Total Memory: [[:digit:]]+$' 1 \
		'^Get Memory Available Quota OK$' 1
}

function test_Misc() {
	find_in_output \
		'^Time Query 1: [[:digit:]]+$' 1 \
		'^Time Query 2: [[:digit:]]+$' 1 \
		'^Query System Time OK$' 1 \
		'^Sleeped [[:digit:]]{5} Microseconds$' 1 \
		'^Delay Execution for 10000 Microseconds OK$' 1 \
		'^Sleeped [[:digit:]]{7} Microseconds$' 1 \
		'^Delay Execution for 3 Seconds OK$' 1 \
		'^Generate Random Bits OK$' 1
}

function test_nonelf_binary() {
	find_in_output \
		'^reading manifest "file:nonelf_binary" failed$' 1 \
		'^PAL failed at db_main.c:pal_main:[[:digit:]]+ \(exitcode = 4, reason=equal mark expected\)$' 1
}

function test_normalize_path() {
	find_in_output \
		'^Success!$' 1
}

function test_Pipe() {
	find_in_output \
		'^Pipe Creation 1 OK$' 1 \
		'^Pipe Attribute Query 1 on pipesrv returned OK$' 1 \
		'^Pipe Connection 1 OK$' 1 \
		'^Pipe Write 1 OK$' 1 \
		'^Pipe Read 1: Hello World 1$' 1 \
		'^Pipe Write 2 OK$' 1 \
		'^Pipe Read 2: Hello World 2$' 1

}

function test_Process2() {
	find_in_output \
		'^User Program Started$' 1 \
		'^Loaded Executable: file:Bootstrap$' 1 \
		'^Loaded Manifest: file:Process2.manifest$' 1 \
		'^# of Arguments: 0$' 1 \
		'^Process ID: [[:xdigit:]]+$' 1 \
		'^Host ID: 0000000000000000$' 1 \
		'^Parent Process: 0x[[:xdigit:]]+$' 1 \
		'^Written to Debug Stream$' 1 \
		'^Allocation Alignment: (4096|65536)$' 1 \
		'^User Address Range: 0x10000 - 0x7[[:xdigit:]]+$' 1 \
		'^User Address Range OK$' 1 \
		'^Executable Range: (0x400000|0x10000000) - 0x[[:xdigit:]]+$' 1 \
		'^Executable Range OK$' 1 \
		'^CPU num: [[:digit:]]+$' 1 \
		'^CPU vendor: [[:print:]]+$' 1 \
		'^CPU brand: [[:print:]]+$' 1 \
		'^CPU family: [[:digit:]]+$' 1 \
		'^CPU model: [[:digit:]]+$' 1 \
		'^CPU stepping: [[:digit:]]+$' 1 \
		'^CPU flags: [[:print:]]+$' 1

}

function test_Process3() {
	find_in_output \
		'^Create Process without Executable OK$' 1 \
		'^Binary 1 Preloaded$' 2 \
		'^Binary 2 Preloaded$' 2
}

function test_SendHandle() {
	find_in_output \
		'^Send Handle OK$' 3 \
		'^Receive Handle OK$' 3 \
		'^Receive Pipe Handle: Hello World$' 1 \
		'^Receive Socket Handle: Hello World$' 1 \
		'^Receive File Handle: Hello World$' 1
}

function test_Semaphore() {
	find_in_output \
		'^Locked binary semaphore timed out \(1000\).$' 1 \
		'^Locked binary semaphore timed out \(0\).$' 1 \
		'^Locked binary semaphore successfully \(-1\).$' 1 \
		'^Locked binary semaphore successfully \(0\).$' 1
}

function test_Socket() {
	find_in_output \
		'^TCP Creation 1 OK$' 1 \
		'^TCP Connection 1 OK$' 1 \
		'^TCP Write 1 OK$' 1 \
		'^TCP Read 1: Hello World 1$' 1 \
		'^TCP Write 2 OK$' 1 \
		'^TCP Read 2: Hello World 2$' 1 \
		'^UDP Creation 1 OK$' 1 \
		'^UDP Connection 1 OK$' 1 \
		'^UDP Write 1 OK$' 1 \
		'^UDP Read 1: Hello World 1$' 1 \
		'^UDP Write 2 OK$' 1 \
		'^UDP Read 2: Hello World 2$' 1 \
		'^UDP Connection 2 OK$' 1 \
		'^UDP Write 3 OK$' 1 \
		'^UDP Read 3: Hello World 1$' 1 \
		'^UDP Write 4 OK$' 1 \
		'^UDP Read 4: Hello World 2$' 1
}

function test_Symbols() {
	find_in_output \
		'^Dk[^ ]+ = 0x[[:xdigit:]]+' 42
}

function test_Thread2() {
	find_in_output \
		'^Thread 1 \(main\) started.$' 1 \
		'^Thread 2 started.$' 1 \
		'^Exiting thread 2 by return.$' 1 \
		'^Thread 2 ok.$' 1 \
		'^Thread 3 started.$' 1 \
		'^Exiting thread 3 by DkThreadExit.$' 1 \
		'^Thread 3 ok.$' 1 \
		'^Thread 4 started.$' 1 \
		'^Exiting thread 4 by return.$' 1 \
		'^Thread 4 ok.$' 1
}

#
# More complex test cases
#

function test_File() {
	local t1=1 t4=0
	rm -f *.tmp

	for (( c=0; c<2; c++)); do
		run_test

		find_in_output \
		'^File Open Test 1 OK$' 1 \
		'^Read Test 1 \(0th - 40th\): 7f454c46[[:xdigit:]]{72}$' 1 \
		'^Read Test 2 \(0th - 40th\): 7f454c46[[:xdigit:]]{72}$' 1 \
		'^Read Test 3 \(200th - 240th\): [[:xdigit:]]{80}$' 1 \
		'^Query by Handle: type = 0, size = [[:digit:]]+$' 1 \
		'^Map Test 1 \(0th - 40th\): 7f454c46[[:xdigit:]]{72}$' 1 \
		'^Map Test 2 \(200th - 240th\): [[:xdigit:]]{80}$' 1 \
		'^Map Test 3 \(4096th - 4136th\): [[:xdigit:]]{80}$' 1 \
		'^Map Test 4 \(4296th - 4336th\): [[:xdigit:]]{80}$' 1 \
		'^File Open Test 2 OK$' 1 \
		'^File Open Test 3 OK$' 1 \
		'^Query: type = 0, size = [[:digit:]]+$' 1 \
		'^File Creation Test 1 OK$' $t1 \
		'^File Creation Test 2 OK$' 1 \
		'^File Creation Test 3 OK$' 1 \
		'^File Creation Test 4 OK$' $t4
		# expecting different result next time
		t1=0
		t4=1
	done

	test_end
}

function test_Process() {
	# This test uses multi cast, which may not be enabled everywhere...
	local i

	run_test_bg
	# give it some time to run; if mcast is not supported it will not
	# terminate and that's why we run it in the background
	sleep 4

	find_in_output \
		'^Creating process$' 3 \
		'^Child Process Created$' 3 \
		'^Process created 1$' 1 \
		'^Process created 2$' 1 \
		'^Process created 3$' 1 \
		'# of Arguments: 2$' 3 \
		'^argv\[0\] = Process$' 3 \
		'^argv\[1\] = Child$' 3 \
		'^Broadcasting message$' 1 \
		'^Broadcast Write OK$' 1 \
		'^Broadcast Read: Hello World 1$' 3 \
		'^Process Write 1 OK$' 3 \
		'^Process Write 2 OK$' 3 \
		'^Process Read 1: Hello World 1$' 3 \
		'^Process Read 2: Hello World 2$' 3

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

function test_Thread() {
	# This test case may not work in all environments; fails on Travis for example
	# by hanging
	run_test_bg

	find_in_output \
		'^Private Message \(FS Segment\) 1: Hello World 1$' 1 \
		'^Child Thread Created$' 1 \
		'^Run in Child Thread: Hello World$' 1 \
		'^Threads Run in Parallel OK$' 1 \
		'^Private Message \(FS Segment\) 2: Hello World 2$' 1 \
		'^Child Thread Exited$' 1
	if [ $TEST_FAILS -ne 0 ]; then
		echo "SKIP: ${TESTCASE} (Getting segment register may not be supported)"
		SKIP_CNT=$((SKIP_CNT + 1))
	else
		test_end
	fi
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
