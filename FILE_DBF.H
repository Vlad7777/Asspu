#include <io.h>
#include <stdio.h>
#include <mem.h>
#include <conio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

class FileDBF
	{
	private:
		struct headDBF
			{
			char memo;
			char yeas;
			char month;
			char day;
			unsigned long quantity;
			unsigned short removal;
			unsigned short step;
			char reserve[20];
			} *indexHeadDBF;
		int handle;
	public:
		FileDBF( char *fullName )
			{
			indexHeadDBF = new headDBF;
			handle = open( fullName, O_RDWR| O_BINARY );
			lseek( handle, 0L, SEEK_SET );
			read( handle, indexHeadDBF, sizeof( headDBF ) );
			};
		~FileDBF()
			{
			delete indexHeadDBF;
			close(handle);
			};
		int readDBF( long nambe, void *back )
			{
			if( nambe >= (long)indexHeadDBF->quantity )
				return( 0 );

			lseek( handle, (long)(indexHeadDBF->removal + 1 + (unsigned long)nambe * (unsigned long)indexHeadDBF->step), SEEK_SET );
			read( handle, back, (unsigned)indexHeadDBF->step-1 );
			return( 1 );
			}
		int writeDBF( long nambe, void *back )
			{
			if( nambe >= (long)indexHeadDBF->quantity )
				return( 0 );

			lseek( handle, (long)(indexHeadDBF->removal + 1 + (unsigned long)nambe * (unsigned long)indexHeadDBF->step), SEEK_SET );
			write( handle, back, (unsigned)indexHeadDBF->step-1 );
			return( 1 );
			}
		int DelRecord( long nambe )
			{
			if( nambe >= (long)indexHeadDBF->quantity )
				return( 0 );

			lseek( handle, (long)(indexHeadDBF->removal + (unsigned long)nambe * (unsigned long)indexHeadDBF->step), SEEK_SET );
			write( handle, "*", 1 );
			return( 1 );
			}
		int IsDelRecord( long nambe )
			{
			if( nambe >= (long)indexHeadDBF->quantity )
				return( 0 );
			char Del;
			lseek( handle, (long)(indexHeadDBF->removal + (unsigned long)nambe * (unsigned long)indexHeadDBF->step), SEEK_SET );
			read( handle, &Del, 1 );
			if ( Del == '*' )
				return( 1 );
			return( 0 );
			}
		int AppendRecord()
			{
			char* tmp = (char*)malloc( indexHeadDBF->step );
			setmem( (void*)tmp, indexHeadDBF->step, ' ' );
			lseek( handle, (long)(indexHeadDBF->removal + (unsigned long)indexHeadDBF->quantity * (unsigned long)indexHeadDBF->step), SEEK_SET );
			write( handle, tmp, (unsigned)indexHeadDBF->step );
			free( tmp );
			(indexHeadDBF->quantity)++;
			lseek( handle, 0L, SEEK_SET );
			write( handle, indexHeadDBF, sizeof( headDBF ) );
			return( 1 );
			}
	};


