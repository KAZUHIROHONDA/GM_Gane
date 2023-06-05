#pragma once
//MessageManager.h
//���b�Z�[�W���Ǘ�����

//�񋓌^
enum MESSAGE_STATE
{
	E_NONE,		//�����o�Ă��Ȃ�
	E_NOW,		//�\����
	E_COMPLETE,	//�\������
	E_WAIT,		//ENTER���͑҂�
	E_TIMER,	//���ԑ҂�
	E_MAX
};

enum MESSAGE_TYPE
{
	E_TYPE_NONE,		//�\���Ȃ�
	E_TYPE_NORMAL,		//�o���ςȂ�
	E_TYPE_ENTER,		//ENTER���͑҂�
	E_TYPE_TIMER,		//���Ԑ�������
	E_TYPE_MAX
};

//�\����
typedef struct _tMessage
{
	wchar_t			szMessage[256]; //���b�Z�[�W
	MESSAGE_TYPE	type;			//�^�C�v
	unsigned long long int	param;			//�ėp
}tMessage;

// �v���g�^�C�v�錾
void InitMessage();
void UninitMessage();
void UpdateMessage();
void DrawMessage();
bool IsMessage();
void SetMessage(tMessage* pMessage);