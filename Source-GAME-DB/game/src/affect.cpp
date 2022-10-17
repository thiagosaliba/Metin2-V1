#include "stdafx.h"

#if !defined(DEBUG_ALLOC)
#include <boost/pool/object_pool.hpp>
#endif

#include "affect.h"

#if !defined(DEBUG_ALLOC)
boost::object_pool<CAffect> affect_pool;
#endif

CAffect* CAffect::Acquire()
{
#if !defined(DEBUG_ALLOC)
	return affect_pool.malloc();
#else
	return M2_NEW CAffect;
#endif
}

void CAffect::Release(CAffect* p)
{
#if !defined(DEBUG_ALLOC)
	affect_pool.free(p);
#else
	M2_DELETE(p);
#endif
}
