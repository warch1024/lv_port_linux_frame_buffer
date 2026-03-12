#include "figure_vending_machine/goods_mangm_con.h"

// 声明全局变量（在goods_mangm.c中定义）
extern tools_dll_t * listed_goods;

// 商品管理交互窗口
void goods_management_console(void) {
    int choice;
    char input_buffer[256];
    
    printf("\n=== 自动售货机商品管理系统 ===\n");
    printf("1. 打印上架商品列表信息\n");
    printf("2. 上架新商品\n");
    printf("3. 修改已存在商品信息\n");
    printf("4. 删除商品\n");
    printf("5. 查找商品\n");
    printf("0. 返回上级菜单\n");
    printf("================================\n");
    
    while(1) {
        printf("\n请选择操作 (0-5): ");
        fflush(stdout);
        
        if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
            choice = atoi(input_buffer);
            
            switch(choice) {
                case 1:
                    print_goods_info_interactive();
                    break;
                case 2:
                    add_new_goods_interactive();
                    break;
                case 3:
                    modify_existing_goods_interactive();
                    break;
                case 4:
                    delete_goods_interactive();
                    break;
                case 5:
                    search_goods_interactive();
                    break;
                case 0:
                    printf("返回上级菜单...\n");
                    return;
                default:
                    printf("无效选择，请输入 0-5 之间的数字\n");
                    break;
            }
        }
    }
}

// 1. 打印商品信息的交互版本
void print_goods_info_interactive(void) {
    printf("\n=== 当前上架商品列表 ===\n");
    
    printf("\n=== 详细信息 ===\n");
    if(listed_goods && listed_goods->next) {
        int count = 0;
        for(tools_dll_t * tmp_node = listed_goods->next; 
            tmp_node != NULL; tmp_node = tmp_node->next) {
            count++;
            goods_info_t * goods = (goods_info_t*)(tmp_node->data);
            printf("商品 %d:\n", count);
            printf("  名称: %s\n", goods->title);
            printf("  价格: %d 元\n", goods->price);
            printf("  库存: %d 件\n", goods->num);
            printf("  图片路径: %s\n", goods->goods_pic_path);
            printf("------------------------\n");
        }
        printf("总计: %d 件商品\n", count);
    } else {
        printf("暂无商品信息\n");
    }
}

// 2. 上架新商品的交互函数
void add_new_goods_interactive(void) {
    char input_buffer[256];
    goods_info_t * new_goods = (goods_info_t*)malloc(sizeof(goods_info_t));
    
    if(!new_goods) {
        printf("内存分配失败！\n");
        return;
    }
    
    // 初始化新商品
    memset(new_goods, 0, sizeof(goods_info_t));
    
    printf("\n=== 上架新商品 ===\n");
    
    // 输入商品名称
    printf("请输入商品名称: ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        strncpy(new_goods->title, input_buffer, sizeof(new_goods->title) - 1);
        new_goods->title[sizeof(new_goods->title) - 1] = '\0';
    }
    
    // 输入商品价格
    printf("请输入商品价格(元): ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        new_goods->price = atoi(input_buffer);
        if(new_goods->price <= 0) {
            printf("错误：价格必须为正数\n");
            free(new_goods);
            return;
        }
    }
    
    // 输入库存数量
    printf("请输入库存数量: ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        new_goods->num = atoi(input_buffer);
        if(new_goods->num < 0) {
            printf("错误：库存不能为负数\n");
            free(new_goods);
            return;
        }
    }
    
    // 输入图片路径
    printf("请输入图片路径 (可选，直接回车使用默认): ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        if(strlen(input_buffer) > 0) {
            strncpy(new_goods->goods_pic_path, input_buffer, sizeof(new_goods->goods_pic_path) - 1);
            new_goods->goods_pic_path[sizeof(new_goods->goods_pic_path) - 1] = '\0';
        } else {
            // 使用默认图片路径
            strcpy(new_goods->goods_pic_path, "S:/IOT/projects/26-1-30/resources/item1.jpg");
        }
    }
    
    // 添加到商品链表
    goods_info_t * result = list_listed_goods_list(listed_goods, *new_goods);
    if(result) {
        printf("✓ 商品添加成功！\n");
        printf("商品信息：\n");
        printf("  名称: %s\n", new_goods->title);
        printf("  价格: %d 元\n", new_goods->price);
        printf("  库存: %d 件\n", new_goods->num);
        printf("  图片: %s\n", new_goods->goods_pic_path);
    } else {
        printf("✗ 商品添加失败！\n");
        free(new_goods);
    }
}

// 3. 修改已存在商品信息的交互函数
void modify_existing_goods_interactive(void) {
    char input_buffer[256];
    char goods_name[40];
    
    printf("\n=== 修改商品信息 ===\n");
    printf("请输入要修改的商品名称: ");
    fflush(stdout);
    
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = 0;
    strcpy(goods_name, input_buffer);
    
    // 查找商品
    goods_info_t * target_goods = find_listed_goods_list(listed_goods, goods_name);
    if(!target_goods) {
        printf("错误：找不到名为 '%s' 的商品！\n", goods_name);
        return;
    }
    
    printf("找到商品：%s\n", target_goods->title);
    printf("当前信息：\n");
    printf("  价格: %d 元\n", target_goods->price);
    printf("  库存: %d 件\n", target_goods->num);
    printf("  图片: %s\n", target_goods->goods_pic_path);
    
    // 选择要修改的字段
    printf("\n请选择要修改的项目：\n");
    printf("1. 商品名称\n");
    printf("2. 价格\n");
    printf("3. 库存\n");
    printf("4. 图片路径\n");
    printf("5. 全部修改\n");
    printf("0. 取消修改\n");
    
    printf("请选择 (0-5): ");
    fflush(stdout);
    
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return;
    }
    
    int modify_choice = atoi(input_buffer);
    
    uint32_t new_num = target_goods->num;
    uint32_t new_price = target_goods->price;
    char new_title[40];
    char new_pic_path[100];
    
    strcpy(new_title, target_goods->title);
    strcpy(new_pic_path, target_goods->goods_pic_path);
    
    switch(modify_choice) {
        case 1: // 修改名称
            printf("请输入新名称: ");
            fflush(stdout);
            if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                input_buffer[strcspn(input_buffer, "\n")] = 0;
                strncpy(new_title, input_buffer, sizeof(new_title) - 1);
                new_title[sizeof(new_title) - 1] = '\0';
            }
            break;
            
        case 2: // 修改价格
            printf("请输入新价格: ");
            fflush(stdout);
            if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                int price = atoi(input_buffer);
                if(price > 0) {
                    new_price = price;
                    printf("✓ 价格修改成功\n");
                } else {
                    printf("错误：价格必须为正数\n");
                    return;
                }
            }
            break;
            
        case 3: // 修改库存
            printf("请输入新库存: ");
            fflush(stdout);
            if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                int num = atoi(input_buffer);
                if(num >= 0) {
                    new_num = num;
                    printf("✓ 库存修改成功\n");
                } else {
                    printf("错误：库存不能为负数\n");
                    return;
                }
            }
            break;
            
        case 4: // 修改图片路径
            printf("请输入新图片路径: ");
            fflush(stdout);
            if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                input_buffer[strcspn(input_buffer, "\n")] = 0;
                strncpy(new_pic_path, input_buffer, sizeof(new_pic_path) - 1);
                new_pic_path[sizeof(new_pic_path) - 1] = '\0';
                printf("✓ 图片路径修改成功\n");
            }
            break;
            
        case 5: // 全部修改
            modify_goods_all_fields_interactive(target_goods, new_title, &new_price, &new_num, new_pic_path);
            break;
            
        case 0:
            printf("取消修改\n");
            return;
            
        default:
            printf("无效选择\n");
            return;
    }
    
    // 执行修改
    if(modify_choice != 5) {  // 如果不是全部修改（全部修改已在函数内处理）
        int result = modify_listed_goods_list(listed_goods, goods_name, new_num, new_price);
        if(result) {
            // 更新其他字段
            strcpy(target_goods->title, new_title);
            strcpy(target_goods->goods_pic_path, new_pic_path);
            printf("✓ 商品信息修改成功\n");
        } else {
            printf("✗ 商品信息修改失败\n");
        }
    }
}

// 辅助函数：全部字段修改
void modify_goods_all_fields_interactive(goods_info_t * goods, char * new_title, 
                                        uint32_t * new_price, uint32_t * new_num, 
                                        char * new_pic_path) {
    char input_buffer[256];
    
    printf("=== 全字段修改 ===\n");
    
    // 修改名称
    printf("当前名称: %s\n", goods->title);
    printf("请输入新名称 (回车跳过): ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if(strlen(input_buffer) > 1) {
            input_buffer[strcspn(input_buffer, "\n")] = 0;
            strncpy(new_title, input_buffer, sizeof(goods->title) - 1);
            new_title[sizeof(goods->title) - 1] = '\0';
        }
    }
    
    // 修改价格
    printf("当前价格: %d 元\n", goods->price);
    printf("请输入新价格 (回车跳过): ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if(strlen(input_buffer) > 1) {
            int price = atoi(input_buffer);
            if(price > 0) {
                *new_price = price;
            }
        }
    }
    
    // 修改库存
    printf("当前库存: %d 件\n", goods->num);
    printf("请输入新库存 (回车跳过): ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if(strlen(input_buffer) > 1) {
            int num = atoi(input_buffer);
            if(num >= 0) {
                *new_num = num;
            }
        }
    }
    
    // 修改图片路径
    printf("当前图片: %s\n", goods->goods_pic_path);
    printf("请输入新图片路径 (回车跳过): ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if(strlen(input_buffer) > 1) {
            input_buffer[strcspn(input_buffer, "\n")] = 0;
            strncpy(new_pic_path, input_buffer, sizeof(goods->goods_pic_path) - 1);
            new_pic_path[sizeof(goods->goods_pic_path) - 1] = '\0';
        }
    }
    
    // 执行修改
    int result = modify_listed_goods_list(listed_goods, goods->title, *new_num, *new_price);
    if(result) {
        strcpy(goods->title, new_title);
        strcpy(goods->goods_pic_path, new_pic_path);
        printf("✓ 所有字段修改完成\n");
    } else {
        printf("✗ 修改失败\n");
    }
}

// 4. 删除商品的交互函数
void delete_goods_interactive(void) {
    char input_buffer[256];
    char goods_name[40];
    
    printf("\n=== 删除商品 ===\n");
    printf("请输入要删除的商品名称: ");
    fflush(stdout);
    
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = 0;
    strcpy(goods_name, input_buffer);
    
    goods_info_t * target_goods = find_listed_goods_list(listed_goods, goods_name);
    if(!target_goods) {
        printf("错误：找不到名为 '%s' 的商品！\n", goods_name);
        return;
    }
    
    printf("找到商品：%s\n", target_goods->title);
    printf("价格: %d 元, 库存: %d 件\n", target_goods->price, target_goods->num);
    
    printf("确认删除此商品？(y/N): ");
    fflush(stdout);
    
    if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if(input_buffer[0] == 'y' || input_buffer[0] == 'Y') {
            // 从链表中删除
            goods_info_t * result = delete_listed_goods_list(listed_goods, target_goods);
            if(result) {
                printf("✓ 商品删除成功\n");
            } else {
                printf("✗ 商品删除失败\n");
            }
        } else {
            printf("取消删除\n");
        }
    }
}

// 5. 查找商品的交互函数
void search_goods_interactive(void) {
    char input_buffer[256];
    int search_choice;
    
    printf("\n=== 查找商品 ===\n");
    printf("1. 按名称查找\n");
    printf("2. 按价格范围查找\n");
    printf("3. 显示所有商品\n");
    printf("请选择查找方式 (1-3): ");
    fflush(stdout);
    
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return;
    }
    
    search_choice = atoi(input_buffer);
    
    switch(search_choice) {
        case 1: // 按名称查找
            printf("请输入商品名称关键词: ");
            fflush(stdout);
            if(fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                input_buffer[strcspn(input_buffer, "\n")] = 0;
                search_goods_by_name_interactive(input_buffer);
            }
            break;
            
        case 2: // 按价格范围查找
            search_goods_by_price_range_interactive();
            break;
            
        case 3: // 显示所有商品
            print_goods_info_interactive();
            break;
            
        default:
            printf("无效选择\n");
            break;
    }
}

// 按名称查找商品（交互版）
void search_goods_by_name_interactive(const char * keyword) {
    if(!listed_goods || !listed_goods->next) {
        printf("暂无商品信息\n");
        return;
    }
    
    int found_count = 0;
    printf("\n包含 '%s' 的商品：\n", keyword);
    
    for(tools_dll_t * tmp_node = listed_goods->next; 
        tmp_node != NULL; tmp_node = tmp_node->next) {
        goods_info_t * goods = (goods_info_t*)(tmp_node->data);
        if(strstr(goods->title, keyword)) {
            printf("  名称: %s\n", goods->title);
            printf("  价格: %d 元\n", goods->price);
            printf("  库存: %d 件\n", goods->num);
            printf("  图片: %s\n", goods->goods_pic_path);
            printf("------------------------\n");
            found_count++;
        }
    }
    
    if(found_count == 0) {
        printf("未找到包含 '%s' 的商品\n", keyword);
    } else {
        printf("共找到 %d 件商品\n", found_count);
    }
}

// 按价格范围查找（交互版）
void search_goods_by_price_range_interactive(void) {
    char input_buffer[256];
    int min_price, max_price;
    
    printf("请输入最低价格: ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) return;
    min_price = atoi(input_buffer);
    
    printf("请输入最高价格: ");
    fflush(stdout);
    if(fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) return;
    max_price = atoi(input_buffer);
    
    if(min_price > max_price) {
        printf("错误：最低价格不能大于最高价格\n");
        return;
    }
    
    printf("\n价格范围 %d-%d 元的商品：\n", min_price, max_price);
    search_goods_by_price_range(min_price, max_price);
}

// 按价格范围查找实现
void search_goods_by_price_range(int min_price, int max_price) {
    if(!listed_goods || !listed_goods->next) {
        printf("暂无商品信息\n");
        return;
    }
    
    int found_count = 0;
    for(tools_dll_t * tmp_node = listed_goods->next; 
        tmp_node != NULL; tmp_node = tmp_node->next) {
        goods_info_t * goods = (goods_info_t*)(tmp_node->data);
        if((int)goods->price >= min_price && (int)goods->price <= max_price) {
            if(found_count == 0) {
                printf("找到以下商品：\n");
            }
            printf("  名称: %s, 价格: %d 元, 库存: %d 件\n", 
                   goods->title, goods->price, goods->num);
            found_count++;
        }
    }
    
    if(found_count == 0) {
        printf("未找到价格在 %d-%d 元范围内的商品\n", min_price, max_price);
    } else {
        printf("共找到 %d 件商品\n", found_count);
    }
}





// 菜单入口函数
void show_goods_management_menu(void) {
    goods_management_console();
}