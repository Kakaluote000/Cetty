/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 *
 * Distributed under under the Apache License, version 2.0 (the "License").
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "cetty/util/Character.h"
#include "cetty/util/StringUtil.h"
#include "cetty/util/Exception.h"
#include "cetty/handler/codec/http/HttpMethod.h"


namespace cetty { namespace handler { namespace codec { namespace http {

using namespace cetty::util;

/**
 * The OPTIONS method represents a request for information about the communication options available on the request/response
 * chain identified by the Request-URI. This method allows the client to determine the options and/or requirements
 * associated with a resource, or the capabilities of a server, without implying a resource action or initiating a
 * resource retrieval.
 */
const HttpMethod HttpMethod::HM_OPTIONS("OPTIONS");

/**
 * The GET method means retrieve whatever information (in the form of an entity) is identified by the Request-URI.
 * If the Request-URI refers to a data-producing process, it is the produced data which shall be returned as the entity
 * in the response and not the source text of the process, unless that text happens to be the output of the process.
 */
const HttpMethod HttpMethod::HM_GET("GET");

/**
 * The HEAD method is identical to GET except that the server MUST NOT return a message-body in the response.
 */
const HttpMethod HttpMethod::HM_HEAD("HEAD");

/**
 * The POST method is used to request that the origin server accept the entity enclosed in the request as a new
 * subordinate of the resource identified by the Request-URI in the Request-Line.
 */
const HttpMethod HttpMethod::HM_POST("POST");

/**
 * The PUT method requests that the enclosed entity be stored under the supplied Request-URI.
 */
const HttpMethod HttpMethod::HM_PUT("PUT");

/**
 * The PATCH method requests that a set of changes described in the
 * request entity be applied to the resource identified by the Request-URI.
 */
const HttpMethod HttpMethod::HM_PATCH("PATCH");

/**
 * The DELETE method requests that the origin server delete the resource identified by the Request-URI.
 */
const HttpMethod HttpMethod::HM_DELETE("DELETE");

/**
 * The TRACE method is used to invoke a remote, application-layer loop- back of the request message.
 */
const HttpMethod HttpMethod::HM_TRACE("TRACE");

/**
 * This specification reserves the method name CONNECT for use with a proxy that can dynamically switch to being a tunnel
 */
const HttpMethod HttpMethod::HM_CONNECT("CONNECT");


static const HttpMethod * const HTTP_METHODS[] = {
    &HttpMethod::HM_OPTIONS,
    &HttpMethod::HM_GET,
    &HttpMethod::HM_HEAD,
    &HttpMethod::HM_POST,
    &HttpMethod::HM_PUT,
    &HttpMethod::HM_PATCH,
    &HttpMethod::HM_DELETE,
    &HttpMethod::HM_TRACE,
    &HttpMethod::HM_CONNECT
};

static const int HTTP_METHODS_CNT = sizeof(HTTP_METHODS) / sizeof(const HttpMethod * const);

static const HttpMethod* getHttpMethod(const std::string& name) {
    for (int i = 0; i < HTTP_METHODS_CNT; ++i) {
        if (name.compare(HTTP_METHODS[i]->getName()) == 0) {
            return HTTP_METHODS[i];
        }
    }
    return NULL;
}
HttpMethod HttpMethod::valueOf(const std::string& name) {
    std::string method = boost::trim_copy(name);
    boost::to_upper(method);
    
    if (method.size() == 0) {
        throw InvalidArgumentException("empty name");
    }

    const HttpMethod* result = getHttpMethod(name);
    if (result != NULL) {
        return *result;
    }
    else {
        return HttpMethod(name);
    }
}

HttpMethod::HttpMethod(const std::string& name) {
    this->name = boost::trim_copy(name);
    boost::to_upper(this->name);
    if (this->name.size() == 0) {
        throw InvalidArgumentException("empty name");
    }

    for (size_t i = 0; i < this->name.size(); ++i) {
        if (Character::isISOControl(this->name.at(i)) ||
            Character::isWhitespace(this->name.at(i))) {
                throw InvalidArgumentException("invalid character in name");
        }
    }
}

int HttpMethod::hashCode() const {
    return StringUtil::hashCode(getName());
}

}}}}