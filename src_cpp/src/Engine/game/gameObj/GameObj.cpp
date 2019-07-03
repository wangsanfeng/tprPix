/*
 * ========================= GameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GameObj.h" 

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "esrc_colliEntSet.h"

//#include "tprDebug.h" //- tmp


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void GameObj::init(){
    this->goPos.init( std::bind( [this](){ return this->rootColliEntHeadPtr->rootAnchorCompass; }), 
                      std::bind( [this](){ return this->rootColliEntHeadPtr->off_from_rootAnchor_2_mapEntMid; }) );
    //...
}


/* ===========================================================
 *                     creat_new_goMesh
 * -----------------------------------------------------------
 * -- 通过一组参数来实现 gomesh 的初始化。
 * -- 在这个函数结束hou，仅剩下一件事要做： gomesh.bind_animAction( "god", "jump" );
 */
GameObjMesh &GameObj::creat_new_goMesh( const std::string &name_,
                                        RenderLayerType    layerType_,
                                        ShaderProgram     *pixShaderPtr_,
                                        ShaderProgram     *shadowShaderPtr_,
                                        const glm::vec2   pposOff_,
                                        float             off_z_,
                                        bool              isVisible_,
                                        bool              isCollide_,
                                        bool              isFlipOver_ ){

        tprAssert( this->goMeshs.find(name_) == this->goMeshs.end() );
    this->goMeshs.insert({ name_, std::make_unique<GameObjMesh>(*this) }); 
    GameObjMesh &gmesh = *(this->goMeshs.at(name_));

    //----- init -----//
    gmesh.set_pic_renderLayer( layerType_ ); 
    gmesh.set_pic_shader_program( pixShaderPtr_ );
    gmesh.set_shadow_shader_program( shadowShaderPtr_ );

    //-- goMesh pos in go --
    gmesh.set_pposOff(pposOff_);
    gmesh.set_off_z(off_z_);

    //-- flags --//
    gmesh.isVisible = isVisible_;
    gmesh.isCollide = isCollide_;
    gmesh.isFlipOver = isFlipOver_;

    //-----
    return gmesh;
}


/* ===========================================================
 *                    reset_chunkKeys
 * -----------------------------------------------------------
 * -- 重装填
 * 此函数 只是单纯记录 本go相关的所有 chunk key 信息。
 * 此过程中并不访问 chunk实例 本事。所有，就算相关 chunk 尚未创建，也不影响本函数的执行。
 */
void GameObj::reset_chunkKeys(){
    MapCoord     cesMCPos    {}; //- 每个 ces左下角的 mcpos （世界绝对pos）
    MapCoord     tmpEntMCPos {};
    chunkKey_t   tmpChunkKey {};
    //-------
    this->chunkKeys.clear();
    //-------
    
    //- 只有 rootGoMesh 参与 mapent 登记
    if( this->get_goMeshRef("root").isCollide == false ){ //- 不参与碰撞检测的 gomesh 直接跳过
        return;
    }

    cesMCPos.set_by_mpos( this->goPos.get_currentMPos() -
                          this->rootColliEntHeadPtr->mposOff_from_cesLB_2_centerMPos );

    const ColliEntSet &doCesRef = esrc::get_colliEntSetRef(this->rootColliEntHeadPtr->colliEntSetIdx); //- get do_ces_ref

    for( const auto &mcpos : doCesRef.get_colliEnts() ){ //- each collient mcpos
        tmpEntMCPos = mcpos + cesMCPos;
        tmpChunkKey = anyMPos_2_chunkKey( tmpEntMCPos.get_mpos() );
        this->chunkKeys.insert( tmpChunkKey ); //- copy
    } //- each collient mcpos end --
}



/* ===========================================================
 *                       debug
 * -----------------------------------------------------------
 */
/*
void GameObj::debug(){

    cout << "------ GameObj: " << id << " --------" << endl;

    cout<< "\nsizeof(GameObj): "      << sizeof(GameObj)
        << "\nsizeof(GameObjPos): "   << sizeof(GameObjPos)
        << "\nsizeof(Move): "         << sizeof(Move)
        << "\nsizeof(GameObjMesh): "  << sizeof(GameObjMesh)
        << "\nsizeof(ChildMesh): "    << sizeof(ChildMesh)
        << "\nsizeof(ActionSwitch): " << sizeof(ActionSwitch)
        << "\nsizeof(Collision): "    << sizeof(Collision)
        << endl;

    
    cout<< "\nAwake:        " << ( Awake==nullptr ? "nullptr" : "not nullptr" )
        << "\nStart:        " << ( Start==nullptr ? "nullptr" : "not nullptr" )
        << "\nRenderUpdate: " << ( RenderUpdate==nullptr ? "nullptr" : "not nullptr" )
        << "\nLogicUpdate:  " << ( LogicUpdate==nullptr ? "nullptr" : "not nullptr" )
        << "\nBeAffect:     " << ( BeAffect==nullptr ? "nullptr" : "not nullptr" )
        << endl;

    cout<< "\nspecies = " << species
        << "\nid_parent = " << parentId
        << endl;


    cout << "weight = " << weight << endl;

    cout<< "\nisTopGo: "           << ( isTopGo ? "true" : "false" )
        << "\nisActive: "          << ( isActive ? "true" : "false" )
        << "\nisDirty: "           << ( isDirty ? "true" : "false" )
        << "\nisControlByPlayer: " << ( isControlByPlayer ? "true" : "false" )
        << "\nisFlipOver: "        << ( isFlipOver ? "true" : "false" )
        << endl;

    cout << "binary.size() = " << binary.size()
        << endl;

    cout << "\n\n" << endl;
    
}
*/


