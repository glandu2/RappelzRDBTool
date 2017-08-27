#ifndef IOBJECT_H
#define IOBJECT_H

#include "ExportDLL.h"
#include "Guid.h"

class IObject;

class IObject
{
	public:
		virtual long DLLCALLCONV QueryInterface(const Guid& iid, void** ppv) = 0;
		virtual unsigned long DLLCALLCONV AddRef() = 0;
		virtual unsigned long DLLCALLCONV Release() = 0;
		virtual void DLLCALLCONV destroy() = 0;

		void operator delete(void* p) {
			if (p) {
				IObject* i = static_cast<IObject*>(p);
				i->destroy();
			}
		}
};

template<class IInterface>
class CImplement : public IInterface
{
	public:
		CImplement() : refCount(1) {}
		virtual ~CImplement() {}
		void operator delete(void* p) {
			::operator delete(p);
		}

		virtual inline long DLLCALLCONV QueryInterface(const Guid& iid, void** ppv) { return 0x80004002L; }

		virtual inline unsigned long DLLCALLCONV AddRef() { return ++refCount; }

		virtual inline unsigned long DLLCALLCONV Release() {
			refCount--;
			if(!refCount) {
				destroy();
				return 0;
			}
			return refCount;
		}

		virtual void DLLCALLCONV destroy() {
			delete this;
		}


	private:
		int refCount;
};

#endif // IOBJECT_H
