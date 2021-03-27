// rapidjson/example/simpledom/simpledom.cpp`
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"
#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

using namespace rapidjson;

static int cjson_author(lua_State *L) {
    const char *s = "xudong12";
    lua_pushstring(L, s);
    return 1;
}

static int handle_string(lua_State *L) {
    size_t l;
    const char *s = luaL_checklstring(L, 1, &l);
    lua_pushstring(L, s);
    return 1;
}

static int encode_adfeeds(lua_State *L) {
    size_t l;

    const char *error = "error";
    const char *s = luaL_checklstring(L, 1, &l);
    if (s == NULL) {
        s = "args is error";
        lua_pushstring(L, s);
    } else {
        //const char *j = image_order_encode(s); 调试好久返回值有问题
        try {
            Document d ;
            d.Parse(s);
            if(d.HasParseError()){
                lua_pushstring(L, error);
                return 1;
            }
            // 2. 利用 DOM 作出修改。
            Value &result = d["data"][0]["result"][0];
            Value &pic_infos = d["data"][0]["result"][0]["pic_infos"];
            Value &pic_ids = d["data"][0]["result"][0]["pic_ids"];

            Value tmp_pic_infos(kObjectType);
            Document::AllocatorType &allocator = d.GetAllocator();
            Value copy_pic_ids(pic_ids, allocator);
            for (int i = 0; i < pic_ids.Size(); i++) {
                Value &id = copy_pic_ids[i];
                tmp_pic_infos.AddMember(id, pic_infos[id], allocator);
            }
            result.AddMember("pic_infos", tmp_pic_infos, allocator);
            // 3. 把 DOM 转换（stringify）成 JSON。
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            d.Accept(writer);
            const char *content = buffer.GetString();
            lua_pushstring(L, content);
           //用完之后要释放doc，如果不释放，之前在fun()中使用分配器分配的内存将不会自动回收
        } catch (...) {
            lua_pushstring(L, error);
        }

    }

    return 1;
}

static int encode_cardlist(lua_State *L) {
    size_t l;

    const char *error = "error";
    const char *s = luaL_checklstring(L, 1, &l);
    if (s == NULL) {
        s = "args is error";
        lua_pushstring(L, s);
    } else {
        try {
            Document d ;
            d.Parse(s);
            if(d.HasParseError()){
                lua_pushstring(L, error);
                return 1;
            }
            // 2. 利用 DOM 作出修改。
            Value &mblog = d["data"][0]["result"][0]["mblog"];
            Value &pic_infos = d["data"][0]["result"][0]["mblog"]["pic_infos"];
            Value &pic_ids = d["data"][0]["result"][0]["mblog"]["pic_ids"];
            Value tmp_pic_infos(kObjectType);
            Document::AllocatorType &allocator = d.GetAllocator();
            Value copy_pic_ids(pic_ids, allocator);
            for (int i = 0; i < pic_ids.Size(); i++) {
                Value &id = copy_pic_ids[i];
                tmp_pic_infos.AddMember(id, pic_infos[id], allocator);
            }
            mblog.AddMember("pic_infos", tmp_pic_infos, allocator);
            // 3. 把 DOM 转换（stringify）成 JSON。
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            d.Accept(writer);
            const char *content = buffer.GetString();
            lua_pushstring(L, content);
            //用完之后要释放doc，如果不释放，之前在fun()中使用分配器分配的内存将不会自动回收
        } catch (...) {
            lua_pushstring(L, error);
        }

    }

    return 1;
}

static const luaL_Reg image_order[] = {
        {"author",      cjson_author},
        {"sortPicinfoFeeds", encode_adfeeds},
        {"sortPicinfoCard", encode_cardlist},
        {"input",       handle_string},
        {NULL,          NULL},
};

extern "C" int luaopen_image_order(lua_State *l) {
    luaL_register(l, "image_order", image_order);
    return 1;
}

