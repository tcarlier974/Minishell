#!/usr/bin/env bash
make re
valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --suppressions=valgrind.supp --trace-children-skip='*/usr/bin/*,*/bin/*' ./minishell