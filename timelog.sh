#!/bin/bash

echo "Commits:"
git log --since="last week" "--pretty=format:%s" | grep -E ".*\+([0-9]*(\.[0-9]*)?)h"

echo
echo -n "Total: "
git log --since="last week" "--pretty=format:%s" | sed -rn "s/.*\+([0-9]*(\.[0-9]*)?)h/\1/ p" | paste -sd+ | bc
