#include "crashmail.h"

#ifdef OS_HAS_SYSLOG
#include <syslog.h>
bool usesyslog;

int syslogpri[] = {  LOG_INFO,		/* SYSTEMINFO */
   						LOG_ERR, 		/* SYSTEMERR */
   						LOG_INFO,		/* TOSSINGINFO */
	   					LOG_ERR, 		/* TOSSINGERR */
							LOG_INFO,		/* MISCINFO */
							LOG_DEBUG,		/* DEBUG */
							LOG_INFO,	   /* AREAFIX */
							LOG_INFO,		/* ACTIONINFO */
							LOG_ERR  		/* USERERR */ 
   					};

#endif

osFile logfh;

bool OpenLogfile(uchar *logfile)
{
#ifdef OS_HAS_SYSLOG
	if(stricmp(logfile,"syslog")==0)
	{
		usesyslog=TRUE;
		openlog("CrashMail",0,LOG_USER);
		return(TRUE);
	}
#endif

   if(!(logfh=osOpen(logfile,MODE_READWRITE)))
	{
		uint32_t err=osError();
      printf("Failed to open logfile %s\n",config.cfg_LogFile);
		printf("Error: %s\n",osErrorMsg(err));	
      return(FALSE);
	}

   osSeek(logfh,0,OFFSET_END);

   return(TRUE);
}

void CloseLogfile(void)
{
#ifdef OS_HAS_SYSLOG
	if(usesyslog)
	{
		closelog();
		usesyslog=FALSE;
		return;
	}
#endif

   osClose(logfh);
}

uchar *categoryletters="-%=!/D+^?";

void LogWrite(uint32_t level,uint32_t category,uchar *fmt,...)
{
   va_list args;
   time_t t;
   struct tm *tp;
   uchar *monthnames[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec","???"};
   uchar buf[500];
   int i;

   if(level > config.cfg_LogLevel)
      return;

   if(level == 0)
      LogWrite(6,DEBUG,"*** Warning: Loglevel is 0!!! ***");

   if(fmt[0]==0)
   {
      printf("\n");
      return;
   }

   if(handle_nesting > 1 && handle_nesting + strlen(fmt) < 499)
   {
      buf[0]=0;

      for(i=1;i<handle_nesting;i++)
         strcat(buf," ");

      strcat(buf,fmt);

      fmt=buf;
   }

#ifdef OS_HAS_SYSLOG
	if(usesyslog)
	{
	   va_start(args, fmt);
		vprintf(fmt,args);
	   printf("\n");
	   va_end(args);

	   va_start(args, fmt);
		vsyslog(syslogpri[category],fmt,args);
	   va_end(args);

		return;
	}
#endif

   va_start(args, fmt);
   vprintf(fmt,args);
   printf("\n");
   va_end(args);

   time(&t);
   tp=localtime(&t);

   osFPrintf(logfh,"%c %02d-%s-%02d %02d:%02d:%02d ",
      categoryletters[category],
      tp->tm_mday,
      monthnames[tp->tm_mon],
      tp->tm_year%100,
      tp->tm_hour,
      tp->tm_min,
      tp->tm_sec);

   va_start(args, fmt);
   osVFPrintf(logfh,fmt,args);
   osFPrintf(logfh,"\n");

   va_end(args);
}

