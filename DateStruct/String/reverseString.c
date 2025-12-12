/***
 * @file reverseString.c
 * @brief LeetCode problem 151. Reverse Words in a String
 * @author Lee
 * @date 2025-12-11
 * 
 * 
 */

#include <stdio.h>
#include <string.h>

/**
 * @brief 清除输入字符中多余的空格
 * @param [in] s 输入字符串
 * @param [in] size 字符串的长度
 * @return 空
 * 
 */
void cleanBlock(char* s, int size)
{
    if (0 >= size || NULL == s) {
        return;
    }

    int fast = 0;
    int slow = 0;

    // 去除前导空格
    while (fast < size && s[fast] == ' ') {
        fast++;
    }

    // 清除多余空格
    for (; fast < size; ++fast){
        if (' ' != s[fast] || (fast >0 && ' ' != s[fast + 1])) {
            s[slow++] = s[fast];
        }
    }

    // 去除尾部空格
    if (slow > 0 && s[slow - 1] == ' ') {
        slow--;
    }
    s[slow] = '\0'; // 结束字符串
    return;
}


/**
 * @brief 反转字符串
 * @param [in] s 输入字符串
 * @param [in] 字符串开始位置
 * @param [in] 字符串结束位置(不包含'/0')
 * @return 空
 */
void reverseString(char* s, int begin, int end)
{
    if (NULL == s || end <= 0 || begin >= end){
        return;
    }

    int left = begin;
    int right = end;
    while (left < right) 
    {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;

        left++;
        right--;
    }
    return;
}

/***
 * @brief 反转字符中英文字母的大小写
 * @param [in] s 输入字符串
 * @param [in] size 字符串的长度
 * @return 空
 */
void reverseCase(char* s, int size)
{
    if (NULL == s || size <= 0) {
        return;
    }

    for (int i = 0; i < size; ++i)
    {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
        } else if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
    return;
}

/**
 * @brief 反转字符串中的单词
 * @param [in] s 输入字符串
 * @param [in] size 字符串的长度(不包含'\0')
 * @return 空
 */
void reverseWords(char* s, int size)
{
    if (NULL == s || size <= 0) {
        return;
    }

    // 清除多余空格
    cleanBlock(s, size);
    int newSize = strlen(s);

    reverseCase(s, newSize); // 反转大小写

    // 反转整个字符串
    reverseString(s, 0, newSize - 1);

    int start = 0;
    int i = 0;
    for (i = 0; i < newSize; ++i)
    {
        if (' ' == s[i])
        {
            reverseString(s, start, i - 1);
            start = i + 1;
        }
    }

    reverseString(s, start, newSize - 1); // 反转最后一个单词
    return;
}


/**
 * @brief 主函数
 * @param [in] argc 参数个数
 * @param [in] argv 参数列表
 * @return 0 成功
 */
int main(int argc, char* argv[])
{
    char s[] = " h i";
    int size = sizeof(s) / sizeof(s[0]) - 1; // 减去结尾的 '\0'

    reverseWords(s, size);
    printf("Result:[%s]\n", s);

    return 0;
}
