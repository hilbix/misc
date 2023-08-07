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

STDERR() { { printf %q "$1"; printf ' %q' "${@:2}"; printf '\n'; } >&2; }
OOPS() { STDERR OOPS: "$@"; exit 23; }
x() { "$@"; }
o() { x "$@" || OOPS exec $?: "$@"; }
i() { local e=$?; "$@"; return $e; }

[ -n "$*" ] || OOPS Usage: "$0" command args..

DEBUG() { [ -z "$CACHED_DEBUG" ] || STDERR CACHED: "$@"; }

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

# Output file and drop lock after opening the file
$cached && i DEBUG cached: "$@" && exec cat <"$OUT.out" 6<&-

# unbufferd (as invoked this way):
# - forks the command ("$@"),
# - inner catches the command's stderr (-i2) to $OUT.err
# - inner forwards command's stdout to stderr (due to -i2)
# - outer catches the command's stdin  (-i2) to $OUT.tmp
# - and finally both return the command's return status.
# Also the lock file descriptor 6 is not exposed to the command.
# Any questions? Yes: How to do this with pure shell, please? ;)
if	unbuffered -i2 -a"$OUT.tmp" unbuffered -i2 -a"$OUT.err" -- "$@" 6<&-
then
	o mv -f "$OUT.tmp" "$OUT.out"
	i DEBUG cache ok: "$@"
else
	i DEBUG cache fail $?: "$@"
	i rm -f "$OUT.tmp"	# throw away newly cached data
fi
exit	# thanks to i() $? still is what came from the command

