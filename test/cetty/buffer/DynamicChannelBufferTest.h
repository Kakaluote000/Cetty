#if !defined(CETTY_BUFFER_DYNAMICCHANNELBUFFERTEST_H)
#define CETTY_BUFFER_DYNAMICCHANNELBUFFERTEST_H

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
#include "cetty/buffer/AbstractChannelBufferTest.h"
#include "cetty/buffer/DynamicChannelBuffer.h"

namespace cetty { namespace buffer { 

/**
 *
 * 
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @version $Rev: 2080 $, $Date: 2010-01-26 18:04:19 +0900 (Tue, 26 Jan 2010) $
 */
class DynamicChannelBufferTest : public AbstractChannelBufferTest {
public:
    
protected:
    void newBuffer(int length) {
        buffer = ChannelBuffers::dynamicBuffer(length);

        BOOST_ASSERT(0 == buffer->readerIndex());
        BOOST_ASSERT(0 == buffer->writerIndex());
        BOOST_ASSERT(length == buffer->capacity());
        
        buffers.push_back(buffer);
    }

    std::vector<ChannelBufferPtr>& components() {
        return buffers;
    }

public:
    void shouldNotFailOnInitialIndexUpdate() {
        DynamicChannelBuffer* buff = new DynamicChannelBuffer(ByteOrder::BYTE_ORDER_BIG, 10);
        
        ASSERT_NO_THROW(buff->setIndex(0, 10));
    }

    void shouldNotFailOnInitialIndexUpdate2() {
        DynamicChannelBuffer* buff = new DynamicChannelBuffer(ByteOrder::BYTE_ORDER_BIG, 10);
        
        ASSERT_NO_THROW(buff->writerIndex(10));
    }

    void shouldNotFailOnInitialIndexUpdate3() {
        ChannelBufferPtr buf = ChannelBufferPtr(
                                new DynamicChannelBuffer(ByteOrder::BYTE_ORDER_BIG, 10));
        ASSERT_NO_THROW(buf->writerIndex(10));
        ASSERT_NO_THROW(buf->readerIndex(10));
    }

private:
    std::vector<ChannelBufferPtr> buffers;
};

}}

#endif //#if !defined(CETTY_BUFFER_DYNAMICCHANNELBUFFERTEST_H)
