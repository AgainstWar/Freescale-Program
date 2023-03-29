/*******************************************************************************
 * 菜单   function.c
 * file name:          function.c
 * description:        菜单
 * author:             ***
 * version：            v0.0.1
 * date：               2022.02.22
 * note:
 *******************************************************************************/
#include "function.h"
void menu()
{
	if (!KEY1)
	{

		// L_Ki+= 0.005;

		while (!KEY1)
		{
		}
	}
	if (!KEY2)
	{

		// L_Ki -= 0.005;

		while (!KEY2)
		{
		}
	}
	if (!KEY3)
	{
		// L_Kp+= 0.005;
		R_Kp += 0.1;
		// coefficient += 0.001;
		while (!KEY3)
		{
		}
	}
	if (!KEY4)
	{
		// L_Kp-= 0.005;
		R_Kp -= 0.1;
		// expited_L -= 100;
		while (!KEY4)
		{
		}
	}
	if (!KEY5)
	{

		// expited_L -= 100;
		expited_L_R += 20;
		while (!KEY5)
		{
		}
	}
	if (!KEY6)
	{

		// expited_L -= 100;
		expited_L_R -= 20;
		while (!KEY6)
		{
		}
	}
	if (!KEY7)
	{

		while (!KEY7)
		{
		}
	}
	if (!KEY8)
	{

		while (!KEY8)
		{
		}
	}
}
