#!/bin/bash

set -e

if [ $# -ne 4 ]; then
  echo "$0 <queries/tx> <% queries> <% user> <# threads>"
  exit
fi

for a in "alps" "pmdk"; do
  CMD="./build/bench/stamp-kozy/vacation/vacation -n${1} -q${2} -u${3} -r1048576 -t4194304 -c${4}"
  echo "Executing: ${CMD}"

  rm -fr /pmem-fs/*
#  tar -xf pmem-fs-${a}-1048576.tar.gz -C /pmem-fs/

  FILENAME="../vacation-${a}-${1}-${2}-${3}-${4}.log"
  hostname > "${FILENAME}"
  cd "${a}"
  numactl --membind 1 --cpunodebind 1 sh -c "PMEM_MMAP_HINT=0x7ff000000000 ${CMD}" >> "${FILENAME}" 2>&1
  cat mtm.stats >> "${FILENAME}"
  rm mtm.stats
  cd ..
done
