/*
 * PID.c
 *
 *  Created on: 2023��5��29��
 *      Author: gy
 */

#include "PID.h"

PID speed_pid;//�ṹ��ʵ����Ҫ�ŵ�.c�ļ���

void PID_Init(PID *p)
{
    p->sv=1000.0f;    //100r/min
    p->pv=0.0f;
    p->system_scale = 9.60f;//�ٶ�0~960r/min
    p->Kp=P;
    p->Ki=I;
    p->Kd=D;
    p->EK=0.0f;
    p->Last_EK=0.0f;
    p->SEK=0;
}

float Get_speed(PID *p,POSSPEED *p2)
{
    #if speed_method == speed_method_M
        if(p2->Speed_fr<750||p2->Speed_fr>-750)
            p->pv = p2->Speed_fr*10;
        else
            p->pv = 0;
    #endif
    #if speed_method == speed_method_T
        p->pv = p2->Speed_pr*10;
    #endif
    return p->pv;
}

//float Get_Average_Speed(PID *p, POSSPEED *p2)
//{
//    static float speedArray[5]; // �洢ÿ�ε��� Get_speed �����ķ���ֵ
//    static int index = 0; // ��������
//    int i;
//
//    speedArray[index] = Get_speed(p, p2); // ���� Get_speed �������洢����ֵ
//
//    index++; // ��������
//
//    if (index >= 5) // ��������Ѿ��������Ԫ��
//    {
//        // ����������Сֵ
//        float minVal = speedArray[0];
//        float maxVal = speedArray[0];
//        float sum = speedArray[0];
//
//        // �ҳ�������Сֵ���������ܺ�
//        for (i = 1; i < 5; i++)
//        {
//            if (speedArray[i] < minVal)
//                minVal = speedArray[i];
//            if (speedArray[i] > maxVal)
//                maxVal = speedArray[i];
//            sum += speedArray[i];
//        }
//
//        // ��ƽ��ֵ
//        p2->average = (sum - minVal - maxVal) / 3.0;
//        p->pv = p2->average;
//        // ��������
//        index = 0;
//
//        return p2->average;
//    }
//
//    return 0.0; // �������δ�����Ԫ�أ�����0���������ʵ�Ĭ��ֵ
//}


void PID_Calc(PID *p)
{

    p->SEK+=p->EK;
    if(p->EK<7500||p->EK>-7500)
    {
        p->EK = p->sv - p->pv;
    }
    p->OUT=p->Kp*p->EK+p->Ki*p->SEK+p->Kd*(p->EK-p->Last_EK);
    p->OUT /= p->system_scale;

    p->Last_EK=p->EK;
}

