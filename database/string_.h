//
// Created by 王振奎 on 2020/10/6.
//

#ifndef NARUTO_STRING__H
#define NARUTO_STRING__H

#include "object.h"
#include "utils/errors.h"

namespace naruto::database {

class String : public ::naruto::database::object {
public:
    explicit String(std::string v = "") : data_(std::move(v)) {}

    data::TYPE type() override;

    void serialize(data::element&) override;

    void deSeralize(data::element&) override;

    void debugString() override ;

    std::string get();

    void set(const std::string&);
private:
    std::string data_;
};

}



#endif //NARUTO_STRING__H
