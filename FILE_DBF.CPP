#include <io.h>
#include <stdio.h>
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
	 int sizeStruct = sizeof( headDBF );
	 indexHeadDBF = new headDBF;
	 handle = open( fullName, O_RDWR| O_BINARY );
	 lseek( handle, 0L, SEEK_SET );
	 read( handle, indexHeadDBF, sizeStruct );
	 };
      ~FileDBF()
	 {
	 delete indexHeadDBF;
	 close(handle);
	 };
      int readDBF( long nambe, void *back )
	 {
	 if( nambe > (long)indexHeadDBF->quantity )
	    return( 0 );

	 lseek( handle, (long)(indexHeadDBF->removal + 1 +
		(unsigned long)nambe * (unsigned long)indexHeadDBF->step),
		SEEK_SET );
	 read( handle, back, (unsigned)indexHeadDBF->step-1 );
	 return( 1 );
	 }
      int writeDBF( long nambe, void *back )
	 {
	 if( nambe > (long)indexHeadDBF->quantity )
	    return( 0 );

	 lseek( handle, (long)(indexHeadDBF->removal + 1 +
		(unsigned long)nambe * (unsigned long)indexHeadDBF->step),
		SEEK_SET );
	 write( handle, back, (unsigned)indexHeadDBF->step-1 );
	 return( 1 );
	 }
   };

#include "dbf.str";

int main()
   {
   char Name[]="smuzl.dbf";
   class FileDBF *file = new FileDBF( Name );
   struct dbf SmuzlStruct, *indexSmuzlStruct = &SmuzlStruct;
   long i = 0;
   while( file->readDBF( i, (void *)indexSmuzlStruct ) )
      i++;
   file->writeDBF( 1, (void *)indexSmuzlStruct );
   return( 0 );
   }
