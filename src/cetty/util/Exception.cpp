//
// Exception.cpp
//
// $Id: //poco/1.4/Foundation/src/Exception.cpp#1 $
//
// Library: Foundation
// Package: Core
// Module:  Exception
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include "cetty/util/Exception.h"

namespace cetty { namespace util {

Exception::Exception(int code): nested(0), code(code) {}

Exception::Exception(const std::string& msg, int code)
    : message(msg), nested(0), code(code) {}

Exception::Exception(const std::string& msg, const std::string& arg, int code)
    : message(msg), nested(0), code(code) {
	if (!arg.empty()) {
		message.append(": ");
		message.append(arg);
	}
}

Exception::Exception(const std::string& msg, const Exception& nested, int code)
    : message(msg), nested(nested.clone()), code(code) {
}

Exception::Exception(const Exception& exc) :
    std::exception(exc),
	message(exc.message),
	code(exc.code)
{
	nested = exc.nested ? exc.nested->clone() : 0;
}

Exception::~Exception() throw() {
    if (nested) {
	    delete nested;
    }
}

Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this) {
        if (nested) {
		    delete nested;
            nested = NULL;
        }

		message = exc.message;
		nested  = exc.nested ? exc.nested->clone() : 0;
		code    = exc.code;
	}

	return *this;
}

const char* Exception::name() const throw() {
	return "Exception";
}

const char* Exception::className() const throw() {
	return typeid(*this).name();
}

const char* Exception::what() const throw() {
	return name();
}

std::string Exception::getDisplayText() const {
	std::string txt = name();
	if (!message.empty()) {
		txt.append(": ");
		txt.append(message);
	}
	return txt;
}

void Exception::extendedMessage(const std::string& arg)
{
	if (!arg.empty()) {
		if (!message.empty()) message.append(": ");
		message.append(arg);
	}
}

Exception* Exception::clone() const {
	return new Exception(*this);
}

void Exception::rethrow() const {
	throw *this;
}

CETTY_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
CETTY_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
CETTY_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
CETTY_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
CETTY_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
CETTY_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
CETTY_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
CETTY_IMPLEMENT_EXCEPTION(UnsupportedOperationException, LogicException, "Unsupported operation")
CETTY_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
CETTY_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
CETTY_IMPLEMENT_EXCEPTION(NumberFormatException, LogicException, "Number format invalid")
CETTY_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
CETTY_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
CETTY_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

CETTY_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
CETTY_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
CETTY_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
CETTY_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
CETTY_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
CETTY_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
CETTY_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
CETTY_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
CETTY_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
CETTY_IMPLEMENT_EXCEPTION(NotYetConnectedException, RuntimeException, "Not yet Connected")
CETTY_IMPLEMENT_EXCEPTION(InterruptedException, RuntimeException, "Interrupted Exception")
CETTY_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
CETTY_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
CETTY_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
CETTY_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
CETTY_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

CETTY_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
CETTY_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
CETTY_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
CETTY_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
CETTY_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
CETTY_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
CETTY_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
CETTY_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
CETTY_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
CETTY_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
CETTY_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
CETTY_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
CETTY_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
CETTY_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
CETTY_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
CETTY_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
CETTY_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")

CETTY_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
CETTY_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

}}

