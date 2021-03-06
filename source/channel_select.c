#include "channel_select.h"
#include "transfer_usrdesign.h"
#include "wechat_usrdesign.h"
#include "app_wechat_common.h"
#include "usr_data.h"
#include "debug.h"
#include <string.h>

communication_statue_st g_communication_statue = {0};

void usr_set_app_type(app_enum type)
{
	g_communication_statue.app_type |= type;
}

uint32_t app_send_data(uint8_t *data,uint16_t length)
{
	uint32_t error;

	if(g_communication_statue.app_type == WECHAT_APP)
	{	
		error = wechat_send_data(data, length,WECHAT_INDICATE_CHANNEL,0);
	}
	else if(g_communication_statue.app_type == LIFESENSE_APP)
	{	
		error = transfer_send_data(data, length,TRANS_INDICATE_CHANNEL,0);
	}
	return error;
}

uint32_t usr_send_data(uint8_t *data,uint16_t length)
{
	uint32_t error;
	uint8_t send_buffer[220];
	WeChatPackHeader wechat_head;
	SendDataRequest_t	 SendDataRequest;
	uint8_t len,out_len,*p_in_data,*p_out_data;

	trans_header_st	 trans_header;
	
	memset(send_buffer, 0, sizeof(send_buffer));

	if(g_communication_statue.app_type == LIFESENSE_APP)
	{	
		p_in_data 	= data;
		
		trans_header.usTxDataType 		= 0;
	    trans_header.usTxDataPackSeq	= 0x0001;	//包序号
	    trans_header.usLength 			= length;
	    trans_header.usTxDataFrameSeq	= 0x01; 	//帧序号
	    
	    out_len = app_add_pack_head(trans_header,p_in_data,send_buffer, 0);
	}
	else if(g_communication_statue.app_type == WECHAT_APP)
	{	
		wechat_head.ucMagicNumber= WECHAT_PACK_HEAD_MAGICNUM;       	//包头的magic number，这个值是固定的
		wechat_head.ucVersion	= WECHAT_PACK_HEAD_VERSIOM;			//包头的versionr，这个值是固定的	
		wechat_head.usLength 	= 0;
		wechat_head.usCmdID 		= WECHAT_CMDID_REQ_UTC;
		wechat_head.usTxDataPackSequence = usTxWeChatPackSeq++;		// >= 0x0003;
	
		SendDataRequest.BaseRequest =0x00;

		SendDataRequest.Data = data;

		p_out_data 	= ucDataAfterPack;
		
		p_in_data 	= &SendDataRequest.BaseRequest;
		out_len		= PackDataType(DATA_BASE_REQUEST_FIELD, Length_delimit, p_in_data, DATA_BASE_REQUEST_LENGTH, p_out_data);
		p_out_data 	+= out_len;
		len 		= out_len;

		p_in_data 	= SendDataRequest.Data;
		out_len		= PackDataType(DATA_DATA_FIELD, Length_delimit, p_in_data, length, p_out_data);
		len 		+= out_len;

		wechat_head.usLength = len + WECHAT_PACKET_HEAD_LENGTH;
		out_len = app_add_wechat_head(wechat_head, ucDataAfterPack, send_buffer);
	}

	if(g_communication_statue.app_type == LIFESENSE_APP)
	{	
		error = transfer_send_data(send_buffer, out_len,TRANS_INDICATE_CHANNEL,0);
	}
	else if(g_communication_statue.app_type == WECHAT_APP)
	{			
		error = wechat_send_data(send_buffer, out_len,WECHAT_INDICATE_CHANNEL,0);
	}
	
	return error;
}

uint32_t app_add_heap_send_data(uint8_t data_type,uint8_t data_id,uint8_t *data,uint16_t length)
{
	return 0;
}

