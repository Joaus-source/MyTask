#pragma once

//xx_xx_xx_to_game: to_game�����ں˷��͸���Ϸ
//xx_xx_xx_to_driver:to_driver������Ϸ���͸��ں�
//�ṹ����ֶ�ȫ��ʹ��Сд,����ԭ������ͻȻ������ǰд��Ϸ��ʱ��Э�鶨����

#define  MAX_MSG_NUM 10
#define MAX_CHEAT_BUFF_SIZE 512

typedef struct __MsgNode
{
	struct __MsgNode* next;
	int nMsgNo;
	char buff[MAX_CHEAT_BUFF_SIZE];
}MsgNode;

#pragma pack(1)

#define HEARTBEAT_PACKET_TO_GAME 0 //�������͸���Ϸ��������
typedef struct _heartbeat_packet_to_game
{
	int ticket_count;
}heartbeat_packet_to_game;


//֪ͨ��Ϸ�˳�
#define  EXIT_CODE_TO_GAME 1  //��Ϣ��
typedef struct _notify_game_exit
{
#define  EXIT_REASON_FOR_ARK_TOOL 1   //��⵽ark����
#define  EXIT_REASON_FOR_DEBUG_GAME 2 //��⵽���Ե�����Ϸ

	int game_exit_code;
}notify_exit_to_game;


//�����û���ɶ��¼����ں�
#define  SEND_READ_ABLE_EVENT_HANDLE 2 //��Ϣ��
typedef struct _send_event_handle
{
	int event_handle;
}send_read_able_event_to_driver;

//���͸��ں˲���Ҫ�������̺߳ͽ���
#define  SEND_NEED_PROTECTED_THREAD_PROCESS 3//��Ϣ��
typedef struct _send_need_protected_process
{
	int process_id;
	int rcv_msg_thread_id;
	int protect_action;
	int beart_thread_id;
}send_need_protected_process_to_driver;
#define SEND_PROTECTED_ACTION_HNADLEPT (int)(1<<0) //�������
#define SEND_PROTECTED_ACTION_MMPT (int)(1<<1)		//�ڴ汣��
#define SEND_PROTECTED_ACTION_ANTIDEBUG (int)(1<<2)	//������
#define SEND_PROTECTED_ACTION_HIDEPROCESS (int)(1<<3)//��������

//ȡ������
#define CLEAR_PROTECTED_ACTION_HNADLEPT (int)~(1<<0) //�������
#define CLEAR_PROTECTED_ACTION_MMPT (int)~(1<<1)		//�ڴ汣��
#define CLEAR_PROTECTED_ACTION_ANTIDEBUG (int)~(1<<2)	//������
#define CLEAR_PROTECTED_ACTION_HIDEPROCESS (int)~(1<<3)//��������

#pragma pack()