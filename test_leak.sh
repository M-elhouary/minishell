#!/bin/bash
cd /home/sel-abbo/Downloads/minishell
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo "<<o" | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell 2>&1 | grep -E "(definitely lost|indirectly lost|possibly lost|still reachable|ERROR SUMMARY)"
