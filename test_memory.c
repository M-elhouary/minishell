#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Testing memory leak fix...\n");
    
    // Run the command and capture output
    system("cd /home/sel-abbo/Downloads/minishell && make clean >/dev/null 2>&1");
    system("cd /home/sel-abbo/Downloads/minishell && make >/dev/null 2>&1");
    
    printf("Running valgrind test...\n");
    int result = system("cd /home/sel-abbo/Downloads/minishell && echo '<<o' | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell 2>&1 | grep -E '(definitely lost|indirectly lost|still reachable)'");
    
    printf("Test completed with result: %d\n", result);
    return 0;
}
