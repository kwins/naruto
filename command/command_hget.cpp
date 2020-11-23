//
// Created by kwins on 2020/11/23.
//

#include "command_hget.h"

#include "client.h"
#include "protocol/client.pb.h"
#include "utils/pack.h"
#include "database/buckets.h"

void naruto::command::CommandHget::exec(naruto::narutoClient *client) {
    client::CommandHget hget;
    auto type = utils::Pack::deSerialize(client->rbuf, hget);

    client::CommandHgetReply reply;
    reply.mutable_state()->set_errcode(0);

    for (int i = 0; i < hget.fields_size(); ++i) {
        auto feature = reply.add_features();
        std::shared_ptr<database::element> data = database::buckets->get(hget.key(), hget.fields(i));
        if (data){
            data->ptr->serialize(*feature);
        }
    }
    client->sendMsg(reply, type);
}

void naruto::command::CommandHget::execMsg(uint16_t flag, uint16_t type, const unsigned char *msg, size_t n) {

}