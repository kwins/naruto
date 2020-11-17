//
// Created by kwins on 2020/11/6.
//

#include "rotate_file_stream.h"

#include <glog/logging.h>
#include <dirent.h>
#include <algorithm>
#include "utils/errors.h"

namespace naruto::sink {

RotateFileStream::RotateFileStream(const std::string &dir, long long int rotate_aof_file_size) {
    dir_ = dir;
    LOG(INFO) << "rotate file stream dir:" << dir_;
    if (dir[dir.size()-1] != '/'){
        dir_ += "/";
    }
    rotate_aof_file_size_ = rotate_aof_file_size;
    _rotate_init();
}

void RotateFileStream::listAof(const std::string& dir, std::vector<std::string> & list) {
    DIR* dirp = opendir(dir.c_str());
    dirent* ptr;
    while ((ptr = readdir(dirp)) != nullptr){
        std::string name(ptr->d_name);
        if (ptr->d_type == DT_REG && name.find("aof") != std::string::npos){
            list.emplace_back(ptr->d_name);
        }
    }

    std::sort(list.begin(), list.end(), [](const std::string& x, const std::string& y){
        return x < y;
    });
    closedir(dirp);
}

RotateFileStream::fileState RotateFileStream::curRollFile() {
    return fileState{
        .name = cur_aof_file_,
        .offset = stream_->tellp(),
    };
}

long long RotateFileStream::write(const char * s, size_t n) {
    auto size = stream_->tellp();
    if (size >= rotate_aof_file_size_){ _rotate(); }
    stream_->write(s, n);
    return size;
}

void RotateFileStream::flush() { if (stream_){ stream_->flush(); } }

void RotateFileStream::_rotate() {
    if (stream_){
        stream_->flush();
        stream_->close();
    }
    rotate_aof_idx_++;
    cur_aof_file_ = _gen_aof_filename(rotate_aof_idx_);
    stream_ = std::make_shared<std::ofstream>(cur_aof_file_,std::ios::binary|std::ios::app|std::ios::out);
}

std::string RotateFileStream::_gen_aof_filename(int idx) { return dir_ + "naruto.aof." + std::to_string(idx); }

void RotateFileStream::_rotate_init() {
    std::vector<std::string> list;
    listAof(dir_, list);

    if (!list.empty()){
        auto last = list[list.size()-1];
        auto pos = last.find_last_of('.');
        if (pos == std::string::npos){
            throw utils::Error("bad file:" + last);
        }
        auto stridx = last.substr(pos+1);
        rotate_aof_idx_ = std::stoi(stridx) + 1;
    }else{
        rotate_aof_idx_ = 0;
    }

    cur_aof_file_ = _gen_aof_filename(rotate_aof_idx_);
    stream_ = std::make_shared<std::ofstream>(cur_aof_file_,std::ios::binary|std::ios::app|std::ios::out);
}

}