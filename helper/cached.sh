#!/bin/bash
#
# Cache output of commands (producers)
# THIS NEEDS https://github.com/hilbix/unbuffered
#
# Instead
#	producer args.. | ..
# write
#	CACHEDMAX= cached.sh producer.. | ..
# Note that "stdin" is forwarded to "producer",
# but is not taken into account for cached data!
# Only "producer args.." defines what cache belongs where.
#
# Environment:
# CACHEDMAX=string like in "date -d string"
# To re-cache use: CACHEDMAX=$(date)
# To never expire, use: CACHEDMAX= or: unset CACHEDMAX
#
# KNOWN BUG:
#
# If you kick the cache while the program runs, things break.
# This perhaps could be fixed using some clever IO redirection magic
# (which keeps the FD open).  This way, perhaps, we could get rid
# of the lock far more early as well.
#
# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

printf -v cmd -- ' %q' "$@"

DIR="$HOME/.cache/cached"

MD5="$(md5sum <<<"$cmd")"
MD5="${MD5%% *}"
PREF="$DIR/${MD5:0:3}"
OUT="$PREF/$MD5"

OOPS() { { printf 'OOPS:'; printf ' %q' "$@"; printf '\n'; } >&2; exit 23; }
x() { "$@"; }
o() { x "$@" || OOPS exec $?: "$@"; }
i() { local e=$?; "$@"; return $e; }

[ -n "$*" ] || OOPS Usage: "$0" command args..

o mkdir -pm700 "$PREF"
exec 6>>"$OUT.cmd" || OOPS exec "6>>$OUT.cmd"

x flock 6 || OOPS cannot lock "$OUT.cmd"

cached=true
if	! cmp -s -- - "$OUT.cmd" <<< "$cmd"
then
	o echo "$cmd" >"$OUT.cmd" # for stability not: >&6
	cached=false

elif	[ ! -f "$OUT.out" ]
then
	cached=false

elif	[ -n "$CACHEDMAX" ] &&
	rel="$(date +%s -d "$CACHEDMAX")" &&
	dot="$(date +%s --reference "$OUT.out" 2>/dev/null)" &&
	now="$(date +%s)" &&
	(( now+now >= rel+dot ))
then
	# expired: dot..now >= now..rel
	# gives us: now-dot >= rel-now
	# which is: now+now >= rel+dot
	cached=false
fi

$cached ||
if	# unbufferd (as invoked this way):
	# - forks the command ("$@"),
	# - catches the command's stderr (-i2) and outputs this to stdout,
	# - also writes command's stderr to "$OUT.err" (-a"$OUT.err"),
	# - forwards command's stdout to stderr (due to -i2)
	# - and finally returns the command's return status.
	# stdout (command's stderr) then is sent to stderr again
	# while stderr (command's stdout) is written to $OUT.tmp
	# Also the lock file descriptor 6 is not exposed to the command.
	# Any questions? Yes: How to do this with pure shell, please? ;)
	unbuffered -i2 -a"$OUT.err" -- "$@" >&2 2>"$OUT.tmp" 6<&-
then
	o mv -f "$OUT.tmp" "$OUT.out"
else
	i rm -f "$OUT.tmp"	# throw away newly cached data
	exit	# thanks to i() $? still is what came from the command
fi

# Output file and drop lock after opening the file
exec cat <"$OUT.out" 6<&-

