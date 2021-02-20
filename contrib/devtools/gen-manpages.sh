#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

EARTHCOIND=${EARTHCOIND:-$BINDIR/earthcoind}
EARTHCOINCLI=${EARTHCOINCLI:-$BINDIR/earthcoin-cli}
EARTHCOINTX=${EARTHCOINTX:-$BINDIR/earthcoin-tx}
EARTHCOINQT=${EARTHCOINQT:-$BINDIR/qt/earthcoin-qt}

[ ! -x $EARTHCOIND ] && echo "$EARTHCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
EACVER=($($EARTHCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for earthcoind if --version-string is not set,
# but has different outcomes for earthcoin-qt and earthcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$EARTHCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $EARTHCOIND $EARTHCOINCLI $EARTHCOINTX $EARTHCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${EACVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${EACVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
