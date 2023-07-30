#include "gw_grayscale_sensor.h"

uint8_t count = 0;
uint8_t scan_addr[128];

uint8_t gray_sensor[8];
uint8_t gray_sensor_sum = 0;
float gray_sensor_sum_val = 0;
uint8_t digital_data;
uint8_t analog_data;

/* 设置软件IIC驱动 */
sw_i2c_interface_t i2c_interface =
    {
        .sda_in = sda_in,
        .scl_out = scl_out,
        .sda_out = sda_out,
        .user_data = 0, // 用户数据，可在输入输出函数里得到
};

void gw_grayscale_sensor_init(void)
{
    sw_i2cinit();
    count = i2c_scan(&i2c_interface, scan_addr);
#if GW_READ_DIGITAL_DATA
    /* 读取开关量数据 */
    /* 打开开关量数据模式 */
    sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);

    sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data 有1~8号探头开关数据
#else
    /* 读取模拟量数据 */
    /* 打开模拟值模式, 并且读取模拟数值, 后面可以直接读取 */
    sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_ANALOG_MODE, analog_data, 8);

    /* 直接读取 */
    sw_i2c_read(&i2c_interface, 0x4C << 1, analog_data, 8);

#endif
}

// 根据灰度信息计算出具体偏移位置
void Get_GW_Grayscale_Val(void)
{
    static float gray_sensor_sum_val_old = 0;

    gray_sensor_sum = 0;

    sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data 有1~8号探头开关数据

    /* 把字节里的8个开关量存到八个变量里，这里为gray_sensor[0] ~ gray_sensor[7],
     * 也可以是变量val1 ~ val8, 因为是宏定义 */
    SEP_ALL_BIT8(~digital_data,
                 gray_sensor[7], // 探头1
                 gray_sensor[6], // 探头2
                 gray_sensor[5], // 探头3
                 gray_sensor[4], // 探头4
                 gray_sensor[3], // 探头5
                 gray_sensor[2], // 探头6
                 gray_sensor[1], // 探头7
                 gray_sensor[0]  // 探头8
    );

    if (gray_sensor[0])
        gray_sensor_sum++;
    if (gray_sensor[1])
        gray_sensor_sum++;
    if (gray_sensor[2])
        gray_sensor_sum++;
    if (gray_sensor[3])
        gray_sensor_sum++;
    if (gray_sensor[4])
        gray_sensor_sum++;
    if (gray_sensor[5])
        gray_sensor_sum++;
    if (gray_sensor[6])
        gray_sensor_sum++;
    if (gray_sensor[7])
        gray_sensor_sum++;

    if (gray_sensor_sum == 1)
        gray_sensor_sum_val = gray_sensor_sum_val_old = -gray_sensor[0] * 4 - gray_sensor[1] * 3 - gray_sensor[2] * 2 - gray_sensor[3] + gray_sensor[4] + gray_sensor[5] * 2 + gray_sensor[6] * 3 + gray_sensor[7] * 4;
    else if (gray_sensor_sum == 2)
        gray_sensor_sum_val = gray_sensor_sum_val_old = (float)(-gray_sensor[0] * 4 - gray_sensor[1] * 3 - gray_sensor[2] * 2 - gray_sensor[3] + gray_sensor[4] + gray_sensor[5] * 2 + gray_sensor[6] * 3 + gray_sensor[7] * 4) / 2;
    else
        gray_sensor_sum_val = gray_sensor_sum_val_old;
}