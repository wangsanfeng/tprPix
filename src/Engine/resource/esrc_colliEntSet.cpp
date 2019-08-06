/*
 * ========================= esrc_colliEntSet.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  colliEntSets
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "ColliEntSetLoader.h"
#include "esrc_colliEntSet.h" //- 所有资源


#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//

namespace ces_inn {//------------ namespace: ces_inn --------------//

    std::unordered_map<std::string, int> colliEntSet_name_idx {};
    std::unordered_map<int, std::string> colliEntSet_idx_name {};

    //-- key: colliEntSetIdx
    std::unordered_map<int, std::shared_ptr<ColliEntSet>> colliEntSets {};
                                        //-- 在未来，应该被改为 unique_ptr ...

}//---------------- namespace: ces_inn end --------------//


/* ===========================================================
 *                 init_colliEntSet_tables     [pure]
 * -----------------------------------------------------------
 * -- 创建 name - idx 正反表 
 * -- 目前有 16 个 collientSet 预制件 
 */
void init_colliEntSet_tables(){

    std::vector<std::string> names {
        "ces_1_1", //- 0

        "ces_2_1", //- 1
        "ces_1_2", //- 2

        "ces_2_2", //- 3

        "ces_3_2", //- 4
        "ces_2_3", //- 5
        "ces_3_3", //- 6

        "ces_4_3", //- 7
        "ces_5_3", //- 8

        "ces_4_4", //- 9
        "ces_5_4", //- 10 
        "ces_5_5"  //- 11

        //"ces_6_5", //- 12
        //"ces_6_6", //- 13
        //"ces_7_6", //- 14
        //"ces_7_7"  //- 15
    };

    for( size_t i=0; i<names.size(); i++ ){
       ces_inn::colliEntSet_name_idx.insert({ names.at(i), i });
       ces_inn::colliEntSet_idx_name.insert({ i, names.at(i) });
    }
}


/* ===========================================================
 *                   load_colliEntSets
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 colliEntSets 资源
 */
void load_colliEntSets(){

    //- ces 预制件 个数 --
    size_t totalSets  { 4*3 }; 

    //-- 创建 ColliEntSetLoader 实例 --
    ColliEntSetLoader  loader { "colliEntSet_1.png",
                                IntVec2{ 4, 3 },
                                totalSets };
    
    //-- 解析 图片数据，将数据存储自身容器中 --
    loader.init();

    //-- 将 loader 中的数据，手动 搬运到 全局容器中  [ copy ] --
    for( size_t i=0; i<totalSets; i++ ){
        ces_inn::colliEntSets.insert({ i, loader.get_collientSetSPtr(i) });
    }
    
    //-- 自动销毁 ColliEntSetLoader 实例: loader -- 
}


/* ===========================================================
 *                  debug_colliEntSets
 * -----------------------------------------------------------
 */
void debug_colliEntSets(){
    
    auto p = ces_inn::colliEntSets.begin();
    for( ; p!=ces_inn::colliEntSets.end(); p++ ){

        cout << "-------- " << p->first 
             << " -------->"
             << endl;
        //p->second.debug();
        cout << endl;
    }
}



/* ===========================================================
 *                  get_colliEntSetRef
 * -----------------------------------------------------------
 */
const ColliEntSet &get_colliEntSetRef( int colliEntSetIdx_ ){
        tprAssert( ces_inn::colliEntSets.find(colliEntSetIdx_) != ces_inn::colliEntSets.end() );
    return *(ces_inn::colliEntSets.at( colliEntSetIdx_ ).get());
}


}//---------------------- namespace: esrc -------------------------//
