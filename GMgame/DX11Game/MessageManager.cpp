//MessageManager.cpp
#include "main.h"
#include "input.h"
#include "Moji.h"
#include "MessageManager.h"

//�O���[�o���ϐ�
static MESSAGE_STATE	g_state;
static tMessage*		g_pMessage = NULL;
static int				g_nTimer = 0;
static Moji*			g_pMoji = NULL;

//������
void InitMessage()
{
	g_pMoji = new Moji();
	g_pMoji->Init(GetSwapChain());
}
//��Еt��
void UninitMessage()
{
	delete g_pMoji; g_pMoji = NULL;
}
//�X�V����
void UpdateMessage()
{
	if (g_pMoji == NULL) return;
	if (g_pMessage == NULL) return;

	switch (g_pMessage->type)
	{
	case MESSAGE_TYPE::E_TYPE_TIMER:
	{
		g_nTimer--;
		if (g_nTimer <= 0)
		{
			g_pMessage = NULL;
			return;
		}
	}break;
	case MESSAGE_TYPE::E_TYPE_ENTER:
	{
		if (GetKeyTrigger(VK_RETURN))
		{
			//��������΍X�V
			if (g_pMessage->param)
			{
				g_pMessage = (tMessage*)g_pMessage->param;
			}
			else
			{
				g_pMessage = NULL;
			}
			return;
		}
	}break;
	case MESSAGE_TYPE::E_TYPE_NORMAL:
	{

	}break;
		
	}

}
//�`�揈��
void DrawMessage()
{
	if (g_pMoji == NULL) return;
	if (g_pMessage == NULL) return;
	if (g_pMessage->szMessage == NULL) return;
	g_pMoji->Draw(g_pMessage->szMessage, lstrlen(g_pMessage->szMessage),
					200, 500, SCREEN_WIDTH - 200, 550);

}

//�����m�F
bool IsMessage()
{
	return g_state == MESSAGE_STATE::E_NONE
		?false:true;
}

void SetMessage(tMessage * pMessage)
{
	if (pMessage == NULL)return;
	//if(g_state == MESSAGE_STATE::)return;
	g_pMessage = pMessage;

	switch (pMessage->type)
	{
	case MESSAGE_TYPE::E_TYPE_TIMER:
	{
		g_nTimer = (int)pMessage->param;
	}break;
	
	}
}
