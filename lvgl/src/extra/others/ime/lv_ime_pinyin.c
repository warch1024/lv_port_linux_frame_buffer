/**
 * @file lv_ime_pinyin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_ime_pinyin.h"
#if LV_USE_IME_PINYIN != 0

#include <stdio.h>

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS    &lv_ime_pinyin_class

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_ime_pinyin_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_ime_pinyin_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_ime_pinyin_style_change_event(lv_event_t * e);
static void lv_ime_pinyin_kb_event(lv_event_t * e);
static void lv_ime_pinyin_cand_panel_event(lv_event_t * e);

static void init_pinyin_dict(lv_obj_t * obj, lv_pinyin_dict_t * dict);
static void pinyin_input_proc(lv_obj_t * obj);
static void pinyin_page_proc(lv_obj_t * obj, uint16_t btn);
static char * pinyin_search_matching(lv_obj_t * obj, char * py_str, uint16_t * cand_num);
static void pinyin_ime_clear_data(lv_obj_t * obj);

#if LV_IME_PINYIN_USE_K9_MODE
    static void pinyin_k9_init_data(lv_obj_t * obj);
    static void pinyin_k9_get_legal_py(lv_obj_t * obj, char * k9_input, const char * py9_map[]);
    static bool pinyin_k9_is_valid_py(lv_obj_t * obj, char * py_str);
    static void pinyin_k9_fill_cand(lv_obj_t * obj);
    static void pinyin_k9_cand_page_proc(lv_obj_t * obj, uint16_t dir);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_ime_pinyin_class = {
    .constructor_cb = lv_ime_pinyin_constructor,
    .destructor_cb  = lv_ime_pinyin_destructor,
    .width_def      = LV_SIZE_CONTENT,
    .height_def     = LV_SIZE_CONTENT,
    .group_def      = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size  = sizeof(lv_ime_pinyin_t),
    .base_class     = &lv_obj_class
};

#if LV_IME_PINYIN_USE_K9_MODE
static char * lv_btnm_def_pinyin_k9_map[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 20] = {\
                                                                                ",\0", "1#\0",  "abc \0", "def\0",  LV_SYMBOL_BACKSPACE"\0", "\n\0",
                                                                                ".\0", "ghi\0", "jkl\0", "mno\0",  LV_SYMBOL_KEYBOARD"\0", "\n\0",
                                                                                "?\0", "pqrs\0", "tuv\0", "wxyz\0",  LV_SYMBOL_NEW_LINE"\0", "\n\0",
                                                                                LV_SYMBOL_LEFT"\0", "\0"
                                                                               };

static lv_btnmatrix_ctrl_t default_kb_ctrl_k9_map[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 16] = { 1 };
static char   lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 2][LV_IME_PINYIN_K9_MAX_INPUT] = {0};
#endif

static char   lv_pinyin_cand_str[LV_IME_PINYIN_CAND_TEXT_NUM][4];
static char * lv_btnm_def_pinyin_sel_map[LV_IME_PINYIN_CAND_TEXT_NUM + 3];

#if LV_IME_PINYIN_USE_DEFAULT_DICT
// ...existing code...
lv_pinyin_dict_t lv_ime_pinyin_def_dict[] = {
    { "a", "啊阿安爱按" },
    { "ai", "爱矮挨哎碍艾" },
    { "an", "安按案暗岸" },
    { "ang", "昂盎肮" },
    { "ao", "奥傲熬敖翱" },
    { "ba", "把八吧爸拔罢" },
    { "bai", "白百败摆柏" },
    { "ban", "办半班般板版" },
    { "bang", "帮棒榜邦绑" },
    { "bao", "包报保宝抱爆" },
    { "bei", "北被备倍背悲杯" },
    { "ben", "本奔笨苯" },
    { "bi", "比必笔毕避闭彼" },
    { "bian", "变便边编遍辩" },
    { "biao", "表标彪飙" },
    { "bie", "别憋瘪" },
    { "bin", "宾滨斌彬" },
    { "bing", "并兵病冰饼丙" },
    { "bo", "波播博勃玻薄" },
    { "bu", "不部步布补捕" },
    { "ca", "擦猜" },
    { "cai", "才采财材菜裁" },
    { "can", "参残惨餐灿" },
    { "cang", "藏仓沧舱" },
    { "cao", "草操糙槽" },
    { "ce", "册侧测策厕" },
    { "cen", "岑" },
    { "ceng", "层曾蹭" },
    { "cha", "查差插茶察叉" },
    { "chai", "柴拆差" },
    { "chan", "产缠掺蝉馋" },
    { "chang", "长常场厂唱畅昌偿倡" },
    { "chao", "超朝潮炒抄" },
    { "che", "车彻撤扯" },
    { "chen", "陈晨沉臣尘趁称" },
    { "cheng", "成程城承乘诚称" },
    { "chi", "吃持迟尺赤齿池" },
    { "chong", "充冲虫崇宠" },
    { "chu", "出处初除楚储触" },
    { "chuan", "传穿船川串" },
    { "chuang", "创窗床闯" },
    { "chui", "吹垂锤" },
    { "chun", "春纯唇醇" },
    { "chuo", "戳绰" },
    { "ci", "此次词辞刺瓷" },
    { "cong", "从聪丛葱" },
    { "cou", "凑" },
    { "cu", "粗促醋" },
    { "cuan", "窜篡蹿" },
    { "cui", "催脆崔翠粹" },
    { "cun", "村存寸" },
    { "cuo", "错措撮搓" },
    { "da", "大达打答搭" },
    { "dai", "带代待戴贷袋" },
    { "dan", "但单担弹蛋淡胆" },
    { "dang", "当党档挡荡" },
    { "dao", "到道导倒刀岛" },
    { "de", "的得德" },
    { "deng", "等灯登邓" },
    { "di", "地第低底敌递弟帝滴" },
    { "dian", "点电店典殿垫" },
    { "diao", "调掉雕钓吊" },
    { "die", "跌叠蝶谍" },
    { "ding", "定顶丁订钉鼎" },
    { "dong", "东动冬懂洞董" },
    { "dou", "都斗豆抖逗" },
    { "du", "度都读独毒渡堵" },
    { "duan", "断段短锻端" },
    { "dui", "对队堆兑" },
    { "dun", "顿盾吨蹲敦" },
    { "duo", "多夺朵躲舵掇" },
    { "e", "饿额恶俄鹅" },
    { "en", "恩嗯" },
    { "er", "而二儿耳尔" },
    { "fa", "发法罚乏伐" },
    { "fan", "反饭翻犯范凡繁" },
    { "fang", "方放房防访仿" },
    { "fei", "非飞肥费废肺" },
    { "fen", "分份粉奋纷愤" },
    { "feng", "风封峰丰奉疯" },
    { "fou", "否" },
    { "fu", "服复福负富副父付妇幅浮扶符" },
    { "ga", "嘎尬" },
    { "gai", "该改盖概" },
    { "gan", "干感敢赶甘" },
    { "gang", "刚钢港纲岗" },
    { "gao", "高告稿搞膏" },
    { "ge", "个各歌格割革隔" },
    { "gei", "给" },
    { "gen", "根跟" },
    { "geng", "更耕梗" },
    { "gong", "工公共功供攻宫" },
    { "gou", "够购构狗沟" },
    { "gu", "古故顾股鼓固估" },
    { "gua", "挂瓜刮寡" },
    { "guai", "怪拐乖" },
    { "guan", "关管观馆惯官冠" },
    { "guang", "光广逛" },
    { "gui", "归贵规鬼柜桂" },
    { "gun", "滚棍" },
    { "guo", "国过果锅郭" },
    { "ha", "哈蛤" },
    { "hai", "还海害孩" },
    { "han", "汉含汗寒喊" },
    { "hang", "行航杭巷" },
    { "hao", "好号毫豪耗浩" },
    { "he", "和合河喝核何" },
    { "hei", "黑嘿" },
    { "hen", "很狠恨痕" },
    { "heng", "横衡恒哼" },
    { "hong", "红洪宏轰虹" },
    { "hou", "后候厚猴喉" },
    { "hu", "湖户护互呼胡虎" },
    { "hua", "化花华画划滑" },
    { "huai", "坏怀淮槐" },
    { "huan", "还换环欢缓患" },
    { "huang", "黄皇慌晃谎" },
    { "hui", "会回灰汇挥惠慧毁辉徽" },
    { "hun", "混婚魂昏" },
    { "huo", "或活火获货伙" },
    { "ji", "机几及己记级集计极技基纪即济激既继击鸡迹季急寄" },
    { "jia", "家加假价架甲佳嘉" },
    { "jian", "见件间建坚检减简剪剑监渐" },
    { "jiang", "将讲江奖降蒋浆" },
    { "jiao", "教交角较叫觉脚胶焦" },
    { "jie", "接结节解界借街皆姐介阶" },
    { "jin", "进近今仅金尽紧斤禁" },
    { "jing", "经京精竟警景静敬镜井" },
    { "jiu", "就九旧久酒救纠" },
    { "ju", "据局举句聚居拒剧" },
    { "juan", "卷捐倦娟" },
    { "jue", "决觉绝角掘爵" },
    { "jun", "军均君俊菌" },
    { "ka", "卡喀咖" },
    { "kai", "开凯慨揩楷" },
    { "kan", "看刊砍堪" },
    { "kang", "康抗慷炕" },
    { "kao", "考靠烤拷" },
    { "ke", "可科克客刻课颗壳渴" },
    { "ken", "肯恳垦啃" },
    { "keng", "坑吭铿" },
    { "kong", "空控孔恐" },
    { "kou", "口扣寇" },
    { "ku", "苦库酷裤枯哭" },
    { "kua", "跨夸垮" },
    { "kuai", "快块筷会" },
    { "kuan", "宽款" },
    { "kuang", "矿狂况框旷" },
    { "kui", "亏愧奎葵魁" },
    { "kun", "困昆捆坤" },
    { "kuo", "扩阔括廓" },
    { "la", "拉啦腊辣喇蜡" },
    { "lai", "来赖莱" },
    { "lan", "蓝兰烂栏揽览" },
    { "lang", "浪朗郎廊狼" },
    { "lao", "老劳牢捞姥" },
    { "le", "了乐勒肋" },
    { "lei", "类雷泪累垒擂" },
    { "leng", "冷棱楞" },
    { "li", "里理力利立李历离礼丽黎粒例励" },
    { "lian", "连练联脸莲恋链" },
    { "liang", "两量良亮梁粮凉辆" },
    { "liao", "了料疗辽僚聊" },
    { "lie", "列烈裂猎劣" },
    { "lin", "林临邻琳麟磷" },
    { "ling", "领令零龄灵铃岭凌玲" },
    { "liu", "流六留刘柳硫" },
    { "long", "龙隆垄聋笼" },
    { "lou", "楼漏搂篓" },
    { "lu", "路录陆露炉卢鲁鹿" },
    { "lv", "旅律绿率履虑" },
    { "luan", "乱卵峦" },
    { "lue", "略掠" },
    { "lun", "论轮伦仑" },
    { "luo", "落罗络逻锣骆" },
    { "ma", "妈马吗麻码骂嘛" },
    { "mai", "买卖麦迈埋脉" },
    { "man", "满慢曼漫瞒" },
    { "mang", "忙芒盲茫莽" },
    { "mao", "毛猫矛冒贸帽貌" },
    { "me", "么麽" },
    { "mei", "没每美妹眉梅媒枚煤" },
    { "men", "们门闷" },
    { "meng", "梦蒙猛盟孟" },
    { "mi", "米密迷秘弥蜜" },
    { "mian", "面棉免眠绵勉" },
    { "miao", "秒苗庙描妙" },
    { "mie", "灭蔑" },
    { "min", "民敏闽" },
    { "ming", "明名命鸣铭" },
    { "mo", "没模末莫磨墨默摸" },
    { "mou", "某谋牟" },
    { "mu", "母目木亩牧墓幕慕" },
    { "na", "那拿哪纳娜" },
    { "nai", "乃奶耐奈" },
    { "nan", "南难男楠" },
    { "nang", "囊" },
    { "nao", "脑闹恼挠" },
    { "ne", "呢" },
    { "nei", "内那哪" },
    { "nen", "嫩" },
    { "neng", "能" },
    { "ni", "你泥拟逆妮倪" },
    { "nian", "年念粘碾" },
    { "niang", "娘酿" },
    { "nin", "您" },
    { "ning", "宁凝拧" },
    { "niu", "牛纽扭钮" },
    { "nong", "农浓弄" },
    { "nu", "女奴努怒" },
    { "nuan", "暖" },
    { "nue", "虐" },
    { "nuo", "诺挪懦糯" },
    { "o", "哦欧偶" },
    { "ou", "欧偶殴藕呕" },
    { "pa", "怕爬帕扒趴" },
    { "pai", "派排拍牌" },
    { "pan", "判盘盼攀" },
    { "pang", "旁胖庞" },
    { "pao", "跑炮泡抛" },
    { "pei", "配陪培佩赔沛" },
    { "pen", "喷盆" },
    { "peng", "朋碰棚蓬捧" },
    { "pi", "皮批披疲脾匹" },
    { "pian", "片篇骗偏" },
    { "piao", "票漂飘" },
    { "pie", "撇瞥" },
    { "pin", "品贫拼频" },
    { "ping", "平评瓶凭萍" },
    { "po", "破迫坡颇婆泼" },
    { "pu", "普铺朴葡蒲谱仆" },
    { "qi", "起其期气七器奇企汽骑妻旗棋齐" },
    { "qia", "恰洽掐" },
    { "qian", "前千钱签浅迁牵欠" },
    { "qiang", "强抢墙枪腔" },
    { "qiao", "桥巧悄敲乔侨" },
    { "qie", "切且窃" },
    { "qin", "亲琴勤侵秦芹" },
    { "qing", "请情清青轻庆晴" },
    { "qiong", "穷琼" },
    { "qiu", "求球秋丘囚" },
    { "qu", "去取区曲趣屈驱渠" },
    { "quan", "全权圈劝泉拳犬券" },
    { "que", "却缺确雀" },
    { "qun", "群裙" },
    { "ran", "然燃染冉" },
    { "rang", "让嚷壤" },
    { "rao", "绕扰" },
    { "re", "热惹" },
    { "ren", "人任认仁忍韧" },
    { "reng", "仍扔" },
    { "ri", "日" },
    { "rong", "容荣融绒溶熔" },
    { "rou", "肉柔揉" },
    { "ru", "如入乳辱" },
    { "ruan", "软阮" },
    { "rui", "瑞锐蕊" },
    { "run", "润" },
    { "ruo", "若弱" },
    { "sa", "撒洒萨" },
    { "sai", "赛塞腮" },
    { "san", "三散伞叁" },
    { "sang", "桑丧嗓" },
    { "sao", "扫骚嫂" },
    { "se", "色涩瑟" },
    { "sen", "森" },
    { "sha", "杀沙傻啥纱" },
    { "shai", "晒筛" },
    { "shan", "山善闪衫删扇珊" },
    { "shang", "上商尚伤赏" },
    { "shao", "少烧绍稍勺" },
    { "she", "社设舍射涉舌蛇" },
    { "shen", "深身神申甚审慎" },
    { "sheng", "生声省胜升圣剩" },
    { "shi", "是时事十使世市师识史试石诗式室示适食视失施释氏士湿狮驶拾誓释饰尸驶" },
    { "shou", "手受收首守授售瘦寿" },
    { "shu", "书树数属输熟束述舒鼠叔术" },
    { "shua", "刷耍" },
    { "shuai", "帅甩摔衰" },
    { "shuan", "栓拴" },
    { "shuang", "双霜爽" },
    { "shui", "水谁睡税" },
    { "shun", "顺瞬舜" },
    { "shuo", "说硕烁朔" },
    { "si", "四死思斯司私丝寺似饲" },
    { "song", "送松宋诵颂" },
    { "sou", "搜艘嗽" },
    { "su", "苏素速诉俗宿塑肃酥粟" },
    { "suan", "算酸蒜" },
    { "sui", "随岁碎虽遂隋穗绥" },
    { "sun", "孙损笋荪" },
    { "suo", "所索锁缩梭琐" },
    { "ta", "他她它塔踏塌" },
    { "tai", "太台态抬泰胎苔" },
    { "tan", "谈探弹坦摊贪叹炭谭潭" },
    { "tang", "堂糖唐汤躺趟烫" },
    { "tao", "讨套逃桃陶涛掏" },
    { "te", "特忑" },
    { "teng", "腾疼藤誊" },
    { "ti", "提题体替梯踢剃蹄啼" },
    { "tian", "天田填甜添" },
    { "tiao", "条跳挑调" },
    { "tie", "铁贴帖" },
    { "ting", "听停厅挺亭庭艇" },
    { "tong", "同通统童痛铜桶桐" },
    { "tou", "头投透偷" },
    { "tu", "土图突徒途涂吐兔" },
    { "tuan", "团湍" },
    { "tui", "推退腿褪" },
    { "tun", "吞屯囤" },
    { "tuo", "托脱拖妥驼拓" },
    { "wa", "挖娃瓦蛙哇" },
    { "wai", "外歪" },
    { "wan", "完万晚玩湾碗弯挽" },
    { "wang", "王往望忘网旺汪" },
    { "wei", "为位未委围维卫微伟唯味威尾违魏胃" },
    { "wen", "文问闻温稳吻纹蚊" },
    { "weng", "翁瓮" },
    { "wo", "我握窝卧沃蜗" },
    { "wu", "无五物务武舞误污悟雾屋午吴伍吾勿巫乌" },
    { "xi", "西系席习细喜希洗吸戏析悉惜稀袭溪锡膝昔熙牺" },
    { "xia", "下夏吓霞峡侠瞎辖" },
    { "xian", "先现线显县限鲜献险闲贤咸衔掀" },
    { "xiang", "想向象项响香乡详像箱享降祥湘" },
    { "xiao", "小笑校消效销晓肖孝削萧" },
    { "xie", "写谢协鞋斜携泄械卸蟹邪胁" },
    { "xin", "新心信欣薪辛芯" },
    { "xing", "行性型形星兴醒幸姓腥杏刑" },
    { "xiong", "雄兄胸熊凶" },
    { "xiu", "修休秀袖绣宿羞锈" },
    { "xu", "许需续序虚徐须绪叙畜旭蓄絮酗" },
    { "xuan", "选宣旋悬玄炫" },
    { "xue", "学雪血穴薛靴削" },
    { "xun", "寻训迅讯巡询循旬熏勋" },
    { "ya", "亚压呀牙雅押芽鸭涯" },
    { "yan", "言眼烟研严盐演岩延验燕炎宴沿掩雁艳" },
    { "yang", "样养阳洋扬羊仰氧央秧" },
    { "yao", "要药摇腰咬遥邀耀窑谣" },
    { "ye", "也业夜叶爷野液页" },
    { "yi", "一以已义意依议易医艺益移遗疑仪亿忆译乙衣宜姨椅翼亦异伊壹" },
    { "yin", "因引音银印饮阴隐吟殷" },
    { "ying", "应英影营迎映盈硬赢鹰颖" },
    { "yo", "哟" },
    { "yong", "用永勇涌泳庸拥咏雍" },
    { "you", "有又由友右优游油邮幼尤忧诱犹悠" },
    { "yu", "与于语育鱼雨玉遇预余愈宇域誉渔羽欲狱舆豫御" },
    { "yuan", "元原员远院愿圆源缘援冤园" },
    { "yue", "月越约阅跃悦岳粤" },
    { "yun", "云运允晕韵孕匀" },
    { "za", "杂砸咋咱" },
    { "zai", "在再载灾宰栽" },
    { "zan", "赞暂攒咱" },
    { "zang", "藏脏葬" },
    { "zao", "早造遭糟燥澡躁" },
    { "ze", "则责择泽" },
    { "zei", "贼" },
    { "zen", "怎" },
    { "zeng", "增赠憎曾" },
    { "zha", "查炸扎渣诈闸榨" },
    { "zhai", "宅债摘窄斋" },
    { "zhan", "展站战占粘沾斩盏" },
    { "zhang", "长张章掌涨帐障" },
    { "zhao", "找照招赵召兆罩" },
    { "zhe", "这者着折哲浙遮" },
    { "zhen", "真镇针震珍诊阵枕侦" },
    { "zheng", "正政证争整征症" },
    { "zhi", "之只知直指制治至质志纸支值织职植执止脂智枝址致置旨芝" },
    { "zhong", "中种重众终钟忠肿仲" },
    { "zhou", "周州洲舟粥轴昼宙骤" },
    { "zhu", "主住注助逐竹猪祝驻著筑煮株" },
    { "zhua", "抓爪" },
    { "zhuai", "拽" },
    { "zhuan", "转专赚砖传撰" },
    { "zhuang", "装状庄壮撞妆幢" },
    { "zhui", "追坠缀椎锥" },
    { "zhun", "准谆" },
    { "zhuo", "捉桌着卓琢灼酌" },
    { "zi", "自子字资紫姿滋仔兹" },
    { "zong", "总宗纵踪棕综" },
    { "zou", "走奏揍" },
    { "zu", "组足族祖租阻" },
    { "zuan", "钻攥" },
    { "zui", "最罪嘴醉" },
    { "zun", "尊遵" },
    { "zuo", "作做坐左昨佐" },
    {NULL, NULL}
};
// ...existing code...
#endif


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_ime_pinyin_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}


/*=====================
 * Setter functions
 *====================*/

/**
 * Set the keyboard of Pinyin input method.
 * @param obj  pointer to a Pinyin input method object
 * @param dict pointer to a Pinyin input method keyboard
 */
void lv_ime_pinyin_set_keyboard(lv_obj_t * obj, lv_obj_t * kb)
{
    if(kb) {
        LV_ASSERT_OBJ(kb, &lv_keyboard_class);
    }

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    pinyin_ime->kb = kb;
    lv_obj_add_event_cb(pinyin_ime->kb, lv_ime_pinyin_kb_event, LV_EVENT_VALUE_CHANGED, obj);
    lv_obj_align_to(pinyin_ime->cand_panel, pinyin_ime->kb, LV_ALIGN_OUT_TOP_MID, 0, 0);
}

/**
 * Set the dictionary of Pinyin input method.
 * @param obj  pointer to a Pinyin input method object
 * @param dict pointer to a Pinyin input method dictionary
 */
void lv_ime_pinyin_set_dict(lv_obj_t * obj, lv_pinyin_dict_t * dict)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    init_pinyin_dict(obj, dict);
}

/**
 * Set mode, 26-key input(k26) or 9-key input(k9).
 * @param obj  pointer to a Pinyin input method object
 * @param mode   the mode from 'lv_keyboard_mode_t'
 */
void lv_ime_pinyin_set_mode(lv_obj_t * obj, lv_ime_pinyin_mode_t mode)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    LV_ASSERT_OBJ(pinyin_ime->kb, &lv_keyboard_class);

    pinyin_ime->mode = mode;

#if LV_IME_PINYIN_USE_K9_MODE
    if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
        pinyin_k9_init_data(obj);
        lv_keyboard_set_map(pinyin_ime->kb, LV_KEYBOARD_MODE_USER_1, (const char *)lv_btnm_def_pinyin_k9_map,
                            (const)default_kb_ctrl_k9_map);
        lv_keyboard_set_mode(pinyin_ime->kb, LV_KEYBOARD_MODE_USER_1);
    }
#endif
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Set the dictionary of Pinyin input method.
 * @param obj  pointer to a Pinyin IME object
 * @return     pointer to the Pinyin IME keyboard
 */
lv_obj_t * lv_ime_pinyin_get_kb(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    return pinyin_ime->kb;
}

/**
 * Set the dictionary of Pinyin input method.
 * @param obj  pointer to a Pinyin input method object
 * @return     pointer to the Pinyin input method candidate panel
 */
lv_obj_t * lv_ime_pinyin_get_cand_panel(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    return pinyin_ime->cand_panel;
}

/**
 * Set the dictionary of Pinyin input method.
 * @param obj  pointer to a Pinyin input method object
 * @return     pointer to the Pinyin input method dictionary
 */
lv_pinyin_dict_t * lv_ime_pinyin_get_dict(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    return pinyin_ime->dict;
}

/*=====================
 * Other functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_ime_pinyin_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    uint16_t py_str_i = 0;
    uint16_t btnm_i = 0;
    for(btnm_i = 0; btnm_i < (LV_IME_PINYIN_CAND_TEXT_NUM + 3); btnm_i++) {
        if(btnm_i == 0) {
            lv_btnm_def_pinyin_sel_map[btnm_i] = "<";
        }
        else if(btnm_i == (LV_IME_PINYIN_CAND_TEXT_NUM + 1)) {
            lv_btnm_def_pinyin_sel_map[btnm_i] = ">";
        }
        else if(btnm_i == (LV_IME_PINYIN_CAND_TEXT_NUM + 2)) {
            lv_btnm_def_pinyin_sel_map[btnm_i] = "";
        }
        else {
            lv_pinyin_cand_str[py_str_i][0] = ' ';
            lv_btnm_def_pinyin_sel_map[btnm_i] = lv_pinyin_cand_str[py_str_i];
            py_str_i++;
        }
    }

    pinyin_ime->mode = LV_IME_PINYIN_MODE_K26;
    pinyin_ime->py_page = 0;
    pinyin_ime->ta_count = 0;
    pinyin_ime->cand_num = 0;
    lv_memset_00(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));
    lv_memset_00(pinyin_ime->py_num, sizeof(pinyin_ime->py_num));
    lv_memset_00(pinyin_ime->py_pos, sizeof(pinyin_ime->py_pos));

    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(55));
    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, 0);

#if LV_IME_PINYIN_USE_DEFAULT_DICT
    init_pinyin_dict(obj, lv_ime_pinyin_def_dict);
#endif

    /* Init pinyin_ime->cand_panel */
    pinyin_ime->cand_panel = lv_btnmatrix_create(lv_scr_act());
    lv_btnmatrix_set_map(pinyin_ime->cand_panel, (const char **)lv_btnm_def_pinyin_sel_map);
    lv_obj_set_size(pinyin_ime->cand_panel, LV_PCT(100), LV_PCT(5));
    lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);

    lv_btnmatrix_set_one_checked(pinyin_ime->cand_panel, true);

    /* Set cand_panel style*/
    // Default style
    lv_obj_set_style_bg_opa(pinyin_ime->cand_panel, LV_OPA_0, 0);
    lv_obj_set_style_border_width(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_pad_all(pinyin_ime->cand_panel, 8, 0);
    lv_obj_set_style_pad_gap(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_radius(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_pad_gap(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_base_dir(pinyin_ime->cand_panel, LV_BASE_DIR_LTR, 0);

    // LV_PART_ITEMS style
    lv_obj_set_style_radius(pinyin_ime->cand_panel, 12, LV_PART_ITEMS);
    lv_obj_set_style_bg_color(pinyin_ime->cand_panel, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(pinyin_ime->cand_panel, LV_OPA_0, LV_PART_ITEMS);
    lv_obj_set_style_shadow_opa(pinyin_ime->cand_panel, LV_OPA_0, LV_PART_ITEMS);

    // LV_PART_ITEMS | LV_STATE_PRESSED style
    lv_obj_set_style_bg_opa(pinyin_ime->cand_panel, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(pinyin_ime->cand_panel, lv_color_white(), LV_PART_ITEMS | LV_STATE_PRESSED);

    /* event handler */
    lv_obj_add_event_cb(pinyin_ime->cand_panel, lv_ime_pinyin_cand_panel_event, LV_EVENT_VALUE_CHANGED, obj);
    lv_obj_add_event_cb(obj, lv_ime_pinyin_style_change_event, LV_EVENT_STYLE_CHANGED, NULL);

#if LV_IME_PINYIN_USE_K9_MODE
    pinyin_ime->k9_input_str_len = 0;
    pinyin_ime->k9_py_ll_pos = 0;
    pinyin_ime->k9_legal_py_count = 0;
    lv_memset_00(pinyin_ime->k9_input_str, LV_IME_PINYIN_K9_MAX_INPUT);

    pinyin_k9_init_data(obj);

    _lv_ll_init(&(pinyin_ime->k9_legal_py_ll), sizeof(ime_pinyin_k9_py_str_t));
#endif
}


static void lv_ime_pinyin_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    if(lv_obj_is_valid(pinyin_ime->kb))
        lv_obj_del(pinyin_ime->kb);

    if(lv_obj_is_valid(pinyin_ime->cand_panel))
        lv_obj_del(pinyin_ime->cand_panel);
}


static void lv_ime_pinyin_kb_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * kb = lv_event_get_target(e);
    lv_obj_t * obj = lv_event_get_user_data(e);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

#if LV_IME_PINYIN_USE_K9_MODE
    static const char * k9_py_map[8] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
#endif

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint16_t btn_id  = lv_btnmatrix_get_selected_btn(kb);
        if(btn_id == LV_BTNMATRIX_BTN_NONE) return;

        const char * txt = lv_btnmatrix_get_btn_text(kb, lv_btnmatrix_get_selected_btn(kb));
        if(txt == NULL) return;

#if LV_IME_PINYIN_USE_K9_MODE
        if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
            lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
            uint16_t tmp_btn_str_len = strlen(pinyin_ime->input_char);
            if((btn_id >= 16) && (tmp_btn_str_len > 0) && (btn_id < (16 + LV_IME_PINYIN_K9_CAND_TEXT_NUM))) {
                tmp_btn_str_len = strlen(pinyin_ime->input_char);
                lv_memset_00(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));
                strcat(pinyin_ime->input_char, txt);
                pinyin_input_proc(obj);

                for(int index = 0; index < (pinyin_ime->ta_count + tmp_btn_str_len); index++) {
                    lv_textarea_del_char(ta);
                }

                pinyin_ime->ta_count = tmp_btn_str_len;
                pinyin_ime->k9_input_str_len = tmp_btn_str_len;
                lv_textarea_add_text(ta, pinyin_ime->input_char);

                return;
            }
        }
#endif

        if(strcmp(txt, "Enter") == 0 || strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {
            pinyin_ime_clear_data(obj);
            lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
            // del input char
            if(pinyin_ime->ta_count > 0) {
                if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K26)
                    pinyin_ime->input_char[pinyin_ime->ta_count - 1] = '\0';
#if LV_IME_PINYIN_USE_K9_MODE
                else
                    pinyin_ime->k9_input_str[pinyin_ime->ta_count - 1] = '\0';
#endif

                pinyin_ime->ta_count = pinyin_ime->ta_count - 1;
                if(pinyin_ime->ta_count <= 0) {
                    lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
#if LV_IME_PINYIN_USE_K9_MODE
                    lv_memset_00(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
                    strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
                    strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");
#endif
                }
                else if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K26) {
                    pinyin_input_proc(obj);
                }
#if LV_IME_PINYIN_USE_K9_MODE
                else if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
                    pinyin_ime->k9_input_str_len = strlen(pinyin_ime->input_char) - 1;
                    pinyin_k9_get_legal_py(obj, pinyin_ime->k9_input_str, k9_py_map);
                    pinyin_k9_fill_cand(obj);
                    pinyin_input_proc(obj);
                }
#endif
            }
        }
        else if((strcmp(txt, "ABC") == 0) || (strcmp(txt, "abc") == 0) || (strcmp(txt, "1#") == 0)) {
            pinyin_ime->ta_count = 0;
            lv_memset_00(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));
            return;
        }
        else if(strcmp(txt, LV_SYMBOL_KEYBOARD) == 0) {
            if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K26) {
                lv_ime_pinyin_set_mode(pinyin_ime, LV_IME_PINYIN_MODE_K9);
            }
            else {
                lv_ime_pinyin_set_mode(pinyin_ime, LV_IME_PINYIN_MODE_K26);
                lv_keyboard_set_mode(pinyin_ime->kb, LV_KEYBOARD_MODE_TEXT_LOWER);
            }
            pinyin_ime_clear_data(obj);
        }
        else if(strcmp(txt, LV_SYMBOL_OK) == 0) {
            pinyin_ime_clear_data(obj);
        }
        else if((pinyin_ime->mode == LV_IME_PINYIN_MODE_K26) && ((txt[0] >= 'a' && txt[0] <= 'z') || (txt[0] >= 'A' &&
                                                                                                      txt[0] <= 'Z'))) {
            strcat(pinyin_ime->input_char, txt);
            pinyin_input_proc(obj);
            pinyin_ime->ta_count++;
        }
#if LV_IME_PINYIN_USE_K9_MODE
        else if((pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) && (txt[0] >= 'a' && txt[0] <= 'z')) {
            for(uint16_t i = 0; i < 8; i++) {
                if((strcmp(txt, k9_py_map[i]) == 0) || (strcmp(txt, "abc ") == 0)) {
                    if(strcmp(txt, "abc ") == 0)    pinyin_ime->k9_input_str_len += strlen(k9_py_map[i]) + 1;
                    else                            pinyin_ime->k9_input_str_len += strlen(k9_py_map[i]);
                    pinyin_ime->k9_input_str[pinyin_ime->ta_count] = 50 + i;

                    break;
                }
            }
            pinyin_k9_get_legal_py(obj, pinyin_ime->k9_input_str, k9_py_map);
            pinyin_k9_fill_cand(obj);
            pinyin_input_proc(obj);
        }
        else if(strcmp(txt, LV_SYMBOL_LEFT) == 0) {
            pinyin_k9_cand_page_proc(obj, 0);
        }
        else if(strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
            pinyin_k9_cand_page_proc(obj, 1);
        }
#endif
    }
}


static void lv_ime_pinyin_cand_panel_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * cand_panel = lv_event_get_target(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_user_data(e);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(cand_panel);
        if(id == 0) {
            pinyin_page_proc(obj, 0);
            return;
        }
        if(id == (LV_IME_PINYIN_CAND_TEXT_NUM + 1)) {
            pinyin_page_proc(obj, 1);
            return;
        }

        const char * txt = lv_btnmatrix_get_btn_text(cand_panel, id);
        lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
        uint16_t index = 0;
        for(index = 0; index < pinyin_ime->ta_count; index++)
            lv_textarea_del_char(ta);

        lv_textarea_add_text(ta, txt);

        pinyin_ime_clear_data(obj);
    }
}


static void pinyin_input_proc(lv_obj_t * obj)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    pinyin_ime->cand_str = pinyin_search_matching(obj, pinyin_ime->input_char, &pinyin_ime->cand_num);
    if(pinyin_ime->cand_str == NULL) {
        return;
    }

    pinyin_ime->py_page = 0;

    for(uint8_t i = 0; i < LV_IME_PINYIN_CAND_TEXT_NUM; i++) {
        memset(lv_pinyin_cand_str[i], 0x00, sizeof(lv_pinyin_cand_str[i]));
        lv_pinyin_cand_str[i][0] = ' ';
    }

    // fill buf
    for(uint8_t i = 0; (i < pinyin_ime->cand_num && i < LV_IME_PINYIN_CAND_TEXT_NUM); i++) {
        for(uint8_t j = 0; j < 3; j++) {
            lv_pinyin_cand_str[i][j] = pinyin_ime->cand_str[i * 3 + j];
        }
    }

    lv_obj_clear_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
}

static void pinyin_page_proc(lv_obj_t * obj, uint16_t dir)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;
    uint16_t page_num = pinyin_ime->cand_num / LV_IME_PINYIN_CAND_TEXT_NUM;
    uint16_t sur = pinyin_ime->cand_num % LV_IME_PINYIN_CAND_TEXT_NUM;

    if(dir == 0) {
        if(pinyin_ime->py_page) {
            pinyin_ime->py_page--;
        }
    }
    else {
        if(sur == 0) {
            page_num -= 1;
        }
        if(pinyin_ime->py_page < page_num) {
            pinyin_ime->py_page++;
        }
        else return;
    }

    for(uint8_t i = 0; i < LV_IME_PINYIN_CAND_TEXT_NUM; i++) {
        memset(lv_pinyin_cand_str[i], 0x00, sizeof(lv_pinyin_cand_str[i]));
        lv_pinyin_cand_str[i][0] = ' ';
    }

    // fill buf
    uint16_t offset = pinyin_ime->py_page * (3 * LV_IME_PINYIN_CAND_TEXT_NUM);
    for(uint8_t i = 0; (i < pinyin_ime->cand_num && i < LV_IME_PINYIN_CAND_TEXT_NUM); i++) {
        if((sur > 0) && (pinyin_ime->py_page == page_num)) {
            if(i > sur)
                break;
        }
        for(uint8_t j = 0; j < 3; j++) {
            lv_pinyin_cand_str[i][j] = pinyin_ime->cand_str[offset + (i * 3) + j];
        }
    }
}


static void lv_ime_pinyin_style_change_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    if(code == LV_EVENT_STYLE_CHANGED) {
        const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_obj_set_style_text_font(pinyin_ime->cand_panel, font, 0);
    }
}


static void init_pinyin_dict(lv_obj_t * obj, lv_pinyin_dict_t * dict)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    char headletter = 'a';
    uint16_t offset_sum = 0;
    uint16_t offset_count = 0;
    uint16_t letter_calc = 0;

    pinyin_ime->dict = dict;

    for(uint16_t i = 0; ; i++) {
        if((NULL == (dict[i].py)) || (NULL == (dict[i].py_mb))) {
            headletter = dict[i - 1].py[0];
            letter_calc = headletter - 'a';
            pinyin_ime->py_num[letter_calc] = offset_count;
            break;
        }

        if(headletter == (dict[i].py[0])) {
            offset_count++;
        }
        else {
            headletter = dict[i].py[0];
            letter_calc = headletter - 'a';
            pinyin_ime->py_num[letter_calc - 1] = offset_count;
            offset_sum += offset_count;
            pinyin_ime->py_pos[letter_calc] = offset_sum;

            offset_count = 1;
        }
    }
}


static char * pinyin_search_matching(lv_obj_t * obj, char * py_str, uint16_t * cand_num)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    lv_pinyin_dict_t * cpHZ;
    uint8_t index, len = 0, offset;
    volatile uint8_t count = 0;

    if(*py_str == '\0')    return NULL;
    if(*py_str == 'i')     return NULL;
    if(*py_str == 'u')     return NULL;
    if(*py_str == 'v')     return NULL;

    offset = py_str[0] - 'a';
    len = strlen(py_str);

    cpHZ  = &pinyin_ime->dict[pinyin_ime->py_pos[offset]];
    count = pinyin_ime->py_num[offset];

    while(count--) {
        for(index = 0; index < len; index++) {
            if(*(py_str + index) != *((cpHZ->py) + index)) {
                break;
            }
        }

        // perfect match
        if(len == 1 || index == len) {
            // The Chinese character in UTF-8 encoding format is 3 bytes
            * cand_num = strlen((const char *)(cpHZ->py_mb)) / 3;
            return (char *)(cpHZ->py_mb);
        }
        cpHZ++;
    }
    return NULL;
}

static void pinyin_ime_clear_data(lv_obj_t * obj)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;


#if LV_IME_PINYIN_USE_K9_MODE
    if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
        pinyin_ime->k9_input_str_len = 0;
        pinyin_ime->k9_py_ll_pos = 0;
        pinyin_ime->k9_legal_py_count = 0;
        lv_memset_00(pinyin_ime->k9_input_str,  LV_IME_PINYIN_K9_MAX_INPUT);
        lv_memset_00(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
        strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
        strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");
    }
#endif

    pinyin_ime->ta_count = 0;
    lv_memset_00(lv_pinyin_cand_str, (sizeof(lv_pinyin_cand_str)));
    lv_memset_00(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));

    lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
}


#if LV_IME_PINYIN_USE_K9_MODE
static void pinyin_k9_init_data(lv_obj_t * obj)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    uint16_t py_str_i = 0;
    uint16_t btnm_i = 0;
    for(btnm_i = 19; btnm_i < (LV_IME_PINYIN_K9_CAND_TEXT_NUM + 21); btnm_i++) {
        if(py_str_i == LV_IME_PINYIN_K9_CAND_TEXT_NUM) {
            strcpy(lv_pinyin_k9_cand_str[py_str_i], LV_SYMBOL_RIGHT"\0");
        }
        else if(py_str_i == LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1) {
            strcpy(lv_pinyin_k9_cand_str[py_str_i], "\0");
        }
        else {
            strcpy(lv_pinyin_k9_cand_str[py_str_i], " \0");
        }

        lv_btnm_def_pinyin_k9_map[btnm_i] = lv_pinyin_k9_cand_str[py_str_i];
        py_str_i++;
    }

    default_kb_ctrl_k9_map[0]  = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[4]  = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[5]  = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[9]  = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[10] = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[14] = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[15] = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
    default_kb_ctrl_k9_map[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 16] = LV_KEYBOARD_CTRL_BTN_FLAGS | 1;
}

static void pinyin_k9_get_legal_py(lv_obj_t * obj, char * k9_input, const char * py9_map[])
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    uint16_t len = strlen(k9_input);

    if((len == 0) || (len >= LV_IME_PINYIN_K9_MAX_INPUT)) {
        return;
    }

    char py_comp[LV_IME_PINYIN_K9_MAX_INPUT] = {0};
    int mark[LV_IME_PINYIN_K9_MAX_INPUT] = {0};
    int index = 0;
    int flag = 0;
    int count = 0;

    uint32_t ll_len = 0;
    ime_pinyin_k9_py_str_t * ll_index = NULL;

    ll_len = _lv_ll_get_len(&pinyin_ime->k9_legal_py_ll);
    ll_index = _lv_ll_get_head(&pinyin_ime->k9_legal_py_ll);

    while(index != -1) {
        if(index == len) {
            if(pinyin_k9_is_valid_py(obj, py_comp)) {
                if((count >= ll_len) || (ll_len == 0)) {
                    ll_index = _lv_ll_ins_tail(&pinyin_ime->k9_legal_py_ll);
                    strcpy(ll_index->py_str, py_comp);
                }
                else if((count < ll_len)) {
                    strcpy(ll_index->py_str, py_comp);
                    ll_index = _lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index);
                }
                count++;
            }
            index--;
        }
        else {
            flag = mark[index];
            if(flag < strlen(py9_map[k9_input[index] - '2'])) {
                py_comp[index] = py9_map[k9_input[index] - '2'][flag];
                mark[index] = mark[index] + 1;
                index++;
            }
            else {
                mark[index] = 0;
                index--;
            }
        }
    }

    if(count > 0) {
        pinyin_ime->ta_count++;
        pinyin_ime->k9_legal_py_count = count;
    }
}


/*true: visible; false: not visible*/
static bool pinyin_k9_is_valid_py(lv_obj_t * obj, char * py_str)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    lv_pinyin_dict_t * cpHZ = NULL;
    uint8_t index = 0, len = 0, offset = 0;
    uint16_t ret = 1;
    volatile uint8_t count = 0;

    if(*py_str == '\0')    return false;
    if(*py_str == 'i')     return false;
    if(*py_str == 'u')     return false;
    if(*py_str == 'v')     return false;

    offset = py_str[0] - 'a';
    len = strlen(py_str);

    cpHZ  = &pinyin_ime->dict[pinyin_ime->py_pos[offset]];
    count = pinyin_ime->py_num[offset];

    while(count--) {
        for(index = 0; index < len; index++) {
            if(*(py_str + index) != *((cpHZ->py) + index)) {
                break;
            }
        }

        // perfect match
        if(len == 1 || index == len) {
            return true;
        }
        cpHZ++;
    }
    return false;
}


static void pinyin_k9_fill_cand(lv_obj_t * obj)
{
    static uint16_t len = 0;
    uint16_t index = 0, tmp_len = 0;
    ime_pinyin_k9_py_str_t * ll_index = NULL;

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    tmp_len = pinyin_ime->k9_legal_py_count;

    if(tmp_len != len) {
        lv_memset_00(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
        strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
        strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");
        len = tmp_len;
    }

    ll_index = _lv_ll_get_head(&pinyin_ime->k9_legal_py_ll);
    strcpy(pinyin_ime->input_char, ll_index->py_str);
    while(ll_index) {
        if((index >= LV_IME_PINYIN_K9_CAND_TEXT_NUM) || \
           (index >= pinyin_ime->k9_legal_py_count))
            break;

        strcpy(lv_pinyin_k9_cand_str[index], ll_index->py_str);
        ll_index = _lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the next list*/
        index++;
    }
    pinyin_ime->k9_py_ll_pos = index;

    lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
    for(index = 0; index < pinyin_ime->k9_input_str_len; index++) {
        lv_textarea_del_char(ta);
    }
    pinyin_ime->k9_input_str_len = strlen(pinyin_ime->input_char);
    lv_textarea_add_text(ta, pinyin_ime->input_char);
}


static void pinyin_k9_cand_page_proc(lv_obj_t * obj, uint16_t dir)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
    uint16_t ll_len =  _lv_ll_get_len(&pinyin_ime->k9_legal_py_ll);

    if((ll_len > LV_IME_PINYIN_K9_CAND_TEXT_NUM) && (pinyin_ime->k9_legal_py_count > LV_IME_PINYIN_K9_CAND_TEXT_NUM)) {
        ime_pinyin_k9_py_str_t * ll_index = NULL;
        uint16_t tmp_btn_str_len = 0;
        int count = 0;

        ll_index = _lv_ll_get_head(&pinyin_ime->k9_legal_py_ll);
        while(ll_index) {
            if(count >= pinyin_ime->k9_py_ll_pos)   break;

            ll_index = _lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the next list*/
            count++;
        }

        if((NULL == ll_index) && (dir == 1))   return;

        lv_memset_00(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
        strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
        strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");

        // next page
        if(dir == 1) {
            count = 0;
            while(ll_index) {
                if(count >= (LV_IME_PINYIN_K9_CAND_TEXT_NUM - 1))
                    break;

                strcpy(lv_pinyin_k9_cand_str[count], ll_index->py_str);
                ll_index = _lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the next list*/
                count++;
            }
            pinyin_ime->k9_py_ll_pos += count - 1;

        }
        // previous page
        else {
            count = LV_IME_PINYIN_K9_CAND_TEXT_NUM - 1;
            ll_index = _lv_ll_get_prev(&pinyin_ime->k9_legal_py_ll, ll_index);
            while(ll_index) {
                if(count < 0)  break;

                strcpy(lv_pinyin_k9_cand_str[count], ll_index->py_str);
                ll_index = _lv_ll_get_prev(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the previous list*/
                count--;
            }

            if(pinyin_ime->k9_py_ll_pos > LV_IME_PINYIN_K9_CAND_TEXT_NUM)
                pinyin_ime->k9_py_ll_pos -= 1;
        }

        lv_textarea_set_cursor_pos(ta, LV_TEXTAREA_CURSOR_LAST);
    }
}

#endif  /*LV_IME_PINYIN_USE_K9_MODE*/

#endif  /*LV_USE_IME_PINYIN*/

