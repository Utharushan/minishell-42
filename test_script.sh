#!/bin/bash

echo "Testing minishell..."

# Test 1: echo "42"
echo "Test 1: echo \"42\""
echo 'echo "42"' | ./minishell

# Test 2: echo 42
echo "Test 2: echo 42"
echo 'echo 42' | ./minishell

# Test 3: export TOTO="salut"
echo "Test 3: export TOTO=\"salut\""
echo 'export TOTO="salut"' | ./minishell

# Test 4: echo $TOTO
echo "Test 4: echo \$TOTO"
echo 'echo $TOTO' | ./minishell

# Test 5: echo "    "$TOTO"   "
echo "Test 5: echo \"    \"\$TOTO\"   \""
echo 'echo "    "$TOTO"   "' | ./minishell

export TOTO=salut

echo $TOTO
echo '$TOTO'
echo "$TOTO"
echo "   "$TOTO"   "
echo '   '4TOTO'   '
echo "   '4TOTO'   "
echo '   "$TOTO"   '

echo "Tests completed." 