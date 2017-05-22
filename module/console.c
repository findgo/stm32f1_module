
#include "console.h"

//!<    MACROS
//! \brief char interval timer
#define CONSOLE_CHAR_INTERVAL_TIME      (100u)
//! \brief define space key ascii code
#define CONSOLE_SPACE                   (0x20u)
//! \brief Default Separator : Space
#define CONSOLE_SEPARATOR               " "
//! \brief Default Separator : clear
#define CONSOLE_CLEAR_CODE              0x0C
//���������ܴ�С
#define CONSOLE_CMD_CONSOLE_BUFFER_SIZE     (CONSOLE_BUFFER_SIZE + 1)
//!< \bad command note
#define CONSOLE_BADCOMMAND_PRINT    "\tBad command or file name,and use \'help\' or '?' to get more info!\r\n"


//!< ��������
#define KEY_CODE_ESC    0x00001BUL
#define KEY_CODE_F1     0x1B4F50UL
#define KEY_CODE_F3     0x1B4F52UL
#define KEY_CODE_UP     0x1B5B41UL
#define KEY_CODE_DOWN   0x1B5B42UL

typedef struct {
    uint8_t index; // ָ������
    uint8_t chCmd[CONSOLE_CMD_CONSOLE_BUFFER_SIZE];             
} console_curcmd_t;

typedef struct {
    uint8_t chCmdLen; // ָ���
    uint8_t chCmd[CONSOLE_CMD_CONSOLE_BUFFER_SIZE];             
} console_cmd_t;

typedef struct {
    uint8_t s_chCurHisCmdPtr;   // ��ǰ��ʷ�����±�
    uint8_t s_chLastCmdPtr;     // ���һ����ʷ�����±�
    uint8_t s_chHisCmdNum;      // ��ʷ��������
    uint8_t s_chCurHisCmdNum;   // ��ǰ������ʷ����������
    console_cmd_t s_tHisCmd[CONSOLE_HIS_CMD_NUM];
} his_cmd_queue_t;

typedef struct {
	uint8_t argc;//��������
	char *argv[CONSOLE_ARGV_MAX_SIZE];//ָ������
} console_arg_t;

static void cmd_help_hander(uint8_t argc, char *argv[], void *pcmd);
static void cmd_clear_hander(uint8_t argc, char *argv[], void *pcmd);

static const console_command_t s_tCMD[] = {   
    {"clear", &cmd_clear_hander, "clear\t-- Clear Screen",NULL},
    {"help",  &cmd_help_hander,  "help\t-- How to use the console",NULL}, 
    {"?",  &cmd_help_hander,  "?\t-- same as help,How to use the console",NULL},
};

//!< ���յ��������
static console_curcmd_t s_chCurCmd = {0,{'\0'}};
//!< ��ʷ����
static his_cmd_queue_t s_chHisCmd;
//!< ��������
static console_arg_t s_argCmd;

//!< dynamic command ��̬�ⲿ����
static console_command_t *s_ptDynaCmdHead = NULL;

/*============================ IMPLEMENTATION ================================*/

//!< ok
static void console_hiscmd_save(console_cmd_t *ptHistory)
{
    uint8_t i;
    his_cmd_queue_t *ptQ = &s_chHisCmd;
    
    // ������ʷ����
    for ( i = 0; i <= ptHistory->chCmdLen; i++ ){    // ��'\0'Ҳ����
        ptQ->s_tHisCmd[ptQ->s_chLastCmdPtr].chCmd[i] = ptHistory->chCmd[i];
    }
    ptQ->s_tHisCmd[ptQ->s_chLastCmdPtr].chCmdLen = ptHistory->chCmdLen;

    if ( ptQ->s_chHisCmdNum < CONSOLE_HIS_CMD_NUM ){ // ��ʷ��������
        ptQ->s_chHisCmdNum++;
    }
    ptQ->s_chCurHisCmdNum = ptQ->s_chHisCmdNum - 1; // ���µ�ǰ�������������

    ptQ->s_chCurHisCmdPtr = ptQ->s_chLastCmdPtr;  // ���µ�ǰ��ʷ�����±�
    if ( ++ptQ->s_chLastCmdPtr >= CONSOLE_HIS_CMD_NUM){ //������������� ����һ�α��渲�����������
        ptQ->s_chLastCmdPtr = 0;
    }               
}

//!< ɾ��һ���ַ�
static void console_display_backspace(uint8_t chNum)
{
    if(chNum == 0)
        return;

    while(chNum--){
        console_writebyte('\b');    //����һ��
        console_writebyte(' '); //����հ�
        console_writebyte('\b'); //����һ��
    }
}
//!< �س�������һ��
static void console_prn_rn(void)
{
    console_writebyte('\r'); //����س�
    console_writebyte('\n'); //����һ��
}


#define FRONT_END_CHECK24_START     0
#define FRONT_END_CHECK24_SEC       1
#define FRONT_END_CHECK24_THR       2
#define FRONT_END_CHECK24_RESET()   do{s_chState = 0;}while(0)
#define FRONT_END_CHECK24_TIMEOUT_NUM       500UL
/**
  * @brief   ��⹦�ܰ�������(���ܰ���24λ����)
  * @param  pwCmd  -- ���Է��ؼ�⵽�Ĺ��ܼ�����ĵ�ַ
  * @note   
  * @retval     FSM stutas detail in common_type.h
  */
static fsm_rt_t front_end_check24(uint32_t *pwCmd)
{
    static uint8_t s_chState = FRONT_END_CHECK24_START;
    static uint32_t s_wTimeOut;     // ��ʱ������
    static uint8_t s_chTemp;
    static uint32_t s_wCmdTemp;

    assert_param(NULL != pwCmd);

    *pwCmd = 0;
    switch (s_chState) 
    {
        case FRONT_END_CHECK24_START:
            s_wTimeOut = 0;
            s_chState = FRONT_END_CHECK24_SEC;      // fall through
            
        case FRONT_END_CHECK24_SEC:
            if( s_wTimeOut < FRONT_END_CHECK24_TIMEOUT_NUM ) {
                if(console_readbyte(&s_chTemp)) {
                    s_wCmdTemp = (KEY_CODE_ESC << 16) + ( ((uint32_t)s_chTemp) << 8 );
                    s_wTimeOut = 0;
                    s_chState = FRONT_END_CHECK24_THR;
                } else {
                    s_wTimeOut++;
                }
            } else {                    // ��ʱ
                *pwCmd = KEY_CODE_ESC;  // ����Esc������
                FRONT_END_CHECK24_RESET();
                return fsm_rt_cpl;
            }
            break;
            
        case FRONT_END_CHECK24_THR:
            if( s_wTimeOut < FRONT_END_CHECK24_TIMEOUT_NUM ) {
                if( console_readbyte(&s_chTemp) ) {
                    s_wCmdTemp += s_chTemp;
                    s_wTimeOut = 0;
                    *pwCmd = s_wCmdTemp;
                    FRONT_END_CHECK24_RESET();
                    return fsm_rt_cpl;
                } else {
                    s_wTimeOut++;
                }
            } else {                    // ��ʱ
                *pwCmd = KEY_CODE_ESC;  // ����Esc������
                FRONT_END_CHECK24_RESET();
                return fsm_rt_cpl;
            }
            break;
            
		default: 
	   		break;
    }
    return fsm_rt_on_going;
}
/**
  * @brief   ʵ�� F1 ����
  * @param  None
  * @note   
  * @retval none
  */
static void front_fun_key_f1(void)
{
    uint8_t s_chTemp;
    
    if( s_chCurCmd.index < s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmdLen ) {
        s_chTemp = s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmd[s_chCurCmd.index];
        s_chCurCmd.chCmd[s_chCurCmd.index++] = s_chTemp;
        console_writebyte(s_chTemp);
    } 
}
/**
  * @brief  ʵ�� F3 ���ܵĺ���
  * @param  None
  * @note   
  * @retval     none
  */
static void front_fun_key_f3(void)
{
    uint8_t chLen;

    if ( s_chCurCmd.index < s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmdLen ) {
        chLen =s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmdLen;
        for( ; s_chCurCmd.index < chLen; s_chCurCmd.index++ ) {
            s_chCurCmd.chCmd[s_chCurCmd.index] = s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmd[s_chCurCmd.index];
            console_writebyte(s_chCurCmd.chCmd[s_chCurCmd.index]);
        }
    }
}
/**
  * @brief  ʵ�� up ���ܵĺ���  ����һ��ָ��
  * @param  None
  * @note   
  * @retval  none
  */
static void front_fun_key_up(void)
{
    uint8_t chLen;

    if( s_chHisCmd.s_chHisCmdNum > 0 ){             // ����ʷ����    
        console_display_backspace(s_chCurCmd.index); //��ǰȫɾ
        chLen =s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmdLen;
        for( s_chCurCmd.index = 0; s_chCurCmd.index < chLen; s_chCurCmd.index++ ) {
            s_chCurCmd.chCmd[s_chCurCmd.index] = s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmd[s_chCurCmd.index];
            console_writebyte(s_chCurCmd.chCmd[s_chCurCmd.index]);
        }
        if(s_chHisCmd.s_chCurHisCmdPtr == 0){
            s_chHisCmd.s_chCurHisCmdPtr = s_chHisCmd.s_chCurHisCmdNum;
        }else{
            s_chHisCmd.s_chCurHisCmdPtr--;
        }
    }
}
/**
  * @brief  ʵ�� down ���ܵĺ���  ����һ��ָ��
  * @param  None
  * @note   
  * @retval  none
  */
static void front_fun_key_down(void)
{
    uint8_t chLen;

    if( s_chHisCmd.s_chHisCmdNum > 0 ){             // ����ʷ����    
        console_display_backspace(s_chCurCmd.index); //��ǰȫɾ
        chLen =s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmdLen;
        for( s_chCurCmd.index = 0; s_chCurCmd.index < chLen; s_chCurCmd.index++ ) {
            s_chCurCmd.chCmd[s_chCurCmd.index] = s_chHisCmd.s_tHisCmd[s_chHisCmd.s_chCurHisCmdPtr].chCmd[s_chCurCmd.index];
            console_writebyte(s_chCurCmd.chCmd[s_chCurCmd.index]);
        }
        if(s_chHisCmd.s_chCurHisCmdPtr == s_chHisCmd.s_chCurHisCmdNum){
            s_chHisCmd.s_chCurHisCmdPtr = 0;
        }else{
            s_chHisCmd.s_chCurHisCmdPtr++;
        }
    }
}
/**
  * @brief  �ַ����ܴ�����
  * @param  wKey --�ַ�����
  * @note   
  * @note    
  * @note   
  * @retval FSM stutas detail in common_type.h
  */
static void front_end_function_key(uint32_t wKey)
{
    switch(wKey) {
        case KEY_CODE_F1:
            front_fun_key_f1();
            break;

        case KEY_CODE_F3:
            front_fun_key_f3();
            break;
            
        case KEY_CODE_UP:
            front_fun_key_up();
            break;

        case KEY_CODE_DOWN:
            front_fun_key_down();
            break;
        default:            // δ���尴��
             break;
    }
}


//!< �鿴�ַ��Ƿ�Ϊ�ָ���
static bool IsSeparator(uint8_t chByte)
{
    //!< command separator
    const uint8_t c_chSeparator[] = CONSOLE_SEPARATOR;
    uint8_t const *pchBuffer = &c_chSeparator[0];

    while ('\0' != *pchBuffer) {
        if (chByte == *pchBuffer) {
            return true;
        }   
        pchBuffer++;
    }
    
    return false;
}
#define FRONT_END_START                     0
#define FRONT_END_INPUT                     1
#define FRONT_END_CHECK24                   2
#define FRONT_END_TAKEN                     3
#define FRONT_END_RESET_FSM()               do{s_chState = 0;}while(0)

//>! ״̬���������յ�һ���������������󷵻�
static fsm_rt_t front_end(void)
{
    static uint8_t s_chState = FRONT_END_START;
    uint8_t chKey;
    uint32_t wKey;
    console_curcmd_t *pcmd = &s_chCurCmd;
    
    switch (s_chState) {
        case FRONT_END_START:
            pcmd->index = 0;
            console_writebyte('>'); // ���'>'
            s_chState = FRONT_END_INPUT; //fall through
            
        case FRONT_END_INPUT:
            if(console_readbyte(&chKey)) {
                if ('\b' == chKey) {                 //!<  Backspace(BS) �˸�
                    if (0 != pcmd->index) {
                        pcmd->index--;
                        console_display_backspace(1); // ɾ��һ���ַ�
                    }       
                } else if ('\r' == chKey) {     // �������� �س�
                    console_prn_rn();           
                    if(0 == pcmd->index){
                        s_chState = FRONT_END_START;
                    }else{
                        pcmd->chCmd[pcmd->index] = '\0';
                        FRONT_END_RESET_FSM();
                        s_chState = FRONT_END_TAKEN;
                    }
                }else if( (chKey >= 32) && (chKey <= 127) ){
                    if ( pcmd->index < (CONSOLE_CMD_CONSOLE_BUFFER_SIZE - 1)){ // δ��
                        pcmd->chCmd[pcmd->index++] = chKey;
                        console_writebyte(chKey); //!< ����
                    }
                }
                else if(chKey == 0x1B){ // Esc ����24λ���밴��
                    s_chState = FRONT_END_CHECK24; 
                }
            }
            break;
        case FRONT_END_CHECK24:
            if(IS_FSM_CPL(front_end_check24( &wKey ))){
                front_end_function_key(wKey);
                s_chState = FRONT_END_INPUT; 
            }
            break;
            
        case FRONT_END_TAKEN:
            //������ʷ����
            console_hiscmd_save((console_cmd_t *)pcmd);			
            FRONT_END_RESET_FSM();
            return fsm_rt_cpl;
            //break;
        default:
            FRONT_END_RESET_FSM();
            break;
    }
    
    return fsm_rt_on_going;
}

//!< ok
bool console_str_cpy(char *dst, const char *src)
{
    if ((NULL == dst) || (NULL == src)) {
        return false;
    }
    
    while ('\0' != *src) {
        *dst = *src;
        dst++;
        src++;
    }
    
    *dst = *src;
    
    return true;
}

// ok
bool console_str_cmp(char *pchString1, char *pchString2)
{
    if ( (NULL == pchString1) || (NULL == pchString2) ) {
        return false;
    }

    while (*pchString1 == *pchString2) {
        if ('\0' == *pchString1) {
            return true;
        }
        pchString1++;
        pchString2++;
    }
    
    return false;
}

// ok
uint8_t console_str_len(char *pchString)
{
    uint8_t chLen = 0;
    
    while ('\0' != *pchString) {
        pchString++;
        chLen++;
    }
    
    return chLen;
}

//!> search the command map to find it
static const console_command_t *search_cmd_map(char* proutine)
{
    uint8_t chCount = 0;
    console_command_t *srchcmd;

	if(proutine == NULL)
		return NULL;
	
    while (chCount < UBOUND(s_tCMD)) {
        if (strcmp(proutine, s_tCMD[chCount].pchCMD) == 0) {
            return &s_tCMD[chCount];
        }
        chCount++;
    }

    srchcmd = s_ptDynaCmdHead;
    while(srchcmd)
    {
        if(strcmp(proutine,srchcmd->pchCMD) == 0)
            return srchcmd;

        srchcmd = srchcmd->next;
    }
    
    return NULL;
}

// ok
static bool IsMeetHelp(char *pchTokens)
{
    char *s_pchHelpString[] = {"H", "h", "?"};
    uint8_t i = 0;

    if ( NULL == pchTokens ){
        return false;
    }
    
    for ( i = 0; i < UBOUND(s_pchHelpString); i++ ) {
        if ( strcmp(s_pchHelpString[i], pchTokens) == 0) {
            return true;
        }
    }
    return false;
}

static void cmd_help_hander(uint8_t argc, char *argv[], void *pcmd)
{
    uint8_t i;
    console_command_t *srchcmd;
    
    if(argc != 1){
        console_writestring(CONSOLE_BADCOMMAND_PRINT);
        return;
    }
    for(i = 0; i < UBOUND(s_tCMD); i++){
        console_writebyte('\t');
        console_writestring(s_tCMD[i].pchHelp);
        console_prn_rn();
    }

    srchcmd = s_ptDynaCmdHead;
    while(srchcmd)
    {
        if(srchcmd->pchHelp){
            console_writebyte('\t');
            console_writestring(srchcmd->pchHelp);
            console_prn_rn();
        }
        srchcmd = srchcmd->next;
    }
}


static void cmd_clear_hander(uint8_t argc, char *argv[], void *pcmd)
{
    if(argc != 1){
        console_writestring(CONSOLE_BADCOMMAND_PRINT);
        return;
    }
    
    console_writebyte(CONSOLE_CLEAR_CODE);
}


/*   ����Raw command
	ȥ������ո񣬲��Է��ַ�����'\0' ��β */
static void RawCmdtoken(console_arg_t *para)
{
    char *pchRead;
    char *pchWrite;
    bool bstringLast = false;//���ָ���ǰ������ǰ�����ַ���
    bool bSeparator;
    
    pchRead = (char *)&s_chCurCmd.chCmd[0];
    pchWrite = (char *)&s_chCurCmd.chCmd[0];
    para->argc = 0;
    para->argv[0] = pchWrite; 

    for(;;){
        if ('\0' != *pchRead) {
            bSeparator = IsSeparator(*pchRead);
            if (bSeparator) {
                if (bstringLast) {
                    *pchWrite++ = '\0';
                    para->argv[++para->argc] = pchWrite; //next command
                }
            }else{
                *pchWrite++ = *pchRead;
            }
            bstringLast = !bSeparator;
            pchRead++;              
        }else{
            if(bstringLast){
                ++para->argc;
                *pchWrite = '\0';
            }
            return;
        }
    }
}

static void parse(void)
{
	const console_command_t *s_ptCMD;
	
	RawCmdtoken(&s_argCmd);

	if(s_argCmd.argc < 1){
		console_writestring("throw a programs bug");
        console_prn_rn();
    }
	
    s_ptCMD = search_cmd_map(s_argCmd.argv[0]);
    if (NULL == s_ptCMD) { 
        console_writestring(CONSOLE_BADCOMMAND_PRINT);
    }else{//��Ϣ�ҵ�
        if(s_argCmd.argc == 2 && IsMeetHelp(s_argCmd.argv[1])){
            if(s_ptCMD->pchHelp){
                console_writebyte('\t');
                console_writestring(s_ptCMD->pchHelp);
                console_prn_rn();
            }
        }else{
            s_ptCMD->fncmd_handler(s_argCmd.argc,s_argCmd.argv, (void *)s_ptCMD);
        }   
    }
}

void console_init(void)
{
    console_extern_init();  
}

void console_task(void)
{
    fsm_rt_t fsm;
	
    fsm = front_end();
    if (IS_FSM_CPL(fsm)) {
        parse();
    } 
}


/*! \note console dynamic command register
 *  \param ptCmd register command
 * \param cmd show command 
 * \param cmd_handler  cmd handler
 * \param help show command help
 *  \retval true command register succeeded.
 *  \retval false command register  failed
 */
bool console_cmd_register(console_command_t *ptCmd, 
                            const char *cmd,
                            CONSOLE_CMD_HANDLER *cmd_handler,
                            const char *help)
{
    if(ptCmd == NULL || cmd == NULL || cmd_handler == NULL)
        return false;

    ptCmd->pchCMD = cmd;
    ptCmd->fncmd_handler = cmd_handler;
    ptCmd->pchHelp = help;
    ptCmd->next = NULL;
    
    if(!s_ptDynaCmdHead){
        s_ptDynaCmdHead = ptCmd;
    }else{
        ptCmd->next = s_ptDynaCmdHead;
        s_ptDynaCmdHead = ptCmd;
    }

    return true;
}

/* EOF */


