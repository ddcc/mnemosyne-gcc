#!/bin/bash

set -e

FILENAME="vacation"
while [ -f "${FILENAME}.log" ]; do
  FILENAME+="0"
done
FILENAME+=".log"

numactl --membind 1 --cpunodebind 1 sh -c "./build/bench/stamp-kozy/vacation/vacation -c$1 -r4000000 -t2000000 -n1 -q80 -u99 -e1" > "${FILENAME}" 2>&1
cat mtm.stats >> "${FILENAME}"
rm mtm.stats
