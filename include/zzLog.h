#ifndef _ZZLOG_H_
#define _ZZLOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>
#include <fstream>
#include <ctime>
#include <deque>
#include <string>
#include <cstring>

#include "Singleton.h"

#define HANDLE(X) \
    do { \
        if ((X) == -1) { \
            Log.Error(__FUNC_HEAD__, "%s", std::strerror(errno)); \
            exit(1); \
        } \
    } while(0)

#define LOG_LOCK pthread_mutex_lock(&log_mutex);
#define LOG_UNLOCK pthread_mutex_unlock(&log_mutex); 

#define Log_Error(...) Log.Error(__FUNC_HEAD__, __VA_ARGS__)
#define Log_Debug(...) Log.Debug(__FUNC_HEAD__, __VA_ARGS__)
#define Log_Notice(...) Log.Notice(__FUNC_HEAD__, __VA_ARGS__)
#define Log_Warning(...) Log.Warning(__FUNC_HEAD__, __VA_ARGS__)

#ifdef WIN32
#define WGSERVER_INLINE __forceinline
#else
#define WGSERVER_INLINE inline
#endif

#define DEFAULT 		0	//默认	
#define DEFAULT_STYLE	"\033[0m"	//默认样式	

#define HIGHLIGHT 		1	//高亮加粗
// #define BOLD			2	//粗体
#define ITALICS			3	//斜体
#define UNDERLINE		4	//下划线
#define FLASH			5	//闪烁
#define REVERSE_DISPLAY	7	//反显

#define NON_HIGHLIGHT 		21	//非高亮
#define NON_BOLD			22	//非粗体
#define NON_UNDERLINE		24	//非下划线
#define NON_FLASH			25	//非闪烁 
#define NON_REVERSE_DISPLAY	27	//非反显

#define FOREGROUND_BLACK		30
#define FOREGROUND_RED			31
#define FOREGROUND_GREEN		32
#define FOREGROUND_YELLOW		33
#define FOREGROUND_BLUE			34
#define FOREGROUND_PURPLE		35
#define FOREGROUND_CYAN			36
#define FOREGROUND_WHITE		37		

#define BACKGROUND_BLACK		40
#define BACKGROUND_RED			41
#define BACKGROUND_GREEN		42
#define BACKGROUND_YELLOW		43
#define BACKGROUND_BLUE			44
#define BACKGROUND_PURPLE		45
#define BACKGROUND_CYAN			46
#define BACKGROUND_WHITE		47

// #endif


//类名::函数名
static std::string functionname(std::string _name)
{
	_name = _name.substr(_name.find_first_of(' ')+1);

	unsigned int num = 0;
	for(int i = _name.find_last_of('('); i >= 0; --i)
	{
		if(_name[i] == ':')
			num++;

		if(num > 2)
		{
			_name = _name.substr(i+1, _name.size() - i);
			break;
		}
	}
	_name = _name.substr(0,_name.find_first_of('('));

	std::string newString = _name;

	return newString;
}



static std::string itos(int i) // 将int 转换成string
{
    std::stringstream s;
    s << i;
    return s.str();
}

static std::string functionline(int _line)
{
	std::string newString = "[" + itos(_line) + "] ";
	return newString;
}

#define __FUNC_HEAD__ ((functionline(__LINE__) + functionname(__PRETTY_FUNCTION__)).c_str())

// tm g_localTime;

class ZZLog : public Singleton< ZZLog >
{

public:
	void Color(unsigned int foreground_color=DEFAULT, unsigned int style=DEFAULT, unsigned int background_color=BACKGROUND_BLACK)
	{
		std::stringstream s;
		if(DEFAULT == foreground_color)
		{
			s << DEFAULT_STYLE;
		}
		else
		{
			s << "\033[" << style << ";" << foreground_color << ";" << background_color << "m";
		}
		fputs(s.str().c_str(), stdout);
	}

	WGSERVER_INLINE void Time()
	{
		time_t tt = time(NULL);
        tm * t = localtime(&tt);
		printf("%02u:%02u ", t->tm_hour, t->tm_min);
		// printf("%02u:%02u ", g_localTime.tm_hour, g_localTime.tm_min);
	}

	WGSERVER_INLINE void fTime()
	{
		time_t tt = time(NULL);
        tm g_localTime = *localtime(&tt);

		if(m_last_time.tm_mday != g_localTime.tm_mday || m_last_time.tm_wday != g_localTime.tm_wday || m_last_time.tm_yday != g_localTime.tm_yday || m_last_time.tm_mon != g_localTime.tm_mon)
			CreateLogFile();
		fprintf(mLogFile,"%02u:%02u ", g_localTime.tm_hour, g_localTime.tm_min);
	}

	void Success(const char * source, const char * format, ...)
	{
		LOG_LOCK
		va_list ap;
		va_start(ap, format);
		Time();
		// Color(TGREEN);
		Color(FOREGROUND_GREEN);
		fputs("S ", stdout);

		if(*source)
		{
			//Color(TBLUE);
			Color(FOREGROUND_GREEN, ITALICS);
			Color(FOREGROUND_GREEN, UNDERLINE);

			fputs(source, stdout);

			putchar(':');
			putchar(' ');
			// Color(FOREGROUND_GREEN, 1, BACKGROUND_PURPLE);
			Color(FOREGROUND_GREEN);
			// Color(TGREEN);
		}
		vprintf(format, ap);
		va_end(ap);
		Color();
		putchar('\n');


		va_start(ap, format);
		fTime();
		fputs("S ", mLogFile);
		if(*source)
		{
			fprintf(mLogFile,"%s: ",source);
			fprintf(mLogFile,": ");
		}

		vfprintf(mLogFile,format, ap);
		va_end(ap);
		fputc('\n',mLogFile);
		fflush(mLogFile);

		LOG_UNLOCK
	}


	void Error(const char * source, const char * format, ...)
	{
		LOG_LOCK
		va_list ap;
		va_start(ap, format);
		Time();
		Color(FOREGROUND_RED);
		fputs("E ", stdout);

		if(*source)
		{
			Color(FOREGROUND_RED, ITALICS);
			Color(FOREGROUND_RED, UNDERLINE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(FOREGROUND_RED);
		}

		vprintf(format, ap);
		va_end(ap);
		Color();
		putchar('\n');

		va_start(ap, format);
		fTime();
		fputs("E ", mLogFile);
		if(*source)
		{
			fprintf(mLogFile,"%s: ",source);
			fprintf(mLogFile,": ");
		}

		vfprintf(mLogFile,format, ap);
		va_end(ap);
		fputc('\n',mLogFile);
		fflush(mLogFile);

		LOG_UNLOCK
	}

	void Notice(const char * source, const char * format, ...)
	{
		LOG_LOCK

			va_list ap;
		va_start(ap, format);
		Time();
		Color(FOREGROUND_PURPLE);
		fputs("N ", stdout);

		if (*source)
		{
			Color(FOREGROUND_PURPLE, ITALICS);
			Color(FOREGROUND_PURPLE, UNDERLINE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(FOREGROUND_PURPLE);
			//Color(FOREGROUND_PURPLE, FLASH);
		}

		vprintf(format, ap);
		va_end(ap);
		Color();
		putchar('\n');

		va_start(ap, format);
		fTime();
		fputs("N ", mLogFile);
		if(*source)
		{
			fprintf(mLogFile,"%s: ",source);
			fprintf(mLogFile,": ");
		}

		vfprintf(mLogFile,format, ap);
		va_end(ap);
		fputc('\n',mLogFile);
		fflush(mLogFile);

		LOG_UNLOCK
	}

	void Warning(const char * source, const char * format, ...)
	{
		LOG_LOCK

		va_list ap;
		va_start(ap, format);
		Time();
		Color(FOREGROUND_YELLOW);
		fputs("W ", stdout);

		if(*source)
		{
			Color(FOREGROUND_YELLOW, ITALICS);
			Color(FOREGROUND_YELLOW, UNDERLINE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color();
			Color(FOREGROUND_YELLOW, FLASH);
		}

		vprintf(format, ap);
		va_end(ap);
		Color();
		putchar('\n');

		va_start(ap, format);
		fTime();
		fputs("W ", mLogFile);
		if(*source)
		{
			fprintf(mLogFile,"%s: ",source);
			fprintf(mLogFile,": ");
		}

		vfprintf(mLogFile,format, ap);
		va_end(ap);
		fputc('\n',mLogFile);
		fflush(mLogFile);

		LOG_UNLOCK
	}

	void Debug(const char * source, const char * format, ...)
	{
		LOG_LOCK

		va_list ap;
		va_start(ap, format);
		Time();
		Color(FOREGROUND_BLUE);
		fputs("D ", stdout);

		if(*source)
		{
			Color(FOREGROUND_BLUE, ITALICS);
			Color(FOREGROUND_BLUE, UNDERLINE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(FOREGROUND_BLUE);
		}

		vprintf(format, ap);
		va_end(ap);
		Color();
		putchar('\n');

		va_start(ap, format);
		fTime();
		fputs("D ", mLogFile);
		if(*source)
		{
			fprintf(mLogFile,"%s: ",source);
			fprintf(mLogFile,": ");
		}

		vfprintf(mLogFile,format, ap);
		va_end(ap);
		fputc('\n',mLogFile);
		fflush(mLogFile);

		LOG_UNLOCK
	}
	void CreateLogFile(void)
	{
		//m_last_time = g_localTime;
		time_t unix_time = time(0);
		localtime_r(&unix_time,&m_last_time);

		char filename[1024];
		sprintf(filename,"%s-[%04u-%02u-%02u].log",mLogFileBaseName.c_str(),m_last_time.tm_year + 1900,m_last_time.tm_mon + 1,m_last_time.tm_mday);
		mLogFileName = filename;
		if(mLogFile)
		{
			fclose(mLogFile);
			mLogFile = 0;
		}
		mLogFile = fopen(mLogFileName.c_str(),"wt+");
		if(!mLogFile)
		{
			printf("Open Logfile:%s failed.",mLogFileName.c_str());
			_exit(1);
		}
	}
	void InitLog(const std::string & logFile = "game")
	{
		mLogFileBaseName = logFile;
		mLogFileName = "";

		CreateLogFile();
	}

	void PushToFileStream(const char *str, FILE *stream)
	{
		fputs( str, stream );
		fputc(' ', stream );
	}

	ZZLog()
	{
		InitLog();
		pthread_mutex_init(&log_mutex, NULL);
	}
	~ZZLog()
	{
		if(mLogFile)
		{
			fclose(mLogFile);
			mLogFile = 0;
		}
		pthread_mutex_destroy(&log_mutex);
	}
public:
	pthread_mutex_t log_mutex;
	std::deque<std::string> deLog;

protected:
	std::string		mLogFileName;
	std::string		mLogFileBaseName;
	FILE 	  * 	mLogFile;
	tm		m_last_time;
};

#define Log ZZLog::getSingleton()

#endif