/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#if ENABLE(Condition1) || ENABLE(Condition2)

#ifndef V8TestSerializedScriptValueInterface_h
#define V8TestSerializedScriptValueInterface_h

#include "TestSerializedScriptValueInterface.h"
#include "V8DOMWrapper.h"
#include "WrapperTypeInfo.h"
#include <v8.h>
#include <wtf/HashMap.h>
#include <wtf/text/StringHash.h>

namespace WebCore {

class V8TestSerializedScriptValueInterface {
public:
    static const bool hasDependentLifetime = false;
    static bool HasInstance(v8::Handle<v8::Value>);
    static v8::Persistent<v8::FunctionTemplate> GetRawTemplate();
    static v8::Persistent<v8::FunctionTemplate> GetTemplate();
    static TestSerializedScriptValueInterface* toNative(v8::Handle<v8::Object> object)
    {
        return reinterpret_cast<TestSerializedScriptValueInterface*>(object->GetPointerFromInternalField(v8DOMWrapperObjectIndex));
    }
    inline static v8::Handle<v8::Object> wrap(TestSerializedScriptValueInterface*);
    static void derefObject(void*);
    static WrapperTypeInfo info;
    static v8::Handle<v8::Value> constructorCallback(const v8::Arguments&);
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 0;
    static v8::Handle<v8::Object> existingWrapper(TestSerializedScriptValueInterface*);

private:
    static v8::Handle<v8::Object> wrapSlow(TestSerializedScriptValueInterface*);
};

ALWAYS_INLINE v8::Handle<v8::Object> V8TestSerializedScriptValueInterface::existingWrapper(TestSerializedScriptValueInterface* impl)
{
    return getDOMObjectMap().get(impl);
}

v8::Handle<v8::Object> V8TestSerializedScriptValueInterface::wrap(TestSerializedScriptValueInterface* impl)
{
        v8::Handle<v8::Object> wrapper = existingWrapper(impl);
        if (!wrapper.IsEmpty())
            return wrapper;
    return V8TestSerializedScriptValueInterface::wrapSlow(impl);
}

inline v8::Handle<v8::Value> toV8(TestSerializedScriptValueInterface* impl)
{
    if (!impl)
        return v8::Null();
    return V8TestSerializedScriptValueInterface::wrap(impl);
}
inline v8::Handle<v8::Value> toV8(PassRefPtr< TestSerializedScriptValueInterface > impl)
{
    return toV8(impl.get());
}

}

#endif // V8TestSerializedScriptValueInterface_h
#endif // ENABLE(Condition1) || ENABLE(Condition2)
