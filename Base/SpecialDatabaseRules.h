#ifndef SPECIALDATABASERULES_H_INCLUDED
#define SPECIALDATABASERULES_H_INCLUDED

#define SPECIALCASE_NONE 0
#define SPECIALCASE_DOUBLEFORRDB 1		//in rdb, records is [int numSubRecord; record subRecords[numSubRecords];]
#define SPECIALCASE_SKILLJPSP 2			//Skill.rdb is represented with 2 SQL database: SkillResource and SkillJPResource

#endif // SPECIALDATABASERULES_H_INCLUDED
