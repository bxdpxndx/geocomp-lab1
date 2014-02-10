#ifndef _TEXT_INC
#define _TEXT_INC

/*
A classic, used everywhere to read formatted text files without using Lex
and Yacc. Written in C around 1995
Re-vamped a million times, and included in several programs
Converted to C++ in 1.7.98 to use it in the Rayman project.
*/

class CText
{
protected:
	char *mData;
	unsigned int mSl;
	unsigned int mSize;
public:
	CText();
	CText(const char*);
	~CText();

	void Create(char *);
	char *GetWord();
	char *GetCommaWord();
	int GetInt();
	float GetFloat();

	int CountWord(char *);
	int CountChar(char);
	void Reset();
	void Destroy();
	void GoBack();
	bool Seek(std::string);
	int Legal(char c);
	int Eof();
};

#endif
