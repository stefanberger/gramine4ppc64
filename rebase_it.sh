#!/usr/bin/env bash

# Script for rebasing daily powerpc64 branches on the upstream/master
# branch one patch after another

headback=1
curbranch=$(git rev-parse --abbrev-ref HEAD)
upstream_master=upstream/master

git fetch upstream

#set -x
# first determine what the last commit was that the current branch
# was previously rebased on
while :;
do
	commit=$(git log "${upstream_master}" --format="%h" -${headback} |
		tail -n1)
	t="$(git branch "${curbranch}" --contains ${commit})"
	if [ -n "$t" ]; then
		break
	fi
	headback=$((headback + 1))
done

# Now run a full test cycle with the last commit from the upstream/master
# branch that this branch already has; then start rebasing and test again
while :;
do
	commit=$(git log "${upstream_master}" --format="%h" -${headback} |
		tail -n1)
	dt=$(git log "${upstream_master}" --format="%ci" -${headback} |
	     tail -n1 | cut -d " " -f1 | tr -d '-')
	echo "Next commit to rebase on: $commit  $dt"
	git submodule update --init
	curbranch=$(git rev-parse --abbrev-ref HEAD)
	newbranch="powerpc64.${dt}"
	needpush=0
	if [ "${curbranch}" != "${newbranch}" ]; then
		git branch "${newbranch}" || { exit 1; }
		git checkout "${newbranch}" || { exit 1; }
		needpush=1
	else
		if [ -n "$(git diff "github/${newbranch}")" ]; then
			needpush=1
		fi
	fi
	git rebase ${commit} || { exit 1; }
	curbranch=$(git rev-parse --abbrev-ref HEAD)
	if [ "${curbranch}" != "${newbranch}" ]; then
		exit 1
	fi
	make -j$(nproc) DEBUG=1 || { exit 1; }
	./run_tests0 || { exit 1; }
	./run_tests1 || { exit 1; }
	./run_tests2 || { exit 1; }

	if [ $needpush -ne 0 ]; then
		# loop in case user needs to enter password
		# but the connection already timed out
		while :; do
			git push -f github
			if [ $? -eq 0 ]; then
				break
			fi
		done
	fi

	# next loop we start with the actual rebasing
	headback=$((headback - 1))
	if [ ${headback} -eq 0 ]; then
		break
	fi
done

exit 0
