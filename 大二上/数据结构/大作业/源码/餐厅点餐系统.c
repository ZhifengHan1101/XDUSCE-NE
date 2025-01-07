#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Menu
{
    int code;           // 菜品编码
    char name[100];     // 菜品名称
    int price;          // 菜品价格
    struct Menu *next;
}MENU;


typedef struct Desk
{
    int deskCode;                   // 桌子的编号
    int peopleNum;                  // 一桌的人数
    int purchasedDishNum;           // 购买菜品的数量
    MENU *purchasedDish;            // 本桌购买菜品
    int totalPrice;                 // 购买菜品总价
    struct Desk *next;
}DESK;

MENU *menu;     // 菜单信息
DESK *desk;     // 菜桌信息
int count;      // 总菜品数

//===========================================================================
//                              菜单部分
//===========================================================================

void SaveMenu()     // 菜单保存
{
    FILE *fp = NULL;
    fp = fopen("菜单.txt", "w");

    if(fp == NULL)
    {
        printf("菜单保存失败！\n");
        return;
    }
    
    fprintf(fp, "%d\n", count);     // 保存总菜品数

    for(MENU *i = menu; i; i = i -> next)
        fprintf(fp, "%d %s %d\n", i -> code, i -> name, i -> price);    // 分别保存编码、菜名和价格

    fclose(fp);
    printf("菜单保存成功！\n");

}

void ReadMenu()
{
    FILE *fp;
    fp = fopen("菜单.txt", "r");

    if(fp == NULL)
    {
        printf("菜单读取失败！\n");
        return;
    }

    fscanf(fp, "%d", &count);       // 读取总菜品数

    for(MENU *i = menu; i;)     // 删除原有节点，并释放内存
    {
        MENU *j = i -> next;
        free(i);
        if(j)
            i = j;
    }

    MENU *s, *r;
    for(int i = 0; i < count; i ++ )        // 读取菜品编码、菜名和价格，并创建链表
    {
        s = (MENU*)malloc(sizeof(MENU));
        fscanf(fp, "%d%s%d", &s -> code, s -> name, &s -> price);
        if(menu == NULL)
            menu = s;
        else
        {
            r -> next = s;
        }
        r = s;
    }
    if(r != NULL)
        r -> next = NULL;

    fclose(fp);
    printf("菜单读取成功！\n");

}

void MenuOutput()       // 菜单输出
{
    printf("%-15s","编码");
	printf("%-15s","菜名");	
	printf("%-5s","价格");		
	printf("\n");

    for(MENU *i = menu; i; i = i -> next)
    {
        printf("%-15d", i -> code);
		printf("%-15s", i -> name);
		printf("%-5d", i -> price);
		printf("\n");
    }
}


//===========================================================================
//                              餐桌部分
//===========================================================================

void SaveDesk()     // 保存餐桌信息
{
    FILE *fp = NULL;
    fp = fopen("餐桌.txt", "w");

    if(fp == NULL)
    {
        printf("餐桌信息保存失败！\n");
        return;
    }

    for(DESK *i = desk; i; i = i -> next)   // 保存餐桌的编号、用餐人数、购买菜品数目、总价格和所点的菜品存入文件
    {
        fprintf(fp, "%d %d %d %d ", i -> deskCode, i -> peopleNum, i -> purchasedDishNum, i -> totalPrice);

        for(MENU *j = i -> purchasedDish; j; j = j -> next)
            fprintf(fp, "%d %s %d ", j -> code, j -> name, j -> price);

        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("餐桌信息保存成功！\n");

}

void ReadDesk()     // 读取餐桌信息
{
    FILE *fp = NULL;
    fp = fopen("餐桌.txt", "r");

    if(fp == NULL)
    {
        printf("餐桌信息打开失败！\n");
        return;
    }

    for(DESK *i = desk; i;)     // 删除原有的餐桌信息，并释放内存，注意里面菜品链表内存的释放
    {
        for(MENU *k = i -> purchasedDish; k;)
        {
            MENU *q = k -> next;
            free(k);
            if(q)
                k = q;
        }

        DESK *j = i -> next;
        free(i);
        if(j)
            i = j;
    }

    DESK *s = NULL, *r = NULL;
    while(!feof(fp))        // 当文件未结束时，读取文件中的餐桌信息，并构建链表
    {
        MENU *s1 = NULL, *r1 = NULL;
        s = (DESK*)malloc(sizeof(DESK));        // 创建desk节点
        s -> purchasedDish = NULL;
        fscanf(fp, "%d%d%d%d", &s -> deskCode, &s -> peopleNum, &s -> purchasedDishNum, &s -> totalPrice); // 读入信息

        for(int i = 0; i < s -> purchasedDishNum; i ++ )    // 每个desk节点中还要创建menu链表
        {
            s1 = (MENU*)malloc(sizeof(MENU));
            fscanf(fp, "%d %s %d ", &s1 -> code, s1 -> name, &s1 -> price);   // 读取消息
            
            if(s -> purchasedDish == NULL)      // 创建链表
                s -> purchasedDish = s1;
            else
                r1 -> next = s1;
            r1 = s1;
        }
        if(r1 != NULL)
            r1 -> next = NULL;

        if(desk == NULL)
            desk = s;
        else
        {
            r -> next = s;
        }
        r = s;
    }
    if(r != NULL)
        r -> next = NULL;

    fclose(fp);
    printf("餐桌信息读取成功！\n");
}


//===========================================================================
//                              顾客界面
//===========================================================================

void ClientMenu()       // 顾客菜单列表
{
    int choice, deskcode;

    printf("----------------------------------------\n");
	printf("|              欢迎来到XX餐厅            |\n"); 
	printf("----------------------------------------\n");

    printf("----------------------------------------\n");
	printf("|****          1.点餐              ****|\n"); 
	printf("|****          2.查询              ****|\n"); 
	printf("----------------------------------------\n");
	printf("请输入你的选择【1 or 2】:\n");
    scanf("%d", &choice);

    while(choice!=1 && choice!=2)       // 若输入不为1或2，则重新输入
	{
		printf("输入错误，请重新再输入：\n");
		scanf("%d", &choice);
	} 

    if(choice == 1)     // 点餐时
    {
        int flag = 0;   // 判断用户输入是否正确，若不正确则循环输入直到正确为止
        while(!flag)
        {
            printf("请你输入你当前所坐桌的桌号【1~199】:\n");
            scanf("%d", &deskcode);

            while(deskcode < 1 || deskcode > 199)
            {
                printf("输入错误,请输入1-199之间的值\n");
                scanf("%d", &deskcode);
            }

            int flag1 = 1;
            for(DESK *i = desk; i; i = i -> next)   // 判断用户输入的桌号是否有人，若找到已有桌号和输入相同，则错误
                if(i -> deskCode == deskcode)
                {
                    flag1 = 0;
                    break;
                }

            if(flag1)   // 输入都正确则开始点餐
            {
                MenuOutput();   // 输出菜单

                flag = 1;       // 点餐成功，flag变为1，停止循环

                DESK *s;        // 为点餐信息创建desk节点
                s = (DESK*)malloc(sizeof(DESK));
                // 更新s节点信息
                s -> deskCode = deskcode;
                s -> purchasedDish = NULL;  // 置空，防止报错
                printf("请你输入用餐人数：\n");
                scanf("%d", &s -> peopleNum);
                printf("请你输入点菜的个数：\n");
                scanf("%d", &s -> purchasedDishNum);
                s -> totalPrice = 0;

                for(int i = 0; i < s -> purchasedDishNum; i ++ )
                {
                    int dishcode;
                    printf("----------------------------------------\n");
                    printf("请输入您要购买的第%d个菜的编码:\n", i + 1);
                    scanf("%d", &dishcode);

                    int flag2 = 0;  // 判断输入菜品是否在菜单表中
                    MENU *t;
                    MENU *k = (MENU*)malloc(sizeof(MENU));  // 用于复制菜品信息
                    while(!flag2)
                    {
                        for(MENU *j = menu; j; j = j -> next)   // 查找并复制菜节点
                            if(j -> code == dishcode)
                            {
                                flag2 = 1;
                                k -> code = j -> code;
                                strcpy(k -> name, j -> name);
                                k -> price = j -> price;
                                break;
                            }

                        if(!flag2)
                        {
                            printf("非常抱歉,您需要的菜品今日不供应,请重新输入:\n");
                            printf("请输入您要购买的第%d个菜的编码:\n", i + 1);
                            scanf("%d", &dishcode);
                        }
                    }
                    // 菜品输入正确，则创建新的menu节点
                    s -> totalPrice += k -> price;

                    if(s -> purchasedDish == NULL)  // 构造s节点purchasedDish链表
                        s -> purchasedDish = k;
                    else
                        t -> next = k;
                    t = k;
                }
                // 将菜节点置首
                s -> next = desk;
                desk = s;

                printf("您需要支付的金额为:%d\n", s -> totalPrice);
		        printf("请选择是否提交订单【1 or 2】");
                int issubmit;
                scanf("%d", &issubmit);
                while(issubmit != 1 && issubmit != 2)
                {
                    printf("输入错误，请重新再输入：\n");
			        scanf("%d",&issubmit);
                }
                if(issubmit == 1)   // 如果确认点单则保存此数据
                {
                    SaveDesk();
                }
                else
                {
                    ClientMenu();   // 否则重新运行该函数
                }

            }
            else
                printf("该桌已经有顾客,请输入自己当前所在的桌号!\n");

        }
    }
    else if(choice == 2)    // 展示该桌所点单
    {
        int deskcode, c;
        DESK *desknow;
        printf("输入当前的所在桌位：");
		scanf("%d",&deskcode); 
		printf("%5s","餐桌号码");
		printf("%15s","餐桌人数");
		printf("%13s","点菜道数");
		printf("%11s","需付金额");
		printf("%20s","所点菜品"); 
		printf("\n"); 

        for(DESK *i = desk; i; i = i -> next)
        {
            if(i -> deskCode == deskcode)
            {
                desknow = i;

                printf("%4d号", i -> deskCode);
                printf("%11d人", i -> peopleNum);
                printf("%11d道", i -> purchasedDishNum);
                printf("%11dRmb", i -> totalPrice);
                printf("\t");

                for(MENU *j = i -> purchasedDish; j; j = j -> next)
                    printf("%s ", j -> name);

                printf("\n");
                break;
            }
        }

        printf("\n"); 
		//顾客点单完查看后，根据需要加餐和减餐 
		printf("----------------------------------------\n");
		printf("|****          1.加菜              ****|\n"); 
		printf("|****          2.减菜              ****|\n"); 
		printf("|****          3.返回              ****|\n"); 
		printf("----------------------------------------\n");
		printf("请输入你的选择【1 or 2 or 3】:\n");
        scanf("%d", &c);

        while(c != 1 && c != 2 && c != 3)
        {
            printf("输入错误，请重新再输入:\n");
			scanf("%d", &c);
        }

        printf("----------------------------------------\n");

        switch (c)
        {
            case 1:
            {
                int dishcode;
                printf("----------------------------------------\n");
                printf("请输入您要添加的菜的编码:\n");
                scanf("%d", &dishcode);

                int flag2 = 0;  // 判断输入菜品是否在菜单表中
                MENU *k = (MENU*)malloc(sizeof(MENU));
                while(!flag2)   // 当输入编码非菜单菜品时一直循环
                {
                    for(MENU *j = menu; j; j = j -> next)   // 查找并复制菜节点
                        if(j -> code == dishcode)
                        {
                            flag2 = 1;
                            k -> code = j -> code;
                            strcpy(k -> name, j -> name);
                            k -> price = j -> price;
                            break;
                        }

                    if(!flag2)
                    {
                        printf("非常抱歉,您需要的菜品今日不供应,请重新输入:\n");
                        printf("请输入您要添加的菜的编码:\n");
                        scanf("%d", &dishcode);
                    }
                }
                // 更新desknow节点信息
                desknow -> totalPrice += k -> price;
                desknow -> purchasedDishNum ++ ;
                k -> next = desknow -> purchasedDish;
                desknow -> purchasedDish = k;

                SaveDesk();     // 保存
                break;
            }

            case 2:
            {
                int dishcode;
                printf("----------------------------------------\n");
                printf("请输入您要删除的菜的编码:\n");
                scanf("%d", &dishcode);

                int flag2 = 0;  // 判断输入菜品是否在用户点单菜品中
                MENU *k = (MENU*)malloc(sizeof(MENU));
                while(!flag2)
                {
                    if(desknow -> purchasedDish -> code == dishcode)    // 若链表头为要删除节点，直接让链表头等于next
                    {
                        // 更新节点信息
                        desknow -> totalPrice -= desknow -> purchasedDish -> price;
                        desknow -> purchasedDishNum -- ;

                        desknow -> purchasedDish = desknow -> purchasedDish -> next;
                        flag2 = 1;
                    }
                    else    // 否则采用两个节点协助删除
                        for(MENU *i = desknow -> purchasedDish; i; i = i -> next)
                        {
                            MENU *j = NULL;
                            if(i -> next)
                                j = i -> next;

                            if(j -> code == dishcode)
                            {
                                // 更新节点信息
                                desknow -> totalPrice -= j -> price;
                                desknow -> purchasedDishNum -- ;

                                i -> next = j -> next;
                                j -> next = NULL;
                                free(j);    // 释放内存

                                flag2 = 1;
                                break;
                            }
                        }

                    if(!flag2)
                    {
                        printf("非常抱歉,您并未点该菜品,无法删除，请重新输入:\n");
                        printf("请输入您要删除的菜的编码:\n");
                        scanf("%d", &dishcode);
                    }
                }

                SaveDesk();
                break;
            }

            case 3:
            {
                ClientMenu();
                break;
            }

        }

    }
}


//===========================================================================
//                              管理员界面
//===========================================================================

void AdminMenu()    // 管理员界面
{
    int code, choice, n;
    printf("----------------------------------\n");
	printf("|       欢迎来到管理者系统界面   |\n");
	printf("----------------------------------\n");
	printf("|          1.添加菜品            |\n"); //ok 
	printf("|          2.删除菜品            |\n"); //ok 
	printf("|          3.结账删除            |\n");
	printf("|          4.查看订单            |\n");
	printf("----------------------------------\n");
	printf("已有菜品%d个:\n\n",count);

    printf("请输入管理员登入密码：\n");
	scanf("%d",&code);

    if(code == 6666)    // 密码匹配则进入管理员界面
    {
        printf("恭喜你登入成功！！！\n");
		printf("请输入你的选择【1-4】:\n");
        scanf("%d", &choice);

        if(choice == 1) // 添加菜品，即增加链表元素，并保存到文档中
        {
            printf("请输入添加菜品数:\n");
            scanf("%d", &n);
            
            MENU *s;
            for(int i = 0; i < n; i ++ )    // 增加链表节点
            {
                s = (MENU*)malloc(sizeof(MENU));

                printf("----------------------------------------\n");
				printf("请输入第%d个菜品编码:\n", i + 1);
                scanf("%d", &s -> code);
                printf("请输入第%d个菜品名:\n",i+1);
                scanf("%s", s -> name);
                printf("请输入价格\n");
                scanf("%d", &s -> price);
                count ++ ;      // 总菜品数+1

                s -> next = menu;   // 将新节点放入链表尾
                menu = s;
            }
            SaveMenu();  // 保存菜单
        }
        else if(choice == 2)    // 删除菜品，即删除链表节点
        {
            printf("----------------------------------------\n");
			printf("输入要删除的菜品编码:\n");
            scanf("%d", &code);

            int flag = 0;
            if(menu -> code == code)    // 若为首节点，直接删除首节点
            {
                menu = menu -> next;
                flag = 1;
                count -- ;
            }
            else    // 否则使用一个辅助节点，对菜品链表节点进行删除
                for(MENU *i = menu; i; i = i -> next)
                {
                    MENU *j = NULL;
                    if(i -> next)   
                        j = i -> next;
                    if(j -> code == code)
                    {
                    	count -- ;
                        i -> next = j -> next;
                        j -> next = NULL;
                        free(j);     // 释放内存

                        flag = 1;
                        break;
                    }
                }

            if(flag == 1)
                printf("删除成功！\n");
            else if(flag == 0)
                printf("不存在指定节点，删除失败！\n");
                
            SaveMenu();  // 保存菜单
        }
        else if(choice == 3)    // 结账，删除desk中的节点
        {
            printf("--------------------------------------\n");
			printf("请输入结账的桌号：\n");
            scanf("%d", &code);

            int flag = 0;
            if(desk -> deskCode == code)    // 首节点直接将其置后
            {
                desk = desk -> next;
                flag = 1;
            }
            else    // 否则使用一个辅助节点对节点进行删除
                for(DESK *i = desk; i; i = i -> next)
                {
                    DESK *j = NULL;
                    if(i -> next)
                        j = i -> next;
                    if(j -> deskCode == code)
                    {
                        i -> next = j -> next;
                        j -> next = NULL;
                        
                        free(j);

                        flag = 1;
                        break;
                    }
                }

            if(flag == 1)
            {
                printf("结账成功！\n");
                SaveDesk();
            }
            else if(flag == 0)
                printf("结账桌号不存在，请检查后重试！\n");
        }
        else if(choice == 4)    // 查询餐桌状况
        {
			printf("%5s","餐桌号码");
			printf("%15s","餐桌人数");
			printf("%13s","点菜道数");
			printf("%11s","需付金额");
			printf("%20s","所点菜品"); 
			printf("\n"); 
			printf("-------------------------------------------------------------------------\n");

            for(DESK *i = desk; i; i = i -> next)
            {
                printf("%4d号", i -> deskCode);
                printf("%11d人", i -> peopleNum);
                printf("%11d道", i -> purchasedDishNum);
                printf("%11dRmb", i -> totalPrice);
                printf("\t\t");

                for(MENU *j = i -> purchasedDish; j; j = j -> next)
                    printf("%s ", j -> name);
                printf("\n");
            }


        }
    }
    else
    {
        printf("密码错误!请重新输入\n");
    }
}

void MainMenu()     // 主菜单
{
    int choice;
    printf("----------------------------------------\n");
	printf("|              欢迎来到XX餐厅            |\n"); 
	printf("----------------------------------------\n");
	printf("请问你是餐厅管理者还是点餐者【1 or 2】:\n");
	scanf("%d", &choice);

    while(choice != 1 && choice != 2)
    {
        printf("输入错误，请重新再输入:\n");
        scanf("%d", &choice);
    }

    if(choice == 1)
        AdminMenu();
    else if(choice == 2)
        ClientMenu();

}

int main()
{
    ReadMenu();
    ReadDesk();
    //MenuOutput();
    while(1)
        MainMenu();
    return 0;
}