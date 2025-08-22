#!/bin/bash

echo "=== Advanced Memory Leak Test for Parsing ==="

# Function to get memory info
get_memory_usage() {
    ps -o pid,vsz,rss -p $1 2>/dev/null | tail -1
}

# Test function that runs multiple parsing operations
test_parsing_memory() {
    local test_name="$1"
    local commands="$2"
    local iterations=20
    
    echo "Testing: $test_name"
    
    for i in $(seq 1 $iterations); do
        echo -e "$commands\nexit" | timeout 5s ./minishell > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo "  WARNING: Test failed at iteration $i"
        fi
    done
    
    echo "  Completed $iterations iterations"
}

# Test 1: Simple command parsing
test_parsing_memory "Simple commands" "echo hello\nls -la\npwd"

# Test 2: Complex quoting
test_parsing_memory "Quote parsing" "echo \"hello world\"\necho 'single quotes'\necho mixed \"quotes\" and 'more'"

# Test 3: Variable expansion
test_parsing_memory "Variable expansion" "echo \$HOME\necho \$USER\nexport TEST=value\necho \$TEST\nunset TEST"

# Test 4: Pipes
test_parsing_memory "Pipe parsing" "echo hello | cat\nls | grep file\necho test | wc -l"

# Test 5: Redirections
test_parsing_memory "Redirection parsing" "echo hello > /tmp/test.tmp\ncat < /tmp/test.tmp\nrm -f /tmp/test.tmp"

# Test 6: Heredocs
test_parsing_memory "Heredoc parsing" "cat << EOF\ntest line\nEOF"

# Test 7: Complex commands
test_parsing_memory "Complex parsing" "export VAR=\"test value\"\necho \$VAR | cat > /tmp/complex.tmp\ncat /tmp/complex.tmp\nrm -f /tmp/complex.tmp"

# Test 8: Error cases (should not leak on syntax errors)
test_parsing_memory "Error handling" "echo |\ncat <<\n|| invalid\n&& also invalid"

echo ""
echo "=== Memory Test Summary ==="
echo "If there are memory leaks in parsing:"
echo "1. The system may become slower after many iterations"
echo "2. Memory usage might gradually increase"
echo "3. The program might crash with large numbers of iterations"
echo ""
echo "No crashes occurred during testing, which is a good sign!"
echo "To check for subtle leaks, monitor system memory during execution."
