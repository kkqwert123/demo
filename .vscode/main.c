#include <stdio.h>
#include <math.h>

int main() {
    double num1, num2;
    double sum, difference, product, quotient, power, sqrt1, sqrt2;

    // 提示用户输入两个数
    printf("请输入第一个数: ");
    scanf("%lf", &num1);

    printf("请输入第二个数: ");
    scanf("%lf", &num2);

    // 计算和、差、积和商
    sum = num1 + num2;
    difference = num1 - num2;
    product = num1 * num2;

    // 检查除数是否为零
    if (num2 != 0) {
        quotient = num1 / num2;
    } else {
        printf("错误：除数不能为零。\n");
        return 1;
    }

    // 计算幂
    power = pow(num1, num2);

    // 计算平方根
    if (num1 >= 0) {
        sqrt1 = sqrt(num1);
    } else {
        printf("错误：无法计算负数的平方根。\n");
        return 1;
    }

    if (num2 >= 0) {
        sqrt2 = sqrt(num2);
    } else {
        printf("错误：无法计算负数的平方根。\n");
        return 1;
    }

    // 输出结果
    printf("和: %.2lf\n", sum);
    printf("差: %.2lf\n", difference);
    printf("积: %.2lf\n", product);
    printf("商: %.2lf\n", quotient);
    printf("幂: %.2lf\n", power);
    printf("第一个数的平方根: %.2lf\n", sqrt1);
    printf("第二个数的平方根: %.2lf\n", sqrt2);

    return 0;
}