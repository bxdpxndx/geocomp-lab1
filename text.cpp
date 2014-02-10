#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "text.h"

CText::CText()
{

}


CText::CText(const char *name)
{
	FILE *f;
	struct stat stbuffer;

	stat(name,&stbuffer);
	if ( (f=fopen(name,"rb"))!=NULL)
	{
		mSize=stbuffer.st_size;
		mData=new char[mSize];
		mSl=0;
		fread(mData,mSize,1,f);
		fclose(f);
	}
	else
	{
        std::cout << "Error opening file \"" << name << "\"" << std::endl;
        exit(1);
	}
}



void CText::Create(char *name)
{
	FILE *f;
	struct stat stbuffer;

	stat(name,&stbuffer);
	f=fopen(name,"rb");
	
	mSize=stbuffer.st_size;
	mData=new char[mSize];
	mSl=0;
	fread(mData,mSize,1,f);
	fclose(f);
}

int CText::Legal(char c)
{
	int res;

	res=(c>32);
	return res;
}

char *CText::GetWord()
{
	int p0,p1,i;
	char *res;

	p0=mSl;
	while ((!Legal(mData[p0]))  && (p0<int(mSize)) ) p0++;
	
	p1=p0+1;
	
	while (Legal(mData[p1]))p1++;
	
	res=(char *)malloc(sizeof(char)*(1+p1-p0));
	
	for (i=p0;i<p1;i++)
	{
		if ((mData[i]<='z') && (mData[i]>='a')) 
			mData[i]+=('A'-'a');
		res[i-p0]=mData[i];
	}
	
	res[p1-p0]='\0';
	mSl=p1;
	return res;
}


char *CText::GetCommaWord()
{
	int p0,p1,i;
	char *res;

	p0=mSl;
	while (mData[p0]!='"') p0++;
	
	p0++;
	p1=p0+1;
	
	while (mData[p1]!='"') p1++;

	res=(char *)malloc(sizeof(char)*(1+p1-p0));
	
	for (i=p0;i<p1;i++)
	{
		//if ((mData[i]<='z') && (mData[i]>='a')) mData[i]+=('A'-'a');
		res[i-p0]=mData[i];
	}
	
	res[p1-p0]='\0';
	mSl=p1+1;
	return res;
}


int CText::GetInt()
{
	int res;
	char *tmp;

	tmp=GetWord();
	res=atoi(tmp);
	free(tmp);
	return res;
}

float CText::GetFloat()
{
	float res;
	char *tmp;

	tmp=GetWord();
	res=atof(tmp);
	free(tmp);
	return res;
}

void CText::GoBack()
{
	int p0,p1;
	p0 = mSl;

	while (!Legal(mData[p0])) p0--;
	p1 = p0-1;
	while (Legal(mData[p1])) p1--;
	mSl = p1;
}


int CText::CountChar(char c)
{
	int res, i;
	res=0;

	for (i=0;i<int(mSize);i++)
		if (mData[i]==c) 
			res++;
	return res;
}


void CText::Reset()
{
	mSl=0;
}

void CText::Destroy()
{
	if (mData!=NULL) 
		delete mData;
}


int CText::CountWord(char *s)
{
	int res,i;
	int final;
	unsigned int si;

	res=0;
	final=0;
	i=0;
	while (!final)
	{
		si=0;
		while (mData[i]==s[si])
		{
			i++;
			si++;
		}
		res+=(si==strlen(s));
		i+=si;
		i++;
		final=(i>=int(mSize));
	}
	return res;
}

int CText::Eof()
{
	return (mSl>mSize);
}


bool CText::Seek(std::string token)
{
	char *dummy = GetWord();
	int mSlAnt = mSl;

	while (strcmp(dummy,token.c_str()) && (mSl<mSize))
	{	
		free(dummy);
		dummy=GetWord();
	}
	free(dummy);
	if (mSl>=mSize)
	{
		mSl = mSlAnt;
		return false;
	}
	else
		return true;
}

CText::~CText()
{
	if (mData!=NULL) 
		delete [] mData;
}


