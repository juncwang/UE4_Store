#pragma once

// 地址前缀
// 测试前缀 http://192.168.78.110:8080/atomic-vr-service
// 正式版前缀 http://www.boomyun.cn
#define INTERFACE_ADDRESS "http://www.boomyun.cn"

// 首页检测更新 ============================================================================》

// 检测版本更新
#define VERSION_INTERFACE "/boomAjax/clientInfo/getClientInfo?gid=1"

// 3D视窗添加商品及更换材质 ============================================================================》

// 品牌列表 后面需增加参数 1：家具 2：地面 3：墙面
#define BRAND_LIST_INTERFACE "/boomAjax/brandInfo/getBrandInfoList?typeGid="

// 风格列表 
#define STYLE_LIST_INTERFACE "/boomAjax/configStyle/getConfigStyleList"

// 商品类型列表
#define TYPE_LIST_INTERFACE "/boomAjax/productType/productTypeInfo?typeGid="

// 商品列表 *curPage:页码 *pageSize:当前页需要的数据多少 *typeParentGid:1家具,2地板,3墙面 !typeGid:类型id !brandGid:品牌id !styleGid:风格id
#define COMMODITY_LIST_INTERFACE "/boomAjax/productInfo/getProductInfoList"

// 商品详情 
#define PRODUCT_INTERFACE "/boomAjax/productInfo/getProductInfo?gid="

// 3D视窗获取所有价格列表 ============================================================================》

// 商品价格列表 商品gid(多个id用逗号隔开)
#define PRICE_LIST_INTERFACE "/boomAjax/productInfo/getProductUnitPrice?productGids="

// 户型房型及搜索============================================================================》

// 户型列表 *curPage 当前页 *pageSize 返回最大数据量
#define HOUSETYPE_LIST_INTERFACE "/boomAjax/houseType/getHouseTypeList"

// 方案详情 *gid 
#define SCHEME_INFO_INTERFACE "/boomAjax/memberSchemeInfo/getMemberSchemeInfo?gid="

// 房型详情 *gid
#define ROOMTYPE_INFO_INTERFACE "/boomAjax/roomTypeInfo/getRoomTypeInfo?gid="

// 根据id列表获取模型列表 字符串以逗号隔开
#define MODELINFOBYGID_INTERFACE "/boomAjax/modelInfo/getModelInfoByGidStr?productModelGids="

// 根据id列表获取材质列表 字符串以逗号隔开
#define MATERIALINFOBYGID_INTERFACE "/boomAjax/modelInfo/getMaterialInfoByGidStr?gid="

// 户型详情 * gid 户型id
#define HOUSEINFO_INTERFACE "/boomAjax/houseTypeInfo/getHouseTypeInfo?gid="

// 房型列表 * houseTypeGid 户型id 通过户型ID 查找风格
#define ROOMLIST_INFO_INTERFACE "/boomAjax/roomTypeInfoList/getRoomTypeInfoList?houseTypeGid="

// 方案保存 * roomTypeInfoGid, userPhone, schemeText, showImage
#define SAVESCHEME_INTERFACE "/boomAjax/memberSchemeInfo/saveMemberSchemeInfo"

// 上传文件
#define UPLOAD_FILE "/boomAjax/tempFileInfo/unloadTempFile"

// 方案列表 *curPage 页码 pageSize 每页多少条数据 / userPhone 手机号码 / schemeName 方案名称
#define SCHEME_LIST_INTERFACE "/boomAjax/memberSchemeInfo/getMemberSchemeInfoList"

// ================================================================================================== 全景图接口

// 全景图上传 file
#define PANO_UP_INTERFACE "/boomAjax/panoramaInfo/unloadPanoramaImage"

// 全景图保存 memberSchemeInfGid_方案id userPhone_手机号码
#define PANO_SAVE_INTERFACE "/boomAjax/panoramaInfo/savePanoramaInfo"

// 多张全景图保存 memberSchemeInfGid_方案id userPhone_手机号码
#define PANO_SAVEMANY_INTERFACE "/boomAjax/panoramaInfo/saveManyPanoramaInfo"

// 全景图热点保存 
#define PANO_HOTSPOT_INTERFACE "/boomAjax/hotspotInfo/saveHotSpotInfo"

// 用户登录接口
#define USER_LOGIN_INTERFACE "/boomAjax/userInfo/userLogin?userPhone="