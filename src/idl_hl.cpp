#ifdef EMSCRIPTEN

#include <emscripten.h>
#define HL_PRIM
#define HL_NAME(n)	EMSCRIPTEN_KEEPALIVE eb_##n
#define DEFINE_PRIM(ret, name, args)
#define _OPT(t) t*
#define _GET_OPT(value,t) *value
#define alloc_ref(r, _) r
#define alloc_ref_const(r,_) r
#define _ref(t)			t
#define _unref(v)		v
#define free_ref(v) delete (v)
#define HL_CONST const

#else

#define HL_NAME(x) clipper2_##x
#include <hl.h>
#include "hl-idl-helpers.hpp"
// Need to link in helpers
//HL_API hl_type hltx_ui16;
//HL_API hl_type hltx_ui8;
HL_PRIM hl_type hltx_ui16 = { HUI16 };
HL_PRIM hl_type hltx_ui8 = { HUI8 };

#define _IDL _BYTES
#define _OPT(t) vdynamic *
#define _GET_OPT(value,t) (value)->v.t

static  hl_type *strType = nullptr;
void hl_cache_string_type( vstring *str) {
   strType = str->t;
}

vstring * hl_utf8_to_hlstr( const char *str) {
    int strLen = (int)strlen( str );
    uchar * ubuf = (uchar*)hl_gc_alloc_noptr((strLen + 1) << 1);
    hl_from_utf8( ubuf, strLen, str );

    vstring* vstr = (vstring *)hl_gc_alloc_raw(sizeof(vstring));

    vstr->bytes = ubuf;
    vstr->length = strLen;
    vstr->t = strType;
    return vstr;
}
vstring * hl_utf8_to_hlstr( const std::string &str) {
	return hl_utf8_to_hlstr(str.c_str());
}

HL_PRIM vstring * HL_NAME(getdllversion)(vstring * haxeversion) {
	strType = haxeversion->t;
	return haxeversion;
}
DEFINE_PRIM(_STRING, getdllversion, _STRING);

class HNativeBuffer {
    unsigned char *_ptr;
    int _size;

   public:
   inline unsigned char * ptr() { return _ptr; }
   inline int size() { return _size; }
   HNativeBuffer(unsigned char *ptr, int size) : _ptr(ptr), _size(size) {}
   HNativeBuffer(int size) : _ptr(new unsigned char[size]), _size(size) {}
    ~HNativeBuffer() {
        if (_ptr != nullptr)
            delete [] _ptr;
    }
};
template <typename T> struct pref {
	void (*finalize)( pref<T> * );
	T *value;
};

#define _ref(t) pref<t>
#define _unref(v) v->value
#define _unref_ptr_safe(v) (v != nullptr ? v->value : nullptr)
#define alloc_ref(r,t) _alloc_ref(r,finalize_##t)
#define alloc_ref_const(r, _) _alloc_const(r)
#define HL_CONST

template<typename T> void free_ref( pref<T> *r ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	delete r->value;
	r->value = NULL;
	r->finalize = NULL;
}

template<typename T> void free_ref( pref<T> *r, void (*deleteFunc)(T*) ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	deleteFunc( r->value );
	r->value = NULL;
	r->finalize = NULL;
}

inline void testvector(_h_float3 *v) {
  printf("v: %f %f %f\n", v->x, v->y, v->z);
}
template<typename T> pref<T> *_alloc_ref( T *value, void (*finalize)( pref<T> * ) ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_finalizer(sizeof(pref<T>));
	r->finalize = finalize;
	r->value = value;
	return r;
}

template<typename T> pref<T> *_alloc_const( const T *value ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_noptr(sizeof(pref<T>));
	r->finalize = NULL;
	r->value = (T*)value;
	return r;
}

inline static varray* _idc_alloc_array(float *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hlt_f32, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}
inline static varray* _idc_alloc_array(unsigned char *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hltx_ui8, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array( char *src, int count) {
	return _idc_alloc_array((unsigned char *)src, count);
}

inline static varray* _idc_alloc_array(int *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	int *p;
	a = hl_alloc_array(&hlt_i32, count);
	p = hl_aptr(a, int);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;

}

inline static varray* _idc_alloc_array(double *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	double *p;
	a = hl_alloc_array(&hlt_f64, count);
	p = hl_aptr(a, double);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}


inline static varray* _idc_alloc_array(const unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array(unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static void _idc_copy_array( float *dst, varray *src, int count) {
	float *p = hl_aptr(src, float);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, float *src,  int count) {
	float *p = hl_aptr(dst, float);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( int *dst, varray *src, int count) {
	int *p = hl_aptr(src, int);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( unsigned short *dst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( const unsigned short *cdst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	unsigned short *dst = (unsigned short *)cdst;
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, int *src,  int count) {
	int *p = hl_aptr(dst, int);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( double *dst, varray *src, int count) {
	double *p = hl_aptr(src, double);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, double *src,  int count) {
	double *p = hl_aptr(dst, double);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}

#endif

#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#include "hl-clip.h"
#include "polypartition.h"
using namespace Clipper2Lib;





extern "C" {

static JoinType JoinType__values[] = { JoinType::Square,JoinType::Round,JoinType::Miter };
HL_PRIM int HL_NAME(JoinType_toValue0)( int idx ) {
	return (int)JoinType__values[idx];
}
DEFINE_PRIM(_I32, JoinType_toValue0, _I32);
HL_PRIM int HL_NAME(JoinType_indexToValue1)( int idx ) {
	return (int)JoinType__values[idx];
}
DEFINE_PRIM(_I32, JoinType_indexToValue1, _I32);
HL_PRIM int HL_NAME(JoinType_valueToIndex1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)JoinType__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, JoinType_valueToIndex1, _I32);
HL_PRIM int HL_NAME(JoinType_fromValue1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)JoinType__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, JoinType_fromValue1, _I32);
HL_PRIM int HL_NAME(JoinType_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, JoinType_fromIndex1, _I32);
static EndType EndType__values[] = { EndType::Polygon,EndType::Joined,EndType::Butt,EndType::Square,EndType::Round };
HL_PRIM int HL_NAME(EndType_toValue0)( int idx ) {
	return (int)EndType__values[idx];
}
DEFINE_PRIM(_I32, EndType_toValue0, _I32);
HL_PRIM int HL_NAME(EndType_indexToValue1)( int idx ) {
	return (int)EndType__values[idx];
}
DEFINE_PRIM(_I32, EndType_indexToValue1, _I32);
HL_PRIM int HL_NAME(EndType_valueToIndex1)( int value ) {
	for( int i = 0; i < 5; i++ ) if ( value == (int)EndType__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, EndType_valueToIndex1, _I32);
HL_PRIM int HL_NAME(EndType_fromValue1)( int value ) {
	for( int i = 0; i < 5; i++ ) if ( value == (int)EndType__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, EndType_fromValue1, _I32);
HL_PRIM int HL_NAME(EndType_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, EndType_fromIndex1, _I32);
static void finalize_PathD( pref<PathD>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PathD_delete)( pref<PathD>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PathD_delete, _IDL);
static void finalize_PathTreeD( pref<PolyTreeD>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PathTreeD_delete)( pref<PolyTreeD>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PathTreeD_delete, _IDL);
static void finalize_PathsD( pref<PathsD>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PathsD_delete)( pref<PathsD>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PathsD_delete, _IDL);
static void finalize_PartitionPolyIt( pref<PartitionPolyIt>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PartitionPolyIt_delete)( pref<PartitionPolyIt>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PartitionPolyIt_delete, _IDL);
static void finalize_PartitionPolyList( pref<TPPLPolyList>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PartitionPolyList_delete)( pref<TPPLPolyList>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PartitionPolyList_delete, _IDL);
static void finalize_PartitionPoly( pref<TPPLPoly>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PartitionPoly_delete)( pref<TPPLPoly>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PartitionPoly_delete, _IDL);
HL_PRIM int HL_NAME(PathD_numPoints0)(pref<PathD>* _this) {
	return (_unref(_this)->size());
}
DEFINE_PRIM(_I32, PathD_numPoints0, _IDL);

HL_PRIM void HL_NAME(PathD_getPoint2)(pref<PathD>* _this, int idx, vbyte* coordinates2D) {
	(HLClip::PathD_getPoint( _unref(_this) , idx, (double*)coordinates2D));
}
DEFINE_PRIM(_VOID, PathD_getPoint2, _IDL _I32 _BYTES);

HL_PRIM void HL_NAME(PathD_getPoints1)(pref<PathD>* _this, vbyte* coordinates2D) {
	(HLClip::PathD_getPoints( _unref(_this) , (double*)coordinates2D));
}
DEFINE_PRIM(_VOID, PathD_getPoints1, _IDL _BYTES);

HL_PRIM int HL_NAME(PathTreeD_numChildren0)(pref<PolyTreeD>* _this) {
	return (_unref(_this)->Count());
}
DEFINE_PRIM(_I32, PathTreeD_numChildren0, _IDL);

HL_PRIM HL_CONST pref<PathD>* HL_NAME(PathTreeD_getPath0)(pref<PolyTreeD>* _this) {
	return alloc_ref_const((HLClip::PolyTreeD_getPath( _unref(_this) )),PathD);
}
DEFINE_PRIM(_IDL, PathTreeD_getPath0, _IDL);

HL_PRIM HL_CONST pref<PolyTreeD>* HL_NAME(PathTreeD_getChild1)(pref<PolyTreeD>* _this, int idx) {
	return alloc_ref_const((HLClip::PolyTreeD_getChild( _unref(_this) , idx)),PathTreeD);
}
DEFINE_PRIM(_IDL, PathTreeD_getChild1, _IDL _I32);

HL_PRIM pref<PathsD>* HL_NAME(PathsD_new0)() {
	return alloc_ref((new PathsD()),PathsD);
}
DEFINE_PRIM(_IDL, PathsD_new0,);

HL_PRIM void HL_NAME(PathsD_addPolygonD2)(pref<PathsD>* _this, vbyte* coordinates, int vertCount) {
	(HLClip::addPolygonD( _unref(_this) , (double*)coordinates, vertCount));
}
DEFINE_PRIM(_VOID, PathsD_addPolygonD2, _IDL _BYTES _I32);

HL_PRIM void HL_NAME(PathsD_addPolygonsD3)(pref<PathsD>* _this, vbyte* coordinates, vbyte* vertCount, int polyCount) {
	(HLClip::addPolygonsD( _unref(_this) , (double*)coordinates, (int*)vertCount, polyCount));
}
DEFINE_PRIM(_VOID, PathsD_addPolygonsD3, _IDL _BYTES _BYTES _I32);

HL_PRIM pref<PathsD>* HL_NAME(PathsD_unionAll1)(pref<PathsD>* _this, bool fillHoles) {
	return alloc_ref((HLClip::unionAll( _unref(_this) , fillHoles)),PathsD);
}
DEFINE_PRIM(_IDL, PathsD_unionAll1, _IDL _BOOL);

HL_PRIM pref<PolyTreeD>* HL_NAME(PathsD_unionAllAsTree0)(pref<PathsD>* _this) {
	return alloc_ref((HLClip::unionAllAsTree( _unref(_this) )),PathTreeD);
}
DEFINE_PRIM(_IDL, PathsD_unionAllAsTree0, _IDL);

HL_PRIM int HL_NAME(PathsD_numPolygons0)(pref<PathsD>* _this) {
	return (_unref(_this)->size());
}
DEFINE_PRIM(_I32, PathsD_numPolygons0, _IDL);

HL_PRIM int HL_NAME(PathsD_polygonVertCounts1)(pref<PathsD>* _this, vbyte* counts) {
	return (HLClip::polygonCounts( _unref(_this) , (int*)counts));
}
DEFINE_PRIM(_I32, PathsD_polygonVertCounts1, _IDL _BYTES);

HL_PRIM void HL_NAME(PathsD_getAllCoordinates1)(pref<PathsD>* _this, vbyte* coordinates2D) {
	(HLClip::getAllCoordinates( _unref(_this) , (double*)coordinates2D));
}
DEFINE_PRIM(_VOID, PathsD_getAllCoordinates1, _IDL _BYTES);

HL_PRIM pref<PathsD>* HL_NAME(PathsD_inflate5)(pref<PathsD>* _this, double delta, int jointype, int endtype, double miter_limit, int precision) {
	return alloc_ref((HLClip::polyList_inflate( _unref(_this) , delta, JoinType__values[jointype], EndType__values[endtype], miter_limit, precision)),PathsD);
}
DEFINE_PRIM(_IDL, PathsD_inflate5, _IDL _F64 _I32 _I32 _F64 _I32);

HL_PRIM int HL_NAME(PartitionPolyIt_getNumPoints0)(pref<PartitionPolyIt>* _this) {
	return (_unref(_this)->GetNumPoints());
}
DEFINE_PRIM(_I32, PartitionPolyIt_getNumPoints0, _IDL);

HL_PRIM void HL_NAME(PartitionPolyIt_getPoints1)(pref<PartitionPolyIt>* _this, vbyte* points) {
	(_unref(_this)->GetPoints((double*)points));
}
DEFINE_PRIM(_VOID, PartitionPolyIt_getPoints1, _IDL _BYTES);

HL_PRIM bool HL_NAME(PartitionPolyIt_isValid0)(pref<PartitionPolyIt>* _this) {
	return (_unref(_this)->IsValid());
}
DEFINE_PRIM(_BOOL, PartitionPolyIt_isValid0, _IDL);

HL_PRIM bool HL_NAME(PartitionPolyIt_next0)(pref<PartitionPolyIt>* _this) {
	return (_unref(_this)->Next());
}
DEFINE_PRIM(_BOOL, PartitionPolyIt_next0, _IDL);

HL_PRIM HL_CONST pref<TPPLPoly>* HL_NAME(PartitionPolyIt_getPoly0)(pref<PartitionPolyIt>* _this) {
	return alloc_ref_const((_unref(_this)->Ptr()),PartitionPoly);
}
DEFINE_PRIM(_IDL, PartitionPolyIt_getPoly0, _IDL);

HL_PRIM pref<TPPLPolyList>* HL_NAME(PartitionPolyList_new0)() {
	return alloc_ref((new TPPLPolyList()),PartitionPolyList);
}
DEFINE_PRIM(_IDL, PartitionPolyList_new0,);

HL_PRIM void HL_NAME(PartitionPolyList_clear0)(pref<TPPLPolyList>* _this) {
	(_unref(_this)->clear());
}
DEFINE_PRIM(_VOID, PartitionPolyList_clear0, _IDL);

HL_PRIM int HL_NAME(PartitionPolyList_numPolys0)(pref<TPPLPolyList>* _this) {
	return (HLPartition::NumPolys( _unref(_this) ));
}
DEFINE_PRIM(_I32, PartitionPolyList_numPolys0, _IDL);

HL_PRIM HL_CONST pref<TPPLPoly>* HL_NAME(PartitionPolyList_addPoly2)(pref<TPPLPolyList>* _this, int pointCount, vbyte* coordinates) {
	return alloc_ref_const((HLPartition::PolyList_AddPoly( _unref(_this) , pointCount, (double*)coordinates)),PartitionPoly);
}
DEFINE_PRIM(_IDL, PartitionPolyList_addPoly2, _IDL _I32 _BYTES);

HL_PRIM pref<PartitionPolyIt>* HL_NAME(PartitionPolyList_getPolyIt0)(pref<TPPLPolyList>* _this) {
	return alloc_ref((HLPartition::GetPolyIt( _unref(_this) )),PartitionPolyIt);
}
DEFINE_PRIM(_IDL, PartitionPolyList_getPolyIt0, _IDL);

HL_PRIM bool HL_NAME(PartitionPolyList_removeHoles1)(pref<TPPLPolyList>* _this, pref<TPPLPolyList>* result) {
	return (HLPartition::PolyList_RemoveHoles( _unref(_this) , _unref_ptr_safe(result)));
}
DEFINE_PRIM(_BOOL, PartitionPolyList_removeHoles1, _IDL _IDL);

HL_PRIM bool HL_NAME(PartitionPolyList_convexPartition1)(pref<TPPLPolyList>* _this, pref<TPPLPolyList>* result) {
	return (HLPartition::PolyList_ConvexPartition( _unref(_this) , _unref_ptr_safe(result)));
}
DEFINE_PRIM(_BOOL, PartitionPolyList_convexPartition1, _IDL _IDL);

HL_PRIM pref<TPPLPoly>* HL_NAME(PartitionPoly_new0)() {
	return alloc_ref((new TPPLPoly()),PartitionPoly);
}
DEFINE_PRIM(_IDL, PartitionPoly_new0,);

HL_PRIM void HL_NAME(PartitionPoly_setHole1)(pref<TPPLPoly>* _this, bool hole) {
	(_unref(_this)->SetHole(hole));
}
DEFINE_PRIM(_VOID, PartitionPoly_setHole1, _IDL _BOOL);

HL_PRIM bool HL_NAME(PartitionPoly_isHole0)(pref<TPPLPoly>* _this) {
	return (_unref(_this)->IsHole());
}
DEFINE_PRIM(_BOOL, PartitionPoly_isHole0, _IDL);

HL_PRIM int HL_NAME(PartitionPoly_getNumPoints0)(pref<TPPLPoly>* _this) {
	return (_unref(_this)->GetNumPoints());
}
DEFINE_PRIM(_I32, PartitionPoly_getNumPoints0, _IDL);

HL_PRIM void HL_NAME(PartitionPoly_getPoints1)(pref<TPPLPoly>* _this, vbyte* points) {
	(HLPartition::Poly_GetPoints( _unref(_this) , (double*)points));
}
DEFINE_PRIM(_VOID, PartitionPoly_getPoints1, _IDL _BYTES);

HL_PRIM void HL_NAME(PartitionPoly_initD2)(pref<TPPLPoly>* _this, vbyte* coordinates, int vertCount) {
	(HLPartition::InitD( _unref(_this) , (double*)coordinates, vertCount));
}
DEFINE_PRIM(_VOID, PartitionPoly_initD2, _IDL _BYTES _I32);

HL_PRIM void HL_NAME(PartitionPoly_convexPartitionOptimal1)(pref<TPPLPoly>* _this, pref<TPPLPolyList>* result) {
	(HLPartition::ConvexPartitionOptimal( _unref(_this) , _unref_ptr_safe(result)));
}
DEFINE_PRIM(_VOID, PartitionPoly_convexPartitionOptimal1, _IDL _IDL);

HL_PRIM bool HL_NAME(PartitionPoly_triangulateOptimal1)(pref<TPPLPoly>* _this, pref<TPPLPolyList>* result) {
	return (HLPartition::TriangulateOptimal( _unref(_this) , _unref_ptr_safe(result)));
}
DEFINE_PRIM(_BOOL, PartitionPoly_triangulateOptimal1, _IDL _IDL);

}
