



#include "mcheck.h"


#define CKS_STATE_IDLE		0
#define CKS_STATE_CHECK		1
#define CKS_STATE_FILTER	2
#define CKS_STATE_WAITUP	3
#define CKS_STATE_UP_FILTER 4

static cks_mem_t *cks_head = NULL;

/**
  * @brief	checkregisterע��
  * @param	None
  * @note		pcks check�ṹָ�룬filtetime �˲�ʱ�䣬msΪ׼  pfuc ����check�ļ������ţ� pdowncallback ���»ص������� pUpcallfuc ����ص�����
  * @retval  None
  */
void checkregister(cks_mem_t *pcks, uint8_t filtetime, IsDownFunc pfuc, CheckCallBackFunc pdowncallback, CheckCallBackFunc pUpcallfuc)
{
	if(pcks == NULL || pfuc == NULL){
		return;
	}

	pcks->state = CKS_STATE_IDLE;
	pcks->filtertime = filtetime;
	pcks->filtercount = 0;
	pcks->pfuc = pfuc;
	pcks->pdowncallfuc = pdowncallback;
	pcks->pUpcallfuc = pUpcallfuc;
	pcks->next = NULL;

	if(!cks_head){
		cks_head = pcks;
	}else{						//��ջ
		pcks->next = cks_head;
		cks_head = pcks;
	}
}


/**
  * @brief	check ����
  * @param	ElapseTime ����ʱ��
  * @note		�½��غ������ض������˲����˲�һ����
  * @retval  None
  */
void checkupdate(uint16_t ElapseTime)
{
	cks_mem_t *pcur;

	pcur = cks_head; // ����
	while(pcur)
	{
		switch(pcur->state){
			case CKS_STATE_IDLE: // fall through
			case CKS_STATE_CHECK:
				if(pcur->pfuc()){
					pcur->filtercount = 0;
					pcur->state = CKS_STATE_FILTER;
				}
				break;
				
			case CKS_STATE_FILTER:
				pcur->filtercount += ElapseTime;
				if(pcur->filtercount >= pcur->filtertime){
					if(pcur->pfuc()){
						if(pcur->pdowncallfuc)
							pcur->pdowncallfuc();
						pcur->state = CKS_STATE_WAITUP;
					}else{
						pcur->state = CKS_STATE_IDLE;
					}
				}
				break;
				
			case CKS_STATE_WAITUP:
				if(!pcur->pfuc()){
					pcur->filtercount = 0;
					pcur->state = CKS_STATE_UP_FILTER;
				}
				break;
			case CKS_STATE_UP_FILTER:
				pcur->filtercount += ElapseTime;
				if(pcur->filtercount >= pcur->filtertime){
					if(!pcur->pfuc()){
						if(pcur->pUpcallfuc)
							pcur->pUpcallfuc();
						pcur->state = CKS_STATE_IDLE;
					}else{
						pcur->state = CKS_STATE_WAITUP;
					}
				}
				break;
			default:
				pcur->state = CKS_STATE_IDLE;
				break;
		}
		pcur = pcur->next;
	}
}


