#pragma once

// ��ַǰ׺
// ����ǰ׺ http://192.168.78.110:8080/atomic-vr-service
// ��ʽ��ǰ׺ http://www.boomyun.cn
#define INTERFACE_ADDRESS "http://www.boomyun.cn"

// ��ҳ������ ============================================================================��

// ���汾����
#define VERSION_INTERFACE "/boomAjax/clientInfo/getClientInfo?gid=1"

// 3D�Ӵ������Ʒ���������� ============================================================================��

// Ʒ���б� ���������Ӳ��� 1���Ҿ� 2������ 3��ǽ��
#define BRAND_LIST_INTERFACE "/boomAjax/brandInfo/getBrandInfoList?typeGid="

// ����б� 
#define STYLE_LIST_INTERFACE "/boomAjax/configStyle/getConfigStyleList"

// ��Ʒ�����б�
#define TYPE_LIST_INTERFACE "/boomAjax/productType/productTypeInfo?typeGid="

// ��Ʒ�б� *curPage:ҳ�� *pageSize:��ǰҳ��Ҫ�����ݶ��� *typeParentGid:1�Ҿ�,2�ذ�,3ǽ�� !typeGid:����id !brandGid:Ʒ��id !styleGid:���id
#define COMMODITY_LIST_INTERFACE "/boomAjax/productInfo/getProductInfoList"

// ��Ʒ���� 
#define PRODUCT_INTERFACE "/boomAjax/productInfo/getProductInfo?gid="

// 3D�Ӵ���ȡ���м۸��б� ============================================================================��

// ��Ʒ�۸��б� ��Ʒgid(���id�ö��Ÿ���)
#define PRICE_LIST_INTERFACE "/boomAjax/productInfo/getProductUnitPrice?productGids="

// ���ͷ��ͼ�����============================================================================��

// �����б� *curPage ��ǰҳ *pageSize �������������
#define HOUSETYPE_LIST_INTERFACE "/boomAjax/houseType/getHouseTypeList"

// �������� *gid 
#define SCHEME_INFO_INTERFACE "/boomAjax/memberSchemeInfo/getMemberSchemeInfo?gid="

// �������� *gid
#define ROOMTYPE_INFO_INTERFACE "/boomAjax/roomTypeInfo/getRoomTypeInfo?gid="

// ����id�б��ȡģ���б� �ַ����Զ��Ÿ���
#define MODELINFOBYGID_INTERFACE "/boomAjax/modelInfo/getModelInfoByGidStr?productModelGids="

// ����id�б��ȡ�����б� �ַ����Զ��Ÿ���
#define MATERIALINFOBYGID_INTERFACE "/boomAjax/modelInfo/getMaterialInfoByGidStr?gid="

// �������� * gid ����id
#define HOUSEINFO_INTERFACE "/boomAjax/houseTypeInfo/getHouseTypeInfo?gid="

// �����б� * houseTypeGid ����id ͨ������ID ���ҷ��
#define ROOMLIST_INFO_INTERFACE "/boomAjax/roomTypeInfoList/getRoomTypeInfoList?houseTypeGid="

// �������� * roomTypeInfoGid, userPhone, schemeText, showImage
#define SAVESCHEME_INTERFACE "/boomAjax/memberSchemeInfo/saveMemberSchemeInfo"

// �ϴ��ļ�
#define UPLOAD_FILE "/boomAjax/tempFileInfo/unloadTempFile"

// �����б� *curPage ҳ�� pageSize ÿҳ���������� / userPhone �ֻ����� / schemeName ��������
#define SCHEME_LIST_INTERFACE "/boomAjax/memberSchemeInfo/getMemberSchemeInfoList"

// ================================================================================================== ȫ��ͼ�ӿ�

// ȫ��ͼ�ϴ� file
#define PANO_UP_INTERFACE "/boomAjax/panoramaInfo/unloadPanoramaImage"

// ȫ��ͼ���� memberSchemeInfGid_����id userPhone_�ֻ�����
#define PANO_SAVE_INTERFACE "/boomAjax/panoramaInfo/savePanoramaInfo"

// ����ȫ��ͼ���� memberSchemeInfGid_����id userPhone_�ֻ�����
#define PANO_SAVEMANY_INTERFACE "/boomAjax/panoramaInfo/saveManyPanoramaInfo"

// ȫ��ͼ�ȵ㱣�� 
#define PANO_HOTSPOT_INTERFACE "/boomAjax/hotspotInfo/saveHotSpotInfo"

// �û���¼�ӿ�
#define USER_LOGIN_INTERFACE "/boomAjax/userInfo/userLogin?userPhone="