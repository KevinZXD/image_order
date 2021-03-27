**背景**

针对多图的广告，pic_infos字典 key是按照pic_ids数组的值有序的，
由于lua table无法按照插入顺序排序输出，故借助C++帮忙排序（业务强相关），
字符串输入，字符串返回（中间不允许lua的编解码，否则依旧乱序）



**部署流程**

1. 进入项目，检查系统环境和Makefile是否匹配

2. 进入项目目录下
  
   清理代码 make clean

   编译  make  (g++ image_order.cpp  -fPIC  -shared -o image_order.so  -I /usr/local/openresty/luajit/include/luajit-2.1)
   
   
   
3. 拷贝包到lua的公共文件路径下

   sudo cp image_order.so  /usr/local/sina_mobile/luajit/lib/lua/5.1/

4. 统一渲染侧代码使用demo

(```)

    local resultJsonString = ''
    local image_order = require('image_order')
    local encodeResult
    if isAdFeeds() then -- 
        encodeResult = image_order.sortPicinfoFeeds(result) -- 对picinfo按照picids进行排序
    elseif isCardList() then -- 
        encodeResult = image_order.sortPicinfoCard(result) -- 对picinfo按照picids进行排序
    end
    if type(encodeResult) == 'string' and encodeResult ~= 'error' then
        return encodeResult
    end
    
(```) 
  
**风险点**

1. 数据结构强相关（只要结构修改，该项目就得修改，否则就会报错），不利于拓展数据;openresty框架只限于吐出数据的时候使用（不允许编解码，否则依旧乱序）
2. 性能影响，以及不可预估的内存使用风险
3. 尽管吐出的数据保证图片有序，鉴于广告链路很长，中间只要有一个平台打乱顺序，前功尽弃



