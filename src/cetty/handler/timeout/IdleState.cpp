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

#include "cetty/handler/timeout/IdleState.h"

namespace cetty { namespace handler { namespace timeout { 

const IdleState IdleState::READER_IDLE = 1;
const IdleState IdleState::WRITER_IDLE = 2;
const IdleState IdleState::ALL_IDLE    = 4;

std::string IdleState::toString() const {
    switch(m_value) {
        case 1:
            return "READER_IDLE";
        case 2:
            return "WRITER_IDLE";
        case 4:
            return "ALL_IDLE";
        default:
            return "UNKNOW";
    }
}

}}}