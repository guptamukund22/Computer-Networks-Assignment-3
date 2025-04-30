//
// Generated file, do not edit! Created by opp_msgtool 6.1 from ClientMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "ClientMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(ClientMsg)

ClientMsg::ClientMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ClientMsg::ClientMsg(const ClientMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ClientMsg::~ClientMsg()
{
    delete [] this->array;
}

ClientMsg& ClientMsg::operator=(const ClientMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ClientMsg::copy(const ClientMsg& other)
{
    this->sourceId = other.sourceId;
    this->destId = other.destId;
    this->taskId = other.taskId;
    this->subtaskId = other.subtaskId;
    this->hopCount = other.hopCount;
    this->result = other.result;
    this->isResult_ = other.isResult_;
    this->isGossip_ = other.isGossip_;
    this->gossipContent = other.gossipContent;
    delete [] this->array;
    this->array = (other.array_arraysize==0) ? nullptr : new int[other.array_arraysize];
    array_arraysize = other.array_arraysize;
    for (size_t i = 0; i < array_arraysize; i++) {
        this->array[i] = other.array[i];
    }
}

void ClientMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->sourceId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->taskId);
    doParsimPacking(b,this->subtaskId);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->result);
    doParsimPacking(b,this->isResult_);
    doParsimPacking(b,this->isGossip_);
    doParsimPacking(b,this->gossipContent);
    b->pack(array_arraysize);
    doParsimArrayPacking(b,this->array,array_arraysize);
}

void ClientMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->taskId);
    doParsimUnpacking(b,this->subtaskId);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->result);
    doParsimUnpacking(b,this->isResult_);
    doParsimUnpacking(b,this->isGossip_);
    doParsimUnpacking(b,this->gossipContent);
    delete [] this->array;
    b->unpack(array_arraysize);
    if (array_arraysize == 0) {
        this->array = nullptr;
    } else {
        this->array = new int[array_arraysize];
        doParsimArrayUnpacking(b,this->array,array_arraysize);
    }
}

int ClientMsg::getSourceId() const
{
    return this->sourceId;
}

void ClientMsg::setSourceId(int sourceId)
{
    this->sourceId = sourceId;
}

int ClientMsg::getDestId() const
{
    return this->destId;
}

void ClientMsg::setDestId(int destId)
{
    this->destId = destId;
}

int ClientMsg::getTaskId() const
{
    return this->taskId;
}

void ClientMsg::setTaskId(int taskId)
{
    this->taskId = taskId;
}

int ClientMsg::getSubtaskId() const
{
    return this->subtaskId;
}

void ClientMsg::setSubtaskId(int subtaskId)
{
    this->subtaskId = subtaskId;
}

int ClientMsg::getHopCount() const
{
    return this->hopCount;
}

void ClientMsg::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int ClientMsg::getResult() const
{
    return this->result;
}

void ClientMsg::setResult(int result)
{
    this->result = result;
}

bool ClientMsg::isResult() const
{
    return this->isResult_;
}

void ClientMsg::setIsResult(bool isResult)
{
    this->isResult_ = isResult;
}

bool ClientMsg::isGossip() const
{
    return this->isGossip_;
}

void ClientMsg::setIsGossip(bool isGossip)
{
    this->isGossip_ = isGossip;
}

const char * ClientMsg::getGossipContent() const
{
    return this->gossipContent.c_str();
}

void ClientMsg::setGossipContent(const char * gossipContent)
{
    this->gossipContent = gossipContent;
}

size_t ClientMsg::getArrayArraySize() const
{
    return array_arraysize;
}

int ClientMsg::getArray(size_t k) const
{
    if (k >= array_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)array_arraysize, (unsigned long)k);
    return this->array[k];
}

void ClientMsg::setArrayArraySize(size_t newSize)
{
    int *array2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = array_arraysize < newSize ? array_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        array2[i] = this->array[i];
    for (size_t i = minSize; i < newSize; i++)
        array2[i] = 0;
    delete [] this->array;
    this->array = array2;
    array_arraysize = newSize;
}

void ClientMsg::setArray(size_t k, int array)
{
    if (k >= array_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)array_arraysize, (unsigned long)k);
    this->array[k] = array;
}

void ClientMsg::insertArray(size_t k, int array)
{
    if (k > array_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)array_arraysize, (unsigned long)k);
    size_t newSize = array_arraysize + 1;
    int *array2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        array2[i] = this->array[i];
    array2[k] = array;
    for (i = k + 1; i < newSize; i++)
        array2[i] = this->array[i-1];
    delete [] this->array;
    this->array = array2;
    array_arraysize = newSize;
}

void ClientMsg::appendArray(int array)
{
    insertArray(array_arraysize, array);
}

void ClientMsg::eraseArray(size_t k)
{
    if (k >= array_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)array_arraysize, (unsigned long)k);
    size_t newSize = array_arraysize - 1;
    int *array2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        array2[i] = this->array[i];
    for (i = k; i < newSize; i++)
        array2[i] = this->array[i+1];
    delete [] this->array;
    this->array = array2;
    array_arraysize = newSize;
}

class ClientMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_sourceId,
        FIELD_destId,
        FIELD_taskId,
        FIELD_subtaskId,
        FIELD_hopCount,
        FIELD_result,
        FIELD_isResult,
        FIELD_isGossip,
        FIELD_gossipContent,
        FIELD_array,
    };
  public:
    ClientMsgDescriptor();
    virtual ~ClientMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ClientMsgDescriptor)

ClientMsgDescriptor::ClientMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ClientMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ClientMsgDescriptor::~ClientMsgDescriptor()
{
    delete[] propertyNames;
}

bool ClientMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ClientMsg *>(obj)!=nullptr;
}

const char **ClientMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ClientMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ClientMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int ClientMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_sourceId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_taskId
        FD_ISEDITABLE,    // FIELD_subtaskId
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_result
        FD_ISEDITABLE,    // FIELD_isResult
        FD_ISEDITABLE,    // FIELD_isGossip
        FD_ISEDITABLE,    // FIELD_gossipContent
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_array
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *ClientMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceId",
        "destId",
        "taskId",
        "subtaskId",
        "hopCount",
        "result",
        "isResult",
        "isGossip",
        "gossipContent",
        "array",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int ClientMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "sourceId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "taskId") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "subtaskId") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "result") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "isResult") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "isGossip") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "gossipContent") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "array") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *ClientMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_sourceId
        "int",    // FIELD_destId
        "int",    // FIELD_taskId
        "int",    // FIELD_subtaskId
        "int",    // FIELD_hopCount
        "int",    // FIELD_result
        "bool",    // FIELD_isResult
        "bool",    // FIELD_isGossip
        "string",    // FIELD_gossipContent
        "int",    // FIELD_array
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **ClientMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ClientMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ClientMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        case FIELD_array: return pp->getArrayArraySize();
        default: return 0;
    }
}

void ClientMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        case FIELD_array: pp->setArrayArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ClientMsg'", field);
    }
}

const char *ClientMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ClientMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: return long2string(pp->getSourceId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_taskId: return long2string(pp->getTaskId());
        case FIELD_subtaskId: return long2string(pp->getSubtaskId());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_result: return long2string(pp->getResult());
        case FIELD_isResult: return bool2string(pp->isResult());
        case FIELD_isGossip: return bool2string(pp->isGossip());
        case FIELD_gossipContent: return oppstring2string(pp->getGossipContent());
        case FIELD_array: return long2string(pp->getArray(i));
        default: return "";
    }
}

void ClientMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: pp->setSourceId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_taskId: pp->setTaskId(string2long(value)); break;
        case FIELD_subtaskId: pp->setSubtaskId(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_result: pp->setResult(string2long(value)); break;
        case FIELD_isResult: pp->setIsResult(string2bool(value)); break;
        case FIELD_isGossip: pp->setIsGossip(string2bool(value)); break;
        case FIELD_gossipContent: pp->setGossipContent((value)); break;
        case FIELD_array: pp->setArray(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ClientMsg'", field);
    }
}

omnetpp::cValue ClientMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: return pp->getSourceId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_taskId: return pp->getTaskId();
        case FIELD_subtaskId: return pp->getSubtaskId();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_result: return pp->getResult();
        case FIELD_isResult: return pp->isResult();
        case FIELD_isGossip: return pp->isGossip();
        case FIELD_gossipContent: return pp->getGossipContent();
        case FIELD_array: return pp->getArray(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ClientMsg' as cValue -- field index out of range?", field);
    }
}

void ClientMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        case FIELD_sourceId: pp->setSourceId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_taskId: pp->setTaskId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_subtaskId: pp->setSubtaskId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_result: pp->setResult(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_isResult: pp->setIsResult(value.boolValue()); break;
        case FIELD_isGossip: pp->setIsGossip(value.boolValue()); break;
        case FIELD_gossipContent: pp->setGossipContent(value.stringValue()); break;
        case FIELD_array: pp->setArray(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ClientMsg'", field);
    }
}

const char *ClientMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ClientMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ClientMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ClientMsg *pp = omnetpp::fromAnyPtr<ClientMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ClientMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

