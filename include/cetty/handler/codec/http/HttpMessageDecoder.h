#if !defined(CETTY_HANDLER_CODEC_HTTP_HTTPMESSAGEDECODER_H)
#define CETTY_HANDLER_CODEC_HTTP_HTTPMESSAGEDECODER_H

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

#include <vector>

#include "cetty/handler/codec/http/HttpMessage.h"
#include "cetty/handler/codec/http/HttpChunkTrailer.h"
#include "cetty/handler/codec/replay/ReplayingDecoderBuffer.h"
#include "cetty/handler/codec/replay/ReplayingDecoder.h"

namespace cetty { namespace handler { namespace codec { namespace http { 

class HttpMessage;

using namespace cetty::channel;
using namespace cetty::handler::codec::replay;

/**
 * Decodes {@link ChannelBuffer}s into {@link HttpMessage}s and
 * {@link HttpChunk}s.
 *
 * <h3>Parameters that prevents excessive memory consumption</h3>
 * <table border="1">
 * <tr>
 * <th>Name</th><th>Meaning</th>
 * </tr>
 * <tr>
 * <td><tt>maxInitialLineLength</tt></td>
 * <td>The maximum length of the initial line
 *     (e.g. <tt>"GET / HTTP/1.0"</tt> or <tt>"HTTP/1.0 200 OK"</tt>)
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
 *     (or the length of each chunk) exceeds this value, the content or chunk
 *     will be split into multiple {@link HttpChunk}s whose length is
 *     <tt>maxChunkSize</tt> at maximum.</td>
 * </tr>
 * </table>
 *
 * <h3>Chunked Content</h3>
 *
 * If the content of an HTTP message is greater than <tt>maxChunkSize</tt> or
 * the transfer encoding of the HTTP message is 'chunked', this decoder
 * generates one {@link HttpMessage} instance and its following
 * {@link HttpChunk}s per single HTTP message to avoid excessive memory
 * consumption. For example, the following HTTP message:
 * <pre>
 * GET / HTTP/1.1
 * Transfer-Encoding: chunked
 *
 * 1a
 * abcdefghijklmnopqrstuvwxyz
 * 10
 * 1234567890abcdef
 * 0
 * Content-MD5: ...
 * <i>[blank line]</i>
 * </pre>
 * triggers {@link HttpRequestDecoder} to generate 4 objects:
 * <ol>
 * <li>An {@link HttpRequest} whose {@link HttpMessage#isChunked() chunked}
 *     property is <tt>true</tt>,</li>
 * <li>The first {@link HttpChunk} whose content is <tt>'abcdefghijklmnopqrstuvwxyz'</tt>,</li>
 * <li>The second {@link HttpChunk} whose content is <tt>'1234567890abcdef'</tt>, and</li>
 * <li>An {@link HttpChunkTrailer} which marks the end of the content.</li>
 * </ol>
 *
 * If you prefer not to handle {@link HttpChunk}s by yourself for your
 * convenience, insert {@link HttpChunkAggregator} after this decoder in the
 * {@link ChannelPipeline}.  However, please note that your server might not
 * be as memory efficient as without the aggregator.
 *
 * <h3>Extensibility</h3>
 *
 * Please note that this decoder is designed to be extended to implement
 * a protocol derived from HTTP, such as
 * <a href="http://en.wikipedia.org/wiki/Real_Time_Streaming_Protocol">RTSP</a> and
 * <a href="http://en.wikipedia.org/wiki/Internet_Content_Adaptation_Protocol">ICAP</a>.
 * To implement the decoder of such a derived protocol, extend this class and
 * implement all abstract methods properly.
 *
 * 
 * @author Andy Taylor (andy.taylor@jboss.org)
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 *
 * @apiviz.landmark
 */

class HttpMessageDecoder
    : public cetty::handler::codec::replay::ReplayingDecoder {
private:
   /**
    * The internal state of {@link HttpMessageDecoder}.
    * <em>Internal use only</em>.
    *
    * 
    * @author <a href="http://gleamynode.net/">Trustin Lee</a>
    * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
    *
    * @apiviz.exclude
    */
    enum State {
        SKIP_CONTROL_CHARS,
        READ_INITIAL,
        READ_HEADER,
        READ_VARIABLE_LENGTH_CONTENT,
        READ_VARIABLE_LENGTH_CONTENT_AS_CHUNKS,
        READ_FIXED_LENGTH_CONTENT,
        READ_FIXED_LENGTH_CONTENT_AS_CHUNKS,
        READ_CHUNK_SIZE,
        READ_CHUNKED_CONTENT,
        READ_CHUNKED_CONTENT_AS_CHUNKS,
        READ_CHUNK_DELIMITER,
        READ_CHUNK_FOOTER
    };

    class FastString {
    public:
        FastString() : data(NULL), size(0) {}
        FastString(char* data, int size) : data(data), size(size) {}

        inline void clear() { data = NULL; size = 0; }
        inline bool empty() const { return size == 0; }
        inline int  length() const { return size; }

        inline operator char*() { return data; }
        
        inline char* c_str() { return data; }
        inline const char* c_str() const { return data; }

        inline char operator[](int index) const {
            return data[index];
        }
        inline char& operator[](int index) {
            return data[index];
        }
        inline char at(int index) const {
            BOOST_ASSERT(index < size);
            return data[index];
        }

        FastString substring(int start, int end) const;

        FastString trim() const;
        bool equalsIgnoreCase(const std::string& str) const;

        char* data;
        int   size;
    };

public:
    virtual ~HttpMessageDecoder() {}

protected:
    /**
     * Creates a new instance with the default
     * <tt>maxInitialLineLength (4096)</tt>, <tt>maxHeaderSize (8192)</tt>, and
     * <tt>maxChunkSize (8192)</tt>.
     *
     * if channel buffer size < (maxInitialLineLength + maxHeaderSize + maxChunkSize),
     * may cause some memory copy.
     * if < (maxInitialLineLength + maxHeaderSize) may cause some memory allocation.
     *
     */
    HttpMessageDecoder();

    /**
     * Creates a new instance with the specified parameters.
     */
    HttpMessageDecoder(
            int maxInitialLineLength, int maxHeaderSize, int maxChunkSize);

protected:
    virtual ChannelMessage decode(ChannelHandlerContext& ctx,
                                  Channel& channel,
                                  const ReplayingDecoderBufferPtr& buffer,
                                  int state);

    bool isContentAlwaysEmpty(const HttpMessage& msg) const;

protected:
    virtual bool isDecodingRequest() const = 0;

    virtual HttpMessagePtr createMessage(const char* str1,
                                         const char* str2,
                                         const char* str3) = 0;

private:
    ChannelMessage reset();

    bool skipControlCharacters(const ReplayingDecoderBufferPtr& buffer) const;
    bool readFixedLengthContent(const ReplayingDecoderBufferPtr& buffer);

    //throws TooLongFrameException
    int readHeaders(const ReplayingDecoderBufferPtr& buffer);

    //throws TooLongFrameException
    HttpChunkTrailerPtr readTrailingHeaders(const ReplayingDecoderBufferPtr& buffer);

    //throws TooLongFrameException 
    FastString readHeader(const ReplayingDecoderBufferPtr& buffer);

    //throws TooLongFrameException
    FastString readLine(const ReplayingDecoderBufferPtr& buffer, int maxLineLength);

    int getChunkSize(const FastString& hex) const;

    /**
     * @return true can read out a line completely. false means need to read more bytes.
     */
    bool splitInitialLine(FastString& sb, std::vector<FastString>& lines);
    void splitHeader(FastString& sb, std::vector<FastString>& header);

    int findNonWhitespace(const FastString& sb, int offset);
    int findWhitespace(const FastString& sb, int offset);
    int findEndOfString(const FastString& sb);

protected:
    int maxInitialLineLength;
    int maxHeaderSize;
    int maxChunkSize;
    
private:
    int  chunkSize;
    int  headerSize;
    HttpMessagePtr message;
    ChannelBufferPtr content;
};

}}}}

#endif //#if !defined(CETTY_HANDLER_CODEC_HTTP_HTTPMESSAGEDECODER_H)
