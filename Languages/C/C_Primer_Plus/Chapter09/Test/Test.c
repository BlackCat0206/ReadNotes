#include <stdio.h>

#define NCOLS 10
#define NROWS 10

/**
 * @brief Main function
 * @return int
 * 
 */
int main(void)
{
    printf("Hello, World!\n");

    // 设置缓冲区大小
    int buffer[NROWS * (NCOLS - 1)] = {0}; 

    // 声明输入数组
    int input_array[2 * (NROWS * NCOLS)] = {0};

    // 初始化输入数组
    init_1d_array(input_array, 2 * (NROWS * NCOLS));

    // 初始化指针指向输入数组
    int *p_input = input_array;



    return 0;
}

/**
 * @brief 对输入指定大小的一维整形数组进行按序初始化
 * @param [in] arr 指向整形数组的指针
 * @param [in] size 数组大小
 * @return void
 */
void init_1d_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = i;
    }
    return;
}

/**
 * @brief 打印标准换行符
 * @param [in] void
 * @return void
 */
void print_newline(void)
{
    printf("\n");
    return; 
}

/**
 * @brief 打印标准换页符
 * @param [in] void
 * @return void
 */
void print_formfeed(void)
{
    printf("\f");
    return; 
}

/**
 * @brief 打印输入的有符号整形数
 * @param [in] int num 输入的有符号整形数
 * @return void
 */
void print_signed_int(int num)
{
    printf("%d", num);
    return;
}

/**
 * @brief 对输入的有符号整数按规定填充进入缓冲区，若缓冲区满则打印并清空缓冲区
 * @param [in] int *buffer 指向缓冲区的指针
 * @param [in] int *p_input 指向输入整数的指针
 * @return void
 */
void fill_and_print_buffer(int *buffer, int *p_input)
{
    // 初始化指针，保存缓冲的起始位置
    int *p_buffer_start = buffer;

    // 初始化指针，保存输入数组的起始位置
    int *p_input_start = p_input;

    // 初始化一个整数，用于记录缓冲区域剩余空间
    int space_remaining = 0;

    // 初始化一个整数，用于记录每次传入缓冲区的整数个数
    int nums_to_copy = 0;

    while(p_input < (p_input_start + 2 * (NROWS * NCOLS)))
    {
        // 计算缓冲区剩余空间
        space_remaining = (NROWS * (NCOLS - 1)) - (buffer - p_buffer_start);

        if (space_remaining > 0)
        {
            // 计算每次传入缓冲区的整数个数
            nums_to_copy = (space_remaining < (buffer - p_buffer_start)) ? space_remaining : (buffer - p_buffer_start);

            // 将输入整数传入缓冲区
            memcpy(buffer, p_input, nums_to_copy * sizeof(int));
            
            // 指针偏移
            buffer += nums_to_copy;
            p_input += nums_to_copy;
        }
        else
        {
            int i , j;
            for (i = 0; i < NROWS; i++)
            {
                for (j = 0; j < (NCOLS - 1); j++)
                {
                    print_signed_int(*(buffer + j * NROWS));
                }
                print_signed_int(*(p_input));
                p_input++;
                print_newline();
            }
            print_formfeed();
            buffer = p_buffer_start;
        }
    }
    return;
}