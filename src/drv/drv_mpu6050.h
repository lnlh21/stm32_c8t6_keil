/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_mpu6050.h
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��12��18��
  ����޸�   :
  ��������   : drv_mpu6050.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_MPU6050_H__
#define __DRV_MPU6050_H__

#define MPU6050_ADDRESS_AD0_LOW     0xD0 // AD0Ϊ�͵�ʱ���豸��д��ַ  
#define MPU6050_ADDRESS_AD0_HIGH    0XD1 // AD0Ϊ�ߵ�ʱ���豸��д��ַ  
#define MPU_ADDR    0xD0    //IICд��ʱ�ĵ�ַ�ֽ�����  
  
  
#define MPU_DEBUG       1  
  
//�����ĵ�δ�����ļĴ��� ��Ҫ���ڹٷ�DMP����  
#define MPU6050_RA_XG_OFFS_TC       0x00 //[bit7] PWR_MODE, [6:1] XG_OFFS_TC, [bit 0] OTP_BNK_VLD  
#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD  
//bit7�Ķ���,������Ϊ1,����I2C���߸ߵ�ƽ��VDD��������Ϊ0,����I2C���߸ߵ�ƽ��VLOGIC  
  
#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD  
#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN  
#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN  
#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN  
  
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS �����Ĵ�������һ��  
#define MPU6050_RA_XA_OFFS_L_TC     0x07  
  
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS �����Ĵ�������һ��  
#define MPU6050_RA_YA_OFFS_L_TC     0x09  
  
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS �����Ĵ�������һ��  
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B  
  
#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR �����Ĵ�������һ��  
#define MPU6050_RA_XG_OFFS_USRL     0x14  
  
#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR �����Ĵ�������һ��  
#define MPU6050_RA_YG_OFFS_USRL     0x16  
  
#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR �����Ĵ�������һ��  
#define MPU6050_RA_ZG_OFFS_USRL     0x18  
  
/*�����ǵĲ���Ƶ��*/  
/*�������ļĴ������,FIFO���,DMP�������˶���⡢ 
 *���˶��������������ⶼ�ǻ��ڲ����ʡ� 
 *ͨ��SMPLRT_DIV������������ʷ�Ƶ���ɵõ������� 
 *������=�����������/ (1 + SMPLRT_DIV) 
 *����DLPF�������(DLPF_CFG = 0��7) �������������= 8 khz 
 *������DLPF(���Ĵ���26)ʱ�������������= 1 khz 
 *���ٶȴ������������1 khz������ζ��,�����ʴ���1 khzʱ, 
 *ͬһ�����ٶȴ���������Ʒ���ܻ������뵽FIFO��DMP�ʹ������Ĵ���*/  
#define MPU6050_RA_SMPLRT_DIV       0x19 //[0-7] �����������Ƶ������  
  
/*�����ⲿ���Ų�����DLPF���ֵ�ͨ�˲���*/  
#define MPU6050_RA_CONFIG           0x1A  
//bit5-bit3  һ�����ӵ�FSYNC�˿ڵ��ⲿ�źſ���ͨ������EXT_SYNC_SET������  
//           Ҳ����˵,��������֮��,FSYNC�ĵ�ƽ0��1�����������ݼĴ���,��������  
//          0 ��ʹ�� 1 FSYNC��ƽ�����������ݼĴ��� 2 FSYNC��ƽ����GYRO_XOUT_L 3 FSYNC��ƽ����GYRO_YOUT_L  
//          4 FSYNC��ƽ����GYRO_ZOUT_L 5 FSYNC��ƽ����ACCEL_XOUT_L 6 FSYNC��ƽ����ACCEL_YOUT_L  
//          7 FSYNC��ƽ����SYNC_ACCEL_ZOUT_L  
//bit2-bit0 ���ֵ�ͨ�˲��� �����˳���Ƶ���� �������Ƶ�ʵĸ��ű��˳���  
/*��Ӧ��ϵ���� 
 * *                  |   ���ٶȴ�����  |          ������ 
 * * DLPF_CFG |    ����   |  �ӳ�  |    ����   |  �ӳ�  | ������ 
 * -------------+--------+-------+--------+------+------------- 
 * 0            | 260Hz     | 0ms    | 256Hz   | 0.98ms | 8kHz 
 * 1            | 184Hz     | 2.0ms  | 188Hz   | 1.9ms  | 1kHz 
 * 2            | 94Hz      | 3.0ms  | 98Hz    | 2.8ms  | 1kHz 
 * 3            | 44Hz      | 4.9ms  | 42Hz    | 4.8ms  | 1kHz 
 * 4            | 21Hz      | 8.5ms  | 20Hz    | 8.3ms  | 1kHz 
 * 5            | 10Hz      | 13.8ms | 10Hz    | 13.4ms | 1kHz 
 * 6            | 5Hz       | 19.0ms | 5Hz     | 18.6ms | 1kHz 
 * 7            | Reserved  | Reserved | Reserved 
 * */  
  
  
/*�����ǵ�����,��Ҫ�����������ǵ��������Լ�(ͨ����Ӧ��λ7 6 5 �����Լ�)*/  
#define MPU6050_RA_GYRO_CONFIG      0x1B  
//bit4-bit3 ������������  
//           0 = +/- 250 ��/��  
//           1 = +/- 500 ��/��  
//           2 = +/- 1000 ��/��  
//           3 = +/- 2000 ��/��*/  
  
/*���ٶȼƵ�����,��Ҫ�����ü��ٶȼƵ��������Լ�(ͨ����Ӧ��λ7 6 5 �����Լ�) 
 *����,��������ϵͳ�ĸ�ͨ�˲���*/  
#define MPU6050_RA_ACCEL_CONFIG     0x1C  
//bit7 ����X�Լ� ���ٶȼƵ��Լ�  
//bit6 ����Y�Լ�  
//bit5 ����Z�Լ�  
//bit4-bit3 ���ٶȴ���������������  
//           0 = +/- 2g  
//           1 = +/- 4g  
//           2 = +/- 8g  
//           3 = +/- 16g*/  
//bit0��bit2 ���ٶȴ������ĸ�ͨ�˲���  
/*DHPF����·�����������˶�̽����(��������,�˶���ֵ,���˶�)��һ���˲���ģ�顣 
 *��ͨ�˲��������ֵ�������ݼĴ����� 
 *��ͨ�˲���������ģʽ�� 
 *����:��һ�������н��˲������ֵ��Ϊ�㡣����Ч�Ľ����˸�ͨ�˲���������ģʽ���Կ����л��˲���������ģʽ�� 
 *����:��ͨ�˲�����ͨ�����ڽ�ֹƵ�ʵ��ź� 
 *����:������,������������ǰ���������������ֵ�����������ͳ�������֮��Ĳ��� 
 *����ֵ������ʾ 
 * ACCEL_HPF | ��ͨ�˲�ģʽ| ��ֹƵ�� 
 * ----------+-------------+------------------ 
 * 0         | Reset       | None 
 * 1         | On          | 5Hz 
 * 2         | On          | 2.5Hz 
 * 3         | On          | 1.25Hz 
 * 4         | On          | 0.63Hz 
 * 7         | Hold        | None 
 */  
  
#define MPU6050_RA_FF_THR           0x1D  
/*����������ٶȵ���ֵ 
 *����Ĵ���Ϊ�����������ֵ���������á� 
 *FF_THR�ĵ�λ��1LSB = 2mg�������ٶȴ������������õ�������ľ���ֵ 
 *��С�ڼ����ֵʱ���Ϳ��Բ����������ֵ����������£�(���ٶȼ�ÿ�μ�⵽��+1����,���Ի�Ҫ�������ٶȲ�����) 
 *��������ʱ�����������һ�� (�Ĵ���30)������������ʱ��������ﵽ 
 *FF_DUR�й涨��ʱ��ʱ���������屻�ж�(�������������ж�) 
 **/  
  
#define MPU6050_RA_FF_DUR           0x1E  
/* 
 *����������ٶȵ�ʱ����ֵ 
* ����Ĵ���Ϊ��������ʱ����ֵ�������������á� 
* ʱ�����Ƶ��Ϊ1 khz,���FF_DUR�ĵ�λ�� 1 LSB = 1���롣 
* �����ٶ����������õľ���ֵ��С�ڼ����ֵʱ�� 
* ��������ʱ�����������һ�Ρ�����������ʱ������� 
* �ﵽ�üĴ����Ĺ涨ʱ��ʱ���������屻�жϡ� 
* (�������������ж�) 
* */  
  
#define MPU6050_RA_MOT_THR          0x1F  
/* 
 *�˶����ļ��ٶ���ֵ 
 *����Ĵ���Ϊ�˶��жϵ���ֵ���������á� 
 *MOT_THR�ĵ�λ�� 1LSB = 2mg�� 
 *�����ٶ����������õľ���ֵ���������˶�������ֵʱ�� 
 *���ɲ�ø��˶�����һ����£��˶�ʱ�������������һ�Ρ� 
 *���˶����������ﵽMOT_DUR�Ĺ涨ʱ��ʱ���˶���ⱻ�жϡ� 
 * �˶��жϱ����˱������˶�MOT_DETECT_STATUS (Register 97)����ͼ��ԡ� 
 */  
  
#define MPU6050_RA_MOT_DUR          0x20  
/* 
*�˶����ʱ�����ֵ�� 
*����Ĵ���Ϊ�˶��жϵ���ֵ���������á� 
*ʱ�����������Ƶ��Ϊ1 kHz �����MOT_THR�ĵ�λ�� 1LSB = 1ms�� 
*�����ٶ����������õľ���ֵ���������˶�������ֵʱ(Register 31)�� 
*�˶����ʱ�����������һ�Ρ����˶����������ﵽ�üĴ����涨��ʱ��ʱ�� 
*�˶���ⱻ�жϡ� 
 **/  
  
#define MPU6050_RA_ZRMOT_THR        0x21  
/* 
*���˶������ٶ���ֵ�� 
* ����Ĵ���Ϊ���˶��жϼ��������á� 
* ZRMOT_THR�ĵ�λ��1LSB = 2mg�� 
* �����ٶ����������õ�������ľ���ֵ��С�ڼ����ֵʱ�� 
* �Ϳ��Բ�����˶�����������£����˶�ʱ�����������һ�� (�Ĵ���34)�� 
* �������˶�ʱ��������ﵽZRMOT_DUR (Register 34)�й涨��ʱ��ʱ�����˶����жϡ� 
* ������������˶���ⲻͬ���ǣ������˶��״μ�⵽�Լ������˶���ⲻ��ʱ�����˶���ⶼ���жϡ� 
* �����˶�����⵽ʱ,��״̬����MOT_DETECT_STATUS�Ĵ���(�Ĵ���97) ����ʾ������ 
* ���˶�״̬��Ϊ���˶�״̬����⵽ʱ,״̬λ����Ϊ1�������˶�״̬��Ϊ�˶�״̬����⵽ʱ, 
* ״̬λ����Ϊ0�� 
 **/  
  
#define MPU6050_RA_ZRMOT_DUR        0x22  
/* 
*���˶�����ʱ����ֵ 
* ����Ĵ���Ϊ���˶��жϼ�����ʱ������������á� 
* ʱ��������ļ���Ƶ��Ϊ16 Hz,���ZRMOT_DUR�ĵ�λ��1 LSB = 64 ms�� 
* �����ٶ����������õľ���ֵ��С�ڼ��������ֵ(Register 33)ʱ�� 
* �˶����ʱ�����������һ�Ρ������˶����������ﵽ�üĴ����涨��ʱ��ʱ�� 
* ���˶���ⱻ�жϡ� 
 **/  
  
  
/* 
 *�豸�ĸ���FIFOʹ��,�����¶� ���ٶ� ������ �ӻ� 
 *����ص�����д��FIFO������ 
 **/  
#define MPU6050_RA_FIFO_EN          0x23  
//bit7 �¶�fifoʹ��  
//bit6 ������Xfifoʹ��  
//bit5 ������Yfifoʹ��  
//bit4 ������Zfifoʹ��  
//bit3 ���ٶȴ�����fifoʹ��  
//bit2 �ⲿ���豸2fifoʹ��  
//bit1 �ⲿ���豸1fifoʹ��  
//bit0 �ⲿ���豸0fifoʹ��  
  
#define MPU6050_RA_I2C_MST_CTRL     0x24  
//���õ��������߶������µ�IIC����  
//bit7 ���Ӵ��豸����,�������Ƿ���� MULT_MST_EN����Ϊ1ʱ,MPU-60X0�������ٲü���߼�����  
//bit6 �ӳ����ݾ����ж�,ֱ����豸����Ҳ���������ٴ��� �൱������ͬ���ȴ�  
//bit5 ������Ϊ1ʱ,��Slave3 �������ⲿ����������(�Ĵ���73 ���Ĵ��� 96)д��FIFO������,ÿ�ζ�д��  
//bit4 ������ȡһ���ӻ�����һ���ӻ���ȡ֮��Ķ��� Ϊ0 ��ȡ֮����һ��restart,Ϊ1 ��һ�ζ�ȡǰ����һ������,Ȼ��  
//      һֱ��ȡֱ���л�д������л��豸  
//bit3-bit0 ����MPU��ΪIIC����ʱ��ʱ��,����MPU�ڲ�8M�ķ�Ƶ  
/* I2C_MST_CLK | I2C ��ʱ���ٶ� | 8MHz ʱ�ӷ�Ƶ�� 
* ------------+------------------------+------------------- 
* 0             | 348kHz          | 23 
* 1             | 333kHz          | 24 
* 2             | 320kHz          | 25 
* 3             | 308kHz          | 26 
* 4             | 296kHz          | 27 
* 5             | 286kHz          | 28 
* 6             | 276kHz          | 29 
* 7             | 267kHz          | 30 
* 8             | 258kHz          | 31 
* 9             | 500kHz          | 16 
* 10            | 471kHz          | 17 
* 11            | 444kHz          | 18 
* 12            | 421kHz          | 19 
* 13            | 400kHz          | 20 
* 14            | 381kHz          | 21 
* 15            | 364kHz          | 22 
* */  
  
  
  
/**************************MPU����IIC���豸���ƼĴ���,ûʹ�ôӻ����ӵĻ������ÿ�����Щ************************************/  
/*ָ��slave (0-3)��I2C��ַ 
* ע��Bit 7 (MSB)�����˶�/дģʽ�����������Bit 7,��ô����һ����ȡ����, 
* ����������,��ô����һ����д����������λ(6-0)��slave�豸��7-bit�豸��ַ�� 
* �ڶ�ȡģʽ��,��ȡ����Ǵ洢����Ϳ��õ�EXT_SENS_DATA�Ĵ����С� 
* MPU-6050֧��ȫ5��slave����Slave 4�������⹦��(getSlave4* ��setSlave4*)�� 
* ��Ĵ���25��������I2C����ת��ͨ�����������֡��û�����ȷ��I2C����ת���ܹ� 
* ��һ����������������ɡ� 
* I2C slave���ݴ������ʿɸ��ݲ���������С�� 
* ��С�Ĵ�����������I2C_MST_DLY(�Ĵ���52)�������ġ� 
* slave���ݴ��������Ƿ���ݲ���������С����I2C_MST_DELAY_CTRL (�Ĵ���103)�������ġ� 
* slave�Ĵ���ָ���ǹ̶��ġ�Slave�Ĵ���˳����Slave 1, Slave 2, Slave 3 �� Slave 4�� 
* ���ĳһ��Slave�������ˣ���ô���ᱻ�Զ����ԡ� 
* ÿ��slave�ɰ������ʻ򽵵͵Ĳ���������ȡ������Щslave�Բ����ʶ�ȡ��Щ�Լ�С 
* �Ĳ����ʶ�ȡ������£�slave�Ķ�ȡ˳�����ɲ��䡣Ȼ���� 
* ���һЩslave�Ķ�ȡ���ʲ������ض�ѭ���н��ж�ȡ����ô���ǻᱻ�Զ����� 
* ���ཱུ�͵Ķ�ȡ���������Ϣ,����ļĴ���52�� 
* Slave�Ƿ񰴲����ʻ򽵵͵Ĳ���������ȡ�ɼĴ���103��Delay Enableλ������ 
 **/  
  
//�ӻ�0�������  
#define MPU6050_RA_I2C_SLV0_ADDR    0x25  
//bit7 ��ǰIIC ���豸0�Ĳ���,1Ϊ��ȡ 0д��  
//bit6-bit0 �ӻ��豸�ĵ�ַ  
/* Ҫ��ȡ����Ҫд����豸�ڲ��ļĴ�����ַ,���ܶ�ȡ����д��*/  
#define MPU6050_RA_I2C_SLV0_REG     0x26  
/*iic�ӻ�ϵͳ���üĴ���*/  
#define MPU6050_RA_I2C_SLV0_CTRL    0x27  
//bit7 �������߽�ֹ����豸��IIC���ݴ��͹���  
//bit6 ������Ϊ1ʱ,�ֽڽ������á��������ֽڽ���ʱ,�ʶԵĸߵ��ֽڼ��ɽ���  
//bit5 �� I2C_SLV0_REG_DIS �� 1��ֻ�ܽ��ж�ȡ����д�����ݡ�����λ�� 0�������ٶ�ȡ  
//      ��д������֮ǰд��һ���Ĵ�����ַ����ָ���ӻ��豸�ڲ��ļĴ�����ַ���з��ͻ����  
//      ����ʱ����λ������� 0  
//bit4  ָ���ӼĴ����յ����ַ��Եķ���˳�򡣵���λ�� 0���Ĵ�����ַ  
//      0�� 1, 2 �� 3 ���ֽ��Ƿֱ�ɶԣ������������Ĵ�����ַ �� ����Ϊһ���ַ��ԡ�����λ�� 1��  
//      �Ĵ�����ַ 1 �� 2�� 3 �� 4 ���ֽ��Ƿֱ�ɶԵģ���Ϊһ���ַ���  
//bit3-bit0  ָ���ӻ� 0 �����ַ��ĳ��ȡ���Slave 0ת��������ת����Slave 0���ֽ���,(IICһ�δ���ĳ���)  
//          ��λ�� 0��I2C_SLV0_EN λ�Զ��� 0.  
  
/*IIC SLAVE1���üĴ���,��0��ͬ*/  
  
#define MPU6050_RA_I2C_SLV1_ADDR    0x28  
#define MPU6050_RA_I2C_SLV1_REG     0x29  
#define MPU6050_RA_I2C_SLV1_CTRL    0x2A  
  
/*IIC SLAVE2���üĴ���,��0��ͬ*/  
#define MPU6050_RA_I2C_SLV2_ADDR    0x2B  
#define MPU6050_RA_I2C_SLV2_REG     0x2C  
#define MPU6050_RA_I2C_SLV2_CTRL    0x2D  
  
/*IIC SLAVE3���üĴ���,��0��ͬ*/  
#define MPU6050_RA_I2C_SLV3_ADDR    0x2E  
#define MPU6050_RA_I2C_SLV3_REG     0x2F  
#define MPU6050_RA_I2C_SLV3_CTRL    0x30  
  
/*slave4��I2C��ַ IIC4��ǰ�����ļĴ�������������ͬ*/  
#define MPU6050_RA_I2C_SLV4_ADDR    0x31        //��IIC SLAVE1����   
#define MPU6050_RA_I2C_SLV4_REG     0x32    /*slave4�ĵ�ǰ�ڲ��Ĵ���*/  
#define MPU6050_RA_I2C_SLV4_DO      0x33  
    /*д��slave4�����ֽ���һ�Ĵ����ɴ���д��slave4�����ݡ� 
    * ���I2C_SLV4_RW����Ϊ1������Ϊ��ȡģʽ������ô�üĴ����޷�ִ�в���*/  
#define MPU6050_RA_I2C_SLV4_CTRL    0x34  
//������Ϊ1ʱ����λ������slave4��ת��������������Ϊ0ʱ������øò���  
#define MPU6050_I2C_SLV4_EN_BIT         7  
//������Ϊ1ʱ����λ������slave4������ɵ��ж��źŵ����ɡ�  
// �����Ϊ0ʱ��������˸��źŵ����ɡ���һ�ж�״̬���ڼĴ���54�п�����  
#define MPU6050_I2C_SLV4_INT_EN_BIT     6  
//������Ϊ1ʱ,ֻ�������ݵĶ���д������������Ϊ0ʱ,  
// �ڶ�д����֮ǰ����дһ���Ĵ�����ַ����ָ���Ĵ�����ַ��slave�豸��ʱ  
// ����Ӧ�õ���0�����ڸüĴ����л�������ݴ���  
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5  
//�������ӳ�,��Ϊ���ݲ����ʼ�С��I2C slaves�������ʽ��������á�  
// ��һ��slave�Ĵ��������Ǹ��ݲ����ʶ����͵�,��ô��slave����ÿ1 / (1 + I2C_MST_DLY) ���������д��䡣  
// ��һ�����Ĳ�����Ҳ����SMPLRT_DIV (�Ĵ��� 25)��DLPF_CFG (�Ĵ���26)�������ĵġ�  
// slave���������Ƿ���ݲ���������С����I2C_MST_DELAY_CTRL (�Ĵ���103)��������  
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4   //[4:0]  
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5  
/*slave4�пɶ�ȡ���������ֽ�*/  
#define MPU6050_RA_I2C_SLV4_DI      0x35  
  
/* 
 * IIC�����ӻ�ϵͳ�ж�״̬ 
 **/  
#define MPU6050_RA_I2C_MST_STATUS   0x36  
//bit7 ��λ��ӳ��һ����MPU-60X0�������ⲿ�豸��FSYNC�ж�״̬��  
//      ������Ϊ1����INT_PIN_CFG(�Ĵ���55)�ж���FSYNC_INT_ENʱ���жϲ�����  
//bit6 ��slave4�������ʱ���豸���Զ�����Ϊ1 ���������INT_ENABLE�е�I2C_MST_INT_EN������ж�  
//bit5 I2C����ʧȥ����I2C���ߣ�һ������״̬�����ٲã���λ�Զ�����Ϊ1.���������INT_ENABLE�Ĵ���  
//      ���Ĵ���56���е�I2C_MST_INT_ENλ�����жϲ���  
//bit4  slave4��NACK״̬  
//bit3  slave3��NACK״̬  
//bit2  slave2��NACK״̬  
//bit1  slave1��NACK״̬  
//bit0  slave0��NACK״̬  
  
  
/*�ж��������üĴ���*/  
#define MPU6050_RA_INT_PIN_CFG      0x37  
//bit7  �жϵ��߼���ƽģʽ,�ߵ�ƽʱ������Ϊ0���͵�ƽʱ������Ϊ1  
//bit6  �ж�����ģʽ,����ģʽ����Ϊ0����©ģʽ����Ϊ1.  
//bit5  �ж�����ģʽ.50us-pulseģʽ����Ϊ0��latch-until-int-clearedģʽ����Ϊ1  
//bit4  �ж��������ģʽ status-read-only״̬����Ϊ0��any-register-read״̬����Ϊ1.  
//bit3  FSYNC�ж��߼���ƽģʽ 0=active-high, 1=active-low  
//bit2  FSYNC�˿��ж�������������Ϊ0ʱ���ã�����Ϊ1ʱ����  
//bit1  I2C֧·����״̬,��λ����1��I2C_MST_EN (�Ĵ��� 106 λ[5])����0ʱ,����Ӧ�ó��������ܹ�ֱ�ӷ���MPU-60X0�ĸ���I2C����  
//      ����������ζ�����ֱ�ӷ���  
//bit0  ����λΪ1ʱ��CLKOUT�˿ڿ�������ο�ʱ�ӡ�����λΪ0ʱ���������  
  
  
/*�����ж�ʹ��*/  
#define MPU6050_RA_INT_ENABLE       0x38  
//bit7  ���������ж�ʹ��  
//bit6  �˶�����ж�ʹ��  
//bit5  ���˶�����ж�ʹ��  
//bit4  FIFO����ж�ʹ��  
//bit3  IIC���������ж�Դʹ��  
//bit0  ���ݾ����ж�ʹ��  
  
  
/*DMP�ж�ʹ��*/  
#define MPU6050_RA_DMP_INT_STATUS   0x39  
//��֪����Щλ�ľ���������ʲô,�ٷ����ɲ���,���ǵ�ȷ����  
#define MPU6050_DMPINT_4_BIT            4  
#define MPU6050_DMPINT_3_BIT            3  
#define MPU6050_DMPINT_2_BIT            2  
#define MPU6050_DMPINT_1_BIT            1  
#define MPU6050_DMPINT_0_BIT            0  
  
/*DMP�ж�����*/  
#define MPU6050_RA_INT_STATUS       0x3A  
//DMP�ж�λ֮һʹ��  
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2  
//DMP�ж�λ֮��ʹ��  
#define MPU6050_INTERRUPT_DMP_INT_BIT       1  
  
/*���ٶ�X���*/  
#define MPU6050_RA_ACCEL_XOUT_H     0x3B  
#define MPU6050_RA_ACCEL_XOUT_L     0x3C  
  
/*���ٶ�Y���*/  
#define MPU6050_RA_ACCEL_YOUT_H     0x3D  
#define MPU6050_RA_ACCEL_YOUT_L     0x3E  
  
/*���ٶ�Z���*/  
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F  
#define MPU6050_RA_ACCEL_ZOUT_L     0x40  
  
/*�¶�ֵ���*/  
#define MPU6050_RA_TEMP_OUT_H       0x41  
#define MPU6050_RA_TEMP_OUT_L       0x42  
  
/*������X���*/  
#define MPU6050_RA_GYRO_XOUT_H      0x43  
#define MPU6050_RA_GYRO_XOUT_L      0x44  
  
/*������Y���*/  
#define MPU6050_RA_GYRO_YOUT_H      0x45  
#define MPU6050_RA_GYRO_YOUT_L      0x46  
  
/*������Z���*/  
#define MPU6050_RA_GYRO_ZOUT_H      0x47  
#define MPU6050_RA_GYRO_ZOUT_L      0x48  
  
/*��IIC�ӻ��ϻ�ȡ��������*/  
#define MPU6050_RA_EXT_SENS_DATA_00 0x49  
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A  
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B  
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C  
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D  
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E  
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F  
#define MPU6050_RA_EXT_SENS_DATA_07 0x50  
#define MPU6050_RA_EXT_SENS_DATA_08 0x51  
#define MPU6050_RA_EXT_SENS_DATA_09 0x52  
#define MPU6050_RA_EXT_SENS_DATA_10 0x53  
#define MPU6050_RA_EXT_SENS_DATA_11 0x54  
#define MPU6050_RA_EXT_SENS_DATA_12 0x55  
#define MPU6050_RA_EXT_SENS_DATA_13 0x56  
#define MPU6050_RA_EXT_SENS_DATA_14 0x57  
#define MPU6050_RA_EXT_SENS_DATA_15 0x58  
#define MPU6050_RA_EXT_SENS_DATA_16 0x59  
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A  
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B  
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C  
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D  
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E  
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F  
#define MPU6050_RA_EXT_SENS_DATA_23 0x60  
  
//�˶�����״̬  
#define MPU6050_RA_MOT_DETECT_STATUS    0x61  
//bit7  x�ᷴ���˶�����ж�״̬  
//bit6  x�������˶�����ж�״̬  
//bit5  Y�ᷴ���˶�����ж�״̬  
//bit4  Y�������˶�����ж�״̬  
//bit3  Z�ᷴ���˶�����ж�״̬  
//bit2  Z�������˶�����ж�״̬  
//bit1  
//bit0  ���˶�����ж�״̬  
//  
  
  
/*д�뵽IIC�ӻ��е�����,ָ����slv�����������*/  
#define MPU6050_RA_I2C_SLV0_DO      0x63  
#define MPU6050_RA_I2C_SLV1_DO      0x64  
#define MPU6050_RA_I2C_SLV2_DO      0x65  
#define MPU6050_RA_I2C_SLV3_DO      0x66  
  
/*�ⲿӰ�ӼĴ���������,����Ĵ�������ָ���ⲿ����������Ӱ�ӵ�ʱ�� 
*��������ĳһ�ض���slave���䴫�����ʾͻ��С�� 
*��һ��slave�Ĵ��������Ǹ��ݲ����ʶ����͵�,��ô��slave���� 
*ÿ1 / (1 + I2C_MST_DLY) ���������д��䡣 
*     1 / (1 + I2C_MST_DLY) Samples 
* ��һ�����Ĳ�����Ҳ����SMPLRT_DIV (�Ĵ��� 25)��DLPF_CFG (�Ĵ���26)�������ĵġ�*/  
#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67  
//DELAY_ES_SHADOW����Ϊ1,�����ⲿ����������Ӱ�ӽ����ӳٵ����е����ݽ�����ϡ�  
#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7  
//slv4-0������  
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4  
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3  
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2  
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1  
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0  
  
/*���������ǣ����ٶȼƣ��¶ȴ�������ģ��������ź�ͨ���ĸ�λ�� 
��λ�ỹԭģ��ת���ź�ͨ����������ǵ��ϵ�����*/  
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68  
//bit2  ���������ǵ��ź�·��  
//bit1  ���ü��ٶȴ��������ź�·��  
//bit0  �����¶ȴ��������ź�·��  
  
  
/*��ȡ���ٶȴ����������ӳ� �����˲�����һЩ���� 
* ���ٶȴ���������·��Ϊ�������Ĵ������˶���⡢ 
* ���˶���������������ģ���ṩ�������ڼ��ģ�鿪ʼ����֮ǰ�� 
* �������������ź�·�������������������á� 
* Ĭ�ϵ�4���뻽���ӳ�ʱ����Լӳ�3�������ϡ���ACCEL_ON_DELAY�й涨 
* ����ӳ���1 LSB = 1 ����Ϊ��λ������InvenSense����ָʾ�� 
* �û�����ѡ���κδ������ֵ��*/  
#define MPU6050_RA_MOT_DETECT_CTRL      0x69  
//�������Ч����λ  
//bit5-bit4 [5:4]1-4ms ��ʱʱ��1-4msѡ��  
//bit3-bit2 ����������������ļ������á�  
//          ��ָ�������������ļ��ٶȲ�������������Ե���ֵ����ʱ��  
//          ������洢������������ģ���С���������ֵ����ʱ��  
//          ��Ӧ�ļ�����������1���û���ͨ��FF_COUNT���ò�������ֵ������������  
//          �����ʿɸ����±�������ã�  
            /* FF_COUNT | ���������� 
            * ---------+------------------ 
            * 0             | ���� 
            * 1             | 1 
            * 2             | 2 
            * 3             | 4 
            * ��FF_COUNT����Ϊ0(��λ)ʱ,�κβ��ϸ����Ʒ��������������Ϊ0*/  
//bit1-bit0  �˶����������ļ������á�  
//          ��ָ�������������ļ��ٶȲ�������������Ե���ֵ����ʱ��  
//          ������洢���˶����ģ���С���������ֵ����ʱ����Ӧ�ļ�����������1��  
//          �û���ͨ��MOT_COUNT���ò�������ֵ�����������������ʿɸ����±�������ã�  
//          MOT_COUNT | ����������  
            /* ----------+------------------ 
            * 0              | ���� 
            * 1              | 1 
            * 2              | 2 
            * 3              | 4 
            * ��MOT_COUNT����Ϊ0(��λ)ʱ,�κβ��ϸ����Ʒ��������������Ϊ0*/  
              
  
/*����Ĵ��������û�ʹ�ܻ�ʹ�� FIFO �������� 
 *I2C ����ģʽ����Ҫ I2C �ӿڡ�FIFO ���� 
����I2C �������������ź�ͨ���ʹ������Ĵ���Ҳ����ʹ������Ĵ�����λ*/  
#define MPU6050_RA_USER_CTRL        0x6A  
//bit7  DMP��ֹ  
//bit6  ����λ����Ϊ0,FIFO�����ǽ��õ�  
//bit5  �����ģʽ������,MPU-60X0����Ϊ����I2C�����ϵ��ⲿ������slave�豸��I2C����  
//      ����λ�����Ϊ0ʱ,����I2C������·(AUX_DA and AUX_CL)����������I2C����  
//      (SDA��SCL)�����ġ�����������·ģʽ��һ��ǰ��  
//bit4  I2Cת����SPIģʽ(ֻ����MPU-6000)  
//bit3  ����DMPģʽ,�ٷ��ĵ�δ˵���ļĴ���  
//bit2  ����FIFO������Ϊ1ʱ����λ������FIFO����������ʱFIFO_EN����0���������ú󣬴�λ���Զ���Ϊ0  
//bit1  ����I2C����������Ϊ1ʱ����λ������I2C��������ʱI2C_MST_EN����0���������ú󣬴�λ���Զ���Ϊ0  
//bit0  �������д������Ĵ������ź�·�� ���ֻ�����ź�·���������ô������Ĵ���������ʹ�üĴ���104  
  
  
/*�����û����õ�Դģʽ��ʱ��Դ�����ṩ�˸�λ�����豸�ͽ����¶ȴ�������λ*/  
#define MPU6050_RA_PWR_MGMT_1       0x6B  
//bit7  ����һ���豸���������á� �������ú�һ��~ 50 �����С�ӳ��Ǻ����  
//bit6  �Ĵ�����SLEEPλ����ʹ�豸���ڷǳ��͹��ʵ�����ģʽ��  
//bit5  ������������״̬����λ��Ϊ1��SLEEP����ʱ.������ģʽ�ͻ���ģʽ��ѭ�����Դ˴ӻ�Ծ�Ĵ������л�ȡ��������  
//bit3  �¶ȴ���������״̬�����ڲ��¶ȴ�������ʹ��  
//bit2-bit0 �趨ʱ��Դ����,һ��Ƶ��Ϊ8 mhz���ڲ�����,���������ǵ�ʱ�ӻ��ⲿ��ϢԴ�����Ա�ѡΪMPU-60X0��ʱ��Դ  
            /* CLK_SEL | ʱ��Դ 
            * --------+-------------------------------------- 
            * 0          | �ڲ����� 
            * 1          | PLL with X Gyro reference 
            * 2          | PLL with Y Gyro reference 
            * 3          | PLL with Z Gyro reference 
            * 4          | PLL with external 32.768kHz reference 
            * 5          | PLL with external 19.2MHz reference 
            * 6          | Reserved 
            * 7          | Stops the clock and keeps the timing generator in reset 
            * */  
  
  
/*����Ĵ��������û����ü��ٶȼ��ڵ͹���ģʽ�»����Ƶ�ʡ�Ҳ�����û��ü��ٶȼƺ� 
�����ǵĸ�����������ģʽ��*/  
#define MPU6050_RA_PWR_MGMT_2       0x6C  
//bit7-bit6 Accel-Only�͵���ģʽ�µĻ���Ƶ��  
            /* ͨ����Power Management 1�Ĵ������Ĵ���107���е�PWRSEL��Ϊ1�� 
            * MPU-60X0���Դ���Accerlerometer Only�ĵ͵���ģʽ��������ģʽ��, 
            �豸���رճ���ԭI2C�ӿ�����������豸��ֻ����accelerometer�Թ̶�ʱ�� 
            ����������в���������Ƶ�ʿ���LP_WAKE_CTRL�������ã����±���ʾ�� 
            * LP_WAKE_CTRL | ������Ƶ�� 
            * -------------+------------------ 
            * 0            | 1.25 Hz 
            * 1            | 2.5 Hz 
            * 2            | 5 Hz 
            * 3            | 10 Hz 
            * */  
//bit5  ���õ�x����ٶȴ���������״̬,Ҳ���ǽ������ģʽ  
//bit4  ���õ�Y����ٶȴ���������״̬  
//bit3  ���õ�Z����ٶȴ���������״̬  
//bit2  ���õ�x������������״̬  
//bit1  ���õ�Y������������״̬  
//bit0  ���õ�Z������������״̬  
  
/*�趨DMPģʽ�µ�bank*/  
#define MPU6050_RA_BANK_SEL         0x6D  
//DMP�ڴ�����  
#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6  
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5  
#define MPU6050_BANKSEL_MEM_SEL_BIT         4  
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5  
//dmp�ڴ��ַ����  
#define MPU6050_DMP_MEMORY_BANKS        8  
#define MPU6050_DMP_MEMORY_BANK_SIZE    256  
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16  
  
/*�趨DMPģʽ�µ���ʼ��ַ*/  
#define MPU6050_RA_MEM_START_ADDR   0x6E  
/*һ���ֽڵ�dmp���ݻ���*/  
#define MPU6050_RA_MEM_R_W          0x6F  
/*DMP���üĴ���1*/  
#define MPU6050_RA_DMP_CFG_1        0x70  
/*DMP���üĴ���2*/  
#define MPU6050_RA_DMP_CFG_2        0x71  
  
/*��ǰFIFO��������С 
* ���ֵ�����˴洢��FIFO���������ֽ����� 
* ���������Ҳ���ܴ�FIFO��������ȡ���ֽ����� 
* ����洢��FIFO(�Ĵ���35��36)�еĴ��������������ṩ�Ŀ��������������ȡ� 
* �����Ĵ���һ�𹹳�һ��16λ����*/  
#define MPU6050_RA_FIFO_COUNTH      0x72  
#define MPU6050_RA_FIFO_COUNTL      0x73  
  
/*����Ĵ������ڴ�FIFO�������ж�ȡ�ͱ�д���ݡ������ڼĴ������(�ӵ͵���)��ָ 
 *���±�д������д��FIFO��������е�FIFO���ñ�־(������)������������ 
 *�����ⲿ���������ݼĴ���(�Ĵ���73���Ĵ���96)����һ��slave�豸���� 
 *,��ô�Ĵ���59���Ĵ���96�����ݶ����ڲ����ʵ�ָ���±�д�� 
* �����������ݼĴ������Ĵ���59���Ĵ���96�������FIFO���ñ�־��FIFO_EN �Ĵ� 
* ��35)�ж���Ϊ1ʱ�����ǵ����ݽ���д��FIFO����������I2C_MST_CTRL (�Ĵ��� 36) 
* �����ҵ�һ����I2C Slave 3�����Ķ���Ĵ��������ݼĴ�����־�� 
* ���FIFO���������,״̬λFIFO_OFLOW_INT�Զ�����Ϊ1�� 
* ��λλ��INT_STATUS (�Ĵ���58)�С���FIFO���������ʱ,��������ݽ��ᶪʧ 
* �������ݽ���д��FIFO�����FIFO������Ϊ��, ��ȡ������ԭ����FIFO�ж�ȡ�� 
* ���һ���ֽڣ�ֱ���п��õ������ݡ��û�Ӧ���FIFO_COUNT,��ȷ������FIFO����Ϊ��ʱ��ȡ��*/  
#define MPU6050_RA_FIFO_R_W         0x74  
  
/*�Ĵ�����������֤�豸����ݵ� Ĭ��ֵ��0X34*/  
#define MPU6050_RA_WHO_AM_I         0x75  
//bit6-bit1 �豸�����֤ 0x34 ���λ�����λ���޳���  




/* MPU6050 Register Address ------------------------------------------------------------*/
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG				0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ�����
#endif /* __DRV_MPU6050_H__ */

