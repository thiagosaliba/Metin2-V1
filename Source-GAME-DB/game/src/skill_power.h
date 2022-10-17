#ifndef __INC_TABLE_BY_SKILL_H__
#define __INC_TABLE_BY_SKILL_H__

class CTableBySkill : public singleton<CTableBySkill>
{
public:
	CTableBySkill()
		: m_aiSkillDamageByLevel(NULL)
	{
		// ��ų ������ �߰������� �ʱ�ȭ
		for (int job = 0; job < JOB_MAX_NUM * 2; ++job)
			m_aiSkillPowerByLevelFromType[job] = NULL;
	}

	~CTableBySkill()
	{
		DeleteAll();
	}

	// ���̺� ���� üũ
	bool Check() const;

	// ����
	void DeleteAll();

	// ��ų�������� ��ų�Ŀ� ���̺�
	int GetSkillPowerByLevelFromType(int job, int skillgroup, int skilllevel, bool bMob) const;
	void SetSkillPowerByLevelFromType(int idx, const int* aTable);
	void DeleteSkillPowerByLevelFromType(int idx);

	// ������ �߰� ��ų������ ���̺�
	int GetSkillDamageByLevel(int Level) const;
	void SetSkillDamageByLevelTable(const int* aTable);
	void DeleteSkillDamageByLevelTable();

private:
	int* m_aiSkillPowerByLevelFromType[JOB_MAX_NUM * 2];
	int* m_aiSkillDamageByLevel;
};

#endif // __INC_TABLE_BY_SKILL_H__
