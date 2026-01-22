#include <stdio.h>
#include <string.h>

int main() {
    const char *str = "aabbcc123def";
    const char *group1 = "abc";   // 包含'a','b','c'
    const char *group2 = "abec12"; // 包含'a','b','c','1','2'
    
    // 计算str中前缀包含在group1中的长度
    size_t len1 = strspn(str, group1);
    printf("前缀包含在\"%s\"中的长度: %zu\n", group1, len1); // 输出3（'a','b','c'）
    
    // 计算str中前缀包含在group2中的长度
    size_t len2 = strspn(str, group2);
    printf("前缀包含在\"%s\"中的长度: %zu\n", group2, len2); // 输出5（'a','b','c','1','2'）
    
    return 0;
}