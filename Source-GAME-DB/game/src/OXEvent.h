#define OXEVENT_MAP_INDEX 113

struct tag_Quiz
{
	char level;
	char Quiz[256];
	bool answer;
};

enum OXEventStatus
{
	OXEVENT_FINISH = 0, // OX�̺�Ʈ�� ������ ���� ����
	OXEVENT_OPEN = 1, // OX�̺�Ʈ�� ���۵�. ������(20012)�� ���ؼ� ���尡��
	OXEVENT_CLOSE = 2, // OX�̺�Ʈ�� ������ ����. ������(20012)�� ���� ������ ���ܵ�
	OXEVENT_QUIZ = 3, // ��� ������.

	OXEVENT_ERR = 0xff
};

typedef std::map<DWORD, DWORD> MapEventChar;
typedef std::vector<std::vector<tag_Quiz>> QuizVector;

class COXEventManager : public singleton<COXEventManager>
{
private:
	MapEventChar m_map_char;
	MapEventChar m_map_attender;
	MapEventChar m_map_miss;

	QuizVector m_vec_quiz;

	LPEVENT m_timedEvent;

protected:
	bool CheckAnswer();

	bool EnterAudience(LPCHARACTER pChar);
	bool EnterAttender(LPCHARACTER pChar);

public:
	bool Initialize();
	void Destroy();

	OXEventStatus GetStatus();
	void SetStatus(OXEventStatus status);

	bool LoadQuizScript(const char* szFileName);

	bool Enter(LPCHARACTER pChar);

	bool CloseEvent();

	void ClearQuiz();
	bool AddQuiz(unsigned char level, const char* pszQuestion, bool answer);
	bool ShowQuizList(LPCHARACTER pChar);

	bool Quiz(unsigned char level, int timelimit);
	bool GiveItemToAttender(DWORD dwItemVnum, WORD count);

	bool CheckAnswer(bool answer);
	void WarpToAudience();

	bool LogWinner();

	DWORD GetAttenderCount() { return m_map_attender.size(); }

};
