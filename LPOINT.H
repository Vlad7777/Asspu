#if !defined(LPOINT_H)
#define LPOINT_H

#if !defined(OWL_POINT_H)
# include <owl/point.h>
#endif

class TLPoint;
class TLSize;
class TLRect;

class TIPoint : public ClassLib::TPoint    // ?????????????????????
	{
	public:
		// Constructors
		TIPoint() {}
		TIPoint( int _x, int _y ) : ClassLib::TPoint( _x, _y ) {}  //##########
		TIPoint( const POINT far& point ) : ClassLib::TPoint( point ) {} //########
        //###############################??????????????????
		TIPoint( const ClassLib::TPoint far& point ) : ClassLib::TPoint( point ) {}   //################
		TIPoint( const SIZE far& size ) : ClassLib::TPoint( size ) {}
		TIPoint( uint32 dw ) : ClassLib::TPoint( dw ) {}

		// Functions/binary-operators that return points or sizes
		TIPoint    	operator * ( const float& arg ) const { return TIPoint( (int)( (float)x * arg ), (int)( (float)y * arg ) ); }
		TIPoint    	operator / ( const float& arg ) const { return TIPoint( (int)( (float)x / arg ), (int)( (float)y / arg ) ); }

		// Functions/assignement-operators that modify this point
		TIPoint&   	operator = ( const TLPoint& point );
		TIPoint&    operator *= ( const float& arg );
		TIPoint&  	operator /= ( const float& arg );
	};

class TISize : public ClassLib::TSize  //#######################
	{
	public:
		// Constructors
		TISize() {}
		TISize( int dx, int dy ) : ClassLib::TSize( dx, dy ) {}
		TISize( const POINT far& point ) : ClassLib::TSize( point ) {}
		TISize( const SIZE far& size ) : ClassLib::TSize( size ) {}
		TISize( const ClassLib::TSize far& size ) : ClassLib::TSize( size ) {}
		TISize( uint32 dw ) : TSize( dw ) {}

		// Functions/binary-operators that return sizes
		TISize		operator * ( const float& arg ) const { return TISize( (int)( (float)cx * arg ), (int)( (float)cy * arg ) ); }
		TISize		operator / ( const float& arg ) const { return TISize( (int)( (float)cx / arg ), (int)( (float)cy / arg ) ); }

		// Functions/assignement-operators that modify this size
		TISize&		operator = ( const TLSize& size );
		TISize&     operator *= ( const float& arg );
		TISize&     operator /= ( const float& arg );
	};

class TIRect : public ClassLib::TRect    //#####################
	{
	public:
		// Constructors
		TIRect() {}
		TIRect( const RECT far& rect ) : ClassLib::TRect( rect ) {}
		TIRect( const ClassLib::TRect far& rect ) : ClassLib::TRect( rect ) {}
		TIRect( int _left, int _top, int _right, int _bottom ) : ClassLib::TRect( _left, _top, _right, _bottom ) {}
		TIRect( const ClassLib::TPoint& upLeft, const ClassLib::TPoint& loRight ) : ClassLib::TRect( upLeft, loRight ) {}
		TIRect( const ClassLib::TPoint& origin, const ClassLib::TSize& extent ) : ClassLib::TRect( origin, extent ) {}

		TIRect		operator * ( const float& arg ) const { return TIRect( (int)( (float)left * arg ), (int)( (float)top * arg ), (int)( (float)right * arg ), (int)( (float)bottom * arg ) ); }
		TIRect		operator / ( const float& arg ) const { return TIRect( (int)( (float)left / arg ), (int)( (float)top / arg ), (int)( (float)right / arg ), (int)( (float)bottom / arg ) ); }

		TIRect&		operator = ( const TLRect& rect );
		TIRect&     operator *= ( const float& arg );
		TIRect&     operator /= ( const float& arg );
	};







//
// Geometry C-structs compatible with MSW
//
struct tagLPOINT
	{
	long x;
	long y;
	};
typedef struct tagLPOINT LPOINT;

struct tagLSIZE
	{
	long cx;
	long cy;
	};
typedef struct tagLSIZE LSIZE;

struct tagLRECT
	{
	long left;
	long top;
	long right;
	long bottom;
	};
typedef struct tagLRECT LRECT;

//
// class TLPoint
// ----- ------
//
class TLPoint : public tagLPOINT
	{
	public:
		// Constructors
		TLPoint() {}
		TLPoint( long _x, long _y ) { x = _x; y = _y; }
		TLPoint( const LPOINT far& point ) { x = point.x; y = point.y; }
		TLPoint( const TLPoint far& point ) { x = point.x; y = point.y; }
		TLPoint( const LSIZE far& size ) { x = size.cx; y = size.cy; }

		// Information functions/operators
		bool        operator == ( const TLPoint& other ) const { return ToBool( other.x == x && other.y == y ); }
		bool        operator != ( const TLPoint& other ) const { return ToBool( other.x != x || other.y != y ); }

		// Functions/binary-operators that return points or sizes
		TLPoint     OffsetBy( long dx, long dy ) const { return TLPoint( x + dx, y + dy ); }
		TLPoint     operator + ( const TLSize& size ) const;
		TLSize      operator - ( const TLPoint& point ) const;
		TLPoint     operator - ( const TLSize& size ) const;
		TLPoint     operator - () const { return TLPoint( -x, -y ); }
		TLPoint  	operator * ( const float& arg ) const { return TLPoint( (long)( (float)x * arg ), (long)( (float)y * arg ) ); }
		TLPoint    	operator / ( const float& arg ) const { return TLPoint( (long)( (float)x / arg ), (long)( (float)y / arg ) ); }

		// Functions/assignement-operators that modify this point
		TLPoint&    Offset( long dx, long dy );
		TLPoint&    operator += ( const TLSize& size );
		TLPoint&    operator -= ( const TLSize& size );
		TLPoint&   	operator = ( const TIPoint& point );
		TLPoint&    operator *= ( const float& arg );
		TLPoint&  	operator /= ( const float& arg );
	};
inline ipstream& operator >> ( ipstream& is, TLPoint& p )
	{ return is >> p.x >> p.y; }
inline opstream& operator << ( opstream& os, const TLPoint& p )
	{ return os << p.x << p.y; }
inline ostream& operator << ( ostream& os, const TLPoint& p )
	{ return os << '(' << p.x << ',' << p.y << ')'; }

//
// class TLSize
// ----- -----
//
class TLSize : public tagLSIZE
	{
	public:
		// Constructors
		TLSize() {}
		TLSize( long dx, long dy) { cx = dx; cy = dy; }
		TLSize( const LPOINT far& point ) { cx = point.x; cy = point.y; }
		TLSize( const SIZE far& size ) { cx = size.cx; cy = size.cy; }
		TLSize( const TLSize far& size ) { cx = size.cx; cy = size.cy; }

		// Information functions/operators
		bool        operator == ( const TLSize& other ) const { return ToBool( other.cx == cx && other.cy == cy ); }
		bool        operator != ( const TLSize& other ) const { return ToBool( other.cx != cx || other.cy != cy ); }
		long        Magnitude() const { return Sqrt( long( cx ) * long( cx ) + long( cy ) * long( cy ) ); }

		// Functions/binary-operators that return sizes
		TLSize      operator + ( const TLSize& size ) const { return TLSize( cx + size.cx, cy + size.cy ); }
		TLSize      operator - ( const TLSize& size ) const { return TLSize( cx - size.cx, cy - size.cy ); }
		TLSize      operator - () const { return TLSize( -cx, -cy ); }
		TLSize  	   operator * ( const float& arg ) const { return TLSize( (long)( (float)cx * arg ), (long)( (float)cy * arg ) ); }
		TLSize		operator / ( const float& arg ) const { return TLSize( (long)( (float)cx / arg ), (long)( (float)cy / arg ) ); }

		// Functions/assignement-operators that modify this size
		TLSize&     operator += ( const TLSize& size );
		TLSize&     operator -= ( const TLSize& size );
		TLSize&     operator = ( const TISize& size );
		TLSize&     operator *= ( const float& arg );
		TLSize&     operator /= ( const float& arg );
	};
inline ipstream& operator >> ( ipstream& is, TLSize& s )
	{ return is >> s.cx >> s.cy; }
inline opstream& operator << ( opstream& os, const TLSize& s )
	{ return os << s.cx << s.cy; }
inline ostream& operator << ( ostream& os, const TLSize& s )
	{ return os << '(' << s.cx << 'x' << s.cy << ')'; }

//
// class TLRect
// ----- -----
//
class TLRect : public tagLRECT
	{
	public:
		// Constructors
		TLRect() {}
		TLRect( const LRECT far& rect );
		TLRect( const TLRect far& rect );
		TLRect( long _left, long _top, long _right, long _bottom );
		TLRect( const TLPoint& upLeft, const TLPoint& loRight );
		TLRect( const TLPoint& origin, const TLSize& extent );

		// (re)Initializers
		void        SetNull() { left = top = right = bottom = 0; }
		void        SetEmpty() { SetNull(); }
		void        Set( long _left, long _top, long _right, long _bottom ) { left = _left, top = _top, right = _right, bottom = _bottom; }


		// Type Conversion operators
		operator    const TLPoint*() const { return (const TLPoint*)this; }
		operator    TLPoint*() { return (TLPoint*)this; }

		// Testing functions
		bool        IsEmpty() const;
		bool        IsNull() const;
		bool        operator == ( const TLRect& other ) const;
		bool        operator != ( const TLRect& other ) const;

		// Information/access functions(const and non-const)
		const TLPoint& TopLeft() const { return *(TLPoint*)&left; }
		TLPoint&       TopLeft() { return *(TLPoint*)&left; }
		TLPoint      	TopRight() const { return TLPoint( right, top ); }
		TLPoint      	BottomLeft() const { return TLPoint( left, bottom );}
		const TLPoint& BottomRight() const { return *(TLPoint*)&right; }
		TLPoint&     	BottomRight() { return *(TLPoint*)&right; }
		long           Width() const { return right-left; }
		long           Height() const { return bottom-top; }
		TLSize         LSize() const { return TLSize( Width(), Height() ); }
		long           Area() const { return long( Width() ) * long( Height() ); }

		bool     	Contains( const TLPoint& point ) const;
		bool        Contains( const TLRect& other ) const;
		bool        Touches( const TLRect& other ) const;
		TLRect      OffsetBy( long dx, long dy ) const;
		TLRect      operator + ( const TLSize& size ) const;
		TLRect      operator - ( const TLSize& size ) const;
		TLRect      operator * ( const float& arg ) const;
		TLRect      operator / ( const float& arg ) const;
		TLRect      InflatedBy( long dx, long dy ) const;
		TLRect      InflatedBy( const TLSize& size ) const;
		TLRect      Normalized() const;
		TLRect      operator & ( const TLRect& other ) const;
		TLRect      operator | ( const TLRect& other ) const;

		// Manipulation functions/operators
		TLRect&      	Normalize();
		TLRect&      	Offset( long dx, long dy);
		TLRect&        operator += ( const TLSize& delta );
		TLRect&      	operator -= ( const TLSize& delta );
		TLRect&      	operator = ( const TIRect& rect );
		TLRect&      	operator *= ( const float& arg );
		TLRect&      	operator /= ( const float& arg );
		TLRect&      	Inflate( long dx, long dy );
		TLRect&        Inflate( const TLSize& delta );
		TLRect&        operator &= ( const TLRect& other );
		TLRect&        operator |= ( const TLRect& other );

	};
inline ipstream& operator >> ( ipstream& is, TLRect& r )
	{ return is >> r.left >> r.top >> r.right >> r.bottom; }
inline opstream& operator << ( opstream& os, const TLRect& r )
	{ return os << r.left << r.top << r.right << r.bottom; }
inline ostream& operator << ( ostream& os, const TLRect& r )
	{ return os << '(' << r.left << ',' << r.top << '-' << r.right << ',' << r.bottom << ')'; }



//----------------------------------------------------------------------------
// Inlines
//----------------------------------------------------------------------------

inline TIPoint& TIPoint::operator = ( const TLPoint& point )
	{
	x = (int)point.x;
	y = (int)point.y;
	return *this;
	}

inline TIPoint& TIPoint::operator *= ( const float& arg )
	{
	x = (int)( (float)x * arg );
	y = (int)( (float)y * arg );
	return *this;
	}

inline TIPoint& TIPoint::operator /= ( const float& arg )
	{
	x = (int)( (float)x / arg );
	y = (int)( (float)y / arg );
	return *this;
	}

inline TISize& TISize::operator = ( const TLSize& size )
	{
	cx = (int)size.cx;
	cy = (int)size.cy;
	return *this;
	}

inline TISize& TISize::operator *= ( const float& arg )
	{
	cx = (int)( (float)cx * arg );
	cy = (int)( (float)cy * arg );
	return *this;
	}

inline TISize& TISize::operator /= ( const float& arg )
	{
	cx = (int)( (float)cx / arg );
	cy = (int)( (float)cy / arg );
	return *this;
	}

inline TIRect& TIRect::operator = ( const TLRect& rect )
	{
	left   = (int)rect.left;
	top    = (int)rect.top;
	right  = (int)rect.right;
	bottom = (int)rect.bottom;
	return *this;
	}

inline TIRect& TIRect::operator *= ( const float& arg )
	{
	left   = (int)( (float)left * arg );
	top    = (int)( (float)top * arg );
	right  = (int)( (float)right * arg );
	bottom = (int)( (float)bottom * arg );
	return *this;
	}

inline TIRect& TIRect::operator /= ( const float& arg )
	{
	left   = (int)( (float)left / arg );
	top    = (int)( (float)top / arg );
	right  = (int)( (float)right / arg );
	bottom = (int)( (float)bottom / arg );
	return *this;
	}



inline TLPoint TLPoint::operator + ( const TLSize& size ) const
	{ return TLPoint( x + size.cx, y + size.cy ); }

inline TLSize TLPoint::operator - ( const TLPoint& point ) const
	{ return TLSize( x - point.x, y - point.y ); }

inline TLPoint TLPoint::operator - ( const TLSize& size ) const
	{ return TLPoint( x - size.cx, y - size.cy ); }

inline TLPoint& TLPoint::Offset( long dx, long dy )
	{
	x += dx;
	y += dy;
	return *this;
	}

inline TLPoint& TLPoint::operator += ( const TLSize& size )
	{
	x += size.cx;
	y += size.cy;
	return *this;
	}

inline TLPoint& TLPoint::operator -= ( const TLSize& size )
	{
	x -= size.cx;
	y -= size.cy;
	return *this;
	}

inline TLPoint& TLPoint::operator = ( const TIPoint& point )
	{
	x = (long)point.x;
	y = (long)point.y;
	return *this;
	}

inline TLPoint& TLPoint::operator *= ( const float& arg )
	{
	x = (long)( (float)x * arg );
	y = (long)( (float)y * arg );
	return *this;
	}

inline TLPoint& TLPoint::operator /= ( const float& arg )
	{
	x = (long)( (float)x / arg );
	y = (long)( (float)y / arg );
	return *this;
	}

inline TLSize& TLSize::operator += ( const TLSize& size )
	{
	cx += size.cx;
	cy += size.cy;
	return *this;
	}

inline TLSize& TLSize::operator -= ( const TLSize& size )
	{
	cx -= size.cx;
	cy -= size.cy;
	return *this;
	}

inline TLSize& TLSize::operator = ( const TISize& size )
	{
	cx = (long)size.cx;
	cy = (long)size.cy;
	return *this;
	}

inline TLSize& TLSize::operator *= ( const float& arg )
	{
	cx = (long)( (float)cx * arg );
	cy = (long)( (float)cy * arg );
	return *this;
	}

inline TLSize& TLSize::operator /= ( const float& arg )
	{
	cx = (long)( (float)cx / arg );
	cy = (long)( (float)cy / arg );
	return *this;
	}

inline TLRect::TLRect( const LRECT far& rect )
	{ *(LRECT far*)this = rect; }

inline TLRect::TLRect( const TLRect far& rect )
	{ *(LRECT far*)this = *(LRECT far*)&rect; }

inline TLRect::TLRect( long _left, long _top, long _right, long _bottom )
	{ Set( _left, _top, _right, _bottom ); }

inline TLRect::TLRect( const TLPoint& topLeft, const TLPoint& bottomRight )
	{ Set( topLeft.x, topLeft.y, bottomRight.x, bottomRight.y ); }

inline TLRect::TLRect( const TLPoint& origin, const TLSize& extent )
	{ Set( origin.x, origin.y, origin.x + extent.cx, origin.y + extent.cy ); }

inline bool TLRect::IsEmpty() const
	{ return ToBool( left >= right || top >= bottom ); }

inline bool TLRect::IsNull() const
	{ return ToBool( !left && !right && !top && !bottom ); }

inline bool TLRect::operator == ( const TLRect& other ) const
	{ return ToBool( other.left == left && other.top == top && other.right == right && other.bottom == bottom ); }

inline bool TLRect::operator != ( const TLRect& other ) const
	{ return ToBool( !( other == *this ) ); }

inline bool TLRect::Contains( const TLPoint& point ) const
	{ return ToBool( point.x >= left && point.x < right && point.y >= top  && point.y < bottom ); }

inline bool TLRect::Contains( const TLRect& other ) const
	{ return ToBool( other.left >= left && other.right <= right && other.top >= top && other.bottom <= bottom ); }

inline bool TLRect::Touches( const TLRect& other ) const
	{ return ToBool( other.right > left && other.left < right && other.bottom > top && other.top < bottom ); }

inline TLRect TLRect::OffsetBy( long dx, long dy ) const
	{ return TLRect(left + dx, top + dy, right + dx, bottom + dy ); }

inline TLRect TLRect::operator + ( const TLSize& size ) const
	{ return OffsetBy( size.cx, size.cy ); }

inline TLRect TLRect::operator - ( const TLSize& size ) const
	{ return OffsetBy( -size.cx, -size.cy ); }

inline TLRect TLRect::operator * ( const float& arg ) const
	{ return TLRect( (long)( (float)left * arg ), (long)( (float)top * arg ), (long)( (float)right * arg ),(long)( (float)bottom * arg ) ); }

inline TLRect TLRect::operator / ( const float& arg ) const
	{ return TLRect( (long)( (float)left / arg ), (long)( (float)top / arg ), (long)( (float)right / arg ), (long)( (float)bottom / arg ) ); }

inline TLRect TLRect::InflatedBy( long dx, long dy ) const
	{ return TLRect( left - dx, top - dy, right + dx, bottom + dy ); }

inline TLRect TLRect::InflatedBy( const TLSize& size ) const
	{ return InflatedBy( size.cx, size.cy ); }

inline TLRect TLRect::Normalized() const
	{ return TLRect( Min( left, right ), Min( top, bottom ), Max( left, right ), Max( top, bottom ) ); }

inline TLRect TLRect::operator & ( const TLRect& other ) const
	{ return TLRect( Max( left, other.left ), Max( top, other.top ), Min( right, other.right ), Min( bottom, other.bottom ) ); }

inline TLRect TLRect::operator | ( const TLRect& other ) const
	{ return TLRect( Min( left, other.left ), Min( top, other.top ), Max( right, other.right ), Max( bottom, other.bottom ) ); }

inline TLRect& TLRect::operator += ( const TLSize& delta )
	{
	Offset( delta.cx, delta.cy );
	return *this;
	}

inline TLRect& TLRect::operator -= ( const TLSize& delta )
	{ return *this += -delta; }

inline TLRect& TLRect::operator = ( const TIRect& rect )
	{
	left   = (long)rect.left;
	top    = (long)rect.top;
	right  = (long)rect.right;
	bottom = (long)rect.bottom;
	return *this;
	}

inline TLRect& TLRect::operator *= ( const float& arg )
	{
	left   = (long)( (float)left * arg );
	top    = (long)( (float)top * arg );
	right  = (long)( (float)right * arg );
	bottom = (long)( (float)bottom * arg );
	return *this;
	}

inline TLRect& TLRect::operator /= ( const float& arg )
	{
	left   = (long)( (float)left / arg );
	top    = (long)( (float)top / arg );
	right  = (long)( (float)right / arg );
	bottom = (long)( (float)bottom / arg );
	return *this;
	}

inline TLRect& TLRect::Inflate( const TLSize& delta )
	{ return Inflate( delta.cx, delta.cy ); }

inline TLRect& TLRect::Normalize()
	{
	if ( left > right )
		Swap( left, right );
	if ( top > bottom )
		Swap( top, bottom );
	return *this;
	}

inline TLRect& TLRect::Offset( long dx, long dy )
	{
	left += dx;
	top += dy;
	right += dx;
	bottom += dy;
	return *this;
	}

inline TLRect& TLRect::Inflate( long dx, long dy )
	{
	left -= dx;
	top -= dy;
	right += dx;
	bottom += dy;
	return *this;
	}

inline TLRect& TLRect::operator &= ( const TLRect& other )
	{
	if ( !IsNull() ) 
		{
		if ( other.IsNull() )
			SetNull();
		else 
			{
			left = Max( left, other.left );
			top = Max( top, other.top );
			right = Min( right, other.right );
			bottom = Min( bottom, other.bottom );
			}
		}
	return *this;
	}

inline TLRect& TLRect::operator |= ( const TLRect& other )
	{
	if ( !other.IsNull() ) 
		{
		if ( IsNull() )
			*this = other;
		else 
			{
			left = Min( left, other.left );
			top = Min( top, other.top );
			right = Max( right, other.right );
			bottom = Max( bottom, other.bottom );
			}
		}
	return *this;
	}


#endif  // LPOINT_H
