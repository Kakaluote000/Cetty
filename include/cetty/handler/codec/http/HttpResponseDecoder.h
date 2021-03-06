#if !defined(CETTY_HANDLER_CODEC_HTTP_HTTPRESPONSEDECODER_H)
#define CETTY_HANDLER_CODEC_HTTP_HTTPRESPONSEDECODER_H

/*
 * Copyright 2009 Red Hat, Inc.
 *
 * Red Hat licenses this file to you under the Apache License, version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 * Distributed under under the Apache License, version 2.0 (the "License").
 */

#include "cetty/handler/codec/http/DefaultHttpResponse.h"
#include "cetty/handler/codec/http/HttpMessageDecoder.h"

namespace cetty { namespace handler { namespace codec { namespace http { 


/**
 * Decodes {@link ChannelBuffer}s into {@link HttpResponse}s and
 * {@link HttpChunk}s.
 *
 * <h3>Parameters that prevents excessive memory consumption</h3>
 * <table border="1">
 * <tr>
 * <th>Name</th><th>Meaning</th>
 * </tr>
 * <tr>
 * <td><tt>maxInitialLineLength</tt></td>
 * <td>The maximum length of the initial line (e.g. <tt>"HTTP/1.0 200 OK"</tt>)
 *     If the length of the initial line exceeds this value, a
 *     {@link TooLongFrameException} will be raised.</td>
 * </tr>
 * <tr>
 * <td><tt>maxHeaderSize</tt></td>
 * <td>The maximum length of all headers.  If the sum of the length of each
 *     header exceeds this value, a {@link TooLongFrameException} will be raised.</td>
 * </tr>
 * <tr>
 * <td><tt>maxChunkSize</tt></td>
 * <td>The maximum length of the content or each chunk.  If the content length
 *     exceeds this value, the transfer encoding of the decoded response will be
 *     converted to 'chunked' and the content will be split into multiple
 *     {@link HttpChunk}s.  If the transfer encoding of the HTTP response is
 *     'chunked' already, each chunk will be split into smaller chunks if the
 *     length of the chunk exceeds this value.  If you prefer not to handle
 *     {@link HttpChunk}s in your handler, insert {@link HttpChunkAggregator}
 *     after this decoder in the {@link ChannelPipeline}.</td>
 * </tr>
 * </table>
 *
 * <h3>Decoding a response for a <tt>HEAD</tt> request</h3>
 * <p>
 * Unlike other HTTP requests, the successful response of a <tt>HEAD</tt>
 * request does not have any content even if there is <tt>Content-Length</tt>
 * header.  Because {@link HttpResponseDecoder} is not able to determine if the
 * response currently being decoded is associated with a <tt>HEAD</tt> request,
 * you must override {@link #isContentAlwaysEmpty(HttpMessage)} to return
 * <tt>true</tt> for the response of the <tt>HEAD</tt> request.
 * </p><p>
 * If you are writing an HTTP client that issues a <tt>HEAD</tt> request,
 * please use {@link HttpClientCodec} instead of this decoder.  It will perform
 * additional state management to handle the responses for <tt>HEAD</tt>
 * requests correctly.
 * </p>
 *
 * <h3>Decoding a response for a <tt>CONNECT</tt> request</h3>
 * <p>
 * You also need to do additional state management to handle the response of a
 * <tt>CONNECT</tt> request properly, like you did for <tt>HEAD</tt>.  One
 * difference is that the decoder should stop decoding completely after decoding
 * the successful 200 response since the connection is not an HTTP connection
 * anymore.
 * </p><p>
 * {@link HttpClientCodec} also handles this edge case correctly, so you have to
 * use {@link HttpClientCodec} if you are writing an HTTP client that issues a
 * <tt>CONNECT</tt> request.
 * </p>
 *
 * 
 * @author Andy Taylor (andy.taylor@jboss.org)
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 */
class HttpResponseDecoder : public HttpMessageDecoder {
public:
    /**
     * Creates a new instance with the default
     * <tt>maxInitialLineLength (4096)</tt>, <tt>maxHeaderSize (8192)</tt>, and
     * <tt>maxChunkSize (8192)</tt>.
     */
    HttpResponseDecoder()
        : response(new DefaultHttpResponse),
          HttpMessageDecoder(4096, 8192, 8192) {
    }

    /**
     * Creates a new instance with the specified parameters.
     */
    HttpResponseDecoder(
            int maxInitialLineLength, int maxHeaderSize, int maxChunkSize)
            : response(new DefaultHttpResponse),
              HttpMessageDecoder(maxInitialLineLength,
                                 maxHeaderSize,
                                 maxChunkSize) {
    }

    virtual ~HttpResponseDecoder() {}

    virtual ChannelHandlerPtr clone() {
        return ChannelHandlerPtr(
            new HttpResponseDecoder(maxInitialLineLength,
                                    maxHeaderSize,
                                    maxChunkSize));
    }
    
    virtual std::string toString() const {
        return "HttpResponseDecoder";
    }

protected:
    virtual HttpMessagePtr createMessage(const char* str1,
                                         const char* str2,
                                         const char* str3) {
        response->clear();
        response->setProtocolVersion(HttpVersion::valueOf(str1));
        response->setStatus(HttpResponseStatus(Integer::parse(str2), str3));
        return response;
    }

    virtual bool isDecodingRequest() const {
        return false;
    }

private:
    DefaultHttpResponsePtr response;
};

}}}}

#endif //#if !defined(CETTY_HANDLER_CODEC_HTTP_HTTPRESPONSEDECODER_H)
